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
#include <format>
#include <optional>
#include <string>

class Parser {
  public:
    std::optional<AstProg> run(const std::vector<Token>& tokens);
    const char* getErrorMsg() const;

  private:
    std::optional<AstDecl> parseDecl(Token token);
    std::optional<AstDecl> parseFn(Token name);
    std::optional<AstDecl> parseVar(Token name);

    std::optional<AstStmt> parseStmt();
    std::optional<AstStmt> parseReturn();

    std::optional<AstExpr> parseExpr();
    std::optional<AstExpr> parseConstI32();
    static size_t parseNum(const char* str);

    Token nextToken();
    Token peekToken() const;
    Token expectToken(Token token);

    template<typename... Args>
    void fail(std::format_string<Args...> fmt, Args&&... args);

    const Token* tokens{nullptr};
    std::string err_msg{};
};
