// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_MYASSERT_H_
#define SRC_MYASSERT_H_

#include <cassert>

#ifdef NDEBUG

inline void myassert(bool assertion, const char * const what = "") { }

#else

inline void myassert(bool assertion, const char * const what = "") {
    assert(assertion && what);
}

#endif  // NDEBUG

#endif  // SRC_MYASSERT_H_
