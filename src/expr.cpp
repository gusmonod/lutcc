// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./expr.h"

#include <map>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "./errors.h"

/*virtual*/ Token * Variable::newCopy() const {
    return new Variable(m_name, m_inBrackets);
}

/*virtual*/ int64_t Variable::eval(SymbolsTable * values, bool used) const {
    auto entry = values->find(m_name);

    if (entry == values->end()) throw undeclared_error(m_name);
    if (!entry->second.defined) throw undefined_error(m_name);

    if (used) entry->second.used = used;

    return entry->second.value;
}

/*virtual*/ std::ostream& Variable::print(std::ostream& stream) const {
    if (m_inBrackets) stream << '(';
    stream << this->m_name;
    if (m_inBrackets) stream << ')';
    return stream;
}

/*virtual*/ Token * Number::newCopy() const {
    return new Number(m_value, m_inBrackets);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
/*virtual*/ int64_t Number::eval(SymbolsTable * values,
                                  bool used) const {
    return m_value;
}

/*virtual*/ std::ostream& Number::print(std::ostream& stream) const {
    if (m_inBrackets) stream << '(';
    stream << this->m_value;
    if (m_inBrackets) stream << ')';
    return stream;
}

BinExpr::BinExpr(Token::Id id, Expr * left, Expr * right, bool inBrackets)
    : Expr(id, inBrackets), m_left(left), m_right(right) { }

void BinExpr::left(Expr * left, bool shouldDelete) {
    if (shouldDelete) delete m_left;
    m_left = left;
}

void BinExpr::right(Expr * right, bool shouldDelete) {
    if (shouldDelete) delete m_right;
    m_right = right;
}

/*virtual*/ std::ostream& BinExpr::print(std::ostream& stream) const {
    // TODO(nautigsam, florianbouron) Add parens display
    if (m_inBrackets) stream << '(';
    stream << *m_left;
    Token::print(stream);
    stream << *m_right;
    if (m_inBrackets) stream << ')';
    return stream;
}

AddExpr::AddExpr(Expr * left, Expr * right, bool inBrackets)
    : BinExpr(Token::Id::plu, left, right, inBrackets) { }

/*virtual*/ Token * AddExpr::newCopy() const {
    return new AddExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()),
                        m_inBrackets);
}

/*virtual*/ int64_t AddExpr::eval(SymbolsTable * values, bool used) const {
    return m_left->eval(values, used) + m_right->eval(values, used);
}

SubExpr::SubExpr(Expr * left, Expr * right, bool inBrackets)
    : BinExpr(Token::Id::min, left, right, inBrackets) { }

/*virtual*/ Token * SubExpr::newCopy() const {
    return new SubExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()),
                        m_inBrackets);
}

/*virtual*/ int64_t SubExpr::eval(SymbolsTable * values, bool used) const {
    return m_left->eval(values, used) - m_right->eval(values, used);
}

MulExpr::MulExpr(Expr * left, Expr * right, bool inBrackets)
    : BinExpr(Token::Id::mul, left, right, inBrackets) { }

/*virtual*/ Token * MulExpr::newCopy() const {
    return new MulExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()),
                        m_inBrackets);
}

/*virtual*/ int64_t MulExpr::eval(SymbolsTable * values, bool used) const {
    return m_left->eval(values, used) * m_right->eval(values, used);
}

DivExpr::DivExpr(Expr * left, Expr * right, bool inBrackets)
    : BinExpr(Token::Id::quo, left, right, inBrackets) { }

/*virtual*/ Token * DivExpr::newCopy() const {
    return new DivExpr(dynamic_cast<Expr *>(m_left->newCopy()),
                        dynamic_cast<Expr *>(m_right->newCopy()),
                        m_inBrackets);
}

/*virtual*/ int64_t DivExpr::eval(SymbolsTable * values, bool used) const {
    return m_left->eval(values, used) / m_right->eval(values, used);
}
