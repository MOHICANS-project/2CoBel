//
// Created by nicola on 13/11/17.
//


#include <cmath>
#include "../errors/ConstructorArgumentsError.h"
#include <utility>
#include <iostream>
#include "ClipperPolygon.h"


namespace Geometry {

    ClipperPolygon::ClipperPolygon(std::vector<Point2D> &vertices) {
        if (vertices.size() <= 2)throw ConstructorArgumentsError("A closed polygon should have at least 3 vertices");
        for (const auto &vertex : vertices) {
            polygon << ClipperLib::IntPoint(vertex.X(), vertex.Y());
        }
        ClipperLib::CleanPolygon(polygon, 10e-3);
    }

    ClipperPolygon::ClipperPolygon(ClipperLib::Path polygon) : polygon(std::move(polygon)) {
        ClipperLib::CleanPolygon(this->polygon, 10e-3);
    }


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
        for (int i = 1; i < path.size(); ++i) {
            os << " , " << path[i];
        }
        os << " }";
        return os;
    }

    size_t ClipperPolygon::hash() const {
        //find index of top-left
        long Ymin = INT64_MAX;
        long Xrel = -1;
        size_t start_idx = 0;

        size_t N = polygon.size();
        for (size_t i = 0; i < polygon.size(); ++i) {
            if (polygon[i].Y < Ymin) {
                Ymin = polygon[i].Y;
                Xrel = polygon[i].X;
                start_idx = i;
                continue;
            }
            if (polygon[i].Y == Ymin && polygon[i].X < Xrel) {
                Xrel = polygon[i].X;
                start_idx = i;
            }
        }
        size_t seed = N;
        for (size_t j = 0; j < N; ++j) {
            Geometry::Point2D p(polygon[(j + start_idx) % N].X, polygon[(j + start_idx) % N].Y);
            seed ^= p.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        return seed;
    }

    ClipperPolygon ClipperPolygon::approximateDisk(Point2D center, unsigned long radius, unsigned int num_vertices) {
        double r = (1.0 * radius) / (cos(M_PI / num_vertices));
        ClipperLib::Path poly;
        for (int i = 0; i < num_vertices; ++i) {
            double xd = ceil(center.X() + r * cos(i * 2.0 * M_PI / num_vertices));
            double yd = ceil(center.Y() + r * sin(i * 2.0 * M_PI / num_vertices));
            poly << ClipperLib::IntPoint(static_cast<long>(xd), static_cast<long>(yd));
        }
        return ClipperPolygon(poly);
    }

    Point2D ClipperPolygon::getBarycenter() const {

        double A = 0;
        for (int i = 0; i < polygon.size(); ++i) {
            A += polygon[i].X * polygon[(i + 1) % polygon.size()].Y -
                 polygon[i].Y * polygon[(i + 1) % polygon.size()].X;
        }
        A /= 2;
        double xmean = 0, ymean = 0;
        for (int j = 0; j < polygon.size(); ++j) {
            xmean += (polygon[j].X + polygon[(j + 1) % polygon.size()].X) *
                     (polygon[j].X * polygon[(j + 1) % polygon.size()].Y -
                      polygon[j].Y * polygon[(j + 1) % polygon.size()].X);
            ymean += (polygon[j].Y + polygon[(j + 1) % polygon.size()].Y) *
                     (polygon[j].X * polygon[(j + 1) % polygon.size()].Y -
                      polygon[j].Y * polygon[(j + 1) % polygon.size()].X);
        }
        xmean /= (6 * A);
        ymean /= (6 * A);
        return Point2D(static_cast<long>(xmean), static_cast<long>(ymean));
    }


}


