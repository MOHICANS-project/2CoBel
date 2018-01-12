//
// Created by nicola on 10/11/17.
//

#ifndef FUSION_RECTANGLE_H
#define FUSION_RECTANGLE_H

#include <vector_types.h>
#include <vector>
#include <ostream>
#include "Point2D.h"
#include "Shape.h"

namespace Geometry {
    /**
     * Rectangle parallel to the plane axis
     */
    class Rectangle : public Shape {
        int xmin, xmax, ymin, ymax;

    public:
        Rectangle() { xmin = xmax = ymin = ymax = 0; }

        Rectangle(int xmin, int xmax, int ymin, int ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax) {}

        explicit Rectangle(std::vector<Point2D> &vertices);

        Rectangle(Point2D p1, Point2D p2);

        Rectangle(Point2D p_up_left, int width, int height);

        ~Rectangle() override = default;

        double computeArea() const override;

        bool has_inside(Point2D p) const override;

        Rectangle intersect(const Rectangle &other) const;

        std::vector<Rectangle> difference(const Rectangle &other) const;


        int getXmin() const;

        int getXmax() const;

        int getYmin() const;

        int getYmax() const;

        void setXmin(int xmin);

        void setXmax(int xmax);

        void setYmin(int ymin);

        void setYmax(int ymax);

        friend std::ostream &operator<<(std::ostream &os, const Rectangle &rectangle);


    private:
        bool equal_to(Shape const &rhs) const override;


    };

}


#endif //FUSION_RECTANGLE_H
