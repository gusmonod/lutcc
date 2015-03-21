// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "boost/program_options.hpp"

#include "./lexical.h"
#include "./automaton.h"
#include "./options.h"
#include "./errors.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, const char * argv[]) {
    std::runtime_error a("fdsafds");

    boost::program_options::variables_map vm;

    int err = get_options_map(argc, argv, &vm);
    if (err) return err;

    std::ifstream file;
    file.open(vm["lutin-file"].as<string>());
    if (!file) {
        std::cerr << "Could not open `" << vm["lutin-file"].as<string>() << '`'
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Set the program mode
    if(vm.count("optim") && vm.count("print"))
        Config::SetCurrentMode(ProgramMode::PRINT_TRANSFORM);
    if(vm.count("optim"))
        Config::SetCurrentMode(ProgramMode::TRANSFORM);
    if(vm.count("print"))
        Config::SetCurrentMode(ProgramMode::PRINT);
    if(vm.count("exec"))
        Config::SetCurrentMode(ProgramMode::EXECUTION);
    if(vm.count("analyze"))
        Config::SetCurrentMode(ProgramMode::ANALYSIS);

    Automaton accepter;
    Tokenizer t(&file);

    try {
        bool accepted = accepter.accepts(&t);

#ifdef DEBUG
        if (accepted) {
            cout << "Accepted!!" << endl;
        } else {
            cout << "Not accepted" << endl;
        }
#endif

        return accepted ? EXIT_SUCCESS : EXIT_FAILURE;
    } catch (const compile_error & e) {
        cerr << e.what() << endl;

        return EXIT_SUCCESS;
    } catch (const std::runtime_error & e) {
        cerr << e.what() << endl;

        return EXIT_FAILURE;
    }
}
