//
// Created by nicola on 14/11/17.
//

#ifndef FUSION_UNIDIMENSIONALFOCALELEMENT_H
#define FUSION_UNIDIMENSIONALFOCALELEMENT_H


#include <type_traits>
#include "FocalElement.h"

class UnidimensionalFocalElement: public FocalElement {

    unsigned long long ID;


public:
    explicit UnidimensionalFocalElement(unsigned long long ID) : ID(ID) {}
    ~UnidimensionalFocalElement() override =default;

    double cardinality() const override;
    unsigned long long getID() const;


private:
    bool equal_to(FocalElement const &rhs) const override ;
    bool is_inside(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const  override;
    std::unique_ptr<FocalElement> clone() const override;


};




#endif //FUSION_UNIDIMENSIONALFOCALELEMENT_H
