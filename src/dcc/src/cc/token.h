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

enum class TokenType {
    Invalid = -1,
    Add,
    AddEq,
    And,
    Auto,
    BAnd,
    BAndEq,
    BCom,
    BXor,
    BXorEq,
    BOr,
    BOrEq,
    Break,
    Case,
    Char,
    Colon,
    Comma,
    Const,
    ConstI32,
    Continue,
    Decr,
    Default,
    Deref,
    Div,
    DivEq,
    Do,
    Dot,
    Double,
    Ellipsis,
    Else,
    Enum,
    Eq,
    EqEq,
    Eof,
    Extern,
    Float,
    For,
    Gt,
    GtEq,
    Goto,
    Id,
    If,
    Incr,
    Inline,
    Int,
    LBrace,
    LBracket,
    LParen,
    LShift,
    LShiftEq,
    Lt,
    LtEq,
    Long,
    Mod,
    ModEq,
    Mul,
    MulEq,
    Not,
    NotEq,
    Or,
    Register,
    Restrict,
    Return,
    RBrace,
    RBracket,
    RParen,
    RShift,
    RShiftEq,
    Semicolon,
    Short,
    Signed,
    Sizeof,
    Static,
    Struct,
    Sub,
    SubEq,
    Switch,
    Typedef,
    Union,
    Unsigned,
    Void,
    Volatile,
    While,

    // Keywords that start with underscore.
    Alignas,
    Alignof,
    Atomic,
    Bool,
    Complex,
    Generic,
    Imaginary,
    Noreturn,
    StaticAssert,
    ThreadLocal,
};

struct Token {
    const TokenType type;
    const char* sym;

    static const Token Invalid;
    static const Token Add;
    static const Token AddEq;
    static const Token And;
    static const Token Eq;
    static const Token Auto;
    static const Token BAnd;
    static const Token BAndEq;
    static const Token BCom;
    static const Token BOr;
    static const Token BOrEq;
    static const Token BXor;
    static const Token BXorEq;
    static const Token Break;
    static const Token Case;
    static const Token Char;
    static const Token Colon;
    static const Token Comma;
    static const Token Const;
    static const Token ConstI32;
    static const Token Continue;
    static const Token Decr;
    static const Token Default;
    static const Token Deref;
    static const Token Div;
    static const Token DivEq;
    static const Token Do;
    static const Token Dot;
    static const Token Double;
    static const Token Ellipsis;
    static const Token Else;
    static const Token Enum;
    static const Token EqEq;
    static const Token Eof;
    static const Token Extern;
    static const Token Float;
    static const Token For;
    static const Token Gt;
    static const Token GtEq;
    static const Token Goto;
    static const Token Id;
    static const Token If;
    static const Token Incr;
    static const Token Inline;
    static const Token Int;
    static const Token LBrace;
    static const Token LBracket;
    static const Token LParen;
    static const Token LShift;
    static const Token LShiftEq;
    static const Token Lt;
    static const Token LtEq;
    static const Token Long;
    static const Token Mod;
    static const Token ModEq;
    static const Token Mul;
    static const Token MulEq;
    static const Token Not;
    static const Token NotEq;
    static const Token Or;
    static const Token Register;
    static const Token Restrict;
    static const Token Return;
    static const Token RBrace;
    static const Token RBracket;
    static const Token RParen;
    static const Token RShift;
    static const Token RShiftEq;
    static const Token Semicolon;
    static const Token Short;
    static const Token Signed;
    static const Token Sizeof;
    static const Token Static;
    static const Token Struct;
    static const Token Sub;
    static const Token SubEq;
    static const Token Switch;
    static const Token Typedef;
    static const Token Union;
    static const Token Unsigned;
    static const Token Void;
    static const Token Volatile;
    static const Token While;
    static const Token Alignas;
    static const Token Alignof;
    static const Token Atomic;
    static const Token Bool;
    static const Token Complex;
    static const Token Generic;
    static const Token Imaginary;
    static const Token Noreturn;
    static const Token StaticAssert;
    static const Token ThreadLocal;

    bool operator==(const Token& token) const;
    bool operator!=(const Token& token) const;
    operator bool() const;
};
