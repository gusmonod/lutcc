// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_EXPR_H_
#define SRC_EXPR_H_

#include "token.h"

#include <string>
#include <map>

class Expr : Token {
  public:
    Expr();
    virtual double eval(const std::map<std::string, double> &values) = 0;

  private:
};

class Variable : Expr {
public:
  Variable();

private:
};

class Number : Expr {
public:
  Number();

private:
};

class BinExpr : Expr {
public:
  BinExpr();

private:
  Expr *left;
  Expr *right;
};

class AddExpr : BinExpr {
public:
  AddExpr();

private:
};

class SubExpr : BinExpr {
public:
  SubExpr();

private:
};

class MultExpr : BinExpr {
public:
  MultExpr();

private:
};

class DivExpr : BinExpr {
public:
  DivExpr();

private:
};

#endif //SRC_EXPR_H_
