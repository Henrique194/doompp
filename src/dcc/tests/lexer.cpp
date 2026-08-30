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
#include "cc/lexer.h"

#define INT  Token::Int
#define VOID Token::Void

#define FN_DEF(ret, name, ...)                                                 \
    ret, Token::Id(#name), Token::LParen __VA_OPT__(,) __VA_ARGS__, Token::RParen

#define RET_STMT(val) Token::Return, Token::Literal(#val), Token::Semicolon

#define VAR_DEF(type, name, val)                                               \
    type, Token::Id(#name), Token::Eq, Token::Literal(#val), Token::Semicolon

#define INT_DEF(name, val) VAR_DEF(INT, name, val)

static void test_lexer(const char* src, const std::vector<Token>& tokens);

TEST_CASE("valid programs", "lexer") {
    SECTION("integer definition") {
        const char* src = R"(
            int x = 10;
            int x1 = 123;
            int var_name = 549;
            int _otherInt = 472;
        )";
        std::vector<Token> tokens{
            INT_DEF(x, 10),
            INT_DEF(x1, 123),
            INT_DEF(var_name, 549),
            INT_DEF(_otherInt, 472),
            Token::Eof,
        };
        test_lexer(src, tokens);
    }

    SECTION("function definition") {
        const char* src = R"(
            int main(void) {
                int i = 1;
                return 0;
            }
            void no_op() {
            }
        )";
        std::vector<Token> tokens{
            FN_DEF(INT, main, VOID),
            Token::LBrace,
            INT_DEF(i, 1),
            RET_STMT(0),
            Token::RBrace,
            FN_DEF(VOID, no_op),
            Token::LBrace,
            Token::RBrace,
            Token::Eof,
        };
        test_lexer(src, tokens);
    }

    SECTION("skip comment") {
        const char* src = R"(
            int i = 10; // some comment
            int j = 11  // other comment
            ;
            /*
             * multi-line comment
             */
            int k = 12;
        )";
        std::vector<Token> tokens{
            INT_DEF(i, 10),
            INT_DEF(j, 11),
            INT_DEF(k, 12),
            Token::Eof,
        };
        test_lexer(src, tokens);
    }
}

static void test_lexer(const char* src, const std::vector<Token>& tokens) {
    Lexer lexer{};
    lexer.addSrc(src);
    for (auto& token : tokens) {
        REQUIRE(token == lexer.nextToken());
    }
}
