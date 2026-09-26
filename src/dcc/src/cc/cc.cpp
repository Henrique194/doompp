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
#include <utility>

bool Compiler::runLexer(const char* src) {
    reset();
    lexer.addSrc(src);
    while (true) {
        Token token{lexer.nextToken()};
        tokens.push_back(token);
        switch (token.type) {
            case TokenType::Eof:
                return true;
            case TokenType::Invalid:
                err_msg = "lexer found invalid token";
                return false;
            case TokenType::Atomic:
                err_msg = "no support for atomic operations";
                return false;
            case TokenType::Complex:
            case TokenType::Imaginary:
                err_msg = "no support for complex types";
                return false;
            case TokenType::ThreadLocal:
                err_msg = "no support for multithreading";
                return false;
            default:
                break;
        }
    }
}

bool Compiler::runParser(const char* src) {
    if (!runLexer(src)) {
        return false;
    }
    if (auto ast{parser.run(tokens)}) {
        prog = std::move(*ast);
        return true;
    }
    err_msg = parser.getErrorMsg();
    return false;
}

bool Compiler::emitTac(const char* src) {
    if (!runParser(src)) {
        return false;
    }
    if (auto ast{tac_gen.emit(prog)}) {
        tac_prog = std::move(*ast);
        return true;
    }
    err_msg = parser.getErrorMsg();
    return false;
}

const std::vector<Token>& Compiler::getTokens() {
    return tokens;
}

const AstProg& Compiler::getProg() const {
    return prog;
}

const char* Compiler::getErrorMsg() const {
    return err_msg;
}

void Compiler::reset() {
    err_msg = "";
    lexer = Lexer{};
    tokens.clear();
    parser = Parser{};
    prog = AstProg{};
    tac_prog = TacProg{};
}
