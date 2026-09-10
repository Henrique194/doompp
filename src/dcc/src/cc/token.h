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

#include <array>

enum class TokenType {
    Invalid = -1,
    BCom,
    ConstI32,
    Decr,
    Eof,
    Eq,
    Id,
    Int,
    LBrace,
    LParen,
    Neg,
    RBrace,
    RParen,
    Return,
    Semicolon,
    Void,
};

struct Token {
    const TokenType type;
    const char* val;

    static const Token Invalid;
    static const Token BCom;
    static const Token ConstI32;
    static const Token Decr;
    static const Token Eof;
    static const Token Eq;
    static const Token Id;
    static const Token Int;
    static const Token LBrace;
    static const Token LParen;
    static const Token Neg;
    static const Token RBrace;
    static const Token RParen;
    static const Token Return;
    static const Token Semicolon;
    static const Token Void;

    static const std::array<Token, 3> KEYWORDS;
    static const std::array<Token, 9> SYMBOLS;

    bool operator==(const Token& token) const;
    bool operator!=(const Token& token) const;
    operator bool() const;
};
