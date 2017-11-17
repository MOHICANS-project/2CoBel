//
// Created by nicola on 10/11/17.
//

#ifndef FUSION_POLYGON_H
#define FUSION_POLYGON_H


#include <utility>
#include <vector>
#include <typeinfo>
#include "Point2D.h"

namespace Geometry{


    class Shape {

    public:
        virtual ~Shape()=default;

        virtual double computeArea() const =0;
        virtual bool has_inside(Point2D p) const=0;


        bool operator==(const Shape& rhs) const {
            return (typeid(*this) == typeid(rhs)) && this->equal_to(rhs);
        }
        bool operator!=(const Shape& rhs) const {
            return !(*this == rhs);
        }


    private:
        virtual bool equal_to(const Shape& rhs) const =0;

    };
}



#endif //FUSION_POLYGON_H
