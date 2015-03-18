// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./automaton.h"

#include <stack>
#include <set>

#include "./actions.h"
#include "./transitions.h"
#include "./states.h"
#include "./token.h"

Automaton::Automaton() : m_trans() {
    Action * priority = new PriorityAnalysis;

    m_trans[State::E0] [Token::P]   = new TransAccept;

    m_trans[State::E0] [Token::Ld]  = new TransShift(State::E2, false);
    m_trans[State::E0] [Token::con] =  // TransReduce(0, Token::Ld, false);
    m_trans[State::E0] [Token::var] =  // TransReduce(0, Token::Ld, false);
    m_trans[State::E0] [Token::idv] =  // TransReduce(0, Token::Ld, false);
    m_trans[State::E0] [Token::ecr] =  // TransReduce(0, Token::Ld, false);
    m_trans[State::E0] [Token::lir] =  // TransReduce(0, Token::Ld, false);
    m_trans[State::E0] [Token::con] = new TransReduce(0, Token::Ld, false);

    m_trans[State::E2] [Token::Li]  = new TransShift(State::E3, false);
    m_trans[State::E2] [Token::I]   = new TransReduce(0, Token::Li, false);


    m_trans[State::E3] [Token::idv] =  // TransShift(State::E5);
    m_trans[State::E2] [Token::idv] = new TransShift(State::E5);
    m_trans[State::E2] [Token::ecr] =  // TransShift(State::E6);
    m_trans[State::E3] [Token::ecr] = new TransShift(State::E6);
    m_trans[State::E2] [Token::lir] =  // TransShift(State::E7);
    m_trans[State::E3] [Token::lir] = new TransShift(State::E7);
    m_trans[State::E2] [Token::con] = new TransShift(State::E8);
    m_trans[State::E2] [Token::var] = new TransShift(State::E9);
    m_trans[State::E2] [Token::D]   = new TransReduce(1, Token::Ld);

    m_trans[State::E3] [Token::END] = new TransReduce(2, Token::P);
    m_trans[State::E3] [Token::I]   = new TransReduce(1, Token::Li);

    m_trans[State::E5] [Token::aff] = new TransShift(State::E12);

    m_trans[State::E6] [Token::E]   = new TransShift(State::E13, false);
    m_trans[State::E6] [Token::opp] = new TransShift(State::E14);
    m_trans[State::E6] [Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E6] [Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E7] [Token::idv] = new TransShift(State::E17);

    m_trans[State::E8] [Token::idv] = new TransShift(State::E19);

    m_trans[State::E9] [Token::idv] = new TransShift(State::E18);

    m_trans[State::E12] [Token::E]  = new TransShift(State::E20, false);
    m_trans[State::E12][Token::opp] = new TransShift(State::E14);
    m_trans[State::E12][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E12][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E13][Token::col] = new TransReduce(2, Token::I);
    m_trans[State::E13][Token::plu] = new TransShift(State::E22);
    m_trans[State::E13][Token::min] = new TransShift(State::E23);
    m_trans[State::E13][Token::quo] = new TransShift(State::E24);
    m_trans[State::E13][Token::mul] = new TransShift(State::E25);

    m_trans[State::E14][Token::E]   = new TransShift(State::E26, false);
    m_trans[State::E14][Token::opp] = new TransShift(State::E14);
    m_trans[State::E14][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E14][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E17][Token::col] = new TransReduce(2, Token::I);

    // Shift-reduce conflict for Lv
    m_trans[State::E18][Token::Lv]  = new TransShift(State::E29, false);
    m_trans[State::E18][Token::com] =  // TransReduce(0, Token::Lv, false);
    m_trans[State::E18][Token::col] = new TransReduce(0, Token::Lv, false);

    m_trans[State::E19][Token::equ] = new TransShift(State::E45);

    m_trans[State::E20][Token::col] = new TransReduce(3, Token::I);
    m_trans[State::E20][Token::plu] = new TransShift(State::E22);
    m_trans[State::E20][Token::min] = new TransShift(State::E23);
    m_trans[State::E20][Token::quo] = new TransShift(State::E24);
    m_trans[State::E20][Token::mul] = new TransShift(State::E25);

    m_trans[State::E22][Token::E]   = new TransShift(State::E31, false);
    m_trans[State::E22][Token::opp] = new TransShift(State::E14);
    m_trans[State::E22][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E22][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E23][Token::E]   = new TransShift(State::E32, false);
    m_trans[State::E23][Token::opp] = new TransShift(State::E14);
    m_trans[State::E23][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E23][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E24][Token::E]   = new TransShift(State::E33, false);
    m_trans[State::E24][Token::opp] = new TransShift(State::E14);
    m_trans[State::E24][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E24][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E25][Token::E]   = new TransShift(State::E34, false);
    m_trans[State::E25][Token::opp] = new TransShift(State::E14);
    m_trans[State::E25][Token::num] =  // TransReduce(0, Token::E);
    m_trans[State::E25][Token::idv] = new TransReduce(0, Token::E);

    m_trans[State::E26][Token::clo] = new TransReduce(2, Token::E);
    m_trans[State::E26][Token::plu] = new TransShift(State::E22);
    m_trans[State::E26][Token::min] = new TransShift(State::E23);
    m_trans[State::E26][Token::quo] = new TransShift(State::E24);
    m_trans[State::E26][Token::mul] = new TransShift(State::E25);

    m_trans[State::E28][Token::Lc]  = new TransShift(State::E36, false);
    m_trans[State::E28][Token::com] =  // TransReduce(0, Token::Lc, false);
    m_trans[State::E28][Token::col] = new TransReduce(0, Token::Lc, false);

    m_trans[State::E29][Token::col] = new TransReduce(3, Token::D, true,
                                            new ActionNewSym(false));
    m_trans[State::E29][Token::com] = new TransShift(State::E38);

    // E31, E32, E33, E34
    m_trans[State::E31][Token::plu] =  // TransShift(State::E22);
    m_trans[State::E32][Token::plu] = new TransShift(State::E22);

    m_trans[State::E31][Token::min] =  // TransShift(State::E23);
    m_trans[State::E32][Token::min] = new TransShift(State::E23);

    m_trans[State::E31][Token::quo] =  // TransShift(State::E24);
    m_trans[State::E32][Token::quo] =  // TransShift(State::E24);
    m_trans[State::E33][Token::quo] =  // TransShift(State::E24);
    m_trans[State::E34][Token::quo] = new TransShift(State::E24);

    m_trans[State::E31][Token::mul] =  // TransShift(State::E25);
    m_trans[State::E32][Token::mul] =  // TransShift(State::E25);
    m_trans[State::E33][Token::mul] =  // TransShift(State::E25);
    m_trans[State::E34][Token::mul] = new TransShift(State::E25);

    m_trans[State::E31][Token::col] =  // TransReduce(3, Token::E, false);
    m_trans[State::E32][Token::col] =  // TransReduce(3, Token::E, false);
    m_trans[State::E33][Token::col] =  // TransReduce(3, Token::E, false);
    m_trans[State::E34][Token::col] =  // TransReduce(3, Token::E, false);

    m_trans[State::E33][Token::plu] =  // TransReduce(3, Token::E, false);
    m_trans[State::E34][Token::plu] =  // TransReduce(3, Token::E, false);

    m_trans[State::E33][Token::min] =  // TransReduce(3, Token::E, false);
    m_trans[State::E34][Token::min] =  // TransReduce(3, Token::E, false);

    m_trans[State::E31][Token::clo] =  // TransReduce(3, Token::E, false);
    m_trans[State::E32][Token::clo] =  // TransReduce(3, Token::E, false);
    m_trans[State::E33][Token::clo] =  // TransReduce(3, Token::E, false);
    m_trans[State::E34][Token::clo] = new TransReduce(3, Token::E, false,
                                                      priority);

    m_trans[State::E36][Token::col] = new TransReduce(5, Token::D, true,
                                            new ActionNewSym(true));
    m_trans[State::E36][Token::com] = new TransShift(State::E41);

    m_trans[State::E38][Token::idv] = new TransShift(State::E39);

    m_trans[State::E39][Token::com] = new TransReduce(3, Token::Lv, false);
    m_trans[State::E39][Token::col] = new TransReduce(3, Token::Lv, false,
                                            new ActionNewSym(false));

    m_trans[State::E41][Token::idv] = new TransShift(State::E42);

    m_trans[State::E42][Token::equ] = new TransShift(State::E43);

    m_trans[State::E43][Token::num] = new TransShift(State::E44);

    m_trans[State::E44][Token::com] = new TransReduce(5, Token::Lc, false);
    m_trans[State::E44][Token::col] = new TransReduce(5, Token::Lc, false,
                                            new ActionNewSym(true));

    m_trans[State::E45][Token::num] = new TransShift(State::E28);
}

