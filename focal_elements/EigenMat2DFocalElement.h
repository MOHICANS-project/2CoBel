//
// Created by nicola on 17/11/17.
//

#ifndef FUSION_EIGENMAT2DFOCALELEMENT_H
#define FUSION_EIGENMAT2DFOCALELEMENT_H

#include <Eigen/Dense>
#include <src/evidential/geometry/Rectangle.h>
#include "FocalElement.h"
/**
 * @namespace EigenFE
 * Eigen library focal elements' namespace.
 */
namespace EigenFE {
    /**
     * @typedef
     * Dynamic sized matrix of booleans.
     */
    typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> MatrixXb;

    /**
     * @class EigenMat2DFocalElement
     * 2D FocalElement using a matrix of booleans.
     */
    class EigenMat2DFocalElement : public FocalElement {


        Geometry::Rectangle bounding_box;
        MatrixXb image;

    public:
        /**
         * Constructor.
         * @param bounding_box Bounding box in the discernment frame coordinates.
         * @param image Matrix of booleans.
         */
        EigenMat2DFocalElement(const Geometry::Rectangle &bounding_box, const MatrixXb &image);

        ~EigenMat2DFocalElement() override = default;

        double cardinality() const override;

        /**
         * Get the bounding box.
         * @return Bounding box rectangle.
         */
        const Geometry::Rectangle &getBounding_box() const;

        /**
         * Get the image
         * @return The matrix of booleans.
         */
        const MatrixXb &getImage() const;

        std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

        std::unique_ptr<FocalElement> clone() const override;

        bool isEmpty() const override;

        void clear() override;


    private:
        bool equal_to(FocalElement const &rhs) const override;

        bool is_inside(FocalElement const &rhs) const override;

        std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

        std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

        void print(std::ostream &os) const override;

    };

}
#endif //FUSION_EIGENMAT2DFOCALELEMENT_H
