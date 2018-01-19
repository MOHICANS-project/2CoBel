//
// Created by nicola on 13/11/17.
//

#ifndef FUSION_GENERICPOLYGON_H
#define FUSION_GENERICPOLYGON_H

#include <stdexcept>
#include "Shape.h"
#include "../third_party/clipper/cpp/clipper.hpp"

namespace Geometry {
    /**
     * @class ClipperPolygon
     * Simple polygon representation using the <a href='http://www.angusj.com/delphi/clipper.php'>Clipper library</a>
     */
    class ClipperPolygon : public Shape {

        ClipperLib::Path polygon;


    public:
        /**
         * Constructor.
         * @param vertices Array of ordered Point2D vertices composing the polygon. For positive areas computation, the order must be clockwise.
         */
        explicit ClipperPolygon(std::vector<Point2D> &vertices);

        /**
         * Constructor.
         * @param polygon The polygon shape.
         */
        explicit ClipperPolygon(ClipperLib::Path polygon);

        ~ClipperPolygon() override = default;

        double computeArea() const override;

        bool has_inside(Point2D p) const override;

        /**
         * Get the polygon.
         * @return The polygon.
         */
        const ClipperLib::Path &getPolygon() const;

        friend std::ostream &operator<<(std::ostream &os, const ClipperPolygon &polygon);

    private:
        bool equal_to(Shape const &rhs) const override;

    };
}


#endif //FUSION_GENERICPOLYGON_H
