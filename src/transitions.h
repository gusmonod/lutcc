// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_TRANSITIONS_H_
#define SRC_TRANSITIONS_H_

#include <map>
#include <stack>

#include "./token.h"
#include "./expr.h"
#include "./states.h"

class Trans {
 public:
    typedef std::map<State::Id, std::map<Token::Id, Trans *>> Transitions;

    virtual ~Trans() { }

    static const State initState;  // E0

    virtual bool doTransition(const Trans::Transitions & transitions,
                              std::stack<State> * states) = 0;

    virtual bool isShift() const = 0;
};

class TransShift : public Trans {
 public:
    explicit TransShift(State target, bool terminal = true);

    virtual bool doTransition(const Trans::Transitions & transitions,
                              std::stack<State> * states);

    virtual bool isShift() const { return m_terminal; }

 private:
    const State m_target;
    bool m_terminal;
};

class TransReduce : public Trans {
 public:
    TransReduce(int nbToPop, Token::Id tokenId, bool terminal = true);

    virtual bool doTransition(const Trans::Transitions & transitions,
                              std::stack<State> * states);

    virtual bool isShift() const { return m_terminal; }

 protected:
    int m_nbToPop;
    Token::Id m_left;
    bool m_terminal;
};

class TransAccept : public Trans {
 public:
    TransAccept();

    virtual bool doTransition(const Trans::Transitions & transitions,
                              std::stack<State> * states);

    virtual bool isShift() const { return false; }
};

#endif  // SRC_TRANSITIONS_H_
