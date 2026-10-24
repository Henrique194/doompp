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
#include "common/memory.h"
#include "mem_zone.h"
#include <new>

static bool initialized = false;
static MemZone* small_zone = nullptr;
static MemZone* medium_zone = nullptr;
static MemZone* large_zone = nullptr;

void Mem::init() {
    if (initialized) {
        return;
    }
    small_zone = MemZone::create(1);
    medium_zone = MemZone::create(2);
    large_zone = MemZone::create(4);
    initialized = true;
}

void Mem::shutdown() {
    if (!initialized) {
        return;
    }
    MemZone::destroy(small_zone);
    MemZone::destroy(medium_zone);
    MemZone::destroy(large_zone);
    small_zone = nullptr;
    medium_zone = nullptr;
    large_zone = nullptr;
    initialized = false;
}

void* Mem::malloc(u32 size) {
    if (!initialized) {
        PANIC("memory system not initialized");
    }
    if (!size) {
        return nullptr;
    }
    if (size < 256) {
        return small_zone->malloc(size, TAG_SMALL);
    }
    if (size < 1024) {
        return medium_zone->malloc(size, TAG_MEDIUM);
    }
    return large_zone->malloc(size, TAG_LARGE);
}

void Mem::free(void* ptr) {
    if (!initialized) {
        PANIC("memory system not initialized");
    }
    if (!ptr) {
        PANIC("attempted to free NULL pointer");
    }
    switch (MemZone::getTag(ptr)) {
        case TAG_SMALL:
            small_zone->free(ptr);
            break;
        case TAG_MEDIUM:
            medium_zone->free(ptr);
            break;
        case TAG_LARGE:
            large_zone->free(ptr);
            break;
        case TAG_FREE:
            PANIC("attempted to free an already-freed pointer");
            break;
        case TAG_STATIC:
            PANIC("attempted to free static memory");
            break;
        default:
            PANIC("attempted to free pointer with unknown tag");
            break;
    }
}

void* operator new(size_t size) {
    return Mem::malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
    Mem::free(ptr);
}

void* operator new[](size_t size) {
    return Mem::malloc(size);
}

void operator delete[](void* ptr) noexcept {
    Mem::free(ptr);
}
