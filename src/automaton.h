// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_AUTOMATON_H_
#define SRC_AUTOMATON_H_

#include <stack>
#include <vector>

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

    bool analyze(Tokenizer *tokenizer);

    SymbolsTable & variables() { return m_values; }

    // Returns true if the state or token given is not in the transitions table
    bool error(State::Id s, Token::Id t);

    const std::vector<const Instruction *> & instructions() const
            { return m_instructions; }

 private:
    Trans::Transitions m_trans;

    std::stack<State> m_states;
    std::stack<Token *> m_tokens;
    std::vector<const Instruction *> m_instructions;

    SymbolsTable m_values;
};

#endif  // SRC_AUTOMATON_H_
