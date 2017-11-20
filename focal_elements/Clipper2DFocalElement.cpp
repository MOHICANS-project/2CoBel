//
// Created by nicola on 17/11/17.
//

#include "Clipper2DFocalElement.h"



Clipper2DFocalElement::Clipper2DFocalElement(std::vector<Geometry::ClipperPolygon> polygons) : polygons(std::move(
        polygons)) {}

const std::vector<Geometry::ClipperPolygon> &Clipper2DFocalElement::getPolygons() const {
    return polygons;
}

double Clipper2DFocalElement::cardinality() const {
    double sum=0.0;
    for (auto &&polygon : polygons) {
        sum+=polygon.computeArea();
    }
    return sum;
}

std::unique_ptr<FocalElement> Clipper2DFocalElement::clone() const {
    return std::unique_ptr<FocalElement>(new Clipper2DFocalElement(polygons));
}

bool Clipper2DFocalElement::equal_to(FocalElement const &rhs) const {
    return cardinality()==rhs.cardinality() && intersect(rhs)->cardinality()==cardinality();
}

bool Clipper2DFocalElement::is_inside(FocalElement const &rhs) const {
    return cardinality() <= rhs.cardinality() && *intersect(rhs) == *this;
}

std::unique_ptr<FocalElement> Clipper2DFocalElement::do_intersection(FocalElement const &rhs) const {
    auto rhsr= static_cast<const Clipper2DFocalElement&>(rhs);
    ClipperLib::Clipper c;
    for (const auto &polygon : polygons) {
        c.AddPath(polygon.getPolygon(),ClipperLib::ptSubject,true);
    }
    for (const auto &polygon : rhsr.getPolygons()) {
        c.AddPath(polygon.getPolygon(),ClipperLib::ptClip,true);
    }
    ClipperLib::Paths solution;
    c.Execute(ClipperLib::ctIntersection,solution);
    std::vector<Geometry::ClipperPolygon> outvec;
    for (auto &path : solution) {
        outvec.emplace_back(path);
    }
    return std::unique_ptr<FocalElement>(new Clipper2DFocalElement(outvec));
}

std::unique_ptr<FocalElement> Clipper2DFocalElement::do_union(FocalElement const &rhs) const {
    auto rhsr= static_cast<const Clipper2DFocalElement&>(rhs);
    ClipperLib::Clipper c;
    for (const auto &polygon : polygons) {
        c.AddPath(polygon.getPolygon(),ClipperLib::ptSubject,true);
    }
    for (const auto &polygon : rhsr.getPolygons()) {
        c.AddPath(polygon.getPolygon(),ClipperLib::ptClip,true);
    }
    ClipperLib::Paths solution;
    c.Execute(ClipperLib::ctUnion,solution);
    std::vector<Geometry::ClipperPolygon> outvec;
    for (auto &path : solution) {
        outvec.emplace_back(path);
    }
    return std::unique_ptr<FocalElement>(new Clipper2DFocalElement(outvec));
}


