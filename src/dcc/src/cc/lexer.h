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
#include "token.h"
#include <string>
#include <deque>

class Lexer {
  public:
    void addSrc(const char* src);
    Token nextToken();

  private:
    void skipSpaces();
    void skipComments();
    Token readIdentifier();
    Token readNum();
    const char* registerStr(const char* str, size_t len);
    static Token getKeyword(const char* s, size_t len);
    static bool isLetter(char c);
    static bool isDigit(char c);
    static bool isSpace(char c);
    static bool isComment(const char* str);

    const char* src{nullptr};
    u32 line{1};
    // We need std::deque for pointer stability when creating tokens.
    std::deque<std::string> strs{512};
};
