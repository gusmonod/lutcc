// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ERRORS_H_
#define SRC_ERRORS_H_

#include <exception>
#include <cassert>
#include <string>
#include <iostream>

#ifdef DEBUG

inline void MYASSERT(bool assertion, const char * const file,
                     int line,
                     const char * const what) {
    if (!assertion) {
        std::cerr << what << " in " << file << ":" << line << std::endl;
    }

    assert(assertion);
}

#define myassert(assertion, what) MYASSERT(assertion, __FILE__, __LINE__, what)

#else

#define myassert(assertion, what) 0

#endif  // DEBUG

class compile_error : public std::runtime_error {
 public:
    explicit compile_error(const char *what) : std::runtime_error(what) {}
    explicit compile_error(const std::string &what): std::runtime_error(what){}
};

class lexical_error : public std::runtime_error {
 public:
    explicit lexical_error(const char *what) : std::runtime_error(what) {}
    explicit lexical_error(const std::string &what): std::runtime_error(what){}
};

const std::runtime_error undeclared_error(const std::string & name);

const std::runtime_error undefined_error(const std::string & name);

const std::runtime_error constant_error(const std::string & name);

const std::runtime_error math_error(std::string what);

#endif  // SRC_ERRORS_H_
