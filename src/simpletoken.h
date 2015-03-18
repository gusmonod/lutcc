// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_SIMPLETOKEN_H_
#define SRC_SIMPLETOKEN_H_

#include <iostream>

#include "./token.h"

class SimpleOperator : public Token {
 public:
    explicit SimpleOperator(Token::Id id) : Token(id) { }

    virtual Token * newCopy() const;
 private:
};

class Keyword : public Token {
 public:
    explicit Keyword(Token::Id id) : Token(id) { }

    virtual Token * newCopy() const;
 private:
};

#endif  // SRC_SIMPLETOKEN_H_
