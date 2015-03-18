// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "boost/program_options.hpp"

#include "./lexical.h"
#include "./automaton.h"
#include "./options.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
    boost::program_options::variables_map vm;

    int err = get_options_map(argc, argv, &vm);
    if (err) return err;

    std::ifstream file;
    file.open(vm["lutin-file"].as<string>());
    if (!file) {
        std::cerr << "Could not open " << vm["lutin-file"].as<string>();
        return EXIT_FAILURE;
    }

    Automaton accepter;
    Tokenizer t(&file);
    if (accepter.accepts(&t)) {
        cout << "Accepted!!" << endl;
    } else {
        cout << "Not accepted" << endl;
    }

    return EXIT_SUCCESS;
}
