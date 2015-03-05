// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./simpletoken.h"

std::ostream & operator<<(std::ostream & stream, const Token & token) {
    stream << "Token id: " << token.m_id;
    return stream;
}
