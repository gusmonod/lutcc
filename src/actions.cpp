// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>

#include "./token.h"
#include "./simpletoken.h"
#include "./expr.h"

/*virtual*/ Expr * ActionNewSym::doAction(const Token & readToken,
                                          SymbolsTable * variables,
                                          Expr * currentExpr) {
    // Interpreting value of `readToken` as Variable
    const Variable *v = dynamic_cast<const Variable *>(&readToken);

    // Creating new undefined variable or constant
    (*variables)[v->name()] = {0, false, m_constant};

    return nullptr;
}

/*virtual*/ Expr * ActionInit::doAction(const Token & readToken,
                                        SymbolsTable * variables,
                                        Expr * currentExpr) {
    // Interpreting value of `readToken` as Number
    const Number *n = dynamic_cast<const Number *>(&readToken);

    SymbolsTable::iterator entry;
    // Assigning value to first undefined constant
    for (entry = variables->begin(); entry != variables->end(); ++entry) {
        if (entry->second.constant && !entry->second.defined) {
            entry->second.value = n->value();
            entry->second.defined = true;
            break;
        }
    }

    return nullptr;
}

/*virtual*/ Expr *ActionPriority::doAction(const Token & readToken,
                                           SymbolsTable * variables,
                                           Expr * currentExpr) {
    const Token::Id tId = static_cast<Token::Id>(readToken);

    BinExpr * currentBinExpr = dynamic_cast<BinExpr *>(currentExpr);

    if (!currentBinExpr) {
        ActionLeftAssoc leftAssoc;
        return leftAssoc.doAction(readToken, variables, currentExpr);
    }

    Expr * right = currentBinExpr->right();

    // The right part of `currentBinExpr` becomes a BinExpr
    switch (tId) {
        case Token::plu:
            currentBinExpr->right(new AddExpr(tId, right), false);
            break;
        case Token::min:
            currentBinExpr->right(new SubExpr(tId, right), false);
            break;
        case Token::mul:
            currentBinExpr->right(new MulExpr(tId, right), false);
            break;
        case Token::quo:
            currentBinExpr->right(new DivExpr(tId, right), false);
            break;
        default:
            std::exit(EXIT_FAILURE);
            break;
    }

    return currentExpr;
}

/*virtual*/ Expr * ActionLeftAssoc::doAction(const Token & readToken,
                                             SymbolsTable * variables,
                                             Expr * currentExpr) {
    const Token::Id tId = static_cast<Token::Id>(readToken);
    const Variable *v = nullptr;
    const Number *n = nullptr;
    Expr * newExpr = nullptr;

    // Creates the correct `newExpr`, with `currentExpr` as left part
    // if `newExpr` is a `BinExpr`
    switch (tId) {
        case Token::idv:
            v = dynamic_cast<const Variable *>(&readToken);
            newExpr = new Variable(tId, v->name());
            break;

        case Token::num:
            n = dynamic_cast<const Number *>(&readToken);
            newExpr = new Number(tId, n->value());
            break;

        case Token::plu:
            newExpr = new AddExpr(tId, currentExpr);
            break;

        case Token::min:
            newExpr = new SubExpr(tId, currentExpr);
            break;

        case Token::mul:
            newExpr = new MulExpr(tId, currentExpr);
            break;

        case Token::quo:
            newExpr = new DivExpr(tId, currentExpr);
            break;

        default:
            std::exit(EXIT_FAILURE);
            break;
    }

    BinExpr * currentBinExpr = dynamic_cast<BinExpr *>(currentExpr);
    BinExpr * newBinExpr = dynamic_cast<BinExpr *>(newExpr);

    if (!currentBinExpr || newBinExpr) return newExpr;

    // Fetching the rightmost binary expression
    BinExpr * rightMostBinExpr = currentBinExpr;
    Expr    * rightMostExpr = nullptr;
    while ((rightMostExpr = rightMostBinExpr->right())) {
        rightMostBinExpr = dynamic_cast<BinExpr *>(rightMostExpr);
    }

    rightMostBinExpr->right(newExpr, false);

    return currentBinExpr;
}
