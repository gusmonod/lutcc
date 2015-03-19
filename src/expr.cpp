// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./expr.h"

#include <map>
#include <string>
#include <exception>
#include <iostream>

#include "./errors.h"

/*virtual*/ Token * Variable::newCopy() const {
    return new Variable(this->id(), m_name);
}

/*virtual*/ uint64_t Variable::eval(const SymbolsTable & values) const {
    auto entry = values.find(m_name);

    if (entry == values.end()) {
        std::string what("Undeclared variable: `");
        std::runtime_error undeclared(what + m_name + "`");
        throw undeclared;
    }
    if (!entry->second.defined) {
        std::string what("Undefined variable: `");
        std::runtime_error undefined(what + m_name + "`");
        throw undefined;
    }

    return entry->second.value;
}

/*virtual*/ std::ostream& Variable::print(std::ostream& stream) const {
    return stream << this->m_name;
}

/*virtual*/ Token * Number::newCopy() const {
    return new Number(this->id(), m_value);
}

/*virtual*/ uint64_t Number::eval(const SymbolsTable & values) const {
    return m_value;
}

/*virtual*/ std::ostream& Number::print(std::ostream& stream) const {
    return stream << this->m_value;
}

BinExpr::BinExpr(Token::Id id, Expr * left, Expr * right)
    : Expr(id), m_left(left), m_right(right) { }

void BinExpr::left(Expr * left, bool shouldDelete) {
    if (shouldDelete) delete m_left;
    m_left = left;
}

void BinExpr::right(Expr * right, bool shouldDelete) {
    if (shouldDelete) delete m_right;
    m_right = right;
}

/*virtual*/ std::ostream& BinExpr::print(std::ostream& stream) const {
    stream << *m_left;
    Token::print(stream);
    return stream << *m_right;
}

AddExpr::AddExpr(Token::Id id, Expr * left, Expr * right)
    : BinExpr(id, left, right) { }

/*virtual*/ Token * AddExpr::newCopy() const {
    return new AddExpr(this->id(),
            dynamic_cast<Expr *>(m_left->newCopy()),
            dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t AddExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) + m_right->eval(values);
}

SubExpr::SubExpr(Token::Id id, Expr * left, Expr * right)
    : BinExpr(id, left, right) { }

/*virtual*/ Token * SubExpr::newCopy() const {
    return new SubExpr(this->id(),
            dynamic_cast<Expr *>(m_left->newCopy()),
            dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t SubExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) - m_right->eval(values);
}

MulExpr::MulExpr(Token::Id id, Expr * left, Expr * right)
    : BinExpr(id, left, right) { }

/*virtual*/ Token * MulExpr::newCopy() const {
    return new MulExpr(this->id(),
            dynamic_cast<Expr *>(m_left->newCopy()),
            dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t MulExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) * m_right->eval(values);
}

DivExpr::DivExpr(Token::Id id, Expr * left, Expr * right)
    : BinExpr(id, left, right) { }

/*virtual*/ Token * DivExpr::newCopy() const {
    return new DivExpr(this->id(),
            dynamic_cast<Expr *>(m_left->newCopy()),
            dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t DivExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) / m_right->eval(values);
}
