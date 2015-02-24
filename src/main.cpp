// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "boost/program_options.hpp"

#include "./constants.h"
#include "./options.h"
#include "./lexical.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
    boost::program_options::variables_map vm;

    if (SUCCESS != get_options_map(argc, argv, &vm)) return ARG_ERROR;

    Tokenizer *t = nullptr;
    TokenType type;

    if (vm.count("lutin-file")) {
        std::ifstream file;
        file.open(vm["lutin-file"].as<string>());
        t = new Tokenizer(file);
    } else {
        t = new Tokenizer(std::cin);
    }

    while (t->has_next()) {
        cout << t->top(&type) << " (" << type << ')' << endl;
        t->shift();
    }

    return SUCCESS;
}
