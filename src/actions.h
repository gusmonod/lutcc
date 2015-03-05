// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ACTIONS_H_
#define SRC_ACTIONS_H_

#include <map>
#include <stack>

#include "./token.h"
#include "./states.h"

class Action {
 public:
    virtual ~Action() { }

    typedef std::map<State, std::map<Token::Id, Action *>> Transitions;

    static const State initState;  // E0

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states) = 0;
};

class ActionShift : public Action {
 public:
    explicit ActionShift(State target);

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states);

 private:
    const State m_target;
};

class ActionReduce : public Action {
 public:
    ActionReduce(int nbToPop, Token::Id tokenId);

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states);

 private:
    int m_nbToPop;
    Token::Id m_left;
};

class ActionAccept : public Action {
 public:
    ActionAccept();

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states);
};

#endif  // SRC_ACTIONS_H_
