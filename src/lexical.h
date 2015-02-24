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

void format_whitespace(std::istream & input, std::string * out);

// Tokenizes a Lutin program string. Note: the string **must** be ws-formatted
// Sample usage:
//    std::string lutin_program;
//    format_whitespace(std::cin, &lutin_program);
//    TokenType type;
//    while (t.has_next()) {
//        cout << t.top(&type) << ' ' << type;
//        t.shift();
//    }
class Tokenizer {
 public:
    // Allocates a buffer holding the program in memory
    // Note: the program **must be** ws_formatted
    explicit Tokenizer(const std::string & inString);

    // Gets a reference to a stream for streaming evaluation
//    explicit Tokenizer(std::istream & inStream);

    // Frees the buffer
    ~Tokenizer();

    // Returns true if there is something else to read with next(&type)
    bool has_next() const;

    // Returns the next token **without** moving to the next token
    std::string top(TokenType *type);

    // Moves to the next token
    void shift();

 private:
    const char *m_line;
    const char *m_pos_in_line;

//    std::istream m_input;
    std::string m_current_token;

    static const int BUFFER_SIZE;

    // Keywords regular expressions
    static const boost::regex keyword;

    // Operator regular expressions
    static const boost::regex affect;
    static const boost::regex operatorr;

    // Operands regular expressions
    static const boost::regex id;
    static const boost::regex number;
};

#endif  // SRC_LEXICAL_H_
