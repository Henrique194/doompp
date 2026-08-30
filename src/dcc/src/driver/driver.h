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
#include "cc/lexer.h"
#include <string>

class Driver {
    enum RunFlag {
        RF_LEX = 1,
        RF_PARSE = 3,
        RF_CODEGEN = 7,
        RF_EMIT = 15,
    };

  public:
    Driver(int argc, char* argv[]);
    bool run(const char* filename);

  private:
    static void error(const char* fmt, ...);
    bool setSrc(const char* filename);
    bool runLexer();

    u8 run_flag{};
    std::string src{};
    Lexer lexer{};
};
