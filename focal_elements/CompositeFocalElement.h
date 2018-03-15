//
// Created by nicola on 11/01/18.
//

#ifndef FUSION_COMPOSITEFOCALELEMENT_H
#define FUSION_COMPOSITEFOCALELEMENT_H


#include "../errors/ConstructorArgumentsError.h"
#include "FocalElement.h"

/**
 * @class CompositeFocalElement
 * FocalElement in a cross product space.
 */
class CompositeFocalElement : public FocalElement {

    std::unique_ptr<FocalElement> left;
    std::unique_ptr<FocalElement> right;

public:
    /**
     * Constructor.
     * @param _left Left FocalElement
     * @param _right Right FocalElement
     */
    explicit CompositeFocalElement(std::unique_ptr<FocalElement> _left, std::unique_ptr<FocalElement> _right) : left(
            std::move(_left)), right(std::move(_right)) {}

    CompositeFocalElement(const CompositeFocalElement &other);

    CompositeFocalElement &operator=(const CompositeFocalElement &other);


    ~CompositeFocalElement() override = default;


    double cardinality() const override;

    /**
     * Get the left FocalElement
     * @return The left FocalElement
     */
    const std::unique_ptr<FocalElement> &getLeft() const;

    /**
     * Get the right FocalElement
     * @return The right FocalElement
     */
    const std::unique_ptr<FocalElement> &getRight() const;

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

    bool isEmpty() const override;

    void clear() override;

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    void print(std::ostream &os) const override;
};


#endif //FUSION_COMPOSITEFOCALELEMENT_H
