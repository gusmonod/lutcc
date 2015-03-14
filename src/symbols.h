// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_SYMBOLS_H_
#define SRC_SYMBOLS_H_

#include <string>
#include <map>

struct Symbol {
    uint64_t value;
    bool defined;
    bool constant;
};

typedef std::map<std::string, Symbol> SymbolsTable;

#endif  // SRC_SYMBOLS_H_
