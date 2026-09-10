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

static void testParserPass(const char* src, const char* result);
static void testParserFail(const char* src);
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
        testParserPass(src, result);
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
        testParserPass(src, result);
    }

    SECTION("bitwise complement") {
        const char* src = R"(
            int main(void) {
                return ~-2147483647;
            }
            int main(void) {
                return ~0;
            }
        )";
        const char* result = R"(
            int main(void) {
                return (~(-(2147483647)));
            }
            int main(void) {
                return (~(0));
            }
        )";
        testParserPass(src, result);
    }

    SECTION("negation operator") {
        const char* src = R"(
            int main(void) {
                return -2147483647;
            }
            int main(void) {
                return -0;
            }
            int main(void) {
                return (-2);
            }
        )";
        const char* result = R"(
            int main(void) {
                return (-(2147483647));
            }
            int main(void) {
                return (-(0));
            }
            int main(void) {
                return (-(2));
            }
        )";
        testParserPass(src, result);
    }
}

TEST_CASE("Parser - invalid programs", "[parser]") {
    SECTION("extra parenthesis") {
        const char* src = R"(
            int main(void)
            {
                return (3));
            }
        )";
        testParserFail(src);
    }

    SECTION("missing constant") {
        const char* src = R"(
            int main(void) {
                return ~;
            }
        )";
        testParserFail(src);
    }

    SECTION("missing semicolon") {
        const char* src = R"(
            int main(void) {
                return -5
            }
        )";
        testParserFail(src);
    }

    SECTION("unclosed parenthesis") {
        const char* src = R"(
            int main(void)
            {
                return (1;
            }
        )";
        testParserFail(src);
    }
}

static void testParserPass(const char* src, const char* result) {
    Compiler compiler{};
    REQUIRE(compiler.runParser(src) == true);
    std::string progStr{compiler.getProg().toStr()};
    const char* prog = progStr.c_str();
    while (*result && *prog) {
        result = skipSpaces(result);
        prog = skipSpaces(prog);
        if (*result != *prog || *result == 0) {
            break;
        }
        result++;
        prog++;
    }
    // Skip any remaining space.
    result = skipSpaces(result);
    prog = skipSpaces(prog);
    REQUIRE((*result == 0 && *prog == 0));
}

static void testParserFail(const char* src) {
    Compiler compiler{};
    REQUIRE(compiler.runParser(src) == false);
}

static const char* skipSpaces(const char* s) {
    while (std::isspace(*s)) {
        s++;
    }
    return s;
}
