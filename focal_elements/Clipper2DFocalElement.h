//
// Created by nicola on 17/11/17.
//

#ifndef FUSION_CLIPPER2DFOCALELEMENT_H
#define FUSION_CLIPPER2DFOCALELEMENT_H


#include <src/evidential/geometry/ClipperPolygon.h>
#include "FocalElement.h"

/**
 * @class Clipper2DFocalElement
 * 2D FocalElement using the <a href='http://www.angusj.com/delphi/clipper.php'>Clipper library</a>
 */
class Clipper2DFocalElement : public FocalElement {

    std::vector<Geometry::ClipperPolygon> polygons;

public:
    /**
     * Constructor.
     * @param polygons Array of polygons.
     */
    explicit Clipper2DFocalElement(std::vector<Geometry::ClipperPolygon> polygons);

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

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    void print(std::ostream &os) const override;
};


#endif //FUSION_CLIPPER2DFOCALELEMENT_H
