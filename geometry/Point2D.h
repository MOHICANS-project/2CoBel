//
// Created by nicola on 10/11/17.
//

#ifndef FUSION_POINT2D_H
#define FUSION_POINT2D_H


#include <ostream>

namespace Geometry {

    /**
     * @class Point2D
     * 2D point in integer coordinates.
     */
    class Point2D {

        int x;
        int y;

    public:
        /**
         * Construcor.
         * @param x
         * @param y
         */
        explicit Point2D(int x = 0, int y = 0) : x(x), y(y) {}

        virtual ~Point2D() = default;

        /**
         * Get x.
         * @return x.
         */
        int X() const {
            return x;
        }

        /**
         * Get y.
         * @return y.
         */
        int Y() const {
            return y;
        }

        bool operator==(const Point2D &rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const Point2D &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const Point2D &d) {
            os << "[" << d.x << "," << d.y << "]";
            return os;
        }

    };
}

#endif //FUSION_POINT2D_H
