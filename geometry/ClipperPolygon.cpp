//
// Created by nicola on 13/11/17.
//


#include <cmath>
#include <src/evidential/errors/ConstructorArgumentsError.h>
#include <utility>
#include "ClipperPolygon.h"


namespace Geometry {

    ClipperPolygon::ClipperPolygon(std::vector<Point2D> &vertices) {
        if (vertices.size() <= 2)throw ConstructorArgumentsError("A closed polygon should have at least 3 vertices");
        for (const auto &vertex : vertices) {
            polygon << ClipperLib::IntPoint(vertex.X(), vertex.Y());
        }
    }

    ClipperPolygon::ClipperPolygon(ClipperLib::Path polygon) : polygon(std::move(polygon)) {}


    double ClipperPolygon::computeArea() const {
        return ClipperLib::Area(polygon);
    }


    bool ClipperPolygon::has_inside(const Point2D p) const {
        return ClipperLib::PointInPolygon(ClipperLib::IntPoint(p.X(), p.Y()), polygon) == 1;
    }

    bool ClipperPolygon::equal_to(Shape const &rhs) const {
        auto rhsr = static_cast<const ClipperPolygon &>(rhs);
        if (polygon.size() != rhsr.getPolygon().size())return false;
        ClipperLib::IntPoint init = polygon[0];
        //look for it in the other path
        const ClipperLib::Path &rhs_polygon = rhsr.getPolygon();
        size_t index = rhs_polygon.size();
        for (size_t i = 0; i < rhs_polygon.size(); ++i) {
            if (rhs_polygon[i] == init) {
                index = i;
                break;
            }
        }
        if (index == rhs_polygon.size())return false;
        for (size_t i = 0; i < polygon.size(); ++i) {
            if (polygon[i] != rhs_polygon[(i + index) % polygon.size()])return false;
        }
        return true;

    }

    const ClipperLib::Path &ClipperPolygon::getPolygon() const {
        return polygon;
    }

    std::ostream &operator<<(std::ostream &os, const ClipperPolygon &polygon) {

        const ClipperLib::Path &path = polygon.getPolygon();
        os << "{ " << path[0];
        for (int i = 0; i < path.size(); ++i) {
            os << " , " << path[i];
        }
        os << " }";
        return os;
    }


}


