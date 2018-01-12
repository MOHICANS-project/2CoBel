//
// Created by nicola on 17/11/17.
//

#include <src/evidential/errors/ConstructorArgumentsError.h>
#include "EigenMat2DFocalElement.h"
#include "ConflictFocalElement.h"

namespace EigenFE {


    EigenMat2DFocalElement::EigenMat2DFocalElement(const Geometry::Rectangle &bounding_box, const MatrixXb &image)
            : bounding_box(bounding_box), image(image) {
        if ((bounding_box.getXmax() - bounding_box.getXmin() + 1) != image.cols() ||
            (bounding_box.getYmax() - bounding_box.getYmin() + 1) != image.rows() || bounding_box.computeArea() == 0)
            throw ConstructorArgumentsError(
                    "EigenMat2DFocalElement: bounding box must have the same size has the image.");

    }

    double EigenMat2DFocalElement::cardinality() const {
        unsigned int count = 0;
        for (int i = 0; i < image.rows(); ++i) {
            for (int j = 0; j < image.cols(); ++j) {
                if (image(i, j))count++;
            }
        }
        return count;
    }

    const Geometry::Rectangle &EigenMat2DFocalElement::getBounding_box() const {
        return bounding_box;
    }

    const MatrixXb &EigenMat2DFocalElement::getImage() const {
        return image;
    }

    bool EigenMat2DFocalElement::equal_to(FocalElement const &rhs) const {

        auto rhsr = static_cast<const EigenMat2DFocalElement &>(rhs);
        if (bounding_box != rhsr.getBounding_box())return false;

        for (int i = 0; i < image.rows(); ++i) {
            for (int j = 0; j < image.cols(); ++j) {

                if (image(i, j) != rhsr.getImage()(i, j))return false;
            }
        }

        return true;
    }

    bool EigenMat2DFocalElement::is_inside(FocalElement const &rhs) const {
        return rhs.cardinality() >= cardinality() && *intersect(rhs) == *this;
    }

    std::unique_ptr<FocalElement> EigenMat2DFocalElement::do_intersection(FocalElement const &rhs) const {
        auto rhsr = static_cast<const EigenMat2DFocalElement &>(rhs);
        Geometry::Rectangle new_bounding_box = bounding_box.intersect(rhsr.getBounding_box());

        if (new_bounding_box.computeArea() == 0)
            return std::unique_ptr<FocalElement>(new EigenMat2DFocalElement(new_bounding_box, MatrixXb::Zero(1, 1)));

        int x_off1 = bounding_box.getXmin() - new_bounding_box.getXmin();
        int y_off1 = -bounding_box.getYmax() + new_bounding_box.getYmax();

        int x_off2 = rhsr.getBounding_box().getXmin() - new_bounding_box.getXmin();
        int y_off2 = -rhsr.getBounding_box().getYmax() + new_bounding_box.getYmax();

        int numrows = new_bounding_box.getYmax() - new_bounding_box.getYmin() + 1;
        int numcols = new_bounding_box.getXmax() - new_bounding_box.getXmin() + 1;
        MatrixXb new_image(numrows, numcols);
        bool isempty = true;
        for (int i = 0; i < numrows; ++i) {
            for (int j = 0; j < numcols; ++j) {
                new_image(i, j) = image(i - y_off1, j - x_off1) & rhsr.getImage()(i - y_off2, j - x_off2);
                if (isempty && new_image(i, j)) isempty = false;
            }
        }
        if (isempty)return std::unique_ptr<FocalElement>(new ConflictFocalElement());
        return std::unique_ptr<FocalElement>(new EigenMat2DFocalElement(new_bounding_box, new_image));
    }

    std::unique_ptr<FocalElement> EigenMat2DFocalElement::do_union(FocalElement const &rhs) const {
        auto rhsr = static_cast<const EigenMat2DFocalElement &>(rhs);
        Geometry::Rectangle new_bounding_box(std::min(bounding_box.getXmin(), rhsr.getBounding_box().getXmin()),
                                             std::max(bounding_box.getXmax(), rhsr.getBounding_box().getXmax()),
                                             std::min(bounding_box.getYmin(), rhsr.getBounding_box().getYmin()),
                                             std::max(bounding_box.getYmax(), rhsr.getBounding_box().getYmax()));

        int x_off1 = bounding_box.getXmin() - new_bounding_box.getXmin();
        int y_off1 = -bounding_box.getYmax() + new_bounding_box.getYmax();

        int x_off2 = rhsr.getBounding_box().getXmin() - new_bounding_box.getXmin();
        int y_off2 = -rhsr.getBounding_box().getYmax() + new_bounding_box.getYmax();

        int numrows = new_bounding_box.getYmax() - new_bounding_box.getYmin() + 1;
        int numcols = new_bounding_box.getXmax() - new_bounding_box.getXmin() + 1;
        MatrixXb new_image(numrows, numcols);

        for (int i = 0; i < numrows; ++i) {
            for (int j = 0; j < numcols; ++j) {
                bool val1 = false;
                if ((i - y_off1) >= 0 && (i - y_off1) < image.rows() && (j - x_off1) >= 0 &&
                    (j - x_off1) < image.cols())
                    val1 = image(i - y_off1, j - x_off1);
                bool val2 = false;
                if ((i - y_off2) >= 0 && (i - y_off2) < rhsr.getImage().rows() && (j - x_off2) >= 0 &&
                    (j - x_off2) < rhsr.getImage().cols())
                    val2 = rhsr.getImage()(i - y_off2, j - x_off2);
                new_image(i, j) = val1 | val2;
            }
        }

        return std::unique_ptr<FocalElement>(new EigenMat2DFocalElement(new_bounding_box, new_image));
    }

    std::unique_ptr<FocalElement> EigenMat2DFocalElement::clone() const {
        return std::unique_ptr<FocalElement>(new EigenMat2DFocalElement(bounding_box, image));
    }

    std::vector<std::unique_ptr<FocalElement>> EigenMat2DFocalElement::getInnerSingletons(int step_size) const {
        std::vector<std::unique_ptr<FocalElement>> singletons;
        for (int i = 0; i < image.rows(); i += step_size)
            for (int j = 0; j < image.cols(); j += step_size)
                if (image(i, j)) {
                    int xmin = bounding_box.getXmin() + i;
                    int ymax = bounding_box.getYmax() - j;
                    MatrixXb new_image(1, 1);
                    new_image(0, 0) = true;
                    singletons.push_back(std::unique_ptr<FocalElement>(
                            new EigenMat2DFocalElement(Geometry::Rectangle(xmin, xmin + 1, ymax - 1, ymax),
                                                       new_image)));
                }
        return singletons;
    }

    void EigenMat2DFocalElement::print(std::ostream &os) const {
        for (int i = 0; i < image.rows(); ++i) {
            for (int j = 0; j < image.cols(); ++j) {

                os << image(i, j) << " ";
            }
            os << std::endl;
        }
    }

}