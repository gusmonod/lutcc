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

class Variable : public Expr {
 public:
    explicit Variable(Token::Id id) : Expr(id) { }

 private:
};

class Number : public Expr {
 public:
    explicit Number(Token::Id id) : Expr(id) { }

 private:
};

class BinExpr : public Expr {
 public:
    explicit BinExpr(Token::Id id) : Expr(id) { }

 private:
    Expr *left;
    Expr *right;
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
