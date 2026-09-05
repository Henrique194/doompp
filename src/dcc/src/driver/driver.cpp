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
    // Default to code emission if no option is supplied.
    run_level = RL_EMIT;
    for (int i = 1; i < argc; i++) {
        if (!std::strcmp(argv[i], "--lex")) {
            run_level = RL_LEX;
        } else if (!std::strcmp(argv[i], "--parse")) {
            run_level = RL_PARSE;
        } else if (!std::strcmp(argv[i], "--codegen")) {
            run_level = RL_CODEGEN;
        }
    }
}

bool Driver::run(const char* filename) {
    if (!setSrc(filename)) {
        return false;
    }
    if (run_level >= RL_PARSE) {
        return runParser();
    }
    if (run_level >= RL_LEX) {
        return runLexer();
    }
    return true;
}

bool Driver::setSrc(const char* filename) {
    this->filename = filename;
    std::ifstream file{filename};
    if (!file) {
        error("error opening file");
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!file) {
        error("error reading file");
        return false;
    }
    this->src = buffer.str();
    return true;
}

bool Driver::runLexer() {
    if (compiler.runLexer(src.c_str())) {
        return true;
    }
    error(compiler.getErrorMsg());
    return false;
}

bool Driver::runParser() {
    if (compiler.runParser(src.c_str())) {
        return true;
    }
    error(compiler.getErrorMsg());
    return false;
}

void Driver::error(const char* fmt, ...) const {
    std::fprintf(stderr, "%s: error: ", filename);

    va_list ap;
    va_start(ap, fmt);
    std::vfprintf(stderr, fmt, ap);
    std::fprintf(stderr, "\n");
    va_end(ap);
}
