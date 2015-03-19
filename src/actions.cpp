// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>
#include <sstream>

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
    tokens->pop();
    Expr * left = dynamic_cast<Expr *>(tokens->top());
    tokens->pop();

    myassert(right && left, "There must be 2 `Expr` and an operator on the top of the stack");

    Expr * newExpr = nullptr;
    
    switch (opId) {
        case Token::plu:
            newExpr = new AddExpr(opId, left, right);
            break;
        case Token::min:
            newExpr = new SubExpr(opId, left, right);
            break;
        case Token::mul:
            newExpr = new MulExpr(opId, left, right);
            break;
        case Token::quo:
            newExpr = new DivExpr(opId, left, right);
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
        std::string what("Undeclared variable `");
        std::runtime_error undeclared(what + lValue->name() + '`');
        throw undeclared;
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

    //TODO Refactor to match the new prototype
    /*
    //TODO check if user is not trying to use Read with a constant

    //first, we have to check if the variable is declared
    Variable * v = dynamic_cast<Variable *>(currentExpr);
    if (v) {
        if(variables->find(v->name()) != variables->end()){

            uint64_t value;

            std::string userEntry;
            std::cin >> userEntry;
            std::istringstream iss(userEntry);

            while((iss >> value).fail()){
                std::cout << "Please type a number.";
                std::cin >> userEntry;
            }

            variables->find(v->name())->second.value = value;
            variables->find(v->name())->second.defined = true;

        }else{
        std::cerr << "Undeclared variable `" << v->name() << '`';
        }
    }else{
        std::cerr << "Symbol is not a variable.";
    }

    return currentExpr;
    */
    return nullptr;  // Default behavior: create a new non terminal Token
}

/*virtual*/ Token * ActionWrite::doAction(const Token & currentToken,
                      SymbolsTable * variables,
                      std::stack<Token *> * tokens) const {

    //TODO Refactor to match the new prototype
    /*
    Token::Id eId = static_cast<Token::Id>(*currentExpr);
    bool toPrint = false;

    // Check if the current expression is a variable or a constant
    if(eId == Token::Id::idv) {
      // Check if the symbol exists
      Variable* v = dynamic_cast<Variable*>(currentExpr);
      auto it = variables->find(v->name());
      if(it == variables->end()) {
        std::cerr << "Undeclared variable `" << v->name() << '`' << std::endl;
      }
      else {
        // And if it has a value
        if(!(it->second.defined)) {
          std::cerr << "Undefined variable `" << v->name() << '`' << std::endl;
        }
        else {
          toPrint = true;
        }
      }
    }
    else { // It's a litteral, we just print it
      toPrint = true;
    }
    if(toPrint) {
      std::cout << currentExpr->eval(*variables) << std::endl;
    }

    return currentExpr;
    */
    return nullptr;  // Default behavior: create a new non terminal Token
}
