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
        return nullptr;                                                        \
    }

std::optional<AstProg> Parser::run(const std::vector<Token>& tks) {
    if (tks.empty()) {
        return std::nullopt;
    }
    reset(tks);
    AstProg prog{};
    while (true) {
        Token token{nextToken()};
        if (token == Token::Eof) {
            return prog;
        }
        if (auto decl{parseDecl(token)}) {
            prog.decls.push_back(decl);
            continue;
        }
        return std::nullopt;
    }
}

const char* Parser::getErrorMsg() const {
    return err_msg.c_str();
}


AstDecl* Parser::parseDecl(Token token) {
    switch (token.type) {
        case TokenType::Int: {
            Token name{expectToken(Token::Id)};
            if (!name) {
                return nullptr;
            }
            if (peekToken() == Token::LParen) {
                return parseFn(name);
            }
            return parseVar(name);
        }
        default:
            fail("no declaration found for token '{}'", token.sym);
            return nullptr;
    }
}

AstDecl* Parser::parseFn(Token name) {
    EXPECT_TOKEN(Token::LParen);
    EXPECT_TOKEN(Token::Void);
    EXPECT_TOKEN(Token::RParen);
    EXPECT_TOKEN(Token::LBrace);
    AstStmt* body = parseStmt();
    if (!body) {
        return nullptr;
    }
    EXPECT_TOKEN(Token::RBrace);
    AstDeclFn fn{.name = name.sym, .body = body};
    return newDecl(AstDecl{.fn = fn});
}

AstDecl* Parser::parseVar(Token name) {
    EXPECT_TOKEN(Token::Assign);
    Token val{expectToken(Token::ConstI32)};
    if (!val) {
        return nullptr;
    }
    EXPECT_TOKEN(Token::Semicolon);
    AstDeclVar var{.name = name.sym, .val = val.sym};
    return newDecl(AstDecl{.var = var});
}


AstStmt* Parser::parseStmt() {
    Token token{peekToken()};
    switch (token.type) {
        case TokenType::Return:
            return parseReturn();
        default:
            fail("no statement found for token '{}'", token.sym);
            return nullptr;
    }
}

AstStmt* Parser::parseReturn() {
    EXPECT_TOKEN(Token::Return);
    AstExpr* expr = parseExpr();
    if (!expr) {
        return nullptr;
    }
    EXPECT_TOKEN(Token::Semicolon);
    AstStmtRet ret{.expr = expr};
    return newStmt(AstStmt{.ret = ret});
}


AstExpr* Parser::parseExpr() {
    Token token{peekToken()};
    switch (token.type) {
        case TokenType::BCom:
            return parseUnary(AstUnOp::Complement);
        case TokenType::ConstI32:
            return parseConstI32();
        case TokenType::LParen: {
            nextToken();
            AstExpr* expr = parseExpr();
            EXPECT_TOKEN(Token::RParen);
            return expr;
        }
        case TokenType::Neg:
            return parseUnary(AstUnOp::Negate);
        default:
            fail("no expression found for token '{}'", token.sym);
            return nullptr;
    }
}

AstExpr* Parser::parseConstI32() {
    Token token{expectToken(Token::ConstI32)};
    if (!token) {
        return nullptr;
    }
    i32 val = (i32) parseNum(token.sym);
    return newExpr(AstExpr{.int32 = {.val = val}});
}

AstExpr* Parser::parseUnary(AstUnOp op) {
    // Discard unary operator.
    nextToken();
    bool hasParen = false;
    if (peekToken() == Token::LParen) {
        hasParen = true;
        nextToken();
    }
    AstExpr* expr = parseExpr();
    if (!expr) {
        return nullptr;
    }
    if (hasParen) {
        EXPECT_TOKEN(Token::RParen);
    }
    AstExprUnary unary{.op = op, .expr = expr};
    return newExpr(AstExpr{.unary = unary});
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
AstDecl* Parser::newDecl(Args&&... args) {
    return &decls.emplace_back(std::forward<Args>(args)...);
}

template<typename... Args>
AstExpr* Parser::newExpr(Args&&... args) {
    return &exprs.emplace_back(std::forward<Args>(args)...);
}

template<typename... Args>
AstStmt* Parser::newStmt(Args&&... args) {
    return &stmts.emplace_back(std::forward<Args>(args)...);
}


template<typename... Args>
void Parser::fail(std::format_string<Args...> fmt, Args&&... args) {
    err_msg = std::format(fmt, std::forward<Args>(args)...);
}

void Parser::reset(const std::vector<Token>& tks) {
    tokens = tks.data();
    err_msg = "";
    decls.clear();
    exprs.clear();
    stmts.clear();
}
