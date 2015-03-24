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

bool doWithOptions(Automaton *accepter,
                   const boost::program_options::variables_map &vm,
                   Tokenizer *t) {
    bool accepted = accepter->analyze(t);

    SymbolsTable * variables = accepter->variables();
    std::vector<Instruction *> *instructions = accepter->instructions();

    if (vm.count("optim2")) {
        for (auto instruction : *instructions) {
            instruction->optimize(variables);
        }
    }

    if (vm.count("print")) {
        print_var(*variables);
        print_ins(*instructions);
    }

    if (vm.count("analyze")) {
        for (auto instruction : *instructions) {
            instruction->analyze(variables);
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

    if (vm.count("exec")) {
        for (auto instruction : *instructions) {
            instruction->execute(variables);
        }
    }

#ifdef DEBUG
    if (accepted) {
        cout << "DEBUG: Accepted!!" << endl;
    } else {
        cout << "DEBUG: Not accepted" << endl;
    }
#endif

    return accepted;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Token>& vec) {
    if (vec.size() < 1) return os;

    auto el = vec.begin();
    os << *el;

    for (; el != vec.end(); ++el) {
        os << " or " << *el;
    }

    return os;
}

int main(int argc, const char * argv[]) {
    boost::program_options::variables_map vm;

    int err = get_options_map(argc, argv, &vm);
    if (err) return err;

    std::ifstream file;
    file.open(vm["lutin-file"].as<string>());
    if (!file) {
        cerr << "Could not open `" << vm["lutin-file"].as<string>() << '`'
             << endl;
        return EXIT_FAILURE;
    }

    Automaton accepter(vm.count("optim1") || vm.count("optim2"));
    Tokenizer t(&file);

    bool tryAgain = true;
    while (tryAgain) {
        try {
            tryAgain = !doWithOptions(&accepter, vm, &t);
        } catch (const syntactic_error & e) {
            cerr << e.what() << e.expected() << endl;
            volatile int i = 0;
        } catch (const recoverable_error & e) {
            cerr << e.what() << endl;
            t.shift();  // shift and try again
        } catch (const std::runtime_error & e) { tryAgain = false; }
    }

    return EXIT_SUCCESS;
}
