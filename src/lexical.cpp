// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./lexical.h"

#include <iostream>
#include <string>

#include "boost/regex.hpp"

#include "./simpletoken.h"
#include "./expr.h"
#include "./errors.h"

using std::string;

const int Tokenizer::BUFFER_SIZE = 20;
const Token Tokenizer::END_OF_FILE(Token::END);

// Keywords regular expressions
const boost::regex Tokenizer::keyword("^(const |var |ecrire |lire ).*");

// Operator regular expressions
const boost::regex Tokenizer::affect("^(:=).*");
const boost::regex Tokenizer::operatorr("^(\\+|-|\\*|/|\\(|,|\\)|;|=).*");

// Operands regular expressions
const boost::regex Tokenizer::id("^([a-zA-Z][a-zA-Z0-9_]*).*");
const boost::regex Tokenizer::number("^([0-9]+).*");
const boost::regex Tokenizer::whitespace("^[\\r\\n\\t ]+.*");

Tokenizer::Tokenizer(std::istream * inStream, bool shift /* = true */)
    : m_inputStream(*inStream), m_currentToken(nullptr), m_shifted(shift) {
    m_line = 1;
    m_column = 1;

#ifdef DEBUG
    m_logNewLine = true;
#endif

    if (shift) this->shift();
}

bool Tokenizer::has_next() const {
    return m_inputStream.good() || m_buffer != "";
}

const Token * Tokenizer::top() {
    if (!has_next()) return &(Tokenizer::END_OF_FILE);

    // If there was a shift, the current type and token must be analyzed again
    if (m_shifted) this->analyze();

    return m_currentToken;
}

void Tokenizer::shift() {
    if (!this->has_next()) return;

    delete m_currentToken;
    m_currentToken = nullptr;
    m_shifted = true;

    // Consumes the current token from the buffer
    m_buffer = m_buffer.substr(m_currentTokenStr.size());
    m_column += m_currentTokenStr.size();

    int nbToAdd;
    char tmp[Tokenizer::BUFFER_SIZE + 1];

    // Increases the buffer size until it hits Tokenizer::BUFFER_SIZE or EOF
    while (m_buffer.size() < Tokenizer::BUFFER_SIZE) {
        nbToAdd = static_cast<int>(Tokenizer::BUFFER_SIZE - m_buffer.size());

        // Sets `tmp` to "" in case the input stream isn't good
        tmp[0] = '\0';
        m_inputStream.getline(tmp, nbToAdd + 1, '\0');

        m_buffer += tmp;

        // Removes leading whitespaces while counting lines and columns
        this->trim_left();

        // If the stream hits EOF, stop
        if (m_inputStream.eof() && !m_inputStream.good()) break;

        m_inputStream.clear();
    }
}

void Tokenizer::trim_left() {
    // If there are spaces at the beginning, removes them from the buffer
    boost::cmatch _;
    while (regex_match(m_buffer.c_str(), _, Tokenizer::whitespace)) {
        switch (m_buffer[0]) {
            case '\t':
            case ' ':
                ++m_column;
                break;
            case '\r':
                // If a '\r' is followed by a '\n', remove '\r'
                if ('\n' == m_buffer[1]) m_buffer = m_buffer.substr(1);
                m_column = 1;
                ++m_line;
                break;
            case '\n':
                m_column = 1;
                ++m_line;
                break;
            default:
                myassert(false, "A whitespace must match ^[\\r\\n\\t ]+");
                break;
        }
        // Removing whitespace
        m_buffer = m_buffer.substr(1);
    }
}

void Tokenizer::analyze() {
    m_shifted = false;
    boost::cmatch matches;

    if (regex_match(m_buffer.c_str(), matches, Tokenizer::keyword)) {
        // If we matched a keyword, the 1st character is enough to recognize it
        m_currentTokenStr = matches[1];

        switch (m_currentTokenStr[0]) {
            case 'c':
                m_currentToken = new Keyword(Token::con);
                break;
            case 'v':
                m_currentToken = new Keyword(Token::var);
                break;
            case 'e':
                m_currentToken = new Keyword(Token::ecr);
                break;
            case 'l':
                m_currentToken = new Keyword(Token::lir);
                break;
            default:
                myassert(false, "Arriving here implies programming error");
                break;
        }
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::affect)) {
        m_currentToken = new SimpleOperator(Token::aff);
        m_currentTokenStr = matches[1];
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::operatorr)) {
        m_currentTokenStr = matches[1];

        switch (m_currentTokenStr[0]) {
            case '+':
                m_currentToken = new SimpleOperator(Token::plu);
                break;
            case '-':
                m_currentToken = new SimpleOperator(Token::min);
                break;
            case '*':
                m_currentToken = new SimpleOperator(Token::mul);
                break;
            case '/':
                m_currentToken = new SimpleOperator(Token::quo);
                break;
            case '(':
                m_currentToken = new SimpleOperator(Token::opp);
                break;
            case ',':
                m_currentToken = new SimpleOperator(Token::com);
                break;
            case ')':
                m_currentToken = new SimpleOperator(Token::clo);
                break;
            case ';':
                m_currentToken = new SimpleOperator(Token::col);
                break;
            case '=':
                m_currentToken = new SimpleOperator(Token::equ);
                break;
            default:
                myassert(false, "Arriving here implies programming error");
                break;
        }
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::id)) {
        m_currentTokenStr = matches[1];
        m_currentToken = new Variable(Token::idv, m_currentTokenStr);
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::number)) {
        m_currentTokenStr = matches[1];
        uint64_t value = std::stoull(m_currentTokenStr);
        m_currentToken = new Number(Token::num, value);
    } else {
        m_currentTokenStr = m_buffer[0];
    }

#ifdef DEBUG
    if (m_currentToken) {
        if (m_logNewLine) {
            std::cout << "Line " << m_line << ": ";
            m_logNewLine = false;
        }

        Token::Id tId = m_currentToken->id();
        if (Token::aff == tId || Token::equ == tId) std::cout << ' ';

        std::cout << *m_currentToken;

        if (dynamic_cast<Keyword *>(m_currentToken)
            || Token::aff == tId || Token::equ == tId) {
            std::cout << ' ';
        }

        if (Token::col == tId) {
            std::cout << std::endl;
            m_logNewLine = true;
        }
    } else {
        std::cout << "ERROR: " << m_currentTokenStr << std::endl;
    }
#endif
}
