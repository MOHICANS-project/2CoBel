//
// Created by nicola on 16/11/17.
//

#ifndef FUSION_BOXSET2DFOCALELEMENT_H
#define FUSION_BOXSET2DFOCALELEMENT_H


#include <vector>
#include <src/evidential/geometry/Rectangle.h>
#include "FocalElement.h"

class BoxSet2DFocalElement: public FocalElement {

    std::vector<Geometry::Rectangle> boxes;
    Geometry::Rectangle bounding_box;

public:

    explicit BoxSet2DFocalElement(const std::vector<Geometry::Rectangle> &boxes);
    ~BoxSet2DFocalElement() override =default;

    double cardinality() const override;



    const std::vector<Geometry::Rectangle> &getBoxes() const;
    const Geometry::Rectangle &getBounding_box() const;

    size_t getNumBoxes();

    void simplify_contigous(); //simplify representation fusing contigous rectangles
    void simplify_included(); //simplify representation with inclusion test

    std::unique_ptr<FocalElement> clone() const override;


private:
    bool equal_to(FocalElement const &rhs) const override ;
    bool is_inside(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const  override;



};


#endif //FUSION_BOXSET2DFOCALELEMENT_H
