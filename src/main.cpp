// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "boost/program_options.hpp"

#include "./lexical.h"
#include "./automaton.h"
#include "./options.h"
#include "./errors.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void print_var(const SymbolsTable & variables) {
    for (auto entry : variables) {
        if (entry.second.constant) {
            cout << "const ";
        } else {
            cout << "var ";
        }

        cout << entry.first;

        if (entry.second.constant) {
            cout << " = " << entry.second.value;
        }

        cout << ";" << endl;
    }
}

void print_ins(const std::vector<Instruction *> instructions) {
    for (auto instruction : instructions) {
        cout << *instruction << endl;
    }
}

void print_terminal(Token::Id terminal, std::ostream & out = std::cerr) {
    if (Token::END == terminal) {
        out << "End of File";
        return;
    }

    assert(terminal > Token::E);  // `terminal` must be a terminal token

    if (Token::con <= terminal && terminal <= Token::lir) {
        out << "keyword `";
    } else if (Token::aff <= terminal && terminal <= Token::quo) {
        out << "operator `";
    } else if (Token::com <= terminal && terminal <= Token::equ) {
        out << "symbol `";
    } else if (Token::idv == terminal) {
        out << "identifier";
        return;
    } else if (Token::num == terminal) {
        out << "number";
        return;
    } else {
        assert((false && "There is no other terminal Token"));
    }

    out << Token(terminal) << '`';
}

std::ostream& operator<<(std::ostream& os, const std::vector<Token::Id>& vec) {
    if (vec.size() < 1) return os;

    auto el = vec.begin();
    os << '`' << *el << '`';

    for (++el; el != vec.end(); ++el) {
        os << " or `" << *el << '`';
    }

    return os;
}

void doWithOptions(const boost::program_options::variables_map & options,
                   SymbolsTable * variables,
                   std::vector<Instruction *> * instructions) {
    if (options.count("optim2")) {
        for (auto instruction : *instructions) {
            instruction->optimize(variables);
        }
    }

    if (options.count("print")) {
        print_var(*variables);
        print_ins(*instructions);
    }

    if (options.count("analyze")) {
        for (auto instruction : *instructions) {
            try {
                instruction->analyze(variables);
            } catch (const std::runtime_error & e) {
                cerr << e.what() << endl;
            }
        }
        for (auto entry : *variables) {
            if (!entry.second.defined) {
                cerr << "Undefined variable `" << entry.first
                     << '`' << endl;
            }
            if (!entry.second.used) {
                cerr << "Unused "
                     << (entry.second.constant ? "constant" :
                         "variable") << " `" << entry.first
                     << '`' << endl;
            }
        }
    }

    if (options.count("exec")) {
        for (auto instruction : *instructions) {
            try {
                instruction->execute(variables);
            } catch (const std::runtime_error & e) {
                cerr << e.what() << endl;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
#ifdef DEBUG
    for (int i = 0; i < argc - 1; ++i) {
        cout << argv[i];
    }
    cout << argv[argc - 1] << endl;
#endif

    boost::program_options::variables_map options;

    int err = get_options_map(argc, argv, &options);
    if (err) return err;

    std::ifstream file;
    file.open(options["lutin-file"].as<string>());
    if (!file) {
        cerr << "Could not open `" << options["lutin-file"].as<string>()
             << '`' << endl;
        return EXIT_FAILURE;
    }

    Automaton automaton(options.count("optim1") || options.count("optim2"));
    Tokenizer tokenizer(&file);

    const Token * currentToken = tokenizer.top();
    const Token * nextToken = nullptr;
    while (!currentToken || Token::END != currentToken->id()) {
        if (!currentToken) {
            cerr << "Lexical error (" << tokenizer.line() << ':'
                 << tokenizer.column() << ") character `"
                 << tokenizer.topStr() << '`';

            // Go to next token:
            tokenizer.shift();
            currentToken = tokenizer.top();

            continue;  // and try again
        }

        try {
            bool isShift = automaton.analyze(*currentToken, tokenizer);

            if (isShift) {
                if (nextToken) {
                    delete currentToken;
                    currentToken = nextToken;
                    nextToken = nullptr;
                } else {
                    tokenizer.shift();
                    currentToken = tokenizer.top();
                }
            }
        } catch (const syntactic_error & e) {
            std::vector<Token::Id> expected(automaton.expected(*currentToken));

            if (expected.size() < 1) continue;

            const Token::Id expectedId = expected[0];

            cerr << e.what() << ' ';
            print_terminal(expectedId);
            cerr << (expected.size() == 1 ? "" : " probably")
                 << " expected";

            if (Token::idv == expectedId || Token::num == expectedId) {
                cerr << " (not recoverable)" << endl;
                return EXIT_FAILURE;
            } else {
                // Inserting the expected token
                nextToken = currentToken;
                currentToken = new Token(expectedId);
            }
            cerr << endl;
        } catch (const recoverable_error & e) {
            cerr << e.what() << endl;
            tokenizer.shift();  // shift and try again
            currentToken = tokenizer.top();
        }
    }

    doWithOptions(options, automaton.variables(), automaton.instructions());

    return EXIT_SUCCESS;
}
