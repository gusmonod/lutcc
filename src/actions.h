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

    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr) = 0;
};

class ActionNewSym : public Action {
 public:
    explicit ActionNewSym(bool constant) : m_constant(constant) { }

    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);

 private:
    bool m_constant;
};

class ActionInit : public Action {
 public:
    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);
};

class ActionLeftAssoc : public Action {
 public:
    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);
};

class ActionPriority : public Action {
 public:
    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);
};

class ActionRead : public Action {
public:
    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);
};


class ActionWrite : public Action {
public:
    virtual Expr * doAction(const Token & readToken,
                            SymbolsTable * variables,
                            Expr * currentExpr);
};

#endif  // SRC_ACTIONS_H_
