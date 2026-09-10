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

#include "cc.h"

bool Compiler::runLexer(const char* src) {
    reset();
    lexer.addSrc(src);
    while (true) {
        Token token{lexer.nextToken()};
        tokens.push_back(token);
        if (!token) {
            error_msg = "lexer found invalid token";
            return false;
        }
        if (token == Token::Eof) {
            return true;
        }
    }
}

bool Compiler::runParser(const char* src) {
    if (!runLexer(src)) {
        return false;
    }
    if (auto ast{parser.run(tokens)}) {
        prog = *ast;
        return true;
    }
    error_msg = parser.getErrorMsg();
    return false;
}

const std::vector<Token>& Compiler::getTokens() {
    return tokens;
}

const AstProg& Compiler::getProg() const {
    return prog;
}

const char* Compiler::getErrorMsg() const {
    return error_msg;
}

void Compiler::reset() {
    error_msg = "";
    lexer = Lexer{};
    tokens.clear();
    parser = Parser{};
    prog = AstProg{};
}
