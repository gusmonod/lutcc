// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include "boost/regex.hpp"

using std::cout;
using std::string;
using std::endl;
using boost::regex_match;

enum ReturnCode {
    SUCCESS = 0,
    ARG_ERROR,
    MATCH_ERROR
};

int main(int argc, const char * argv[]) {
    // Must have exactly one argument + 1 for the exec path
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <string to test>" << endl;
        return ARG_ERROR;  // Wrong arguments
    }

    const char *testString(argv[1]);



    // Keywords regular expressions
    boost::regex co("^const ");
    boost::regex va("^var ");
    boost::regex ec("^ecrire ");
    boost::regex li("^lire ");

    // Operator regular expressions
    boost::regex af("^:=");
    boost::regex op("^\\+|-|\\*|/");

    // Operands regular expressions
    boost::regex id("^[a-z][a-zA-Z0-9_-]*");
    boost::regex nu("^[0-9]+");



    cout << '"' << testString << "\" is ";

    if (regex_match(testString, co)) {
        cout << "recognized as the keyword `const`" << endl;
        return SUCCESS;
    }
    if (regex_match(testString, va)) {
        cout << "recognized as the keyword `var`" << endl;
        return SUCCESS;
    }
    if (regex_match(testString, ec)) {
        cout << "recognized as the keyword `ecrire`" << endl;
        return SUCCESS;
    }
    if (regex_match(testString, li)) {
        cout << "recognized as the keyword `lire`" << endl;
        return SUCCESS;
    }



    if (regex_match(testString, af)) {
        cout << "recognized as the operator `:=`" << endl;
        return SUCCESS;
    }
    if (regex_match(testString, op)) {
        cout << "recognized as one of the operators"
             << "`+`, `-`, `*` and `/`" << endl;
        return SUCCESS;
    }



    if (regex_match(testString, id)) {
        cout << "recognized as a variable name" << endl;
        return SUCCESS;
    }
    if (regex_match(testString, nu)) {
        cout << "recognized as a positive integer" << endl;
        return SUCCESS;
    }

    cout << "not recognized" << endl;
    return MATCH_ERROR;
}
