// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>

#include "./states.h"

const State Action::initState = State::E0;

ActionShift::ActionShift(State target)
    : m_target(target) { }

/*virtual*/ bool ActionShift::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states) {
    states->push(m_target);

    // Ends the recursion, not accepting
    return false;
}

ActionReduce::ActionReduce(int nbToPop, Token::Id left)
    : m_nbToPop(nbToPop), m_left(left) { }

/*virtual*/ bool ActionReduce::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states) {
    for (int i = 0; i < m_nbToPop; ++i) {
        states->pop();
    }

    // Recursive call
    return transitions.find(states->top())->second.find(m_left)
            ->second->doTransition(transitions, states);
}

ActionAccept::ActionAccept() { }

/*virtual*/ bool ActionAccept::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states) {
    // Ends the recursion, accepting
    return true;
}
