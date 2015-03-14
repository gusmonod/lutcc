// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./expr.h"

#include <map>
#include <string>
#include <cstdlib>

/*virtual*/ uint64_t Variable::eval(const SymbolsTable & values) {
    auto entry = values.find(m_name);
    if (entry == values.end() || !entry->second.defined) {
        std::exit(EXIT_FAILURE);
    }
    return entry->second.value;
}

/*virtual*/ uint64_t Number::eval(const SymbolsTable & values) {
    return m_value;
}

/*virtual*/ uint64_t AddExpr::eval(const SymbolsTable & values) {
    return m_left->eval(values) + m_right->eval(values);
}

/*virtual*/ uint64_t SubExpr::eval(const SymbolsTable & values) {
    return m_left->eval(values) - m_right->eval(values);
}

/*virtual*/ uint64_t MulExpr::eval(const SymbolsTable & values) {
    return m_left->eval(values) * m_right->eval(values);
}

/*virtual*/ uint64_t DivExpr::eval(const SymbolsTable & values) {
    return m_left->eval(values) / m_right->eval(values);
}
