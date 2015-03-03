// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_LEXICAL_H_
#define SRC_LEXICAL_H_

#include <iostream>
#include <string>
#include <cstring>

#include "boost/regex.hpp"

enum TokenType {
    ERROR,
    KEYWORD,
    OPERATOR,
    ID,
    NUMBER
};

// Tokenizes a Lutin program string. Note: the string **must** be ws_formatted
// Sample usage:
//    std::string lutin_program;
//    Tokenizer::ws_format(std::cin, &lutin_program);
//    TokenType type;
//    while (t.has_next()) {
//        cout << t.top(&type) << ' ' << type;
//        t.shift();
//    }
class Tokenizer {
 public:
    // Sets the input source to be a stream
    explicit Tokenizer(std::istream & inStream);

    // Returns true if there is something else to read with next(&type)
    bool has_next() const;

    // Returns the next token **without** moving to the next token
    std::string top();

    // Moves to the next token
    void shift();

 private:
    std::istream & m_inputStream;
    std::string    m_buffer;

    std::string    m_currentType;
    std::string    m_currentToken;

    static const size_t BUFFER_SIZE;

    // Keywords regular expressions
    static const boost::regex keyword;

    // Operator regular expressions
    static const boost::regex affect;
    static const boost::regex operatorr;

    // Operands regular expressions
    static const boost::regex id;
    static const boost::regex number;

    void init();
};

#endif  // SRC_LEXICAL_H_
