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

    get_options_map(argc, argv, &vm);

    std::ifstream file;
    file.open(vm["lutin-file"].as<string>());
    if (!file) {
        std::cerr << "Could not open " << vm["lutin-file"].as<string>();
        std::exit(EXIT_FAILURE);
    }

    Automaton accepter;
    Tokenizer t(&file);
    if (accepter.accepts(&t)) {
        cout << "Accepted!!" << endl;
    } else {
        cout << "Not accepted" << endl;
    }

	std::exit(EXIT_SUCCESS);  // Segmentation fault??

    return EXIT_SUCCESS;
}
