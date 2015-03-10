// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./actions.h"

#include <stack>

#include "./states.h"

const State Action::initState = State::E0;

ActionShift::ActionShift(State target, bool epsilon)
    : m_target(target), m_epsilon(epsilon) { }

/*virtual*/ bool ActionShift::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states, bool * epsilon) {
    states->push(m_target);
    *epsilon = m_epsilon;

    // Ends the recursion, continuing the analysis loop
    return false;
}

ActionReduce::ActionReduce(int nbToPop, Token::Id left)
    : m_nbToPop(nbToPop), m_left(left) { }

/*virtual*/ bool ActionReduce::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states, bool * epsilon) {
    for (int i = 0; i < m_nbToPop; ++i) {
        states->pop();
    }
    *epsilon = false;

    // Recursive call
    return transitions.find(states->top())->second.find(m_left)
            ->second->doTransition(transitions, states, epsilon);
}

ActionReduceShift::ActionReduceShift(int nbToPop, Token::Id left,
       Token::Id right)
    : ActionReduce(nbToPop, left), m_right(right) { }

/*virtual*/ bool ActionReduceShift::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states, bool * epsilon) {
    ActionReduce::doTransition(transitions, states, epsilon);
    return transitions.find(states->top())->second.find(m_right)
            ->second->doTransition(transitions, states, epsilon);
}

ActionAccept::ActionAccept() { }

/*virtual*/ bool ActionAccept::doTransition(
        const Action::Transitions & transitions,
        std::stack<State> * states, bool * epsilon) {
    *epsilon = false;

    // Ends the recursion, accepting
    return true;
}


