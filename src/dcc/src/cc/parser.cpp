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

#include "common/types.h"
#include "parser.h"
#include <utility>

// Helper macro to save some typing.
#define EXPECT_TOKEN(token)                                                    \
    if (!expectToken(token)) {                                                 \
        return std::nullopt;                                                   \
    }

std::optional<AstProg> Parser::run(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return std::nullopt;
    }
    this->tokens = tokens.data();
    this->err_msg = "";
    AstProg prog{};
    while (true) {
        Token token{nextToken()};
        if (token == Token::Eof) {
            return prog;
        }
        if (auto decl{parseDecl(token)}) {
            prog.decls.push_back(*decl);
            continue;
        }
        return std::nullopt;
    }
}

const char* Parser::getErrorMsg() const {
    return err_msg.c_str();
}


std::optional<AstDecl> Parser::parseDecl(Token token) {
    switch (token.type) {
        case TokenType::Int: {
            Token name{expectToken(Token::Id)};
            if (!name) {
                return std::nullopt;
            }
            if (peekToken() == Token::LParen) {
                return parseFn(name);
            }
            return parseVar(name);
        }
        default:
            fail("no declaration found for token '{}'", token.sym);
            return std::nullopt;
    }
}

std::optional<AstDecl> Parser::parseFn(Token name) {
    EXPECT_TOKEN(Token::LParen);
    EXPECT_TOKEN(Token::Void);
    EXPECT_TOKEN(Token::RParen);
    EXPECT_TOKEN(Token::LBrace);
    auto body{parseStmt()};
    if (!body) {
        return std::nullopt;
    }
    EXPECT_TOKEN(Token::RBrace);
    AstDeclFn fn{.name = name.sym, .body = *body};
    return AstDecl{.fn = fn};
}

std::optional<AstDecl> Parser::parseVar(Token name) {
    EXPECT_TOKEN(Token::Eq);
    Token val{expectToken(Token::ConstI32)};
    if (!val) {
        return std::nullopt;
    }
    EXPECT_TOKEN(Token::Semicolon);
    AstDeclVar var{.name = name.sym, .val = val.sym};
    return AstDecl{.var = var};
}


std::optional<AstStmt> Parser::parseStmt() {
    Token token{peekToken()};
    switch (token.type) {
        case TokenType::Return:
            return parseReturn();
        default:
            fail("no statement found for token '{}'", token.sym);
            return std::nullopt;
    }
}

std::optional<AstStmt> Parser::parseReturn() {
    EXPECT_TOKEN(Token::Return);
    auto expr{parseExpr()};
    if (!expr) {
        return std::nullopt;
    }
    EXPECT_TOKEN(Token::Semicolon);
    return AstStmt{.ret = {.expr = *expr}};
}


std::optional<AstExpr> Parser::parseExpr() {
    Token token{peekToken()};
    switch (token.type) {
        case TokenType::ConstI32:
            return parseConstI32();
        default:
            fail("no expression found for token '{}'", token.sym);
            return std::nullopt;
    }
}

std::optional<AstExpr> Parser::parseConstI32() {
    Token token{expectToken(Token::ConstI32)};
    if (!token) {
        return std::nullopt;
    }
    i32 val = (i32) parseNum(token.sym);
    return AstExpr{.int32 = {.val = val}};
}

size_t Parser::parseNum(const char* str) {
    size_t num = 0;
    while (*str) {
        num = (10 * num) + (*str - '0');
        str++;
    }
    return num;
}


Token Parser::nextToken() {
    return *tokens++;
}

Token Parser::peekToken() const {
    return *tokens;
}

Token Parser::expectToken(Token token) {
    Token next{nextToken()};
    if (next.type == token.type) {
        return next;
    }
    if (next == Token::Eof) {
        fail("expected '{}' at end of input", token.sym);
    } else {
        fail("expected '{}' before '{}'", token.sym, next.sym);
    }
    return Token::Invalid;
}


template<typename... Args>
void Parser::fail(std::format_string<Args...> fmt, Args&&... args) {
    err_msg = std::format(fmt, std::forward<Args>(args)...);
}
