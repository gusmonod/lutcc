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
        throw this->undeclared_error();
    }
    if (!entry->second.defined) {
        throw this->undefined_error();
    }

    return entry->second.value;
}

const std::runtime_error Variable::undeclared_error() const {
    return std::runtime_error("Undeclared variable: `" + m_name + "`");
}

const std::runtime_error Variable::undefined_error() const {
    return std::runtime_error("Undefined variable: `" + m_name + "`");
}

const std::runtime_error Variable::constant_error() const {
    return std::runtime_error("Variable `" + m_name + "` can't be modified");
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

AddExpr::AddExpr(Expr * left, Expr * right)
    : BinExpr(Token::Id::plu, left, right) { }

/*virtual*/ Token * AddExpr::newCopy() const {
    return new AddExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t AddExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) + m_right->eval(values);
}

SubExpr::SubExpr(Expr * left, Expr * right)
    : BinExpr(Token::Id::min, left, right) { }

/*virtual*/ Token * SubExpr::newCopy() const {
    return new SubExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t SubExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) - m_right->eval(values);
}

MulExpr::MulExpr(Expr * left, Expr * right)
    : BinExpr(Token::Id::mul, left, right) { }

/*virtual*/ Token * MulExpr::newCopy() const {
    return new MulExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t MulExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) * m_right->eval(values);
}

DivExpr::DivExpr(Expr * left, Expr * right)
    : BinExpr(Token::Id::quo, left, right) { }

/*virtual*/ Token * DivExpr::newCopy() const {
    return new DivExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()));
}

/*virtual*/ uint64_t DivExpr::eval(const SymbolsTable & values) const {
    return m_left->eval(values) / m_right->eval(values);
}

const std::runtime_error * DivExpr::Math_error(std::string what) {
    return new std::runtime_error("Math error: "+what);
}
