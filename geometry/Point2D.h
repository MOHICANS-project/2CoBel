//
// Created by nicola on 10/11/17.
//

#ifndef FUSION_POINT2D_H
#define FUSION_POINT2D_H


#include <ostream>
#include <cmath>

namespace Geometry {

    /**
     * @class Point2D
     * 2D point in integer coordinates.
     */
    class Point2D {

        long x;
        long y;

    public:
        /**
         * Construcor.
         * @param x
         * @param y
         */
        explicit Point2D(long x = 0, long y = 0) : x(x), y(y) {}

        virtual ~Point2D() = default;

        /**
         * Get x.
         * @return x.
         */
        long X() const {
            return x;
        }

        /**
         * Get y.
         * @return y.
         */
        long Y() const {
            return y;
        }

        bool operator==(const Point2D &rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const Point2D &rhs) const {
            return !(rhs == *this);
        }

        Point2D operator+(const Point2D &rhs) const {
            Point2D result(x + rhs.x, y + rhs.y);
            return result;
        }

        Point2D& operator+=(const Point2D &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Point2D operator-(const Point2D &rhs) const {
            Point2D result(x - rhs.x, y - rhs.y);
            return result;
        }

        Point2D& operator-=(const Point2D &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        double norm() const {
            return sqrt(x * x + y * y);
        }

        size_t hash() const {
            size_t h = 17;
            h = ((h + x) << 5) - (h + x);
            h = ((h + y) << 5) - (h + y);
            return h;
        }

        friend std::ostream &operator<<(std::ostream &os, const Point2D &d) {
            os << "[" << d.x << "," << d.y << "]";
            return os;
        }


    };
}

#endif //FUSION_POINT2D_H
