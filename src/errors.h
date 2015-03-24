// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ERRORS_H_
#define SRC_ERRORS_H_

#include <stdexcept>
#include <string>
#include <iostream>

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

class syntactic_error : public std::runtime_error {
public:
	explicit syntactic_error(const char *what) : std::runtime_error(what) {}
	explicit syntactic_error(const std::string &what) : std::runtime_error(what){}
};

const std::runtime_error undeclared_error(const std::string & name);

const std::runtime_error undefined_error(const std::string & name);

const std::runtime_error constant_error(const std::string & name);

const std::runtime_error math_error(std::string what);

#endif  // SRC_ERRORS_H_
