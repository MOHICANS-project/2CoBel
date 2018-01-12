//
// Created by nicola on 17/11/17.
//

#ifndef FUSION_EMPTYFOCALELEMENT_H
#define FUSION_EMPTYFOCALELEMENT_H

#include "FocalElement.h"

class ConflictFocalElement : public FocalElement {

public:
    explicit ConflictFocalElement() = default;

    ~ConflictFocalElement() override = default;

    double cardinality() const override {
        return 0;
    }

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override {
        return std::vector<std::unique_ptr<FocalElement>>();
    }

    std::unique_ptr<FocalElement> clone() const override {
        return std::unique_ptr<FocalElement>(new ConflictFocalElement());
    }

private:
    bool equal_to(FocalElement const &rhs) const override {
        return true;
    }

    bool is_inside(FocalElement const &rhs) const override {
        return true;
    }

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override {
        return std::unique_ptr<FocalElement>(new ConflictFocalElement());
    }

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override {
        return rhs.clone();
    }

    void print(std::ostream &os) const {
        os << "Empty set";
    }


};


#endif //FUSION_EMPTYFOCALELEMENT_H
