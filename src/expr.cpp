// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./expr.h"

#include <map>
#include <string>

typedef std::map<std::string, uint64_t> VariablesMap;

/*virtual*/ uint64_t Variable::eval(const VariablesMap & values) {
    return values.find(m_name)->second;
}

/*virtual*/ uint64_t Number::eval(const VariablesMap & values) {
    return m_value;
}

/*virtual*/ uint64_t AddExpr::eval(const VariablesMap & values) {
    return m_left->eval(values) + m_right->eval(values);
}

/*virtual*/ uint64_t SubExpr::eval(const VariablesMap & values) {
    return m_left->eval(values) - m_right->eval(values);
}

/*virtual*/ uint64_t MulExpr::eval(const VariablesMap & values) {
    return m_left->eval(values) * m_right->eval(values);
}

/*virtual*/ uint64_t DivExpr::eval(const VariablesMap & values) {
    return m_left->eval(values) / m_right->eval(values);
}
