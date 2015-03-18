// Copyright (c) 2015 FAT-GYFT, MIT License

#include "./simpletoken.h"

/*virtual*/ Token * SimpleOperator::newCopy() const {
    return new SimpleOperator(this->id());
}

/*virtual*/ Token * Keyword::newCopy() const {
    return new Keyword(this->id());
}
