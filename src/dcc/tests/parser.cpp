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

#include <catch2/catch_test_macros.hpp>
#include "cc/cc.h"
#include <cctype>
#include <string>

static void testParser(const char* src, const char* result);
static const char* skipSpaces(const char* s);

TEST_CASE("Parser - valid programs", "[parser]") {
    SECTION("integer definition") {
        const char* src = R"(
            int i = 10;
            int _j = 11;
        )";
        const char* result = R"(
            int i = (10);
            int _j = (11);
        )";
        testParser(src, result);
    }

    SECTION("function definition") {
        const char* src = R"(
            int main(void) {
                return 0;
            }
        )";
        const char* result = R"(
            int main(void) {
                return (0);
            }
        )";
        testParser(src, result);
    }
}

static void testParser(const char* src, const char* result) {
    Compiler compiler{};
    REQUIRE(compiler.runParser(src) == true);
    std::string prog{compiler.getProg().toStr()};
    const char* progPtr = prog.c_str();
    while (*result && *progPtr) {
        result = skipSpaces(result);
        progPtr = skipSpaces(progPtr);
        if (*result != *progPtr || *result == 0) {
            break;
        }
        result++;
        progPtr++;
    }
    // Skip any remaining space.
    result = skipSpaces(result);
    progPtr = skipSpaces(progPtr);
    REQUIRE((*result == 0 && *progPtr == 0));
}

static const char* skipSpaces(const char* s) {
    while (std::isspace(*s)) {
        s++;
    }
    return s;
}
