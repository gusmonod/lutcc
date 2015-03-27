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
    explicit Automaton(bool optimize = false);

    explicit Automaton(const Trans::Transitions & trans);

    // Frees all Actions from the Transitions table
    ~Automaton();

    bool analyze(const Token & toAnalyze, const Tokenizer & tokenizer);

    bool isAccepted() const { return m_states.empty(); }

    SymbolsTable * variables() { return &m_values; }

    std::vector<Instruction *> * instructions() { return &m_instructions; }

    std::vector<Token::Id> expected(const Token & currentToken) const;

    // Returns true if the state or token given is not in the transitions table
    bool error(State::Id s, Token::Id t);

 private:
    Trans::Transitions m_trans;

    std::stack<State::Id> m_states;
    std::stack<Token *> m_tokens;
    std::vector<Instruction *> m_instructions;

    SymbolsTable m_values;
};

#endif  // SRC_AUTOMATON_H_
