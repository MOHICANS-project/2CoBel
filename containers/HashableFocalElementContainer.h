//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H
#define FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H


#include "FocalElementContainer.h"

/**
 * @class HashableFocalElementContainer
 * A container for HashableFocalElement type objects.
 *
 */
class HashableFocalElementContainer : public FocalElementContainer {

    std::unordered_map<HashableFocalElement *, size_t> mms;
    std::vector<std::unique_ptr<FocalElement>> fes;
    std::vector<double> masses;


public:

    HashableFocalElementContainer() = default;

    ~HashableFocalElementContainer() override = default;

    HashableFocalElementContainer(const HashableFocalElementContainer &other) = delete;

    HashableFocalElementContainer &operator=(const HashableFocalElementContainer &other)= delete;

    HashableFocalElementContainer(HashableFocalElementContainer &&other) = default;

    HashableFocalElementContainer &operator=(HashableFocalElementContainer &&other)= default;

    void clear() override;

    void push(std::unique_ptr<FocalElement> elem, double mass) override;

    const std::vector<std::unique_ptr<FocalElement>> &getFocalElementsArray() override;

    const std::vector<double> &getMassArray() override;

    void set(int index, double mass) override;

    void set(const FocalElement &fe, double mass) override;

    double get(const FocalElement &fe) override;

    void erase(int index) override;

    void erase(const FocalElement &fe) override;
};


#endif //FUSION_HASHABLEFOCALELEMENTARRAYBUILDER_H
