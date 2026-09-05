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

#include "common/types.h"
#include <string>
#include <vector>


enum class AstExprType {
    ConstI32,
};

struct AstExprConstI32 {
    AstExprType type{AstExprType::ConstI32};
    i32 val;

    void fmt(std::string& s) const;
};

union AstExpr {
    struct {
        AstExprType type;
    };
    AstExprConstI32 int32;

    void fmt(std::string& s) const;
};


enum class AstStmtType {
    Return,
};

struct AstStmtRet {
    AstStmtType type{AstStmtType::Return};
    AstExpr expr;

    void fmt(std::string& s, size_t depth) const;
};

union AstStmt {
    struct {
        AstStmtType type;
    };
    AstStmtRet ret;

    void fmt(std::string& s, size_t depth) const;
};


enum class AstDeclType {
    Fn,
    Var,
};

struct AstDeclFn {
    AstDeclType type{AstDeclType::Fn};
    const char* name;
    AstStmt body;

    void fmt(std::string& s, size_t depth) const;
};

struct AstDeclVar {
    AstDeclType type{AstDeclType::Var};
    const char* name;
    const char* val;

    void fmt(std::string& s, size_t depth) const;
};

union AstDecl {
    struct {
        AstDeclType type;
    };
    AstDeclFn fn;
    AstDeclVar var;

    void fmt(std::string& s, size_t depth) const;
};


struct AstProg {
    std::vector<AstDecl> decls{};

    std::string toStr() const;
};
