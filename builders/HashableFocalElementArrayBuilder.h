//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H
#define FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H


#include "FocalElementArrayBuilder.h"


class HashableFocalElementArrayBuilder : public FocalElementArrayBuilder {

    std::unordered_map<std::unique_ptr<HashableFocalElement>, double> mms;
    std::vector<std::unique_ptr<FocalElement>> fes;
    std::vector<double> masses;

    void storeResults();

public:

    HashableFocalElementArrayBuilder() = default;

    ~HashableFocalElementArrayBuilder() override = default;

    HashableFocalElementArrayBuilder(const HashableFocalElementArrayBuilder &other) = delete;

    HashableFocalElementArrayBuilder &operator=(const HashableFocalElementArrayBuilder &other)= delete;

    HashableFocalElementArrayBuilder(HashableFocalElementArrayBuilder &&other) = default;

    HashableFocalElementArrayBuilder &operator=(HashableFocalElementArrayBuilder &&other)= default;

    void clear() override;

    void push(std::unique_ptr<FocalElement> elem, double mass) override;

    std::vector<std::unique_ptr<FocalElement>> getFocalElementsArray() override;

    std::vector<double> getMassArray() override;
};


#endif //FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H
