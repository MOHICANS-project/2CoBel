//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H
#define FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H


#include "FocalElementArrayBuilder.h"

class FocalElementArrayBuilderDispatcher {
public:
    virtual ~FocalElementArrayBuilderDispatcher() = default;

    virtual std::unique_ptr<FocalElementArrayBuilder> getBuilder(const FocalElement &el)=0;

    virtual std::unique_ptr<FocalElementArrayBuilderDispatcher> clone()=0;
};


#endif //FUSION_FOCALELEMENTARRAYBUILDERFACTORY_H
