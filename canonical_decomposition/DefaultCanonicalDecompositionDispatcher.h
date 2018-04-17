//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_DEFAULTCANONICALDECOMPOSITIONDISPATCHER_H
#define PROJECT_DEFAULTCANONICALDECOMPOSITIONDISPATCHER_H


#include "CanonicalDecompositionDispatcher.h"

class DefaultCanonicalDecompositionDispatcher : public CanonicalDecompositionDispatcher {
public:

    ~DefaultCanonicalDecompositionDispatcher() override = default;


    std::unique_ptr<CanonicalDecompositionStrategy> getCanonicalDecompositionStrategy(const FocalElement &df) override;


    std::unique_ptr<CanonicalDecompositionDispatcher> clone() override;
};


#endif //PROJECT_DEFAULTCANONICALDECOMPOSITIONDISPATCHER_H
