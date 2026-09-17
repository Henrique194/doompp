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

#include "lexer.h"
#include <cstring>

// Macro for lexing single-char token.
#define T(c, t)                                                                \
    case c:                                                                    \
        src++;                                                                 \
        return Token::t

// Macro for lexing a char that may form a two-char token.
// If the next char is 'n', return 't1', otherwise return 't2'.
#define TT(c, n, t1, t2)                                                       \
    case c:                                                                    \
        if (src[1] == n) {                                                     \
            src += 2;                                                          \
            Token::t1;                                                         \
        }                                                                      \
        src++;                                                                 \
        return Token::t2

void Lexer::addSrc(const char* src) {
    this->src = src;
    this->line = 1;
}

Token Lexer::nextToken() {
    while (isSpace(*src) || isComment(src)) {
        skipSpaces();
        skipComments();
    }
    switch (*src) {
        T('\0', Eof);
        T('=', Assign);
        T('~', BCom);
        T('{', LBrace);
        T('(', LParen);
        T('}', RBrace);
        T(')', RParen);
        T(';', Semicolon);
        TT('-', '-', Decr, Neg);
        case '0': case '1':
        case '2': case '3':
        case '4': case '5':
        case '6': case '7':
        case '8': case '9':
            return readNum();
        default:
            return readIdentifier();
    }
}

inline void Lexer::skipSpaces() {
    while (isSpace(*src)) {
        if (*src == '\n') {
            line++;
        }
        src++;
    }
}

inline void Lexer::skipComments() {
    // single-line comment
    if (src[0] == '/' && src[1] == '/') {
        src += 2;
        while (*src && *src != '\n') {
            src++;
        }
        if (*src == '\n') {
            src++;
            line++;
        }
    }
    // multi-line comment
    if (src[0] == '/' && src[1] == '*') {
        src += 2;
        while (*src && (src[0] != '*' || src[1] != '/')) {
            if (*src == '\n') {
                line++;
            }
            src++;
        }
        if (src[0] == '*' && src[1] == '/') {
            src += 2;
        }
    }
}

inline Token Lexer::readIdentifier() {
    if (!isLetter(*src)) {
        return Token::Invalid;
    }
    const char* ptr = src;
    while (isLetter(*src) || isDigit(*src)) {
        src++;
    }
    size_t len = src - ptr;
    if (auto t{getKeyword(ptr, len)}) {
        return t;
    }
    const char* sym = registerStr(ptr, len);
    return {TokenType::Id, sym};
}

inline Token Lexer::readNum() {
    const char* ptr = src;
    while (isDigit(*src)) {
        src++;
    }
    if (isLetter(*src)) {
        src = ptr;
        return Token::Invalid;
    }
    const char* sym = registerStr(ptr, src - ptr);
    return {TokenType::ConstI32, sym};
}

inline const char* Lexer::registerStr(const char* str, size_t len) {
    for (auto& s : strs) {
        if (s == str) {
            return s.c_str();
        }
    }
    return strs.emplace_back(str, len).c_str();
}

inline Token Lexer::getKeyword(const char* s, size_t len) {
    switch (len) {
        case 3:
            if (!std::memcmp(s, "int", 3)) {
                return Token::Int;
            }
            return Token::Invalid;
        case 4:
            if (!std::memcmp(s, "void", 4)) {
                return Token::Void;
            }
            return Token::Invalid;
        case 6:
            if (!std::memcmp(s, "return", 6)) {
                return Token::Return;
            }
            return Token::Invalid;
        default:
            return Token::Invalid;
    }
}

inline bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

inline bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

inline bool Lexer::isSpace(char c) {
    return c == ' ' || c == '\f' || c == '\n'
        || c == '\r' || c == '\t' || c == '\v';
}

inline bool Lexer::isComment(const char* str) {
    if (*str == 0 || *str != '/') {
        return false;
    }
    return str[1] == '/' || str[1] == '*';
}
