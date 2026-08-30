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

#include "driver.h"
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <sstream>

Driver::Driver(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!std::strcmp(argv[i], "--lex")) {
            run_flag |= RF_LEX;
        } else if (!std::strcmp(argv[i], "--parse")) {
            run_flag |= RF_PARSE;
        } else if (!std::strcmp(argv[i], "--codegen")) {
            run_flag |= RF_CODEGEN;
        }
    }
    if (run_flag == 0) {
        // Default to code emission if no option was supplied.
        run_flag = RF_EMIT;
    }
}

bool Driver::run(const char* filename) {
    if (run_flag & RF_LEX) {
        if (!setSrc(filename)) {
            return false;
        }
        if (!runLexer()) {
            return false;
        }
    }
    return true;
}

void Driver::error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::vfprintf(stderr, fmt, ap);
    va_end(ap);
}

bool Driver::setSrc(const char* filename) {
    std::ifstream file{filename};
    if (!file) {
        error("Error opening file: %s\n", filename);
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!file) {
        error("Error reading file: %s\n", filename);
        return false;
    }
    src = buffer.str();
    return true;
}

bool Driver::runLexer() {
    lexer.addSrc(src.c_str());
    while (true) {
        Token token{lexer.nextToken()};
        if (!token) {
            return false;
        }
        if (token == Token::Eof) {
            return true;
        }
    }
}
