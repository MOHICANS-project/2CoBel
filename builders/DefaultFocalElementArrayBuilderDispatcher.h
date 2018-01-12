//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H
#define FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H


#include <iostream>
#include "FocalElementArrayBuilderDispatcher.h"

class DefaultFocalElementArrayBuilderDispatcher : public FocalElementArrayBuilderDispatcher {
public:
    std::unique_ptr<FocalElementArrayBuilder> getBuilder(const FocalElement &el) override;

    std::unique_ptr<FocalElementArrayBuilderDispatcher> clone() override;
};


#endif //FUSION_DEFAULTFOCALELEMENTARRAYBUILDERFACTORY_H
