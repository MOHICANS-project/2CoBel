//
// Created by nicola on 15/11/17.
//

#ifndef FUSION_INCOMPATIBLETYPEERROR_H
#define FUSION_INCOMPATIBLETYPEERROR_H
#include <stdexcept>

/**
 * @class IncompatibleTypeError
 * Error in the type of an object.
 */
class IncompatibleTypeError : public std::runtime_error {
public:
    explicit IncompatibleTypeError(const char* msg) : runtime_error(msg) {}

};
#endif //FUSION_INCOMPATIBLETYPEERROR_H
