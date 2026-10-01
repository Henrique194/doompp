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
#include <string_view>

void Lexer::addSrc(const char* src) {
    this->src = src;
    this->line = 1;
    this->strs.clear();
}

Token Lexer::nextToken() {
    while (true) {
        if (isSpace(*src)) {
            skipSpaces();
            continue;
        }
        if (isComment(src)) {
            skipComments();
            continue;
        }
        break;
    }
    switch (*src) {
        default:
            return readIdentifier();
        case '0': case '1':
        case '2': case '3':
        case '4': case '5':
        case '6': case '7':
        case '8': case '9':
            return readNum();
        case '\0':
            return Token::Eof;
        case '[':
            src++;
            return Token::LBracket;
        case ']':
            src++;
            return Token::RBracket;
        case '(':
            src++;
            return Token::LParen;
        case ')':
            src++;
            return Token::RParen;
        case '{':
            src++;
            return Token::LBrace;
        case '}':
            src++;
            return Token::RBrace;
        case ':':
            src++;
            return Token::Colon;
        case ';':
            src++;
            return Token::Semicolon;
        case ',':
            src++;
            return Token::Comma;
        case '~':
            src++;
            return Token::BCom;
        case '.':
            if (src[1] == '.' && src[2] == '.') {
                src += 3;
                return Token::Ellipsis;
            }
            src++;
            return Token::Dot;
        case '+':
            switch (src[1]) {
                case '+':
                    src += 2;
                    return Token::Incr;
                case '=':
                    src += 2;
                    return Token::AddEq;
                default:
                    src++;
                    return Token::Add;
            }
        case '-':
            switch (src[1]) {
                case '>':
                    src += 2;
                    return Token::Deref;
                case '-':
                    src += 2;
                    return Token::Decr;
                case '=':
                    src += 2;
                    return Token::SubEq;
                default:
                    src++;
                    return Token::Sub;
            }
        case '*':
            if (src[1] == '=') {
                src += 2;
                return Token::MulEq;
            }
            src++;
            return Token::Mul;
        case '/':
            if (src[1] == '=') {
                src += 2;
                return Token::DivEq;
            }
            src++;
            return Token::Div;
        case '&':
            switch (src[1]) {
                case '&':
                    src += 2;
                    return Token::And;
                case '=':
                    src += 2;
                    return Token::BAndEq;
                default:
                    src++;
                    return Token::BAnd;
            }
        case '|':
            switch (src[1]) {
                case '|':
                    src += 2;
                    return Token::Or;
                case '=':
                    src += 2;
                    return Token::BOrEq;
                default:
                    src++;
                    return Token::BOr;
            }
        case '!':
            if (src[1] == '=') {
                src += 2;
                return Token::NotEq;
            }
            src++;
            return Token::Not;
        case '^':
            if (src[1] == '=') {
                src += 2;
                return Token::BXorEq;
            }
            src++;
            return Token::BXor;
        case '%':
            if (src[1] == '=') {
                src += 2;
                return Token::ModEq;
            }
            src++;
            return Token::Mod;
        case '=':
            if (src[1] == '=') {
                src += 2;
                return Token::EqEq;
            }
            src++;
            return Token::Eq;
        case '<':
            switch (src[1]) {
                case '<':
                    if (src[2] == '=') {
                        src += 3;
                        return Token::LShiftEq;
                    }
                    src += 2;
                    return Token::LShift;
                case '=':
                    src += 2;
                    return Token::LtEq;
                default:
                    src++;
                    return Token::Lt;
            }
        case '>':
            switch (src[1]) {
                case '>':
                    if (src[2] == '=') {
                        src += 3;
                        return Token::RShiftEq;
                    }
                    src += 2;
                    return Token::RShift;
                case '=':
                    src += 2;
                    return Token::GtEq;
                default:
                    src++;
                    return Token::Gt;
            }
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
    std::string view{str, len};
    for (auto& s : strs) {
        if (s == view) {
            return s.c_str();
        }
    }
    return strs.emplace_back(str, len).c_str();
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
