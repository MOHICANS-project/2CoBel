//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_GENERICFOCALELEMENTARRAYBUILDER_H
#define FUSION_GENERICFOCALELEMENTARRAYBUILDER_H


#include "FocalElementArrayBuilder.h"

class GenericFocalElementArrayBuilder : public FocalElementArrayBuilder {

    std::vector<std::unique_ptr<FocalElement>> fes;
    std::vector<double> mms;

public:

    GenericFocalElementArrayBuilder() = default;

    ~GenericFocalElementArrayBuilder() override = default;

    GenericFocalElementArrayBuilder(const GenericFocalElementArrayBuilder &other) = delete;

    GenericFocalElementArrayBuilder &operator=(const GenericFocalElementArrayBuilder &other)= delete;

    GenericFocalElementArrayBuilder(GenericFocalElementArrayBuilder &&other) = default;

    GenericFocalElementArrayBuilder &operator=(GenericFocalElementArrayBuilder &&other)= default;

    void clear() override;

    void push(std::unique_ptr<FocalElement> elem, double mass) override;

    std::vector<std::unique_ptr<FocalElement>> getFocalElementsArray() override;

    std::vector<double> getMassArray() override;

};


#endif //FUSION_GENERICFOCALELEMENTARRAYBUILDER_H
