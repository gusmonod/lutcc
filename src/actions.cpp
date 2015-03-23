// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>
#include <sstream>
#include <string>

#include "./token.h"
#include "./simpletoken.h"
#include "./expr.h"
#include "./errors.h"
#include "./options.h"

/*virtual*/ Token * ActionNewSym::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Token * tmp = tokens->top();
    if (Token::Lc == tmp->id() || Token::Lv == tmp->id()) {
        delete tmp;
        tokens->pop();
    }
    tmp = nullptr;

    const Number *n = nullptr;
    const Variable *v = nullptr;

    if (m_constant) {
        // Getting the value of the constant
        n = dynamic_cast<const Number *>(tokens->top());
        myassert(n, "`n` must be pointing to a `Number *`");
        tokens->pop();

        // Removing the `=` sign
        delete tokens->top();
        tokens->pop();
    }

    // Getting the symbol's name
    v = dynamic_cast<const Variable *>(tokens->top());
    myassert(v, "`v` must be pointing to a `Variable *`");
    tokens->pop();

    if (variables->find(v->name()) != variables->end()) {
        std::ostringstream oss;
        oss << "The variable `" << v->name() << "` was already declared";
        throw compile_error(oss.str());
    }

    // Creating new defined (and constant)
    // or new undefined (and variable) symbol entry
    // Note: the symbol is always unused
    (*variables)[v->name()] = {n ? n->value() : 0, m_constant, m_constant, false};

    // Deleting value and name of the constant
    delete n;
    n = nullptr;
    delete v;
    v = nullptr;

    // Deleting comma and (`Lv` or `Lc`), if there is a comma
    if (Token::com == tokens->top()->id()) {
        delete tokens->top();
        tokens->pop();
        delete tokens->top();
        tokens->pop();
    }

    tmp = tokens->top();
    if (Token::var == tmp->id() || Token::con == tmp->id()) {
        delete tmp;
        tokens->pop();
    }
    tmp = nullptr;

    return nullptr;  // Default behavior: create a new non terminal Token
}

/*virtual*/ Token * ActionParenthesisExpr::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Expr * e = dynamic_cast<Expr *>(tokens->top());
    myassert(e, "`e` must be pointing to a `Expr *`");

    tokens->pop();
    // Remove the opening parenthesis
    delete tokens->top();
    tokens->pop();

    return e;
}

/*virtual*/ Token * ActionSimpleExpr::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Expr * e = dynamic_cast<Expr *>(tokens->top());
    myassert(e, "`e` must be pointing to a `Expr *`");

    tokens->pop();

    return e;
}

/*virtual*/ Token * ActionExpr::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    // Retrieving the 3 last Tokens to reduce them
    Expr * right = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();
    Token::Id opId = tokens->top()->id();
    delete tokens->top();
    tokens->pop();
    Expr * left = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();

    myassert(right && left,
            "There must be 2 `Expr` and an operator on the top of the stack");

    Expr * newExpr = nullptr;

    uint64_t denominator;
    switch (opId) {
        case Token::plu:
            if (Config::IsTransformMode()) {
                if (0 == left->eval(variables)) {
                    newExpr = right;
                } else if (0 == right->eval(variables)) {
                    newExpr = left;
                } else {
                    newExpr = new AddExpr(left, right);
                }
            } else {
                newExpr = new AddExpr(left, right);
            }
            break;
        case Token::min:
            if (Config::IsTransformMode() && right->eval(variables) == 0) {
                newExpr = left;
            } else {
                newExpr = new SubExpr(left, right);
            }
            break;
        case Token::mul:
            if (Config::IsTransformMode()) {
                if (left->eval(variables) == 1) {
                    newExpr = right;
                } else if (right->eval(variables) == 1) {
                    newExpr = left;
                } else {
                    newExpr = new MulExpr(left, right);
                }
            } else {
                newExpr = new MulExpr(left, right);
            }
            break;
        case Token::quo:
            denominator = right->eval(variables);
            if (denominator == 0) {
                throw math_error("division by zero");
            }
            if (Config::IsTransformMode() && denominator == 1) {
                newExpr = left;
            } else {
                newExpr = new DivExpr(left, right);
            }
            break;
        default:
            myassert(false, "Only operators can be at this position");
            break;
    }

    return newExpr;
}

/*virtual*/ Token * ActionAssign::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Expr * rValue = dynamic_cast<Expr *>(tokens->top());
    myassert(rValue, "`rValue` must be pointing to a `Expr *`");
    tokens->pop();

    // Removing the ':=' operator
    delete tokens->top();
    tokens->pop();

    Variable * lValue = dynamic_cast<Variable *>(tokens->top());
    myassert(lValue, "`lValue` must be pointing to a `Variable *`");
    tokens->pop();

    m_instructions->push_back(new Assignment(lValue->name(), rValue));

    delete rValue;
    rValue = nullptr;
    delete lValue;
    lValue = nullptr;

    return nullptr;  // Default behavior: create a new non terminal Token
}

/*virtual*/ Token * ActionRead::doAction(const Token & currentToken,
                      SymbolsTable * variables,
                      std::stack<Token *> * tokens) const {
    Variable * dest = dynamic_cast<Variable *>(tokens->top());
    tokens->pop();
    // Removes 'lire' keyword
    delete tokens->top();
    tokens->pop();

    myassert(dest, "`dest` must be a well-formed `Variable *`");

    m_instructions->push_back(new Read(dest->name()));

    delete dest;
    dest = nullptr;

    return nullptr;  // Default behavior: create a new non terminal Token
}

/*virtual*/ Token * ActionWrite::doAction(const Token & currentToken,
                      SymbolsTable * variables,
                      std::stack<Token *> * tokens) const {
    // Get the expr to print and pop it
    Expr * toWrite = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();
    // Remove 'ecrire' keyword
    delete tokens->top();
    tokens->pop();

    myassert(toWrite, "`toWrite` must be a well-formed `Expr *`");

    m_instructions->push_back(new Write(toWrite));

    delete toWrite;
    toWrite = nullptr;

    return nullptr;  // Default behavior: create a new non terminal Token
}
