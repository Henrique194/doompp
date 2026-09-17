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


#define CONST_I32(val) {TokenType::ConstI32, #val}
#define ID(name)       {TokenType::Id, #name}
#define INT            Token::Int
#define VOID           Token::Void

#define FN_DEF(ret, name, ...)                                                 \
    ret, ID(name), Token::LParen __VA_OPT__(,) __VA_ARGS__, Token::RParen

#define RET_STMT(val) Token::Return, CONST_I32(val), Token::Semicolon

#define VAR_DEF(type, name, val)                                               \
    type, ID(name), Token::Assign, CONST_I32(val), Token::Semicolon

#define INT_DEF(name, val) VAR_DEF(INT, name, val)


static void testLexer(const char* src, const std::vector<Token>& tokens);

TEST_CASE("Lexer - valid programs", "[lexer]") {
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
        testLexer(src, tokens);
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
        testLexer(src, tokens);
    }

    SECTION("function without return type") {
        const char* src = R"(
            intmain(void) {
                return 0;
            }
        )";
        std::vector<Token> tokens{
            ID(intmain),
            Token::LParen,
            VOID,
            Token::RParen,
            Token::LBrace,
            RET_STMT(0),
            Token::RBrace,
            Token::Eof,
        };
        testLexer(src, tokens);
    }

    SECTION("multi-digit constant") {
        const char* src = R"(
            int main(void) {
                return 100;
            }
        )";
        std::vector<Token> tokens{
            FN_DEF(INT, main, VOID),
            Token::LBrace,
            RET_STMT(100),
            Token::RBrace,
            Token::Eof,
        };
        testLexer(src, tokens);
    }

    SECTION("skip comments") {
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
        testLexer(src, tokens);
    }

    SECTION("skip tabs") {
        const char* src = R"(
            int	main	(	void)	{	return	0	;	}
        )";
        std::vector<Token> tokens{
            FN_DEF(INT, main, VOID),
            Token::LBrace,
            RET_STMT(0),
            Token::RBrace,
            Token::Eof,
        };
        testLexer(src, tokens);
    }
}

TEST_CASE("Lexer - invalid programs", "[lexer]") {
    SECTION("at sign") {
        const char* src = R"(
            int main(void) {
                return 0@1;
            }
        )";
        std::vector<Token> tokens{
            FN_DEF(INT, main, VOID),
            Token::LBrace,
            Token::Return,
            CONST_I32(0),
            Token::Invalid,
        };
        testLexer(src, tokens);
    }

    SECTION("single backslash") {
        const char* src = "\\";
        std::vector<Token> tokens{Token::Invalid};
        testLexer(src, tokens);
    }

    SECTION("single backtick") {
        const char* src = "`";
        std::vector<Token> tokens{Token::Invalid};
        testLexer(src, tokens);
    }

    SECTION("identifier starts with digit") {
        const char* src = R"(
            int main(void) {
                return 1foo;
            }
        )";
        std::vector<Token> tokens{
            FN_DEF(INT, main, VOID),
            Token::LBrace,
            Token::Return,
            Token::Invalid,
        };
        testLexer(src, tokens);
    }
}

static void testLexer(const char* src, const std::vector<Token>& tokens) {
    Compiler compiler{};
    compiler.runLexer(src);
    REQUIRE(tokens == compiler.getTokens());
}
