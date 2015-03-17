// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./lexical.h"

#include <iostream>
#include <string>

#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"

#include "./simpletoken.h"
#include "./expr.h"

using std::string;
using std::cout;
using std::endl;

const size_t Tokenizer::BUFFER_SIZE = 20;
const Token Tokenizer::END_OF_FILE(Token::END);

// Keywords regular expressions
const boost::regex Tokenizer::keyword("^(const |var |ecrire |lire ).*");

// Operator regular expressions
const boost::regex Tokenizer::affect("^(:=).*");
const boost::regex Tokenizer::operatorr("^(\\+|-|\\*|/|\\(|,|\\)|;|=).*");

// Operands regular expressions
const boost::regex Tokenizer::id("^([a-zA-Z][a-zA-Z0-9_-]*).*");
const boost::regex Tokenizer::number("^([0-9]+).*");

Tokenizer::Tokenizer(std::istream * inStream, bool shift /* = true */)
    : m_inputStream(*inStream), m_currentToken(nullptr), m_shifted(shift) {
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

    using boost::regex_replace;
    using boost::regex_constants::format_literal;

    int nbToAdd;
    char tmp[Tokenizer::BUFFER_SIZE + 1];

    // Consumes the current token from the buffer
    m_buffer = m_buffer.substr(m_currentTokenStr.size());

    // If there are spaces after the token, removes them from the buffer
    boost::algorithm::trim_left(m_buffer);

    // Increases the buffer size until it hits Tokenizer::BUFFER_SIZE or EOF
    while (m_buffer.size() < Tokenizer::BUFFER_SIZE) {
        nbToAdd = static_cast<int>(Tokenizer::BUFFER_SIZE - m_buffer.size());

        // Puts to "" in case the input stream isn't good
        tmp[0] = '\0';
        m_inputStream.getline(tmp, nbToAdd + 1);

        // Replaces whitespaces with a single ' '
        m_buffer += regex_replace(string(tmp), boost::regex("[\\r\\n\\t ]+"),
                " ", format_literal);
        if (m_inputStream.eof() && !m_inputStream.good()) break;
        m_inputStream.clear();
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
#ifdef DEBUG
                std::cout << "Detection - Constante : " << m_currentTokenStr << " ";
#endif
                break;
            case 'v':
                m_currentToken = new Keyword(Token::var);
#ifdef DEBUG
                std::cout << "Detection - Variable : " << m_currentTokenStr << " ";
#endif
                break;
            case 'e':
                m_currentToken = new Keyword(Token::ecr);
#ifdef DEBUG
                std::cout << "Detection - Ecriture : " << m_currentTokenStr << " ";
#endif
                break;
            case 'l':
                m_currentToken = new Keyword(Token::lir);
#ifdef DEBUG
                std::cout << "Detection - Lecture : " << m_currentTokenStr << " ";
#endif
                break;
            // If the keyword does not start from this, programming error
            default:
                std::exit(EXIT_FAILURE);
        }
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::affect)) {
        m_currentToken = new SimpleOperator(Token::aff);
        m_currentTokenStr = matches[1];
        #ifdef DEBUG
                std::cout << ":= ";
		#endif
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::operatorr)) {
        m_currentTokenStr = matches[1];

		// Formats the string to be printed to the screen
		//std::size_t pos = m_buffer.find(" ") + 1;
		//std::string formattedStr = m_buffer.substr (0,pos);

        switch (m_currentTokenStr[0]) {
            case '+':
                m_currentToken = new SimpleOperator(Token::plu);
                #ifdef DEBUG
					std::cout << " + ";
				#endif
                break;
            case '-':
                m_currentToken = new SimpleOperator(Token::min);
                #ifdef DEBUG
					std::cout << " - ";
				#endif
                break;
            case '*':
                m_currentToken = new SimpleOperator(Token::mul);
                #ifdef DEBUG
					std::cout << " * ";
				#endif
                break;
            case '/':
                m_currentToken = new SimpleOperator(Token::quo);
                #ifdef DEBUG
					std::cout << " / ";
				#endif
                break;
            case '(':
                m_currentToken = new SimpleOperator(Token::opp);
                #ifdef DEBUG
					std::cout << " ( ";
				#endif
                break;
            case ',':
                m_currentToken = new SimpleOperator(Token::com);
                #ifdef DEBUG
					std::cout << " , ";
				#endif
                break;
            case ')':
                m_currentToken = new SimpleOperator(Token::clo);
                #ifdef DEBUG
					std::cout << " ) ";
				#endif
                break;
            case ';':
                m_currentToken = new SimpleOperator(Token::col);
                #ifdef DEBUG
					std::cout << " ; " << std::endl;
				#endif
                break;
            case '=':
                m_currentToken = new SimpleOperator(Token::equ);
                #ifdef DEBUG
					std::cout << " = ";
				#endif
                break;
            // If the operator does not start from this, programming error
            default:
                std::exit(EXIT_FAILURE);
        }
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::id)) {
        m_currentTokenStr = matches[1];
        m_currentToken = new Variable(Token::idv, m_currentTokenStr);
        #ifdef DEBUG
                std::cout << m_currentTokenStr << " " ;

                // We found a variable

                // top sur la pile pour récuperer la variable qui précede le signe d'affectation
                // on fait alors une recherche dans la map SymbolsTable (find)
                // Si on ne trouve pas => erreur de compil, la variable est affectée sans etre instanciee
                // Si on la trouve, OK
		#endif
    } else if (regex_match(m_buffer.c_str(), matches, Tokenizer::number)) {
        m_currentTokenStr = matches[1];
        uint64_t value = std::stoull(m_currentTokenStr);
        m_currentToken = new Number(Token::num, value);
        #ifdef DEBUG
                std::cout << value;
		#endif
    } else {
        m_currentTokenStr = m_buffer[0];
    }
}
