//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_CANONICALDECOMPOSITIONSTRATEGY_H
#define PROJECT_CANONICALDECOMPOSITIONSTRATEGY_H


#include "../containers/FocalElementContainer.h"


class CanonicalDecompositionStrategy {
public:
    virtual std::unique_ptr<FocalElementContainer>
    getCanonicalDecomposition(const FocalElementContainer &fe_container, const FocalElement &df, double ignorance,
                              double conflict)=0;
};


#endif //PROJECT_CANONICALDECOMPOSITIONSTRATEGY_H
