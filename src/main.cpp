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

void print_ins(const std::vector<const Instruction *> instructions) {
    for (auto instruction : instructions) {
        cout << *instruction << endl;
    }
}

int errorHandlingLoop( Automaton *accepter, const boost::program_options::variables_map &vm, Tokenizer *t) {
    try {
        bool accepted = accepter->analyze(t);

        if (vm.count("print")) {
            print_var(accepter->variables());
            print_ins(accepter->instructions());
        }

        if (vm.count("analyze")) {
            // Copying the variables to `analysisTable`
            SymbolsTable analysisTable = accepter->variables();
            for (auto instruction : accepter->instructions()) {
                instruction->analyze(&analysisTable);
            }
            for (auto entry : analysisTable) {
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
            for (auto instruction : accepter->instructions()) {
                instruction->execute(&(accepter->variables()));
            }
        }

#ifdef DEBUG
        if (accepted) {
            cout << "DEBUG: Accepted!!" << endl;
        } else {
            cout << "DEBUG: Not accepted" << endl;
        }
#endif

        return accepted ? EXIT_SUCCESS : EXIT_FAILURE;

        // TODO(titouan, thibautbremand) better error handling
        } catch (const lexical_error & e) {
            cerr << e.what() << endl;
            t->shift();  // Next token
            return errorHandlingLoop(accepter, vm, t);

            // No return statement: try again
        } catch (const compile_error & e) {
            cerr << e.what() << endl;
            t->shift();  // Next token
            return errorHandlingLoop(accepter, vm, t);

            // No return statement: try again

        // Errors that are not compile or lexical 
        } catch (const std::runtime_error & e) {
            cerr << e.what() << endl;

            return EXIT_SUCCESS;
        }
    
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

    Automaton accepter(vm.count("optim"));
    Tokenizer t(&file);

    return errorHandlingLoop(&accepter, vm, &t);

/*
    while (true) {
        try {
            bool accepted = accepter.analyze(&t);

            if (vm.count("print")) {
                print_var(accepter.variables());
                print_ins(accepter.instructions());
            }

            if (vm.count("analyze")) {
                // Copying the variables to `analysisTable`
                SymbolsTable analysisTable = accepter.variables();
                for (auto instruction : accepter.instructions()) {
                    instruction->analyze(&analysisTable);
                }
                for (auto entry : analysisTable) {
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
                for (auto instruction : accepter.instructions()) {
                    instruction->execute(&accepter.variables());
                }
            }

#ifdef DEBUG
            if (accepted) {
                cout << "DEBUG: Accepted!!" << endl;
            } else {
                cout << "DEBUG: Not accepted" << endl;
            }
#endif

            return accepted ? EXIT_SUCCESS : EXIT_FAILURE;

        // TODO(titouan, thibautbremand) better error handling
        } catch (const lexical_error & e) {
            cerr << e.what() << endl;
            t.shift();  // Next token
            // No return statement: try again
        } catch (const compile_error & e) {
            cerr << e.what() << endl;
            t.shift();  // Next token
            // No return statement: try again

        // Errors that are not compile or lexical 
        } catch (const std::runtime_error & e) {
            cerr << e.what() << endl;

            return EXIT_SUCCESS;
        }
    }*/
}
