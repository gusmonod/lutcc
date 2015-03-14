// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ACTIONS_H_
#define SRC_ACTIONS_H_

#include <map>
#include <stack>

#include "./states.h"
#include "./token.h"
#include "./expr.h"

class Action {
 public:
    typedef std::map<State::Id, std::map<Token::Id, Action *>> Actions;

    virtual ~Action() { }

    virtual void doAction(const Token & t,
                          SymbolsTable * variables) = 0;
};

class ActionNewSym : public Action {
 public:
    explicit ActionNewSym(bool constant) : m_constant(constant) { }

    virtual void doAction(const Token & t,
                          SymbolsTable * variables);

 private:
    bool m_constant;
};

class ActionInit : public Action {
 public:
    virtual void doAction(const Token & t,
                          SymbolsTable * variables);
};

#endif  // SRC_ACTIONS_H_
