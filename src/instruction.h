// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_INSTRUCTION_H_
#define SRC_INSTRUCTION_H_

#include "./token.h"

class Instruction : public Token {
public:
  //Instruction() {}
  virtual ~Instruction() {}
};

class Read : public Instruction {
public:
  //Read() {}
  virtual ~Read() {}
};

class Write : public Instruction {
public:
  //Write() {}
  virtual ~Write() {}
};

class Assignment : public Instruction {
public:
  //Assignment() {}
  virtual ~Assignment() {}
};

#endif //SRC_INSTRUCTION_H_
