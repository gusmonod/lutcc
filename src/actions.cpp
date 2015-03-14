// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>

#include "./token.h"
#include "./expr.h"

/*virtual*/ void ActionNewSym::doAction(const Token & t,
                                        SymbolsTable * variables) {
    // Interpreting value of `t` as Variable
    const Variable *v = dynamic_cast<const Variable *>(&t);

    // Creating new undefined variable or constant
    (*variables)[v->name()] = {0, false, m_constant};
}

/*virtual*/ void ActionInit::doAction(const Token & t,
                                        SymbolsTable * variables) {
    // Interpreting value of `t` as Number
    const Number *n = dynamic_cast<const Number *>(&t);

    // Assigning value to first undefined constant
    for (auto entry : *variables) {
        if (entry.second.constant && !entry.second.defined) {
            entry.second.value = n->value();
            entry.second.defined = true;
            break;
        }
    }
}
