// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_AUTOMATON_H_
#define SRC_AUTOMATON_H_

#include "./lexical.h"
#include "./transitions.h"
#include "./actions.h"
#include "./states.h"
#include "./token.h"

class Automaton {
 public:
    // Empty constructor => uses the default transitions table
    Automaton();

    explicit Automaton(const Trans::Transitions & trans);

    // Frees all Actions from the Transitions table
    virtual ~Automaton();

    bool accepts(Tokenizer *tokenizer, State init = Trans::initState);

    // Returns true if the state or token given is not in the transitions table
    bool error(State::Id s, Token::Id t);

    bool actionExists(State::Id s, Token::Id t);

 private:
    Trans::Transitions m_trans;
    Action::Actions m_actions;
};

#endif  // SRC_AUTOMATON_H_
