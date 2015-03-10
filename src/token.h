// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include <map>
#include <string>

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
        // Non-terminals (see above)
        A, P, Ld, Li, D, Lv, Lc, I, E,
        // Terminals
        con, var, ecr, lir, aff, plu, min, mul, quo, opp, com, clo, col, equ,
        idv, num
    };

    explicit Token(Token::Id id) : m_id(id) { }

    virtual ~Token() { }

    operator Token::Id() const { return m_id; }

    friend std::ostream& operator<<(std::ostream& stream, const Token & token);

 protected:
    Token::Id m_id;

 private:
    static const std::map<Token::Id, std::string> ID_NAMES;
};

#endif  // SRC_TOKEN_H_
