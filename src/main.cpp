// Copyright (c) 2015 FAT-GYFT, MIT License

#include <iostream>
#include <string>
#include <sstream>
#include "boost/regex.hpp"
#include "boost/program_options.hpp"

using std::cout;
using std::cerr;
using std::string;
using std::endl;
using boost::regex_match;

enum ReturnCode {
    SUCCESS = 0,
    ARG_ERROR,
    EXCEPTION_ERROR,
    MATCH_ERROR
};

int options(int argc, const char * argv[],
        boost::program_options::variables_map *vm);

int lexical(const string & token);

int main(int argc, const char * argv[]) {
    boost::program_options::variables_map vm;

    int ret = options(argc, argv, &vm);
    if (SUCCESS != ret) return ret;

    if (vm.count("token")) {
       ret = lexical(vm["token"].as<string>());
       if (SUCCESS != ret) return ret;
    }

    cout << "file path: " << vm["lutin-file"].as<string>() << endl;

    return SUCCESS;
}

int options(int argc, const char * argv[],
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
            ("lutin-file", po::value<string>()->required(),
                    ": Sets the input lutin file to analyze")
            ("token,t", po::value<string>(), ": Token to analyze");

        try {
            po::options_description allowed_opts;
            allowed_opts.add(visible_opts).add(hidden_opts);

            po::store(po::command_line_parser(argc, argv).
                    options(allowed_opts).positional(pos_opts).run(), *vm);

            po::notify(*vm);  // throws error if any

            /** --help option
             */
            if (vm->count("help")) {
                cout << visible_opts;
                return SUCCESS;
            }
        } catch(po::error& e) {
            cerr << "ERROR: " << e.what() << endl << endl;
            cerr << visible_opts;
            return ARG_ERROR;
        }

        // application code here //
    } catch(std::exception& e) {
        cerr << "Unhandled Exception reached the top of main: "
        << e.what() << ", application will now exit" << endl;
        return EXCEPTION_ERROR;
    }

    return SUCCESS;
}

int lexical(const string & token) {
    // Keywords regular expressions
    boost::regex co("^const");
    boost::regex va("^var");
    boost::regex ec("^ecrire");
    boost::regex li("^lire");

    // Operator regular expressions
    boost::regex af("^:=");
    boost::regex op("^\\+|-|\\*|/");

    // Operands regular expressions
    boost::regex id("^[a-zA-Z][a-zA-Z0-9_-]*");
    boost::regex nu("^[0-9]+");

    cout << '"' << token << "\" is ";

    if (regex_match(token, co)) {
        cout << "recognized as the keyword `const`" << endl;
        return SUCCESS;
    }
    if (regex_match(token, va)) {
        cout << "recognized as the keyword `var`" << endl;
        return SUCCESS;
    }
    if (regex_match(token, ec)) {
        cout << "recognized as the keyword `ecrire`" << endl;
        return SUCCESS;
    }
    if (regex_match(token, li)) {
        cout << "recognized as the keyword `lire`" << endl;
        return SUCCESS;
    }



    if (regex_match(token, af)) {
        cout << "recognized as the operator `:=`" << endl;
        return SUCCESS;
    }
    if (regex_match(token, op)) {
        cout << "recognized as one of the operators"
        << "`+`, `-`, `*` and `/`" << endl;
        return SUCCESS;
    }



    if (regex_match(token, id)) {
        cout << "recognized as a variable name" << endl;
        return SUCCESS;
    }
    if (regex_match(token, nu)) {
        cout << "recognized as a positive integer" << endl;
        return SUCCESS;
    }

    cout << "not recognized" << endl;
    return MATCH_ERROR;
}
