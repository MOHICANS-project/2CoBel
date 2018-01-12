//
// Created by nicola on 13/11/17.
//

#ifndef FUSION_GENERICPOLYGON_H
#define FUSION_GENERICPOLYGON_H

#include <stdexcept>
#include "Shape.h"
#include "../../third_party/clipper/cpp/clipper.hpp"

namespace Geometry {
    class ClipperPolygon : public Shape {

        ClipperLib::Path polygon;


    public:
        explicit ClipperPolygon(std::vector<Point2D> &vertices);

        explicit ClipperPolygon(ClipperLib::Path polygon);

        ~ClipperPolygon() override = default;

        double computeArea() const override;

        bool has_inside(Point2D p) const override;


        const ClipperLib::Path &getPolygon() const;

        friend std::ostream &operator<<(std::ostream &os, const ClipperPolygon &polygon);

    private:
        bool equal_to(Shape const &rhs) const override;

    };
}


#endif //FUSION_GENERICPOLYGON_H
