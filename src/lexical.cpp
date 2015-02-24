// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./lexical.h"
#include <fstream>
#include <sstream>
#include <string>

using std::string;
using std::cout;
using std::endl;

const int Tokenizer::BUFFER_SIZE = 10;

// Keywords regular expressions
const boost::regex Tokenizer::keyword("^(const |var |ecrire |lire ).*");

// Operator regular expressions
const boost::regex Tokenizer::affect("^(:=).*");
const boost::regex Tokenizer::operatorr("^(\\+|-|\\*|/|;|=).*");

// Operands regular expressions
const boost::regex Tokenizer::id("^([a-zA-Z][a-zA-Z0-9_-]*).*");
const boost::regex Tokenizer::number("^([0-9]+).*");

string Tokenizer::ws_format(std::istream & input) {
    std::stringstream output;
    string token;
    while (input >> token) {
        boost::regex kw(".*(const|var|ecrire|lire)");

        if (regex_match(token, kw)) {
            output << token << ' ';
        } else {
            output << token;
        }
    }

    return output.str();
}

Tokenizer::Tokenizer(const string & inString)
    : m_current_token("") {
    init(inString);
}

Tokenizer::Tokenizer(std::istream & inStream)
    : m_current_token("") {
    init(Tokenizer::ws_format(inStream));
}

void Tokenizer::init(const string & inString) {
    size_t size = inString.size() + 1;
    char *buffer = new char[size];
    std::strncpy(buffer, inString.c_str(), size);

    m_line = buffer;
    m_pos_in_line = buffer;
}

Tokenizer::~Tokenizer() {
    delete m_line;
    m_line = nullptr;
    m_pos_in_line = nullptr;
}

bool Tokenizer::has_next() const {
//    if (!input && !*m_pos_in_line) return false;
//    m_input >> std::ws;
    return *m_pos_in_line;
}

string Tokenizer::top(TokenType *type) {
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
        m_current_token = matches[1];
    } else {
        // No match -> error or EOL, extracting pb char
        m_current_token = *m_pos_in_line;
    }

    return m_current_token;
}

void Tokenizer::shift() {
    m_pos_in_line += m_current_token.size();
}
