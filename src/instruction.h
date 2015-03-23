// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_INSTRUCTION_H_
#define SRC_INSTRUCTION_H_

#include <iostream>

#include "./expr.h"
#include "./symbols.h"

class Instruction {
 public:
    virtual void execute(SymbolsTable * variables) const = 0;
    virtual void analyze(SymbolsTable * variables) const = 0;

    friend std::ostream & operator<<(std::ostream & o, const Instruction & i);

 private:
    virtual std::ostream & print(std::ostream & o) const = 0;
};

class Assignment : public Instruction {
public:
    Assignment(const std::string varName,
               const Expr * rValue);
    
    virtual void execute(SymbolsTable * variables) const;
    virtual void analyze(SymbolsTable * variables) const;

private:
    virtual std::ostream & print(std::ostream & o) const;

    const std::string m_varName;
    const Expr * m_rValue;
};

class Read : public Instruction {
 public:
    explicit Read(const std::string varName,
         std::istream & inStream = std::cin,
         std::ostream & outStream = std::cout);  // NOLINT

    virtual void execute(SymbolsTable * variables) const;
    virtual void analyze(SymbolsTable * variables) const;

private:
    virtual std::ostream & print(std::ostream & o) const;

    std::string m_varName;
    std::istream & m_inStream;
    std::ostream & m_outStream;
};

class Write : public Instruction {
 public:
    explicit Write(const Expr * rValue,
          std::ostream & outStream = std::cout);  // NOLINT

    virtual void execute(SymbolsTable * variables) const;
    virtual void analyze(SymbolsTable * variables) const;

private:
    virtual std::ostream & print(std::ostream & o) const;

    const Expr * m_rValue;
    std::ostream & m_outStream;
};

#endif  // SRC_INSTRUCTION_H_
