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

    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const = 0;
};

class ActionNewSym : public Action {
 public:
    explicit ActionNewSym(bool constant) : m_constant(constant) { }

    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;

 private:
    const bool m_constant;
};

class ActionParenthesisExpr : public Action {
 public:
     virtual Token * doAction(const Token & currentToken,
                              SymbolsTable * variables,
                              std::stack<Token *> * tokens) const;
};

class ActionSimpleExpr : public Action {
 public:
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

class ActionAddExpr : public Action {
 public:
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

class ActionAssign : public Action {
 public:
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

class ActionRead : public Action {
 public:
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};


class ActionWrite : public Action {
 public:
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

#endif  // SRC_ACTIONS_H_
