// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./errors.h"

#include <string>
#include <stdexcept>

const std::runtime_error undeclared_error(const std::string & name) {
    return std::runtime_error("Undeclared variable `" + name + "`");
}

const std::runtime_error undefined_error(const std::string & name) {
    return std::runtime_error("Undefined variable `" + name + "`");
}

const std::runtime_error constant_error(const std::string & name) {
    return std::runtime_error("Variable `" + name + "` can't be modified");
}

const std::runtime_error math_error(const std::string & what) {
    return std::runtime_error("Math error: " + what);
}
