//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_FOCALELEMENTARRAYBUILDER_H
#define FUSION_FOCALELEMENTARRAYBUILDER_H

#include <vector>
#include <src/evidential/focal_elements/FocalElement.h>

class FocalElementArrayBuilder {


public:
    virtual ~FocalElementArrayBuilder() = default;

    virtual void clear()=0;

    virtual void push(std::unique_ptr<FocalElement> elem, double mass)=0;

    virtual std::vector<std::unique_ptr<FocalElement>> getFocalElementsArray()=0;

    virtual std::vector<double> getMassArray()=0;

};

#endif //FUSION_FOCALELEMENTARRAYBUILDER_H
