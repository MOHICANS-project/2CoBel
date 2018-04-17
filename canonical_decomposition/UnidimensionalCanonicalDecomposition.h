//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_UNIDIMENSIONALCANONICALDECOMPOSITION_H
#define PROJECT_UNIDIMENSIONALCANONICALDECOMPOSITION_H


#include "CanonicalDecompositionStrategy.h"

class UnidimensionalCanonicalDecomposition : public CanonicalDecompositionStrategy {
public:
    std::unique_ptr<FocalElementContainer>
    getCanonicalDecomposition(const FocalElementContainer &fe_container, const FocalElement &df, double ignorance,
                              double conflict) override;
};


#endif //PROJECT_UNIDIMENSIONALCANONICALDECOMPOSITION_H
