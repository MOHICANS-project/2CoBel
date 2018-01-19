//
// Created by nicola on 30/11/17.
//

#include <src/evidential/focal_elements/HashableFocalElement.h>
#include "DefaultFocalElementContainerDispatcher.h"
#include "GenericFocalElementContainer.h"
#include "HashableFocalElementContainer.h"


std::unique_ptr<FocalElementContainer>
DefaultFocalElementContainerDispatcher::getContainer(const FocalElement &el) {

    auto *rhs = dynamic_cast<const HashableFocalElement *>(&el);
    if (rhs != nullptr) return std::unique_ptr<FocalElementContainer>(new HashableFocalElementContainer());
    return std::unique_ptr<FocalElementContainer>(new GenericFocalElementContainer());

}

std::unique_ptr<FocalElementContainerDispatcher> DefaultFocalElementContainerDispatcher::clone() {
    return std::unique_ptr<FocalElementContainerDispatcher>(new DefaultFocalElementContainerDispatcher());
}


