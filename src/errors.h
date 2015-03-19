// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_ERRORS_H_
#define SRC_ERRORS_H_

#include <exception>
#include <cassert>
#include <string>

#ifdef NDEBUG

inline void myassert(bool assertion, const char * const what = "") { }

#else

inline void myassert(bool assertion, const char * const what = "") {
    assert(assertion && what);
}

#endif  // NDEBUG

class compile_error : public std::runtime_error {
 public:
    explicit compile_error(const char *what) : std::runtime_error(what) {}
    explicit compile_error(const std::string &what) : std::runtime_error(what) {}
};

#endif  // SRC_ERRORS_H_
