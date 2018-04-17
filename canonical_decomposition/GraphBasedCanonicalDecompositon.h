//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_GRAPHBASEDCANONICALDECOMPOSITON_H
#define PROJECT_GRAPHBASEDCANONICALDECOMPOSITON_H


#include "CanonicalDecompositionStrategy.h"

class GraphBasedCanonicalDecompositon : public CanonicalDecompositionStrategy {
public:
    std::unique_ptr<FocalElementContainer>
    getCanonicalDecomposition(const FocalElementContainer &fe_container, const FocalElement &df, double ignorance,
                              double conflict) override;
};


#endif //PROJECT_GRAPHBASEDCANONICALDECOMPOSITON_H
