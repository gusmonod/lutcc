// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_TRANSITIONS_H_
#define SRC_TRANSITIONS_H_

#include <map>
#include <stack>

#include "./token.h"
#include "./expr.h"
#include "./states.h"
#include "./actions.h"

class Trans {
 public:
    typedef std::map<State::Id, std::map<Token::Id, Trans *>> Transitions;

    virtual ~Trans() { }

    static const State::Id initState;  // E0

    virtual bool doTransition(const Trans::Transitions & transitions,
                              const Token & currentToken,
                              std::stack<State::Id> * states,
                              std::stack<Token *> * tokens,
                              SymbolsTable * variables) const = 0;

    virtual bool isShift() const = 0;

    virtual State::Id target(const Trans::Transitions & transitions,
                             std::stack<State::Id> states) const = 0;
};

class TransShift : public Trans {
 public:
    explicit TransShift(State::Id target, bool terminal = true);

    bool doTransition(const Trans::Transitions & transitions,
                      const Token & currentToken,
                      std::stack<State::Id> * states,
                      std::stack<Token *> * tokens,
                      SymbolsTable * variables) const override;

    bool isShift() const override { return m_terminal; }

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    State::Id target(const Trans::Transitions & transitions,
                     std::stack<State::Id> states) const override
            { return m_target; }

 private:
    const State::Id m_target;
    bool m_terminal;
};

class TransAccept : public Trans {
 public:
    TransAccept();

    bool doTransition(const Trans::Transitions & transitions,
                      const Token & currentToken,
                      std::stack<State::Id> * states,
                      std::stack<Token *> * tokens,
                      SymbolsTable * variables) const override;

    bool isShift() const override { return false; }

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    State::Id target(const Trans::Transitions & transitions,
                     std::stack<State::Id> states) const override
            { return State::E3; }
};

class TransReduce : public Trans {
 public:
    TransReduce(int nbToPop, Token::Id tokenId,
                bool terminal = true, Action * reduceAction = nullptr);

    ~TransReduce() override;

    bool doTransition(const Trans::Transitions & transitions,
                      const Token & currentToken,
                      std::stack<State::Id> * states,
                      std::stack<Token *> * tokens,
                      SymbolsTable * variables) const override;

    bool isShift() const override { return m_terminal; }

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    State::Id target(const Trans::Transitions & transitions,
                     std::stack<State::Id> states) const override;

 private:
    int m_nbToPop;
    Token::Id m_left;
    bool m_terminal;
    const Action * m_reduceAction;
};

#endif  // SRC_TRANSITIONS_H_
