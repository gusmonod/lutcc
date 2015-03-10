// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_INSTRUCTION_H_
#define SRC_INSTRUCTION_H_

#include "./token.h"
#include "./expr.h"

class Instruction : public Token {
public:
  explicit Instruction(Token::Id id) : Token(id) {}
  virtual ~Instruction() {}
};

class Read : public Instruction {
public:
  Read(Token::Id id, Variable *_pV) : Instruction(id), pV(_pV) {}
  virtual ~Read() {}
protected:
  Variable *pV;
};

class Write : public Instruction {
public:
  Write(Token::Id id, Variable *_pV) : Instruction(id), pV(_pV) {}
  virtual ~Write() {}
protected:
  Variable *pV;
};

class Assignment : public Instruction {
public:
  Assignment(Token::Id id, Variable *_pV, Expr *_pE) : Instruction(id), left(_pV), right(_pE) {}
  virtual ~Assignment() {}
protected:
  Variable *left;
  Expr *right;
};

#endif //SRC_INSTRUCTION_H_
