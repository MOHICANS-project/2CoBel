//
// Created by nicola on 17/11/17.
//

#ifndef FUSION_EIGENMAT2DFOCALELEMENT_H
#define FUSION_EIGENMAT2DFOCALELEMENT_H

#include <Eigen/Dense>
#include <src/evidential/geometry/Rectangle.h>
#include "FocalElement.h"
namespace EigenFE{

    typedef Eigen::Matrix< bool , Eigen::Dynamic , Eigen::Dynamic > MatrixXb;

class EigenMat2DFocalElement: public FocalElement {


    Geometry::Rectangle bounding_box;
    MatrixXb image;

public:
    EigenMat2DFocalElement(const Geometry::Rectangle &bounding_box, const MatrixXb &image);

    ~EigenMat2DFocalElement() override =default;

    double cardinality() const override;

    const Geometry::Rectangle &getBounding_box() const;

    const MatrixXb &getImage() const;

    std::unique_ptr<FocalElement> clone() const override ;

private:
    bool equal_to(FocalElement const &rhs) const override ;
    bool is_inside(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;
    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const  override;


};

}
#endif //FUSION_EIGENMAT2DFOCALELEMENT_H
