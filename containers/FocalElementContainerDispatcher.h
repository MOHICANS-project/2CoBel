//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H
#define FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H


#include "FocalElementContainer.h"

/**
 * @class FocalElementContainerDispatcher
 * Dispatcher of the correct FocalElementContainer, given the FocalElement type.
 */
class FocalElementContainerDispatcher {
public:

    virtual ~FocalElementContainerDispatcher() = default;

    /**
     * Get the container given the FocalElement type.
     * @param df Target discernment frame
     * @return The related FocalElementContainer
     */
    virtual std::unique_ptr<FocalElementContainer> getContainer(const FocalElement &df) const =0;

    /**
     * Clone method.
     * @return A copy of the FocalElementContainerDispatcher object.
     */
    virtual std::unique_ptr<FocalElementContainerDispatcher> clone()=0;
};


#endif //FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H
