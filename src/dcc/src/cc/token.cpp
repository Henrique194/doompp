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

#include "token.h"
#include <cstring>

const Token Token::Invalid{TokenType::Invalid, nullptr};
const Token Token::Assign{TokenType::Assign, "="};
const Token Token::Auto{TokenType::Auto, "auto"};
const Token Token::BCom{TokenType::BCom, "~"};
const Token Token::Break{TokenType::Break, "break"};
const Token Token::Case{TokenType::Case, "case"};
const Token Token::Char{TokenType::Char, "char"};
const Token Token::Const{TokenType::Const, "const"};
const Token Token::ConstI32{TokenType::ConstI32, "constant"};
const Token Token::Continue{TokenType::Continue, "continue"};
const Token Token::Decr{TokenType::Decr, "--"};
const Token Token::Default{TokenType::Default, "default"};
const Token Token::Do{TokenType::Do, "do"};
const Token Token::Double{TokenType::Double, "double"};
const Token Token::Else{TokenType::Else, "else"};
const Token Token::Enum{TokenType::Enum, "enum"};
const Token Token::Eof{TokenType::Eof, ""};
const Token Token::Extern{TokenType::Extern, "extern"};
const Token Token::Float{TokenType::Float, "float"};
const Token Token::For{TokenType::For, "for"};
const Token Token::Goto{TokenType::Goto, "goto"};
const Token Token::Id{TokenType::Id, "identifier"};
const Token Token::If{TokenType::If, "if"};
const Token Token::Inline{TokenType::Inline, "inline"};
const Token Token::Int{TokenType::Int, "int"};
const Token Token::LBrace{TokenType::LBrace, "{"};
const Token Token::LParen{TokenType::LParen, "("};
const Token Token::Long{TokenType::Long, "long"};
const Token Token::Neg{TokenType::Neg, "-"};
const Token Token::Register{TokenType::Register, "register"};
const Token Token::Restrict{TokenType::Restrict, "restrict"};
const Token Token::Return{TokenType::Return, "return"};
const Token Token::RBrace{TokenType::RBrace, "}"};
const Token Token::RParen{TokenType::RParen, ")"};
const Token Token::Semicolon{TokenType::Semicolon, ";"};
const Token Token::Short{TokenType::Short, "short"};
const Token Token::Signed{TokenType::Signed, "signed"};
const Token Token::Sizeof{TokenType::Sizeof, "sizeof"};
const Token Token::Static{TokenType::Static, "static"};
const Token Token::Struct{TokenType::Struct, "struct"};
const Token Token::Switch{TokenType::Switch, "switch"};
const Token Token::Typedef{TokenType::Typedef, "typedef"};
const Token Token::Union{TokenType::Union, "union"};
const Token Token::Unsigned{TokenType::Unsigned, "unsigned"};
const Token Token::Void{TokenType::Void, "void"};
const Token Token::Volatile{TokenType::Volatile, "volatile"};
const Token Token::While{TokenType::While, "while"};
const Token Token::Alignas{TokenType::Alignas, "_Alignas"};
const Token Token::Alignof{TokenType::Alignof, "_Alignof"};
const Token Token::Atomic{TokenType::Atomic, "_Atomic"};
const Token Token::Bool{TokenType::Bool, "_Bool"};
const Token Token::Complex{TokenType::Complex, "_Complex"};
const Token Token::Generic{TokenType::Generic, "_Generic"};
const Token Token::Imaginary{TokenType::Imaginary, "_Imaginary"};
const Token Token::Noreturn{TokenType::Noreturn, "_Noreturn"};
const Token Token::StaticAssert{TokenType::StaticAssert, "_Static_assert"};
const Token Token::ThreadLocal{TokenType::ThreadLocal, "_Thread_local"};

bool Token::operator==(const Token& token) const {
    if (type != token.type) {
        return false;
    }
    if (sym == token.sym) {
        return true;
    }
    if (!sym || !token.sym) {
        return false;
    }
    return std::strcmp(sym, token.sym) == 0;
}

bool Token::operator!=(const Token& token) const {
    return !(*this == token);
}

Token::operator bool() const {
    return *this != Token::Invalid;
}
