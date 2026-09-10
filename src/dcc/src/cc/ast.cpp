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

#include "ast.h"
#include <format>

#define FORMAT(s, fmt, ...)                                                    \
    std::format_to(std::back_inserter(s), fmt __VA_OPT__(, ) __VA_ARGS__)

#define APPEND(d, s, fmt, ...)                                                 \
    {                                                                          \
        s.append(4 * (d), ' ');                                                \
        FORMAT(s, fmt __VA_OPT__(, ) __VA_ARGS__);                             \
    }

std::string AstProg::toStr() const {
    std::string str{};
    str.reserve(1024);
    for (auto decl : decls) {
        decl->fmt(str, 0);
    }
    return str;
}


void AstDecl::fmt(std::string& s, size_t depth) const {
    switch (type) {
        case AstDeclType::Fn:
            fn.fmt(s, depth);
            break;
        case AstDeclType::Var:
            var.fmt(s, depth);
            break;
    }
}

void AstDeclFn::fmt(std::string& s, size_t depth) const {
    APPEND(depth, s, "int {}(void) {{\n", name);
    body->fmt(s, depth + 1);
    APPEND(depth, s, "}}\n");
}

void AstDeclVar::fmt(std::string& s, size_t depth) const {
    APPEND(depth, s, "int {} = ({});\n", name, val);
}


void AstStmt::fmt(std::string& s, size_t depth) const {
    switch (type) {
        case AstStmtType::Return:
            ret.fmt(s, depth);
            break;
    }
}

void AstStmtRet::fmt(std::string& s, size_t depth) const {
    APPEND(depth, s, "return ");
    expr->fmt(s);
    FORMAT(s, ";\n");
}


void AstExpr::fmt(std::string& s) const {
    FORMAT(s, "(");
    switch (type) {
        case AstExprType::ConstI32:
            int32.fmt(s);
            break;
        case AstExprType::Unary:
            unary.fmt(s);
            break;
    }
    FORMAT(s, ")");
}

void AstExprConstI32::fmt(std::string& s) const {
    FORMAT(s, "{}", val);
}

void AstExprUnary::fmt(std::string& s) const {
    switch (op) {
        case AstUnOp::Complement:
            FORMAT(s, "~");
            break;
        case AstUnOp::Negate:
            FORMAT(s, "-");
            break;
    }
    expr->fmt(s);
}
