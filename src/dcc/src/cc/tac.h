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
#include "tac_ast.h"
#include <deque>
#include <format>
#include <optional>
#include <string>
#include <vector>

//
// TAC Generator.
// Lowers the C AST into a TAC representation.
//
class TacGen {
  public:
    std::optional<TacProg> emit(const AstProg& prog);
    const char* getErrorMsg() const;

  private:
    std::optional<TacFn> emitDecl(const AstDecl* decl);
    std::optional<TacFn> emitFn(const AstDeclFn& fn);

    bool emitStmt(const AstStmt* stmt);
    bool emitReturn(const AstStmtRet& ret);

    std::optional<TacVal> emitExpr(const AstExpr* expr);
    std::optional<TacVal> emitUnary(const AstExprUnary& unary);
    std::optional<TacUnOp> converUnOp(AstUnOp op);

    TacVal newVar();

    template<typename... Args>
    void fail(std::format_string<Args...> fmt, Args&&... args);

    // Instructions generated while lowering the current expression.
    std::vector<TacInst> insts{};
    // Names of generated temporary variables.
    std::deque<std::string> vars_name{};
    // Counter used to generate unique temporary variable names.
    u32 var_id{0};
    // Error message when TAC generation fails.
    std::string err_msg{};
};
