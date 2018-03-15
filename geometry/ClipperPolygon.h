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
         * @param vertices Array of ordered Point2D vertices composing the polygon. For positive areas computation, the order MUST be counterclockwise.
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

        size_t hash() const;

        /**
         * Get the polygon.
         * @return The polygon.
         */
        const ClipperLib::Path &getPolygon() const;


        friend std::ostream &operator<<(std::ostream &os, const ClipperPolygon &polygon);

        /**
         * Build the approximation of a disk by a regular polygon.
         * @param center Center of the disk.
         * @param radius Radius of the disk.
         * @param num_vertices Numebr of vertices of the regular polygon.
         * @return The approximated disk.
         */
        static ClipperPolygon approximateDisk(Point2D center, unsigned long radius, unsigned int num_vertices);

        /**
         * Get the polygon's barycenter.
         * @return Barycenter point.
         */
        Point2D getBarycenter() const;


    private:
        bool equal_to(Shape const &rhs) const override;

    };
}


#endif //FUSION_GENERICPOLYGON_H
