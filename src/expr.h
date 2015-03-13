// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_EXPR_H_
#define SRC_EXPR_H_

#include <string>
#include <map>

#include "./token.h"

class Expr : public Token {
 public:
    explicit Expr(Token::Id id) : Token(id) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values) = 0;

 private:
};

class BinExpr : public Expr {
public:
    explicit BinExpr(Token::Id id) : Expr(id) { }

protected:
    Expr *m_left;
    Expr *m_right;
};

class Variable : public Expr {
 public:
    Variable(Token::Id id, std::string name) : Expr(id), m_name(name) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
    std::string m_name;
};

class Number : public Expr {
 public:
    Number(Token::Id id, uint64_t value) : Expr(id), m_value(value) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
    uint64_t m_value;
};

class AddExpr : public BinExpr {
 public:
    explicit AddExpr(Token::Id id) : BinExpr(id) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
};

class SubExpr : public BinExpr {
 public:
    explicit SubExpr(Token::Id id) : BinExpr(id) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
};

class MulExpr : public BinExpr {
 public:
    explicit MulExpr(Token::Id id) : BinExpr(id) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
};

class DivExpr : public BinExpr {
 public:
    explicit DivExpr(Token::Id id) : BinExpr(id) { }
    virtual uint64_t eval(const std::map<std::string, uint64_t> & values);

 private:
};

#endif  // SRC_EXPR_H_
