//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_CANONICALDECOMPOSITIONDISPATCHER_H
#define PROJECT_CANONICALDECOMPOSITIONDISPATCHER_H


#include <memory>
#include <focal_elements/FocalElement.h>
#include "CanonicalDecompositionStrategy.h"

class CanonicalDecompositionDispatcher {
public:

    virtual ~CanonicalDecompositionDispatcher() = default;


    virtual std::unique_ptr <CanonicalDecompositionStrategy>
    getCanonicalDecompositionStrategy(const FocalElement &df)=0;


    virtual std::unique_ptr <CanonicalDecompositionDispatcher> clone()=0;

};


#endif //PROJECT_CANONICALDECOMPOSITIONDISPATCHER_H
