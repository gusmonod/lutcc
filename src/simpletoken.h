// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_SIMPLETOKEN_H_
#define SRC_SIMPLETOKEN_H_

#include <iostream>

class Token {
 public:
    explicit Token(int id) : m_id(id) { }
    operator int() const { return m_id; }

    friend std::ostream& operator<<(std::ostream& stream, const Token & token);

 protected:
    int m_id;
};

class SimpleOperator : Token {
 public:
    explicit SimpleOperator(int id) : Token(id) { }

 private:
};

class Keyword : Token {
 public:
    explicit Keyword(int id) : Token(id) { }

 private:
};

#endif  // SRC_SIMPLETOKEN_H_
