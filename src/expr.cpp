// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./expr.h"

#include <map>
#include <string>

typedef std::map<std::string, uint64_t> VariablesMap;

/*virtual*/ uint64_t AddExpr::eval(const VariablesMap & values) {
    uint64_t result = 0;
    for (auto entry : values) {
        result += entry.second;
    }
    return result;
}

/*virtual*/ uint64_t SubExpr::eval(const VariablesMap & values) {
    uint64_t result = 0;
    for (auto entry : values) {
        result -= entry.second;
    }
    return result;
}

/*virtual*/ uint64_t MulExpr::eval(const VariablesMap & values) {
    uint64_t result = 1;
    for (auto entry : values) {
        result *= entry.second;
    }
    return result;
}

/*virtual*/ uint64_t DivExpr::eval(const VariablesMap & values) {
    uint64_t result = 1;
    for (auto entry : values) {
        result /= entry.second;
    }
    return result;
}
