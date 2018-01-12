//
// Created by nicola on 11/01/18.
//

#ifndef FUSION_COMPOSITEFOCALELEMENT_H
#define FUSION_COMPOSITEFOCALELEMENT_H


#include <src/evidential/errors/ConstructorArgumentsError.h>
#include "FocalElement.h"
#include "ConflictFocalElement.h"

class CompositeFocalElement : public FocalElement {

    std::unique_ptr<FocalElement> left;
    std::unique_ptr<FocalElement> right;

public:
    explicit CompositeFocalElement(std::unique_ptr<FocalElement> _left, std::unique_ptr<FocalElement> _right) : left(
            std::move(_left)), right(std::move(_right)) {
        ConflictFocalElement empty_set;
        if (*left == empty_set && *right == empty_set)
            throw ConstructorArgumentsError("One of the two arguments must not be an empty set.");
    }

    CompositeFocalElement(const CompositeFocalElement &other);

    CompositeFocalElement &operator=(const CompositeFocalElement &other);


    ~CompositeFocalElement() override = default;


    double cardinality() const override;

    const std::unique_ptr<FocalElement> &getLeft() const;

    const std::unique_ptr<FocalElement> &getRight() const;

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    void print(std::ostream &os) const;
};


#endif //FUSION_COMPOSITEFOCALELEMENT_H
