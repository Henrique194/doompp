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

#include "ast.h"
#include "token.h"
#include <deque>
#include <format>
#include <optional>
#include <string>

class Parser {
  public:
    std::optional<AstProg> run(const std::vector<Token>& tks);
    const char* getErrorMsg() const;

  private:
    AstDecl* parseDecl(Token token);
    AstDecl* parseFn(Token name);
    AstDecl* parseVar(Token name);

    AstStmt* parseStmt();
    AstStmt* parseReturn();

    AstExpr* parseExpr();
    AstExpr* parseConstI32();
    AstExpr* parseUnary(AstUnOp op);
    static size_t parseNum(const char* str);

    Token nextToken();
    Token peekToken() const;
    Token expectToken(Token token);

    template<typename... Args>
    AstDecl* newDecl(Args&&... args);
    template<typename... Args>
    AstExpr* newExpr(Args&&... args);
    template<typename... Args>
    AstStmt* newStmt(Args&&... args);

    template<typename... Args>
    void fail(std::format_string<Args...> fmt, Args&&... args);

    void reset(const std::vector<Token>& tks);

    const Token* tokens{nullptr};
    std::string err_msg{};
    std::deque<AstDecl> decls{};
    std::deque<AstExpr> exprs{};
    std::deque<AstStmt> stmts{};
};
