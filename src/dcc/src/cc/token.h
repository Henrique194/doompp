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
    Assign,
    Auto,
    BCom,
    Break,
    Case,
    Char,
    Const,
    ConstI32,
    Continue,
    Decr,
    Default,
    Do,
    Double,
    Else,
    Enum,
    Eof,
    Extern,
    Float,
    For,
    Goto,
    Id,
    If,
    Inline,
    Int,
    LBrace,
    LParen,
    Long,
    Neg,
    Register,
    Restrict,
    Return,
    RBrace,
    RParen,
    Semicolon,
    Short,
    Signed,
    Sizeof,
    Static,
    Struct,
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
    static const Token Assign;
    static const Token Auto;
    static const Token BCom;
    static const Token Break;
    static const Token Case;
    static const Token Char;
    static const Token Const;
    static const Token ConstI32;
    static const Token Continue;
    static const Token Decr;
    static const Token Default;
    static const Token Do;
    static const Token Double;
    static const Token Else;
    static const Token Enum;
    static const Token Eof;
    static const Token Extern;
    static const Token Float;
    static const Token For;
    static const Token Goto;
    static const Token Id;
    static const Token If;
    static const Token Inline;
    static const Token Int;
    static const Token LBrace;
    static const Token LParen;
    static const Token Long;
    static const Token Neg;
    static const Token Register;
    static const Token Restrict;
    static const Token Return;
    static const Token RBrace;
    static const Token RParen;
    static const Token Semicolon;
    static const Token Short;
    static const Token Signed;
    static const Token Sizeof;
    static const Token Static;
    static const Token Struct;
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
