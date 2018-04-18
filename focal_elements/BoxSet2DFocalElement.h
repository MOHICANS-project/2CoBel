//
// Created by nicola on 16/11/17.
//

#ifndef FUSION_BOXSET2DFOCALELEMENT_H
#define FUSION_BOXSET2DFOCALELEMENT_H


#include <vector>
#include "../geometry/Rectangle.h"
#include "FocalElement.h"

/**
 * @class BoxSet2DFocalElement
 * 2D FocalElement expressed as a set of rectangles.
 */
class BoxSet2DFocalElement : public FocalElement {

    std::vector<Geometry::Rectangle> boxes;
    Geometry::Rectangle bounding_box;

public:
    /**
     * Constructor.
     * @param boxes Array of boxes.
     */
    explicit BoxSet2DFocalElement(const std::vector<Geometry::Rectangle> &boxes);

    ~BoxSet2DFocalElement() override = default;

    double cardinality() const override;

    /**
     * Get the boxes.
     * @return Array of rectangles.
     */
    const std::vector<Geometry::Rectangle> &getBoxes() const;

    /**
     * Get the bounding box. The bounding box is the smallest rectangle containing all the boxes.
     * @return The bounding box.
     */
    const Geometry::Rectangle &getBounding_box() const;

    /**
     * Get the number of boxes
     * @return Size of the boxes array.
     */
    size_t getNumBoxes();

    /**
     * Simplify representation fusing contigous rectangles.
     */
    void simplify_contigous();

    /**
     * Simplify representation with inclusion test.
     */
    void simplify_included();


    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

    bool isEmpty() const override;

    void clear() override;

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_difference(FocalElement const &rhs) const override;

    void print(std::ostream &os) const override;


};


#endif //FUSION_BOXSET2DFOCALELEMENT_H
