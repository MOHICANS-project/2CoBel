//
// Created by nicola on 30/11/17.
//

#ifndef FUSION_GENERICFOCALELEMENTARRAYBUILDER_H
#define FUSION_GENERICFOCALELEMENTARRAYBUILDER_H


#include "FocalElementContainer.h"

/**
 * @class GenericFocalElementContainer
 * A container for any type of FocalElement object.
 */
class GenericFocalElementContainer : public FocalElementContainer {

    std::vector<std::unique_ptr<FocalElement>> fes;
    std::vector<double> mms;

public:

    GenericFocalElementContainer() = default;

    ~GenericFocalElementContainer() override = default;

    GenericFocalElementContainer(const GenericFocalElementContainer &other) = delete;

    GenericFocalElementContainer &operator=(const GenericFocalElementContainer &other)= delete;

    GenericFocalElementContainer(GenericFocalElementContainer &&other) = default;

    GenericFocalElementContainer &operator=(GenericFocalElementContainer &&other)= default;

    void clear() override;

    void push(std::unique_ptr<FocalElement> elem, double mass) override;


    void
    push(std::unique_ptr<FocalElement> elem, double mass, const std::function<double(double, double)> &acc) override;

    const std::vector<std::unique_ptr<FocalElement>> &getFocalElementsArray() override;

    const std::vector<double> &getMassArray() override;

    void set(int index, double mass) override;

    void set(const FocalElement &fe, double mass) override;

    double get(const FocalElement &fe) override;

    void erase(int index) override;

    void erase(const FocalElement &fe) override;

    bool contains(const FocalElement &fe) override;

    int indexOf(const FocalElement &fe) override;

    size_t size() const override;
};


#endif //FUSION_GENERICFOCALELEMENTARRAYBUILDER_H
