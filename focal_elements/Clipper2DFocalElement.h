//
// Created by nicola on 17/11/17.
//

#ifndef FUSION_CLIPPER2DFOCALELEMENT_H
#define FUSION_CLIPPER2DFOCALELEMENT_H


#include "../geometry/ClipperPolygon.h"
#include "HashableFocalElement.h"

/**
 * @class Clipper2DFocalElement
 * 2D FocalElement using the <a href='http://www.angusj.com/delphi/clipper.php'>Clipper library</a>
 */
class Clipper2DFocalElement : public HashableFocalElement {

    std::vector<Geometry::ClipperPolygon> polygons;


public:
    /**
     * Constructor.
     * @param polygons Array of polygons.
     */
    explicit Clipper2DFocalElement(std::vector<Geometry::ClipperPolygon> polygons);

    /**
     * Constructor.
     * @param polygons Single polygon.
     */
    explicit Clipper2DFocalElement(Geometry::ClipperPolygon polygon);

    ~Clipper2DFocalElement() override = default;

    /**
     * Get the array of polygons.
     * @return Array of ClipperPolygon objects.
     */
    const std::vector<Geometry::ClipperPolygon> &getPolygons() const;

    double cardinality() const override;

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

    bool isEmpty() const override;

    void clear() override;

    size_t hash() const override;

    /**
     * Perform polygon offsetting of the current focal element.
     * @param delta Offsetting radius.
     * @return The dilated focal element.
     */
    std::unique_ptr<FocalElement> dilate(long delta);

private:

    std::unique_ptr<FocalElement> do_operator(ClipperLib::ClipType type, FocalElement const &rhs) const;

    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_difference(FocalElement const &rhs) const override;

    void print(std::ostream &os) const override;
};


#endif //FUSION_CLIPPER2DFOCALELEMENT_H
