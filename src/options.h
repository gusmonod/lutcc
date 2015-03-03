// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_OPTIONS_H_
#define SRC_OPTIONS_H_

#include "./constants.h"

#include "boost/program_options.hpp"

int get_options_map(int argc, const char * argv[],
            boost::program_options::variables_map *vm);

#endif  // SRC_OPTIONS_H_
