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
            std::stack<State> * states, bool * epsilon) = 0;
};

class ActionShift : public Action {
 public:
    explicit ActionShift(State target, bool epsilon = false);

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states, bool * epsilon);

 private:
    const State m_target;
    bool m_epsilon;
};

class ActionReduce : public Action {
 public:
    ActionReduce(int nbToPop, Token::Id tokenId);

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states, bool * epsilon);

 protected:
    int m_nbToPop;
    Token::Id m_left;
};

class ActionReduceShift : public ActionReduce {
 public:
    ActionReduceShift(int nbToPop, Token::Id tokenId, Token::Id right);
    
    virtual bool doTransition(const Action::Transitions & transitions,
                              std::stack<State> * states, bool * epsilon);
 private:
    Token::Id m_right;
};

class ActionAccept : public Action {
 public:
    ActionAccept();

    virtual bool doTransition(const Action::Transitions & transitions,
            std::stack<State> * states, bool * epsilon);
};

#endif  // SRC_ACTIONS_H_
