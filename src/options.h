// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_OPTIONS_H_
#define SRC_OPTIONS_H_

#include "boost/program_options.hpp"

int get_options_map(int argc, const char * argv[],
            boost::program_options::variables_map *vm);

enum ProgramMode {
    NONE,
    EXECUTION
};

class Config {
 public:

    static ProgramMode CurrentMode() {return Config::m_currentMode;}
    // Ensures that the mode can't be modified once the program started
    static bool SetCurrentMode(ProgramMode pm) {
        if(m_currentMode == ProgramMode::NONE) {
            m_currentMode = pm;
            return true;
        }
        return false;
    }

 private:
    static ProgramMode m_currentMode;
};

#endif  // SRC_OPTIONS_H_
