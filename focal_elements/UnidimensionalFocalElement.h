//
// Created by nicola on 14/11/17.
//

#ifndef FUSION_UNIDIMENSIONALFOCALELEMENT_H
#define FUSION_UNIDIMENSIONALFOCALELEMENT_H


#include <type_traits>
#include <vector>
#include <set>
#include "HashableFocalElement.h"

class UnidimensionalFocalElement : public HashableFocalElement {

    unsigned long long ID;


public:
    explicit UnidimensionalFocalElement(unsigned long long ID) : ID(ID) {}

    ~UnidimensionalFocalElement() override = default;

    double cardinality() const override;

    unsigned long long getKey() const;

    size_t hash() const override;

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

    static std::unique_ptr<UnidimensionalFocalElement> createDiscernmentFrame(unsigned char card);

    static std::unique_ptr<UnidimensionalFocalElement> createSingletonHypotesis(size_t hypotesis_number);

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    void print(std::ostream &os) const;


};


#endif //FUSION_UNIDIMENSIONALFOCALELEMENT_H
