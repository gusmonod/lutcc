// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <sstream>

#include "./options.h"

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
            ("analyze,a", ": Static analysis outputs error to stderr")
            ("exec,e", ": Executes the given Lutin program")
            ("optim,o", ": Transforms the program by simplifying it");

        po::positional_options_description pos_opts;
        pos_opts.add("lutin-file", -1);

        // Hidden options, will be allowed but not shown in help
        po::options_description hidden_opts("Hidden options");
        hidden_opts.add_options()
            ("lutin-file", po::value<string>(),
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
                return SUCCESS;
            }
        } catch(po::error& e) {
            cerr << "ERROR: " << e.what() << endl << endl;
            cerr << visible_opts;
            return ARG_ERROR;
        }
    } catch(std::exception& e) {
        cerr << "Unhandled Exception reached the top of main: "
        << e.what() << ", application will now exit" << endl;
        return ARG_ERROR;
    }

    return SUCCESS;
}
