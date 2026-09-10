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
const Token Token::BCom{TokenType::BCom, "~"};
const Token Token::ConstI32{TokenType::ConstI32, "constant"};
const Token Token::Decr{TokenType::Decr, "--"};
const Token Token::Eof{TokenType::Eof, ""};
const Token Token::Eq{TokenType::Eq, "="};
const Token Token::Id{TokenType::Id, "identifier"};
const Token Token::Int{TokenType::Int, "int"};
const Token Token::LBrace{TokenType::LBrace, "{"};
const Token Token::LParen{TokenType::LParen, "("};
const Token Token::Neg{TokenType::Neg, "-"};
const Token Token::RBrace{TokenType::RBrace, "}"};
const Token Token::RParen{TokenType::RParen, ")"};
const Token Token::Return{TokenType::Return, "return"};
const Token Token::Semicolon{TokenType::Semicolon, ";"};
const Token Token::Void{TokenType::Void, "void"};

const std::array<Token, 3> Token::KEYWORDS{
    Token::Int,
    Token::Return,
    Token::Void,
};

const std::array<Token, 9> Token::SYMBOLS{
    Token::Decr,
    Token::BCom,
    Token::Eq,
    Token::LBrace,
    Token::LParen,
    Token::Neg,
    Token::RBrace,
    Token::RParen,
    Token::Semicolon,
};

bool Token::operator==(const Token& token) const {
    if (type != token.type) {
        return false;
    }
    if (val == token.val) {
        return true;
    }
    if (!val || !token.val) {
        return false;
    }
    return std::strcmp(val, token.val) == 0;
}

bool Token::operator!=(const Token& token) const {
    return !(*this == token);
}

Token::operator bool() const {
    return *this != Token::Invalid;
}
