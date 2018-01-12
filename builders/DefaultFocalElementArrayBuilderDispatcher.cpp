//
// Created by nicola on 30/11/17.
//

#include <src/evidential/focal_elements/HashableFocalElement.h>
#include "DefaultFocalElementArrayBuilderDispatcher.h"
#include "GenericFocalElementArrayBuilder.h"
#include "HashableFocalElementArrayBuilder.h"

std::unique_ptr<FocalElementArrayBuilder>
DefaultFocalElementArrayBuilderDispatcher::getBuilder(const FocalElement &el) {

    auto *rhs = dynamic_cast<const HashableFocalElement *>(&el);
    if (rhs != nullptr) return std::unique_ptr<FocalElementArrayBuilder>(new HashableFocalElementArrayBuilder());
    return std::unique_ptr<FocalElementArrayBuilder>(new GenericFocalElementArrayBuilder());

}

std::unique_ptr<FocalElementArrayBuilderDispatcher> DefaultFocalElementArrayBuilderDispatcher::clone() {
    return std::unique_ptr<FocalElementArrayBuilderDispatcher>(new DefaultFocalElementArrayBuilderDispatcher());
}


