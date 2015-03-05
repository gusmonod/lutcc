// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./automaton.h"

#include <stack>

#include "./actions.h"
#include "./states.h"
#include "./token.h"

Automaton::Automaton() : m_trans() {
    m_trans[State::E0] [Token::P]   = new ActionAccept();
    // Shift-reduce conflict for Ld:
    m_trans[State::E0] [Token::Ld]  = new ActionShift(State::E2);
    m_trans[State::E0] [Token::D]   = new ActionReduce(2, Token::P);

    // m_trans[State::E2] [Token::Li]
    // m_trans[State::E2] [Token::I]
    // m_trans[State::E2] [Token::idv]
    // m_trans[State::E2] [Token::ecr]
    // m_trans[State::E2] [Token::lir]
    m_trans[State::E2] [Token::con] = new ActionShift(State::E8);
    m_trans[State::E2] [Token::var] = new ActionShift(State::E9);
    m_trans[State::E2] [Token::D]   = new ActionReduce(2, Token::P);

    // E3, E5, E6, E7

    m_trans[State::E8] [Token::idv] = new ActionShift(State::E19);

    m_trans[State::E9] [Token::idv] = new ActionShift(State::E18);

    // E11, E12, E13, E14, E17

    // Shift-reduce conflict for Lv
    m_trans[State::E18][Token::Lv]  = new ActionShift(State::E29);

    m_trans[State::E19][Token::equ] = new ActionShift(State::E28);

    // E20, E22, E23, E24, E25, E26

    m_trans[State::E28][Token::Lc]  = new ActionShift(State::E36);

    m_trans[State::E29][Token::col] = new ActionReduce(3, Token::D);
    m_trans[State::E29][Token::com] = new ActionShift(State::E38);

    // E31, E32, E33, E34

    m_trans[State::E36][Token::col] = new ActionReduce(4, Token::D);
    m_trans[State::E36][Token::com] = new ActionShift(State::E41);

    m_trans[State::E38][Token::idv] = new ActionReduce(2, Token::Lv);

    m_trans[State::E41][Token::idv] = new ActionShift(State::E42);

    m_trans[State::E42][Token::equ] = new ActionShift(State::E43);

    m_trans[State::E43][Token::num] = new ActionReduce(4, Token::num);
}

Automaton::Automaton(const Action::Transitions & trans) {
    m_trans = trans;
}

// Frees all Actions from the Transitions table
/*virtual*/ Automaton::~Automaton() {
    for (auto map : m_trans) {
        for (auto it : map.second) {
            delete it.second;
        }
    }
}

bool Automaton::accepts(Tokenizer *tokenizer, State init) {
    std::stack<State> states;
    states.push(init);

    while (tokenizer->has_next()) {
        if (this->error(states.top(), tokenizer->top())) {
            return false;
        }
        if (m_trans[states.top()][tokenizer->top()]
                ->doTransition(m_trans, &states)) {
            return true;
        }
        tokenizer->shift();
    }

    return false;
}

// Returns true if the state or token given is not in the transitions table
bool Automaton::error(State s, Token::Id t) {
    return ( m_trans.find(s) == m_trans.end()
        || m_trans.find(s)->second.find(t) == m_trans.find(s)->second.end() );
}
