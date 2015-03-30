// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./options.h"

#include <iostream>
#include <string>
#include <sstream>

#include "boost/program_options.hpp"

using std::cout;
using std::cerr;
using std::string;
using std::endl;

int get_options_map(int argc, const char * argv[],
            boost::program_options::variables_map *vm) {
    namespace po = boost::program_options;

    try {
        std::stringstream desc;
        desc << "Lutin analyzer and executer" << endl
             << "---------------------------" << endl << endl
             << "Usage: " << argv[0] << " <lutin-file> [options]" << endl
             << "Allowed options";

        po::options_description visible_opts(desc.str());
        visible_opts.add_options()
            ("help,h", ": Prints this help")
            ("print,p", ": Prints the internal representation")
            ("analyze,a", ": Analyzes code and outputs error to stderr")
            ("exec,e", ": Executes the given Lutin program")
            ("optim1,1", ": Propagates constants and neutral elements")
            ("optim2,o", ": Propagates variables with constant value");

        po::positional_options_description pos_opts;
        pos_opts.add("lutin-file", -1);

        // Hidden options, will be allowed but not shown in help
        po::options_description hidden_opts("Hidden options");
        hidden_opts.add_options()
            ("lutin-file", po::value<string>()->required(),
                    ": Sets the input lutin file to analyze");

        try {
            po::options_description allowed_opts;
            allowed_opts.add(visible_opts).add(hidden_opts);

                po::store(po::command_line_parser(argc, argv).
                        options(allowed_opts).positional(pos_opts).run(), *vm);

                po::notify(*vm);  // throws error if any

            // --help option
            if (vm->count("help")) {
                cout << visible_opts;
                std::exit(EXIT_FAILURE);
            }
        } catch(po::error& e) {
            cerr << "ERROR: " << e.what() << endl << endl;
            cerr << visible_opts;
            return EXIT_FAILURE;
        }
    } catch(std::exception& e) {
        cerr << "Unhandled Exception reached the top of main: "
        << e.what() << ", application will now exit" << endl;
        return EXIT_FAILURE;
    }

    // Can continue the program
    return EXIT_SUCCESS;
}
