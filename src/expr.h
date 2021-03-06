// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_EXPR_H_
#define SRC_EXPR_H_

#include <string>
#include <map>

#include "./token.h"
#include "./symbols.h"

class Expr : public Token {
 public:
    explicit Expr(Token::Id id, bool inBrackets = false)
            : Token(id), m_inBrackets(inBrackets) { }
    virtual Token * newCopy() const = 0;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const = 0;

    bool isInBrackets() const { return m_inBrackets; }
    void inBrackets(bool inBrackets) { m_inBrackets = inBrackets; }

 protected:
    bool m_inBrackets;
};

class Variable : public Expr {
 public:
    Variable(std::string name, bool inBrackets)
        : Expr(Token::idv, inBrackets), m_name(name) { }
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;

    std::string name() const { return m_name; }

 private:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

    std::string m_name;
};

class Number : public Expr {
 public:
    Number(int64_t value, bool inBrackets)
        : Expr(Token::num, inBrackets), m_value(value) { }
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;

    int64_t value() const { return m_value; }

 private:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

    int64_t m_value;
};

class BinExpr : public Expr {
 public:
    explicit BinExpr(Token::Id id,
                     Expr * left = nullptr,
                     Expr * right = nullptr,
                     bool inBrackets = false);
    ~BinExpr() { delete m_left; delete m_right; }
    virtual Token * newCopy() const = 0;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const = 0;

    void left(Expr * left, bool shouldDelete = true);
    void right(Expr * right, bool shouldDelete = true);
    Expr * left() const { return m_left; }
    Expr * right() const { return m_right; }

 protected:
    Expr * m_left;
    Expr * m_right;

 private:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT
};

class AddExpr : public BinExpr {
 public:
    explicit AddExpr(Expr * left = nullptr,
                     Expr * right = nullptr,
                     bool inBrackets = false);
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;
};

class SubExpr : public BinExpr {
 public:
    explicit SubExpr(Expr * left = nullptr,
                     Expr * right = nullptr,
                     bool inBrackets = false);
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;
};

class MulExpr : public BinExpr {
 public:
    explicit MulExpr(Expr * left = nullptr,
                     Expr * right = nullptr,
                     bool inBrackets = false);
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;
};

class DivExpr : public BinExpr {
 public:
    explicit DivExpr(Expr * left = nullptr,
                     Expr * right = nullptr,
                     bool inBrackets = false);
    virtual Token * newCopy() const;
    virtual int64_t eval(SymbolsTable * values, bool used = false) const;
};

#endif  // SRC_EXPR_H_
