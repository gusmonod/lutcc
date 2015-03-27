// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./transitions.h"

#include <stack>
#include <cstdlib>

#include "./states.h"
#include "./actions.h"

const State::Id Trans::initState = State::E0;

TransShift::TransShift(State::Id target, bool terminal /*= true*/)
    : m_target(target), m_terminal(terminal) { }

/*override*/ bool TransShift::doTransition(
                            const Trans::Transitions & transitions,
                            const Token & currentToken,
                            std::stack<State::Id> * states,
                            std::stack<Token *> * tokens,
                            SymbolsTable * variables) const {
    states->push(m_target);
    tokens->push(currentToken.newCopy());

    // Ends the recursion, continuing the analysis loop
    return false;
}

TransAccept::TransAccept() { }

/*override*/ bool TransAccept::doTransition(
                            const Trans::Transitions & transitions,
                            const Token & currentToken,
                            std::stack<State::Id> * states,
                            std::stack<Token *> * tokens,
                            SymbolsTable * variables) const {
    // Ends the recursion, accepting
    return true;
}

TransReduce::TransReduce(int nbToPop, Token::Id left,
                         bool terminal, Action * reduceAction)
    : m_nbToPop(nbToPop), m_left(left), m_terminal(terminal),
      m_reduceAction(reduceAction) { }

/*override*/ TransReduce::~TransReduce() {
    delete m_reduceAction;
    m_reduceAction = nullptr;
}

/*override*/ bool TransReduce::doTransition(
                            const Trans::Transitions & transitions,
                            const Token & currentToken,
                            std::stack<State::Id> * states,
                            std::stack<Token *> * tokens,
                            SymbolsTable * variables) const {
    for (int i = 0; i < m_nbToPop; ++i) {
        states->pop();
    }

    Token * nextToken = nullptr;
    if (m_reduceAction) {
        nextToken = m_reduceAction->doAction(currentToken, variables, tokens);
    } else {
        for (int i = 0; i < m_nbToPop; ++i) {
            delete tokens->top();
            tokens->pop();
        }
    }

    nextToken = nextToken ? nextToken : new Token(m_left);

    // Recursive call
    bool r = transitions.find(states->top())->second.find(m_left)
            ->second->doTransition(transitions, *nextToken, states,
                                   tokens, variables);

    delete nextToken;
    nextToken = nullptr;

    return r;
}

/*override*/ State::Id TransReduce::target(
                            const Trans::Transitions & transitions,
                            std::stack<State::Id> states) const {
    for (int i = 0; i < m_nbToPop; ++i) {
        states.pop();
    }

    return transitions.find(states.top())->second.find(m_left)
            ->second->target(transitions, states);
}
