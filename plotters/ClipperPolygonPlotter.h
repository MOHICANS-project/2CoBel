//
// Created by nicola on 22/03/18.
//

#ifndef FUSION_CLIPPERPOLYGONPLOTTER_H
#define FUSION_CLIPPERPOLYGONPLOTTER_H


#include <memory>
#include "../focal_elements/FocalElement.h"


class ClipperPolygonPlotter {

public:
    /**
     * Plot Clipper2DFocalElements with matplotlib wrapper.
     * @param elems Focal elements array.
     */
    void plotClipperFocalElements(std::vector<std::reference_wrapper<FocalElement>> &elems);

};


#endif //FUSION_CLIPPERPOLYGONPLOTTER_H
