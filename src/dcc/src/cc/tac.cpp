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

#include "tac.h"
#include <format>
#include <utility>

std::optional<TacProg> TacGen::emit(const AstProg& prog) {
    TacProg tac{};
    for (auto decl : prog.decls) {
        auto fn{emitDecl(decl)};
        if (!fn) {
            return std::nullopt;
        }
        tac.fns.push_back(std::move(*fn));
    }
    return tac;
}

const char* TacGen::getErrorMsg() const {
    return err_msg.c_str();
}


std::optional<TacFn> TacGen::emitDecl(const AstDecl* decl) {
    switch (decl->type) {
        case AstDeclType::Fn:
            return emitFn(decl->fn);
        default:
            fail("unknown declaration");
            return std::nullopt;
    }
}

std::optional<TacFn> TacGen::emitFn(const AstDeclFn& fn) {
    if (!emitStmt(fn.body)) {
        return std::nullopt;
    }
    return TacFn{.name = fn.name, .inst = std::move(insts)};
}


bool TacGen::emitStmt(const AstStmt* stmt) {
    switch (stmt->type) {
        case AstStmtType::Return:
            return emitReturn(stmt->ret);
        default:
            fail("unknown statement");
            return false;
    }
}

bool TacGen::emitReturn(const AstStmtRet& ret) {
    auto val{emitExpr(ret.expr)};
    if (!val) {
        return false;
    }
    TacInstRet inst{.val = *val};
    insts.push_back(TacInst{.ret = inst});
    return true;
}


std::optional<TacVal> TacGen::emitExpr(const AstExpr* expr) {
    switch (expr->type) {
        case AstExprType::ConstI32:
            return TacVal{.int32 = {.val = expr->int32.val}};
        case AstExprType::Unary:
            return emitUnary(expr->unary);
        default:
            fail("unknown expression");
            return std::nullopt;
    }
}

std::optional<TacVal> TacGen::emitUnary(const AstExprUnary& unary) {
    auto op{converUnOp(unary.op)};
    auto src{emitExpr(unary.expr)};
    if (!op || !src) {
        return std::nullopt;
    }
    TacVal dst{newVar()};
    TacInstUnary inst{.op = *op, .src = *src, .dst = dst};
    insts.push_back(TacInst{.unary = inst});
    return dst;
}

std::optional<TacUnOp> TacGen::converUnOp(AstUnOp op) {
    switch (op) {
        case AstUnOp::BCom:
            return TacUnOp::BCom;
        case AstUnOp::Neg:
            return TacUnOp::Neg;
        default:
            fail("unknown unary operator");
            return std::nullopt;
    }
}


TacVal TacGen::newVar() {
    vars_name.push_back(std::format("tmp.{}", var_id++));
    const char* name = vars_name.back().c_str();
    return TacVal{.var = {.name = name}};
}


template<typename... Args>
void TacGen::fail(std::format_string<Args...> fmt, Args&&... args) {
    err_msg = std::format(fmt, std::forward<Args>(args)...);
}

