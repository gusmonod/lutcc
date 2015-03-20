// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_EXPR_H_
#define SRC_EXPR_H_

#include <string>
#include <map>

#include "./token.h"
#include "./symbols.h"

class Variable;

class Expr : public Token {
 public:
    explicit Expr(Token::Id id) : Token(id) { }
    virtual Token * newCopy() const = 0;
    virtual uint64_t eval(const SymbolsTable & values) const = 0;
};

class Variable : public Expr {
 public:
    Variable(Token::Id id, std::string name) : Expr(id), m_name(name) { }
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;

    std::string name() const { return m_name; }

 protected:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

 private:
    std::string m_name;
};

class Number : public Expr {
 public:
    Number(Token::Id id, uint64_t value) : Expr(id), m_value(value) { }
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;

    uint64_t value() const { return m_value; }

 protected:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

 private:
    uint64_t m_value;
};

class BinExpr : public Expr {
 public:
    explicit BinExpr(Token::Id id,
                     Expr * left = nullptr,
                     Expr * right = nullptr);
    ~BinExpr() { delete m_left; delete m_right; }
    virtual Token * newCopy() const = 0;
    virtual uint64_t eval(const SymbolsTable & values) const = 0;

    void left(Expr * left, bool shouldDelete = true);
    void right(Expr * right, bool shouldDelete = true);
    Expr * left() const { return m_left; }
    Expr * right() const { return m_right; }

 protected:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

    Expr * m_left;
    Expr * m_right;
};

class AddExpr : public BinExpr {
 public:
    explicit AddExpr(Expr * left = nullptr,
                     Expr * right = nullptr);
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;
};

class SubExpr : public BinExpr {
 public:
    explicit SubExpr(Expr * left = nullptr,
                     Expr * right = nullptr);
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;
};

class MulExpr : public BinExpr {
 public:
    explicit MulExpr(Expr * left = nullptr,
                     Expr * right = nullptr);
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;
};

class DivExpr : public BinExpr {
 public:
    explicit DivExpr(Expr * left = nullptr,
                     Expr * right = nullptr);
    virtual Token * newCopy() const;
    virtual uint64_t eval(const SymbolsTable & values) const;
};

#endif  // SRC_EXPR_H_
