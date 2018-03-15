//
// Created by nicola on 14/11/17.
//

#ifndef FUSION_FOCALELEMENT_H
#define FUSION_FOCALELEMENT_H

#include "src/evidential/errors/IncompatibleTypeError.h"
#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <ostream>

/**
 * @class FocalElement
 * Interface for a focal element set.
 */
class FocalElement {


public:

    virtual ~FocalElement() = default;

    /**
     * Evaluate the cardinality of the focal element.
     * @return Cardinality of the set.
     */
    virtual double cardinality() const =0;


    bool operator==(const FocalElement &rhs) const {
        if (typeid(*this) != typeid(rhs)) return false;
        if ((this->isEmpty() && rhs.isEmpty())) return true;
        if ((!this->isEmpty() && rhs.isEmpty()) || (this->isEmpty() && !rhs.isEmpty())) return false;
        return this->equal_to(rhs);
    }

    bool operator!=(const FocalElement &rhs) const {
        return !(*this == rhs);
    }

    /**
     * Checks whether the FocalElement is inside another.
     * @param other The other FocalElment.
     * @return True if the current FocalElement is inside the other.
     */
    bool inside(const FocalElement &other) const {
        if (typeid(*this) != typeid(other)) return false;
        if (this->isEmpty()) return true;
        if (other.isEmpty()) return false;
        return this->is_inside(other);
    }

    /**
     * Checks whether the FocalElement contains another.
     * @param other The other FocalElment.
     * @return True if the current FocalElement contains the other.
     */
    bool contains(const FocalElement &other) const {
        return (typeid(*this) == typeid(other)) && other.inside(*this);
    }

    /**
     * Compute the intersection with another FocalElement.
     * @param other The FocalElement to intersect with.
     * @return The resulting intersection FocalElement.
     */


    std::unique_ptr<FocalElement> intersect(const FocalElement &other) const {
        if (typeid(*this) != typeid(other))
            throw IncompatibleTypeError("Intersection is not supported for focal elements of different type");
        if (this->isEmpty()) return this->clone();
        if (other.isEmpty()) return other.clone();
        return do_intersection(other);
    };

    /**
     * Compute the union with another FocalElement.
     * @param other The FocalElement to unite with.
     * @return The resulting union FocalElement.
     */
    std::unique_ptr<FocalElement> unite(const FocalElement &other) const {
        if (typeid(*this) != typeid(other))
            throw IncompatibleTypeError("Union is not supported for focal elements of different type");
        if (this->isEmpty()) return other.clone();
        if (other.isEmpty()) return this->clone();
        return do_union(other);
    }

    /**
     * Extract the singleton FocalElements included in the current one.
     * @param step_size The sampling resulution. Step_size=1 means that all the singletons are retrieved. Step_size > 1 means that singletons are sampled for performance reason.
     * @return Array of singleton FocalElements.
     */
    virtual std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size) const =0;

    /**
     * Clone method.
     * @return A copy of the current FocalElement.
     */
    virtual std::unique_ptr<FocalElement> clone() const =0;

    friend std::ostream &operator<<(std::ostream &os, const FocalElement &element) {
        element.print(os);
        return os;
    }

    /**
     * Check whether the current FocalElement is the empty set.
     * @return True if the object is the empty set.
     */
    virtual bool isEmpty() const =0;

    /**
     * Clear the FocalElement, resulting into the empty set.
     */
    virtual void clear()=0;

private:
    virtual bool equal_to(const FocalElement &rhs) const =0;

    virtual bool is_inside(const FocalElement &rhs) const =0;

    virtual std::unique_ptr<FocalElement> do_intersection(const FocalElement &other) const =0;

    virtual std::unique_ptr<FocalElement> do_union(const FocalElement &other) const =0;

    virtual void print(std::ostream &os) const =0;

};

#endif //FUSION_FOCALELEMENT_H
