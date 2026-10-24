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

#pragma once

#include "common/types.h"

enum MemTag : u32 {
    TAG_FREE,
    TAG_STATIC,
    TAG_SMALL,
    TAG_MEDIUM,
    TAG_LARGE,
};

struct MemBlock {
    // Magic number used to detect memory corruption.
    u32 id{0};
    // Allocation type.
    MemTag tag{0};
    // Size of the block.
    u32 size{0};
    // Previous block in the list.
    MemBlock* prev{nullptr};
    // Next block in the list.
    MemBlock* next{nullptr};
};

struct MemBlockList {
    // Sentinel node for the list.
    MemBlock head{};
    // Used to speed up free block searches.
    // Usually points to a free block.
    MemBlock* rover{nullptr};

    MemBlock* allocBlock(u32 size, MemTag tag);
    void freeBlock(MemBlock* block);
};

struct MemZone {
    static MemZone* create(u32 mb);
    static void destroy(MemZone* zone);
    static MemTag getTag(void* ptr);

    void checkHeap();
    void* malloc(u32 size, MemTag tag);
    void free(void* ptr);

  private:
    // Total allocated memory, including the header.
    u32 size{0};
    // Total memory currently in use.
    u32 used{0};
    // List tracking allocated and free blocks.
    MemBlockList list{};
};
