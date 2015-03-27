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

    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;

 private:
    const bool m_constant;
};

class ActionParenthesisExpr : public Action {
 public:
    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;
};

class ActionSimpleExpr : public Action {
 public:
    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;
};

class ActionExpr : public Action {
 public:
    explicit ActionExpr(bool optimize = false) : m_optimize(optimize) { }
    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;

    Expr * optimize(BinExpr * toOptimize, SymbolsTable * variables,
                    int64_t neutralElement) const;

 private:
    bool m_optimize;
};

class ActionInstruction : public Action {
 protected:
    explicit ActionInstruction(std::vector<Instruction *> * ins)
        : m_instructions(ins) { }

    std::vector<Instruction *> * m_instructions;
};

class ActionAssign : public ActionInstruction {
 public:
    explicit ActionAssign(std::vector<Instruction *> * ins)
        : ActionInstruction(ins) { }

    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;
};

class ActionRead : public ActionInstruction {
 public:
    explicit ActionRead(std::vector<Instruction *> * ins)
        : ActionInstruction(ins) { }

    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;
};

class ActionWrite : public ActionInstruction {
 public:
    explicit ActionWrite(std::vector<Instruction *> * ins)
        : ActionInstruction(ins) { }

    Token * doAction(const Token & currentToken,
                     SymbolsTable * variables,
                     std::stack<Token *> * tokens) const override;
};

#endif  // SRC_ACTIONS_H_
