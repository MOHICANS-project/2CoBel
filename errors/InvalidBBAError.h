//
// Created by nicola on 17/01/18.
//

#ifndef FUSION_INVALIDBBAERROR_H
#define FUSION_INVALIDBBAERROR_H

#include <stdexcept>

/**
 * @class InvalidBBAError
 * Error in the validity of the BBA of the Evidence object in use.
 */
class InvalidBBAError : public std::runtime_error {
public:
    explicit InvalidBBAError(const char *msg) : runtime_error(msg) {}

};

#endif //FUSION_INVALIDBBAERROR_H
