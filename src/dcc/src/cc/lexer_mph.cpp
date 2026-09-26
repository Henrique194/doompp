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
// minimal perfect hash to quickly identify keywords.

#include "common/types.h"
#include "lexer.h"
#include <cstring>

static constexpr u32 HASH_SIZE = 64;
static constexpr u32 HASH_MASK = 63;

static const Token keywords[] = {
    Token::Auto,
    Token::Break,
    Token::Case,
    Token::Char,
    Token::Const,
    Token::Continue,
    Token::Default,
    Token::Do,
    Token::Double,
    Token::Else,
    Token::Enum,
    Token::Extern,
    Token::Float,
    Token::For,
    Token::Goto,
    Token::If,
    Token::Inline,
    Token::Int,
    Token::Long,
    Token::Register,
    Token::Restrict,
    Token::Return,
    Token::Short,
    Token::Signed,
    Token::Sizeof,
    Token::Static,
    Token::Struct,
    Token::Switch,
    Token::Typedef,
    Token::Union,
    Token::Unsigned,
    Token::Void,
    Token::Volatile,
    Token::While,
    Token::Alignas,
    Token::Alignof,
    Token::Atomic,
    Token::Bool,
    Token::Complex,
    Token::Generic,
    Token::Imaginary,
    Token::Noreturn,
    Token::StaticAssert,
    Token::ThreadLocal,
};

static constexpr u8 buckets[HASH_SIZE] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 5, 0,
    6, 0, 0, 1, 1, 0, 1, 0,
    0, 0, 7, 1, 0, 2, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 0, 0, 0, 2, 4,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static constexpr i8 slots[HASH_SIZE] = {
    36, 34, 39, 25, 17, -1, 37, 23,
    22, 30, 4,  26, -1, 10, 33, 42,
    16, 5,  2,  9,  15, 43, 13, 24,
    11, 35, 41, 19, 29, 14, 0,  1,
    -1, -1, -1, -1, -1, 31, 12, 27,
    6,  38, 20, -1, -1, -1, -1, 8,
    -1, 32, -1, 40, -1, -1, 3,  21,
    28, -1, -1, 7,  -1, 18, -1, -1,
};

static u32 keyword_hash(const char* s, size_t len) {
    // Each C keyword can be uniquely identified by
    // first char, last char and length.
    u32 first = (u8) s[0];
    u32 last = (u8) s[len - 1];

    // Bucket hash.
    u32 b = (len + first + last + 2u) & HASH_MASK;
    // Initial hash.
    u32 h = (len + (first << 5) + (last << 3) + 1u) & HASH_MASK;

    return (h + buckets[b]) & HASH_MASK;
}

Token Lexer::getKeyword(const char* s, size_t len) {
    if (len < 2 || len > 14) {
        return Token::Invalid;
    }
    u32 h = keyword_hash(s, len);
    i8 i = slots[h];
    if (i == -1) {
        return Token::Invalid;
    }
    Token kw{keywords[i]};
    const char* sym = kw.sym;
    // gperf optimization:
    // Comparing the first char before calling memcmp
    // apparently gives a performance boost.
    if (*s == *sym && std::memcmp(s+1, sym+1, len-1) == 0) {
        return kw;
    }
    return Token::Invalid;
}
