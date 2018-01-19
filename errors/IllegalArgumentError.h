//
// Created by nicola on 01/12/17.
//

#ifndef FUSION_ILLEGALARGUMENTERROR_H
#define FUSION_ILLEGALARGUMENTERROR_H

#include <stdexcept>

/**
 * @class IllegalArgumentError
 * Error in the parameter of a method.
 */
class IllegalArgumentError : public std::runtime_error {
public:
    explicit IllegalArgumentError(const char *msg) : runtime_error(msg) {}

};

#endif //FUSION_ILLEGALARGUMENTERROR_H
