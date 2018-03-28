//
// Created by nicola on 22/03/18.
//

#ifndef FUSION_CLIPPERPOLYGONPLOTTER_H
#define FUSION_CLIPPERPOLYGONPLOTTER_H


#include <memory>
#include "../focal_elements/FocalElement.h"


class ClipperPolygonPlotter {

public:
    void plotClipperFocalElements(std::vector<std::reference_wrapper<FocalElement>> &elems);

};


#endif //FUSION_CLIPPERPOLYGONPLOTTER_H
