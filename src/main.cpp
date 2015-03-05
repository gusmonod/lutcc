// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "boost/program_options.hpp"

#include "./tokens.h"
#include "./options.h"
#include "./lexical.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
    cout << MulExpr(3) << endl;
    boost::program_options::variables_map vm;

    get_options_map(argc, argv, &vm);

    std::ifstream file;
    file.open(vm["lutin-file"].as<string>());
    if (!file) {
        std::cerr << "Could not open " << vm["lutin-file"].as<string>();
        std::exit(EXIT_FAILURE);
    }

    Tokenizer t(&file);

    while (t.has_next()) {
        cout << t.top() << endl;
        t.shift();
    }

    return EXIT_SUCCESS;
}
