//
// Created by nicola on 13/11/17.
//

#ifndef FUSION_CONSTRUCTORARGUMENTSERROR_H
#define FUSION_CONSTRUCTORARGUMENTSERROR_H


#include <stdexcept>

class ConstructorArgumentsError : public std::runtime_error {
public:
    explicit ConstructorArgumentsError(const char* msg) : runtime_error(msg) {}

};


#endif //FUSION_CONSTRUCTORARGUMENTSERROR_H
