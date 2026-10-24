/*
 * Copyright (C) Henrique Barateli, <henriquejb194@gmail.com>, et al.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "common/host.h"
#include "mem_zone.h"
#include <cstddef>
#include <cstdlib>

#define ALIGN_SIZE(size) (((size) + ALIGN - 1) & ~(ALIGN - 1))

// Memory alignment used for allocations.
static constexpr u32 ALIGN = alignof(std::max_align_t);
// Size of the zone header.
static constexpr u32 ZONE_HEADER = ALIGN_SIZE(sizeof(MemZone));
// Size of the block header.
static constexpr u32 BLOCK_HEADER = ALIGN_SIZE(sizeof(MemBlock));
// Magic number used to check memory integrity.
static constexpr u32 ZONE_ID = 0x1d4a11;
// Minimum memory block size.
// Smaller values lead to more memory fragmentation,
// while larger values waste more memory.
static constexpr u32 MIN_BLOCK = 64;

MemZone* MemZone::create(u32 mb) {
    u32 alloc_size = mb * 1024 * 1024;
    byte* buf = (byte*) std::calloc(alloc_size, 1);
    if (!buf) {
        PANIC("failed to allocate zone");
    }
    auto zone = (MemZone*) buf;
    auto block = (MemBlock*) (buf + ZONE_HEADER);
    MemBlockList* list = &zone->list;

    zone->size = alloc_size;
    zone->used = 0;

    list->rover = block;
    list->head.prev = block;
    list->head.next = block;
    list->head.tag = TAG_STATIC;
    list->head.id = 0;
    list->head.size = 0;

    block->prev = &zone->list.head;
    block->next = &zone->list.head;
    block->tag = TAG_FREE;
    block->id = ZONE_ID;
    block->size = alloc_size - ZONE_HEADER;

    return zone;
}

void MemZone::destroy(MemZone* zone) {
    std::free(zone);
}

MemTag MemZone::getTag(void* ptr) {
    auto block = (MemBlock*) ((byte*) ptr - BLOCK_HEADER);
    return block->tag;
}

void MemZone::checkHeap() {
    if (this->used > this->size) {
        PANIC("used memory exceeds total memory");
    }
    MemBlock* start = &this->list.head;
    MemBlock* block = start->next;
    while (block->next != start) {
        MemBlock* next = block->next;
        if (block->id != ZONE_ID || next->id != ZONE_ID) {
            PANIC("block with invalid id");
        }
        if (!block->tag && !next->tag) {
            PANIC("two consecutive free blocks");
        }
        if (next->prev != block) {
            PANIC("next block has broken back link");
        }
        if ((byte*) block + block->size != (byte*) next) {
            PANIC("non-contiguous blocks");
        }
        block = next;
    }
}

void* MemZone::malloc(u32 size, MemTag tag) {
    size = ALIGN_SIZE(size + BLOCK_HEADER);
    MemBlock* block = list.allocBlock(size, tag);
    if (!block) {
        PANIC("failed to allocate %d bytes", size);
    }
    this->used += size;
#ifdef DEBUG
    checkHeap();
#endif
    return (void*) ((byte*) block + BLOCK_HEADER);
}

void MemZone::free(void* ptr) {
    auto block = (MemBlock*) ((byte*) ptr - BLOCK_HEADER);
    if (block->id != ZONE_ID) {
        PANIC("attempted to free pointer with invalid id");
    }
    this->used -= block->size;
    list.freeBlock(block);
#ifdef DEBUG
    checkHeap();
#endif
}

inline MemBlock* MemBlockList::allocBlock(u32 size, MemTag tag) {
    // Find free block.
    MemBlock* block = this->rover;
    MemBlock* start = block->prev;
    while (block->tag || block->size < size) {
        if (block == start) {
            // Scanned all list.
            return nullptr;
        }
        block = block->next;
    }
    block->tag = tag;
    block->id = ZONE_ID;

    // Split the block if it is large enough.
    u32 extra = block->size - size;
    if (extra >= MIN_BLOCK) {
        auto other = (MemBlock*) ((byte*) block + size);
        other->id = ZONE_ID;
        other->tag = TAG_FREE;
        other->size = extra;
        other->prev = block;
        other->next = block->next;
        other->next->prev = other;
        block->next = other;
        block->size = size;
    }

    // Next block is likely to be free.
    this->rover = block->next;

    return block;
}

inline void MemBlockList::freeBlock(MemBlock* block) {
    this->rover = block;
    block->tag = TAG_FREE;
    if (block->prev->tag == TAG_FREE) {
        // Merge with previous free block.
        MemBlock* other = block->prev;
        other->size += block->size;
        other->next = block->next;
        other->next->prev = other;
        this->rover = other;
        block = other;
    }
    if (block->next->tag == TAG_FREE) {
        // Merge with next free block.
        MemBlock* other = block->next;
        block->size += other->size;
        block->next = other->next;
        block->next->prev = block;
    }
}
