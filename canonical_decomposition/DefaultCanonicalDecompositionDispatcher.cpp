//
// Created by nicola on 17/04/18.
//

#include <focal_elements/UnidimensionalFocalElement.h>
#include "DefaultCanonicalDecompositionDispatcher.h"
#include "UnidimensionalCanonicalDecomposition.h"
#include "GraphBasedCanonicalDecompositon.h"

std::unique_ptr<CanonicalDecompositionStrategy>
DefaultCanonicalDecompositionDispatcher::getCanonicalDecompositionStrategy(const FocalElement &df) {
    auto *rhs = dynamic_cast<const UnidimensionalFocalElement *>(&df);
    if (rhs != nullptr)
        return std::unique_ptr<CanonicalDecompositionStrategy>(new UnidimensionalCanonicalDecomposition());
    return std::unique_ptr<CanonicalDecompositionStrategy>(new GraphBasedCanonicalDecompositon());
}

std::unique_ptr<CanonicalDecompositionDispatcher> DefaultCanonicalDecompositionDispatcher::clone() {
    return std::unique_ptr<CanonicalDecompositionDispatcher>(new DefaultCanonicalDecompositionDispatcher());
}
