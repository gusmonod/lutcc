// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./lexical.h"

#include <iostream>
#include <string>

#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"

using std::string;
using std::cout;
using std::endl;

const size_t Tokenizer::BUFFER_SIZE = 10;

// Keywords regular expressions
const boost::regex Tokenizer::keyword("^(const |var |ecrire |lire ).*");

// Operator regular expressions
const boost::regex Tokenizer::affect("^(:=).*");
const boost::regex Tokenizer::operatorr("^(\\+|-|\\*|/|;|=).*");

// Operands regular expressions
const boost::regex Tokenizer::id("^([a-zA-Z][a-zA-Z0-9_-]*).*");
const boost::regex Tokenizer::number("^([0-9]+).*");

Tokenizer::Tokenizer(std::istream * inStream, bool shift /* = true */)
    : m_inputStream(*inStream), m_shifted(shift) {
    if (shift) this->shift();
}

bool Tokenizer::has_next() const {
    return m_inputStream.good() || m_buffer != "";
}

string Tokenizer::top() {
    // If there was a shift, the current type and token must be analyzed again
    if (m_shifted) this->analyze();

    return m_currentType + ": " + m_currentToken;
}

void Tokenizer::shift() {
    if (!this->has_next()) return;
    m_shifted = true;

    using boost::regex_replace;
    using boost::regex_constants::format_literal;

    int nbToAdd;
    char tmp[Tokenizer::BUFFER_SIZE + 1];

    // Consumes the current token from the buffer
    m_buffer = m_buffer.substr(m_currentToken.size());

    // If there are spaces after the token, removes them from the buffer
    boost::algorithm::trim_left(m_buffer);

    // Increases the buffer size until it hits Tokenizer::BUFFER_SIZE or EOF
    while (m_buffer.size() < Tokenizer::BUFFER_SIZE) {
        nbToAdd = static_cast<int>(Tokenizer::BUFFER_SIZE - m_buffer.size());
        m_inputStream.getline(tmp, nbToAdd + 1);
        if (m_inputStream.eof() && !m_inputStream.good()) break;
        m_inputStream.clear();

        // Replaces whitespaces with a single ' '
        m_buffer += regex_replace(string(tmp), boost::regex("[\\r\\n\\t ]+"),
                " ", format_literal);
    }
}


void Tokenizer::analyze() {
    boost::cmatch matches;
    if (regex_match(m_buffer.c_str(), matches, Tokenizer::keyword)) {
        m_currentType = "keyword";
        m_currentToken = matches[1];
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::affect)) {
        m_currentType = "affect";
        m_currentToken = matches[1];
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::operatorr)) {
        m_currentType = "operator";
        m_currentToken = matches[1];
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::id)) {
        m_currentType = "id";
        m_currentToken = matches[1];
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::number)) {
        m_currentType = "number";
        m_currentToken = matches[1];
    } else {
        m_currentType = "no match";
        m_currentToken = m_buffer[0];
    }
}
