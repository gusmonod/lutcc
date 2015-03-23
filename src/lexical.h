// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_LEXICAL_H_
#define SRC_LEXICAL_H_

#include <iostream>
#include <string>

#include "boost/regex.hpp"

#include "./token.h"

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
    // If shift is set, it will automatically shift once upon construction
    explicit Tokenizer(std::istream * inStream, bool shift = true);

    // Returns true if there is something else to read with next(&type)
    bool has_next() const;

    int column() const { return m_column; }

    int line() const { return m_line; }

    std::string topStr() const { return m_currentTokenStr; }

    // Returns the next token **without** moving to the next token
    // !!!!!!! CAUTION: The Token will be freed on shift
    const Token * top();

    // Moves to the next token
    // !!!!!!! CAUTION: The previous token is deleted
    void shift();

    static const Token END_OF_FILE;

 private:
    std::istream & m_inputStream;
    std::string    m_buffer;

    Token        * m_currentToken;
    std::string    m_currentTokenStr;

    bool           m_shifted;

    int            m_line;
    int            m_column;

#ifdef DEBUG
    bool           m_logNewLine;
#endif

    static const int BUFFER_SIZE;

    // Keywords regular expressions
    static const boost::regex keyword;

    // Operator regular expressions
    static const boost::regex affect;
    static const boost::regex operatorr;

    // Operands regular expressions
    static const boost::regex id;
    static const boost::regex number;

    static const boost::regex whitespace;

    void trim_left();

    void analyze();
};

#endif  // SRC_LEXICAL_H_
