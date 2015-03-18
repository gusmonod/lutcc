// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./simpletoken.h"

/*virtual*/ Token * SimpleOperator::newCopy() const {
    return new SimpleOperator(static_cast<Token::Id>(*this));
}

/*virtual*/ Token * Keyword::newCopy() const {
    return new Keyword(static_cast<Token::Id>(*this));
}
