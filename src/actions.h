// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ACTIONS_H_
#define SRC_ACTIONS_H_

#include <map>
#include <stack>
#include <vector>

#include "./states.h"
#include "./token.h"
#include "./expr.h"
#include "./instruction.h"

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

class ActionExpr : public Action {
 public:
    explicit ActionExpr(bool optimize = false) : m_optimize(optimize) { }
    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;

 private:
    Expr * optimize(Expr * left, Expr * right,
                    SymbolsTable * variables, uint64_t neutralElement) const;

    bool m_optimize;
};

class ActionInstruction : public Action {
 protected:
    explicit ActionInstruction(std::vector<const Instruction *> * ins)
        : m_instructions(ins) { }

    std::vector<const Instruction *> * m_instructions;
};

class ActionAssign : public ActionInstruction {
 public:
    explicit ActionAssign(std::vector<const Instruction *> * ins)
        : ActionInstruction(ins) { }

    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

class ActionRead : public ActionInstruction {
 public:
    explicit ActionRead(std::vector<const Instruction *> * ins)
        : ActionInstruction(ins) { }

    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};


class ActionWrite : public ActionInstruction {
 public:
    explicit ActionWrite(std::vector<const Instruction *> * ins)
        : ActionInstruction(ins) { }

    virtual Token * doAction(const Token & currentToken,
                             SymbolsTable * variables,
                             std::stack<Token *> * tokens) const;
};

#endif  // SRC_ACTIONS_H_