Automaton::Automaton(const Trans::Transitions & trans) {
    m_trans = trans;
}

// Frees all Transitions and Actions from the tables
/*virtual*/ Automaton::~Automaton() {
    std::set<Trans *> freed_trans;
    for (auto map : m_trans) {
        for (auto it : map.second) {
            if (freed_trans.find(it.second) != freed_trans.end()) {
                delete it.second;
                freed_trans.insert(it.second);
            }
            it.second = nullptr;
        }
    }

    std::set<Action *> freed_actions;
    for (auto map : m_actions) {
        for (auto it : map.second) {
            if (freed_actions.find(it.second) != freed_actions.end()) {
                delete it.second;
                freed_actions.insert(it.second);
            }
            it.second = nullptr;
        }
    }
}

bool Automaton::accepts(Tokenizer *tokenizer, State init) {
    std::stack<State> states;
    std::stack<Token *> tokens;
    states.push(init);

    SymbolsTable values;
    while (!states.empty()) {
        State::Id sId = states.top();
        const Token * currentToken = tokenizer->top();
        const Token::Id tId = *currentToken;

        if (this->error(sId, tId)) {
            return false;
        }
        if (m_trans[sId][tId]->doTransition(m_trans,* currentToken,
                                            &states, &tokens, &values)) {
            return true;
        }
        if (m_trans[sId][tId]->isShift()) {
            tokenizer->shift();
        }
    }

    return false;
}

// Returns true if the state or token given is not in the transitions table
bool Automaton::error(State::Id s, Token::Id t) {
    return ( m_trans.find(s) == m_trans.end()
        || m_trans.find(s)->second.find(t) == m_trans.find(s)->second.end() );
}

bool Automaton::actionExists(State::Id s, Token::Id t) {
    return ( m_actions.find(s) != m_actions.end()
    && m_actions.find(s)->second.find(t) != m_actions.find(s)->second.end() );
}
