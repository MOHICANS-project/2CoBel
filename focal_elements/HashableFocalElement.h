//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_HASHABLEFOCALELEMENT_H
#define FUSION_HASHABLEFOCALELEMENT_H

#include <iostream>
#include "FocalElement.h"

/**
 * @class HashableFocalElement
 * FocalElement for which an hash function for fast retrieval can be defined.
 */
class HashableFocalElement : public FocalElement {
public:
    /**
     * Compute the hash function.
     * @return The value of the hash.
     */
    virtual size_t hash() const =0;
};

namespace std {
    template<>
    class hash<HashableFocalElement *> {
    public:
        size_t operator()(const HashableFocalElement *fe) const {
            return fe->hash();
        }
    };

    template<>
    class equal_to<HashableFocalElement *> {
    public:
        bool operator()(const HashableFocalElement *h1,
                        const HashableFocalElement *h2) const {
            return *h1 == *h2;
        }
    };
}

#endif //FUSION_HASHABLEFOCALELEMENT_H
