// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./instruction.h"

#include <sstream>
#include <string>

#include "./actions.h"
#include "./errors.h"

std::ostream & operator<<(std::ostream & o, const Instruction & i) {
    i.print(o);
    return o;
}

Assignment::Assignment(const std::string varName,
                       const Expr * rValue)
: m_varName(varName) {
    m_rValue = dynamic_cast<const Expr *>(rValue->newCopy());
    myassert(m_rValue, "`m_rValue` must be pointing to a `Expr *`");
}

/*virtual*/ void Assignment::execute(SymbolsTable * variables) const {
    this->analyze(variables);  // Marks as defined and `m_rValue` as "used"

    auto symbol = variables->find(m_varName);

    symbol->second.value = m_rValue->eval(variables);
}

/*virtual*/ void Assignment::analyze(SymbolsTable * variables) const {
    auto symbol = variables->find(m_varName);

    if (symbol == variables->end()) throw undeclared_error(m_varName);

    try {
        // Mark right value's variables as used
        m_rValue->eval(variables, true);
    } catch(const std::runtime_error & e) {
        // Adding more info to the error message and throwing again
        std::ostringstream oss;
        oss << e.what() << " in `" << *m_rValue << "`";
        std::runtime_error e2(oss.str());
        throw e2;
    }

    // Simulate assignment:
    symbol->second.defined = true;
}

/*virtual*/ void Assignment::optimize(SymbolsTable * variables) {
    try {
        m_rValue->eval(variables);

        // If the result can be found at compile-time, execute it:
        this->execute(variables);

        Expr * expr = const_cast<Expr *>(m_rValue);
        BinExpr * toOptimize = dynamic_cast<BinExpr *>(expr);

        // Optimize rValue if possible
        if (toOptimize) {
            m_rValue = ActionExpr(true).optimize(toOptimize, variables, 0);
        }
    } catch (std::runtime_error & e) { /* ignore error */ }
}

/*virtual*/ std::ostream & Assignment::print(std::ostream & o) const {
    return o << m_varName << " := " << *m_rValue << ';';
}

Read::Read(const std::string varName, std::istream & inStream,
           std::ostream & outStream)
    : m_varName(varName), m_inStream(inStream), m_outStream(outStream) { }

/*virtual*/ void Read::execute(SymbolsTable * variables) const {
    this->analyze(variables);  // Marks as defined

    auto symbol = variables->find(m_varName);

    uint64_t value;
    std::string input;
    getline(m_inStream, input);
    while ((std::stringstream(input) >> value).fail()) {
        m_outStream << "Error, please type an integer." << std::endl;
        getline(m_inStream, input);
    }

    symbol->second.value = value;
}

/*virtual*/ void Read::analyze(SymbolsTable * variables) const {
    auto symbol = variables->find(m_varName);

    if (symbol == variables->end()) throw undeclared_error(m_varName);
    if (symbol->second.constant) throw constant_error(m_varName);

    // Simulate read:
    symbol->second.defined = true;
}

/*virtual*/ void Read::optimize(SymbolsTable * variables) {
    this->analyze(variables);  // Marks as defined, tests errors

    // Mark as undefined for further optimization
    (*variables)[m_varName].defined = false;
}

/*virtual*/ std::ostream & Read::print(std::ostream & o) const {
    return o << "lire " << m_varName << ';';
}

Write::Write(const Expr * rValue, std::ostream & outStream)
    : m_outStream(outStream) {
    m_rValue = dynamic_cast<const Expr *>(rValue->newCopy());
    myassert(m_rValue, "`m_rValue` must be pointing to a `Expr *`");
}

/*virtual*/ void Write::execute(SymbolsTable * variables) const {
    this->analyze(variables);  // Marks the variable as "used"

    uint64_t value;
    value = m_rValue->eval(variables);

    m_outStream << value << std::endl;
}

/*virtual*/ void Write::analyze(SymbolsTable * variables) const {
    try {
        // Mark the variable as "used"
        m_rValue->eval(variables, true);
    } catch(const std::runtime_error & e) {
        // Adding more info to the error message and throwing again
        std::ostringstream oss;
        oss << e.what() << " in `" << *m_rValue << "`";
        std::runtime_error e2(oss.str());
        throw e2;
    }
}

/*virtual*/ void Write::optimize(SymbolsTable * variables) {
    try {
        Expr * expr = const_cast<Expr *>(m_rValue);
        BinExpr * toOptimize = dynamic_cast<BinExpr *>(expr);
        
        // Optimize rValue if possible
        if (toOptimize) {
            m_rValue = ActionExpr(true).optimize(toOptimize, variables, 0);
        }
    } catch (std::runtime_error & e) { /* ignore error */ }
}

/*virtual*/ std::ostream & Write::print(std::ostream & o) const {
    return o << "ecrire " << *m_rValue << ';';
}
