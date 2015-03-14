// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./transitions.h"

#include <stack>

#include "./states.h"

const State Trans::initState = State::E0;

TransShift::TransShift(State target, bool terminal /*= true*/)
    : m_target(target), m_terminal(terminal) { }

/*virtual*/ bool TransShift::doTransition(
                            const Trans::Transitions & transitions,
                            std::stack<State> * states) {
    states->push(m_target);

    // Ends the recursion, continuing the analysis loop
    return false;
}

TransReduce::TransReduce(int nbToPop, Token::Id left, bool terminal/*false*/)
    : m_nbToPop(nbToPop), m_left(left), m_terminal(terminal) { }

/*virtual*/ bool TransReduce::doTransition(
                            const Trans::Transitions & transitions,
                            std::stack<State> * states) {
    for (int i = 0; i < m_nbToPop; ++i) {
        states->pop();
    }

    // Recursive call
    return transitions.find(states->top())->second.find(m_left)
            ->second->doTransition(transitions, states);
}

TransAccept::TransAccept() { }

/*virtual*/ bool TransAccept::doTransition(
                            const Trans::Transitions & transitions,
                            std::stack<State> * states) {
    // Ends the recursion, accepting
    return true;
}
