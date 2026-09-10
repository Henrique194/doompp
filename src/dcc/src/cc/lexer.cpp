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

void Lexer::addSrc(const char* src) {
    this->src = src;
    this->line = 1;
}

Token Lexer::nextToken() {
    while (isSpace(*src) || isComment(src)) {
        skipSpaces();
        skipComments();
    }
    if (*src == 0) {
        return Token::Eof;
    }
    if (auto t{readSymbol()}) {
        return t;
    }
    if (auto t{readKeyword()}) {
        return t;
    }
    if (auto t{readIdentifier()}) {
        return t;
    }
    if (auto t{readNum()}) {
        return t;
    }
    return Token::Invalid;
}

void Lexer::skipSpaces() {
    while (isSpace(*src)) {
        if (*src == '\n') {
            line++;
        }
        src++;
    }
}

void Lexer::skipComments() {
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

Token Lexer::readSymbol() {
    for (auto& t : Token::SYMBOLS) {
        const char* s1 = src;
        const char* s2 = t.val;
        while (*s1 && *s1 == *s2) {
            s1++;
            s2++;
        }
        if (*s2 == 0) {
            src = s1;
            return t;
        }
    }
    return Token::Invalid;
}

Token Lexer::readKeyword() {
    for (auto& t : Token::KEYWORDS) {
        const char* s1 = src;
        const char* s2 = t.val;
        while (*s1 && *s1 == *s2) {
            s1++;
            s2++;
        }
        if (*s2 == 0 && !isLetter(*s1) && !isDigit(*s1)) {
            src = s1;
            return t;
        }
    }
    return Token::Invalid;
}

Token Lexer::readIdentifier() {
    if (!isLetter(*src)) {
        return Token::Invalid;
    }
    const char* ptr = src;
    while (isLetter(*src) || isDigit(*src)) {
        src++;
    }
    const char* val = registerStr(ptr, src - ptr);
    return {TokenType::Id, val};
}

Token Lexer::readNum() {
    if (!isDigit(*src)) {
        return Token::Invalid;
    }
    const char* ptr = src;
    while (isDigit(*src)) {
        src++;
    }
    if (isLetter(*src)) {
        src = ptr;
        return Token::Invalid;
    }
    const char* val = registerStr(ptr, src - ptr);
    return {TokenType::ConstI32, val};
}

const char* Lexer::registerStr(const char* str, size_t len) {
    for (auto& s : strs) {
        if (s == str) {
            return s.c_str();
        }
    }
    return strs.emplace_back(str, len).c_str();
}

bool Lexer::isLetter(char c) {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isDigit(char c) {
    return std::isdigit(c);
}

bool Lexer::isSpace(char c) {
    return std::isspace(c);
}

bool Lexer::isComment(const char* str) {
    if (*str == 0 || *str != '/') {
        return false;
    }
    return str[1] == '/' || str[1] == '*';
}
