// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include <map>
#include <string>
#include <cassert>

// Terminal symbols
// ----------------

// Keywords:
// + `con` = const
// + `var` = var
// + `ecr` = ecrire
// + `lir` = lire

// Operators:
// + `aff` = affectation (/^:=/)
// + `plu` = `+`
// + `min` = `-`
// + `mul` = `*`
// + `quo` = `/`
// + `opp` = `(`
// + `com` = `,`
// + `clo` = `)`
// + `col` = `;`
// + `equ` = `=`

// Operands:
// + `idv` = name of a variable
// + `num` = number (positive integer)

// Non-terminal symbols
// --------------------

// + `A ` = Axiom
// + `P ` = Program
// + `Ld` = List of declarations (may be empty)
// + `Li` = List of instructions (at least one)
// + `D ` = Declaration
// + `Lv` = List of variables (may be empty)
// + `Lc` = List of constants (may be empty)
// + `I ` = Instruction
// + `E ` = Expression

class Token {
 public:
    enum Id {
        END,  // End of file

        // Non-terminals (see above)
        A, P, Ld, Li, D, Lv, Lc, I, E,

        // Terminals
        con, var, ecr, lir,  // keywords
        idv, num,            // variables or numbers
        plu, min, mul, quo,  // arithmetic operators
        aff, opp, clo,       // other operators
        com, col, equ        // other tokens (symbols)
    };

    explicit Token(Token::Id id) : m_id(id) { }

    virtual ~Token() { }

    virtual Token * newCopy() const { return new Token(m_id); }

    Token::Id id() const { return m_id; }

    bool isTerminal() const { return END == m_id || E < m_id; }

    bool isKeyword() const { return con <= m_id && m_id <= lir; }

    bool isVariable() const { return idv == m_id; }

    bool isNumber() const { return num == m_id; }

    bool isOperator() const { return plu <= m_id && m_id <= clo; }

    bool isArithOperator() const { return plu <= m_id && m_id <= quo; }

    int priority() const {
        assert((this->isArithOperator()
                && "Only arithmetic operators have priority"));

        if (Token::plu == m_id || Token::min == m_id) return 1;
        else return 2;
    }

    bool isSymbol() const { return com <= m_id && m_id <= equ; }

    friend std::ostream& operator<<(std::ostream& stream, const Token & token);

 protected:
    virtual std::ostream& print(std::ostream& stream) const;  // NOLINT

 private:
    Token::Id m_id;
    static const std::map<Token::Id, std::string> ID_NAMES;
};

#endif  // SRC_TOKEN_H_
