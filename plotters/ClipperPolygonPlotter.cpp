//
// Created by nicola on 22/03/18.
//

#include "../focal_elements/Clipper2DFocalElement.h"
#include "ClipperPolygonPlotter.h"
#include "matplotlib-cpp-master/matplotlibcpp.h"

namespace plt = matplotlibcpp;

std::string colors[] = {"r", "g", "b", "c", "m", "y", "k"};


void ClipperPolygonPlotter::plotClipperFocalElements(std::vector<std::reference_wrapper<FocalElement>> &elems) {
    for (int j = 0; j < elems.size(); ++j) {
        auto el = dynamic_cast<const Clipper2DFocalElement &>(elems[j].get());
        auto polys = el.getPolygons();
        for (int k = 0; k < polys.size(); ++k) {
            Geometry::ClipperPolygon &pol = polys[k];
            std::vector<long> xs;
            std::vector<long> ys;
            for (int l = 0; l < pol.getPolygon().size(); ++l) {
                xs.push_back(pol.getPolygon()[l].X);
                ys.push_back(pol.getPolygon()[l].Y);
            }
            xs.push_back(pol.getPolygon()[0].X);
            ys.push_back(pol.getPolygon()[0].Y);

            plt::fill(xs, ys, colors[j % 7], 0.5);


        }
    }
    //plt::axis("equal");
    plt::show();
}
