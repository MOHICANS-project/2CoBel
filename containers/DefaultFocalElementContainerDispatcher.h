//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H
#define FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H


#include <iostream>
#include "FocalElementContainerDispatcher.h"

/**
 * @class DefaultFocalElementContainerDispatcher
 * The default dispatcher for FocalElementContainer. It distinguishes between generic and hashable FocalElement.
 */
class DefaultFocalElementContainerDispatcher : public FocalElementContainerDispatcher {

    static const int MIN_DIM;

public:
    std::unique_ptr<FocalElementContainer> getContainer(const FocalElement &el) const override;

    std::unique_ptr<FocalElementContainerDispatcher> clone() override;
};


#endif //FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H
