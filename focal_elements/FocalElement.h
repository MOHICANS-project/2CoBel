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

class FocalElement {


public:

    virtual ~FocalElement() = default;

    virtual double cardinality() const =0;


    bool operator==(const FocalElement &rhs) {
        return (typeid(*this) == typeid(rhs)) && this->equal_to(rhs);
    }

    bool operator!=(const FocalElement &rhs) {
        return !(*this == rhs);
    }

    bool inside(const FocalElement &other) const {
        return (typeid(*this) == typeid(other)) && this->is_inside(other);
    }

    bool contains(const FocalElement &other) const {
        return (typeid(*this) == typeid(other)) && other.inside(*this);
    }

    std::unique_ptr<FocalElement> intersect(const FocalElement &other) const {
        if (typeid(*this) != typeid(other))
            throw IncompatibleTypeError("Intersection is not supported for focal elements of different type");
        return do_intersection(other);
    };

    std::unique_ptr<FocalElement> unite(const FocalElement &other) const {
        if (typeid(*this) != typeid(other))
            throw IncompatibleTypeError("Union is not supported for focal elements of different type");
        return do_union(other);
    }

    virtual std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size) const =0;

    virtual std::unique_ptr<FocalElement> clone() const =0;

    friend std::ostream &operator<<(std::ostream &os, const FocalElement &element) {
        element.print(os);
        return os;
    }

private:
    virtual bool equal_to(const FocalElement &rhs) const =0;

    virtual bool is_inside(const FocalElement &rhs) const =0;

    virtual std::unique_ptr<FocalElement> do_intersection(const FocalElement &other) const =0;

    virtual std::unique_ptr<FocalElement> do_union(const FocalElement &other) const =0;

    virtual void print(std::ostream &os) const =0;

};

#endif //FUSION_FOCALELEMENT_H
