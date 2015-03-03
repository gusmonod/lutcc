// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./lexical.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

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

Tokenizer::Tokenizer(std::istream & inStream)
    : m_inputStream(inStream) {
    this->shift();
}

void Tokenizer::init() {
    /*
    boost::cmatch matches;
    while (!m_inputStream.eof()) {
        if (regex_match(m_buffer.c_str(), matches, Tokenizer::keyword)
            || regex_match(m_buffer.c_str(), matches, Tokenizer::affect)
            || regex_match(m_buffer.c_str(), matches, Tokenizer::operatorr)
            || regex_match(m_buffer.c_str(), matches, Tokenizer::id)
            || regex_match(m_buffer.c_str(), matches, Tokenizer::number)) {

            string match = matches[1];
            int matchSize = match.size();
            m_buffer = m_buffer.substr(matchSize);
            int max = Tokenizer::BUFFER_SIZE - m_buffer.size();

            for (int i = 0; !m_inputStream.eof() && i < max; ++i) {
                m_buffer += m_inputStream.get();
                m_inputStream >> std::ws;
            }
        } else {
            m_inputStream >> std::setw(Tokenizer::BUFFER_SIZE) >> m_buffer;
        }

        cout << "m_buffer: " << m_buffer << endl;
        m_inputStream >> std::ws;
    }
    */
}

bool Tokenizer::has_next() const {
    return m_inputStream.good() || m_buffer != "";
}

string Tokenizer::top() {
    boost::cmatch matches;
    if (regex_match(m_buffer.c_str(), matches, Tokenizer::keyword)) {
        m_currentType = "keyword";
        m_currentToken = matches[1];
        m_currentToken = m_currentToken.substr(0, m_currentToken.size() - 1);
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

    return m_currentType + ": " + m_currentToken;
/*
    using boost::regex_match;

    // Initializing type to ERROR
    *type = ERROR;

    // Buffer containing the possible matches
    boost::cmatch matches;

    if (regex_match(m_pos_in_line, matches, Tokenizer::keyword)) {
        *type = KEYWORD;
    } else if (regex_match(m_pos_in_line, matches, Tokenizer::affect)
        || regex_match(m_pos_in_line, matches, Tokenizer::operatorr)) {
        *type = OPERATOR;
    } else if (regex_match(m_pos_in_line, matches, Tokenizer::id)) {
        *type = ID;
    } else if (regex_match(m_pos_in_line, matches, Tokenizer::number)) {
        *type = NUMBER;
    }

    if (ERROR != *type) {
        m_currentToken = matches[1];
    } else {
        // No match -> error or EOL, extracting pb char
        m_currentToken = *m_pos_in_line;
    }
*/
}

void Tokenizer::shift() {
    if (!this->has_next()) return;

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

        // Replaces whitespaces with a single ' ', and trims
        m_buffer += regex_replace(string(tmp), boost::regex("[\\r\\n\\t ]+"),
                " ", format_literal);
    }
}
