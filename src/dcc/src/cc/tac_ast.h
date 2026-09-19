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
// Three-address code (TAC) intermediate representation.
// TAC acts as a bridge between the C AST and assembly code.
// Complex expressions are lowered into a sequence of simple
// instructions operating on constants and variables.

#pragma once

#include "common/types.h"
#include <vector>

enum class TacValType {
    ConstI32,
    Var,
};

//
// 32-bit integer constant value.
//
struct TacValConstI32 {
    TacValType type{TacValType::ConstI32};
    i32 val;
};

//
// Named variable value, including generated temporaries.
//
struct TacValVar {
    TacValType type{TacValType::Var};
    const char* name;
};

//
// TAC value.
// Used as an operand of a TAC instruction.
//
union TacVal {
    struct {
        TacValType type;
    };
    TacValConstI32 int32;
    TacValVar var;
};


enum class TacInstType {
    Return,
    Unary,
};

enum class TacUnOp {
    BCom,
    Neg,
};

//
// Return instruction.
//
struct TacInstRet {
    TacInstType type{TacInstType::Return};
    TacVal val;
};

//
// Unary instruction.
// Applies 'op' to 'src' and stores the result in 'dst'.
//
struct TacInstUnary {
    TacInstType type{TacInstType::Unary};
    TacUnOp op;
    TacVal src;
    TacVal dst;
};

//
// TAC instruction.
//
union TacInst {
    struct {
        TacInstType type;
    };
    TacInstRet ret;
    TacInstUnary unary;
};


//
// TAC function.
// Contains the function name and the sequence of
// instructions for its body.
//
struct TacFn {
    const char* name;
    std::vector<TacInst> inst;
};


//
// Root node of a TAC program.
//
struct TacProg {
    std::vector<TacFn> fns;
};
