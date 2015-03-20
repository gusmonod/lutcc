// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>
#include <sstream>
#include <string>

#include "./token.h"
#include "./simpletoken.h"
#include "./expr.h"
#include "./errors.h"

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
    (*variables)[v->name()] = {n ? n->value() : 0, m_constant, m_constant};

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

/*virtual*/ Token * ActionSimpleExpr::doAction(const Token & currentToken,
                          SymbolsTable * variables,
                          std::stack<Token *> * tokens) const {
    Expr * e = dynamic_cast<Expr *>(tokens->top());
    myassert(e, "`e` must be pointing to a `Expr *`");

    tokens->pop();

    return e;
}

/*virtual*/ Token * ActionAddExpr::doAction(const Token & currentToken,
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

    switch (opId) {
        case Token::plu:
            newExpr = new AddExpr(left, right);
            break;
        case Token::min:
            newExpr = new SubExpr(left, right);
            break;
        case Token::mul:
            newExpr = new MulExpr(left, right);
            break;
        case Token::quo:
            newExpr = new DivExpr(left, right);
            break;
        default:
            // Only operators can be at this position
            assert((false));
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

    // Removing thxe `:=` operator
    delete tokens->top();
    tokens->pop();

    Variable * lValue = dynamic_cast<Variable *>(tokens->top());
    myassert(lValue, "`lValue` must be pointing to a `Variable *`");
    tokens->pop();

    auto symbol = variables->find(lValue->name());

    if (symbol == variables->end()) {
        throw *lValue->undeclared_error();
    }

    symbol->second.defined = true;

    try {
        symbol->second.value = rValue->eval(*variables);
    } catch(const std::runtime_error & e) {
        // Adding more info to the error message and throwing again
        std::ostringstream oss;
        oss << e.what() << " in `" << *rValue << "`";
        std::runtime_error e2(oss.str());
        throw e2;
    }

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

    auto symbol = variables->find(dest->name());
    if(symbol == variables->end()) {
        throw *dest->undeclared_error();
    }
    if(symbol->second.constant) {
        throw *dest->constant_error();
    }

    uint64_t value;
    std::string input;
    getline(std::cin,input);
    while((std::stringstream(input) >> value).fail()){
        std::cout << "Error, please type an integer." << std::endl;
        getline(std::cin,input);
    }

    symbol->second.value = value;
    symbol->second.defined = true;

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

    // Check the value of the expr
    int value;
    value = toWrite->eval(*variables);

    std::cout << value << std::endl;

    delete toWrite;
    toWrite = nullptr;

    return nullptr;  // Default behavior: create a new non terminal Token
}
