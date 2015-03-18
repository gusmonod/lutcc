// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>

#include "./token.h"
#include "./simpletoken.h"
#include "./expr.h"

/*virtual*/ void ActionNewSym::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Token * tmp = tokens->top();
    if (Token::Lc == *tmp || Token::Lv == *tmp) {
        delete tmp;
        tokens->pop();
    }
    tmp = nullptr;

    const Number *n = nullptr;
    const Variable *v = nullptr;

    if (m_constant) {
        // Getting the value of the constant
        n = dynamic_cast<const Number *>(tokens->top());
        tokens->pop();

        // Removing the `=` sign
        delete tokens->top();
        tokens->pop();

        // Getting the constant name
        v = dynamic_cast<const Variable *>(tokens->top());
        tokens->pop();

        // Creating new defined (and constant) symbol entry
        (*variables)[v->name()] = {n->value(), true, m_constant};
    } else {
        v = dynamic_cast<const Variable *>(tokens->top());
        tokens->pop();

        // Creating new undefined variable
        (*variables)[v->name()] = {0, false, m_constant};
    }

    // Deleting value and name of the constant
    delete n;
    n = nullptr;
    delete v;
    v = nullptr;

    // Deleting comma and (`Lv` or `Lc`), if there is a comma
    if (Token::com == *tokens->top()) {
        delete tokens->top();
        tokens->pop();
        delete tokens->top();
        tokens->pop();
    }

    tmp = tokens->top();
    if (Token::var == *tmp || Token::con == *tmp) {
        delete tmp;
        tokens->pop();
    }
    tmp = nullptr;
}

/*virtual*/ void PriorityAnalysis::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    // Retrieving the 3 last Tokens to reduce them
    Expr * right = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();
    Token::Id tId = static_cast<Token::Id>(*tokens->top());
    tokens->pop();
    Expr * left = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();

    Expr * newExpr = nullptr;

    switch (tId) {
        case Token::mul:
            newExpr = new MulExpr(tId, left, right);
            break;
        case Token::quo:
            newExpr = new DivExpr(tId, left, right);
            break;
        default:
            std::exit(EXIT_FAILURE);
            break;
    }
}
