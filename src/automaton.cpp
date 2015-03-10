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
    m_trans[State::E0] [Token::con] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::var] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::Li]  =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::idv] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::ecr] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::lir] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::con] =  // ActionShift(State::E2, true);
    m_trans[State::E0] [Token::I]   = new ActionShift(State::E2, true);

    m_trans[State::E2] [Token::Li]  = new ActionShift(State::E3);
    m_trans[State::E2] [Token::I]   = new ActionReduce(0, Token::Li);
    m_trans[State::E2] [Token::idv] = new ActionShift(State::E5);
    m_trans[State::E2] [Token::ecr] = new ActionShift(State::E6);
    m_trans[State::E2] [Token::lir] = new ActionShift(State::E7);
    m_trans[State::E2] [Token::con] = new ActionShift(State::E8);
    m_trans[State::E2] [Token::var] = new ActionShift(State::E9);
    m_trans[State::E2] [Token::D]   = new ActionReduce(1, Token::Ld);

    // E3, E5, E6, E7
    m_trans[State::E3] [Token::I]   = new ActionReduce(1, Token::Li);
    m_trans[State::E3] [Token::idv] = new ActionShift(State::E5);
    m_trans[State::E3] [Token::ecr] = new ActionShift(State::E6);
    m_trans[State::E3] [Token::lir] = new ActionShift(State::E7);

    m_trans[State::E5] [Token::aff] = new ActionShift(State::E12);

    m_trans[State::E6] [Token::E]   = new ActionShift(State::E13);
    m_trans[State::E6] [Token::opp] = new ActionShift(State::E14);
    m_trans[State::E6] [Token::num] = //new ActionReduce(0, Token::E);
    m_trans[State::E6] [Token::idv] = new ActionReduce(0, Token::E);

    m_trans[State::E7] [Token::idv] = new ActionShift(State::E17);

    m_trans[State::E8] [Token::idv] = new ActionShift(State::E19);

    m_trans[State::E9] [Token::idv] = new ActionShift(State::E18);

    // E11, E12, E13, E14, E17
    m_trans[State::E12] [Token::E]  = new ActionShift(State::E20);
    m_trans[State::E12] [Token::opp]= new ActionShift(State::E14);
    m_trans[State::E12] [Token::num]= new ActionShift(State::E15);
    m_trans[State::E12] [Token::idv]= new ActionShift(State::E16);

    m_trans[State::E13] [Token::col]= new ActionShift(State::E21);
    m_trans[State::E13] [Token::plu]= new ActionShift(State::E22);
    m_trans[State::E13] [Token::min]= new ActionShift(State::E23);
    m_trans[State::E13] [Token::div]= new ActionShift(State::E24);
    m_trans[State::E13] [Token::mul]= new ActionShift(State::E25);

    m_trans[State::E14] [Token::E]  = new ActionShift(State::E26);
    m_trans[State::E14] [Token::opp]= new ActionShift(State::E14);
    m_trans[State::E14] [Token::num]= new ActionShift(State::E15);
    m_trans[State::E14] [Token::idv]= new ActionShift(State::E16);

    m_trans[State::E17] [Token::col]= new ActionReduce(2, Token::I);


    // Shift-reduce conflict for Lv
    m_trans[State::E18][Token::Lv]  = new ActionShift(State::E29);
    m_trans[State::E18][Token::com] =  // ActionShift(State::E29, true);
    m_trans[State::E18][Token::col] = new ActionShift(State::E29, true);

    m_trans[State::E19][Token::equ] = new ActionShift(State::E45);

    // E20, E22, E23, E24, E25, E26

    m_trans[State::E28][Token::Lc]  = new ActionShift(State::E36);
    m_trans[State::E28][Token::col] =  // ActionShift(State::E36, true);
    m_trans[State::E28][Token::com] = new ActionShift(State::E36, true);

    m_trans[State::E29][Token::col] = new ActionReduce(3, Token::D);
    m_trans[State::E29][Token::com] = new ActionShift(State::E38);

    // E31, E32, E33, E34

    m_trans[State::E36][Token::col] = new ActionReduce(5, Token::D);
    m_trans[State::E36][Token::com] = new ActionShift(State::E41);

    m_trans[State::E38][Token::idv] = new ActionReduce(2, Token::Lv);

    m_trans[State::E41][Token::idv] = new ActionShift(State::E42);

    m_trans[State::E42][Token::equ] = new ActionShift(State::E43);

    m_trans[State::E43][Token::num] = new ActionReduce(4, Token::Lc);

	m_trans[State::E45][Token::num] = new ActionShift(State::E28);
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

    bool epsilon = true;
    while (tokenizer->has_next()) {
        State s = states.top();
        Token::Id t = tokenizer->top();

        if (this->error(s, t)) {
            return false;
        }
        if (m_trans[s][t]->doTransition(m_trans, &states, &epsilon)) {
            return true;
        }
        if (!epsilon) tokenizer->shift();
    }

    return false;
}

// Returns true if the state or token given is not in the transitions table
bool Automaton::error(State s, Token::Id t) {
    return ( m_trans.find(s) == m_trans.end()
        || m_trans.find(s)->second.find(t) == m_trans.find(s)->second.end() );
}
