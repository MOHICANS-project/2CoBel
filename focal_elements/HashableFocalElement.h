//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_HASHABLEFOCALELEMENT_H
#define FUSION_HASHABLEFOCALELEMENT_H

#include <iostream>
#include "FocalElement.h"


class HashableFocalElement : public FocalElement {
public:

    virtual size_t hash() const =0;
};

namespace std {
    template<>
    class hash<std::unique_ptr<HashableFocalElement>> {
    public:
        size_t operator()(const std::unique_ptr<HashableFocalElement> &fe) const {
            return fe->hash();
        }
    };

    template<>
    class equal_to<std::unique_ptr<HashableFocalElement>> {
    public:
        bool operator()(const std::unique_ptr<HashableFocalElement> &h1,
                        const std::unique_ptr<HashableFocalElement> &h2) const {
            return *h1 == *h2;
        }
    };
}

#endif //FUSION_HASHABLEFOCALELEMENT_H
