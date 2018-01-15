//
// Created by nicola on 11/01/18.
//

#include "CompositeFocalElement.h"
#include "ConflictFocalElement.h"

double CompositeFocalElement::cardinality() const {
    return right->cardinality() * left->cardinality();
}

std::vector<std::unique_ptr<FocalElement>> CompositeFocalElement::getInnerSingletons(int step_size) const {
    std::vector<std::unique_ptr<FocalElement>> left_singletons = left->getInnerSingletons(step_size);
    std::vector<std::unique_ptr<FocalElement>> right_singletons = right->getInnerSingletons(step_size);

    std::vector<std::unique_ptr<FocalElement>> outv;
    for (const auto &sl : left_singletons) {
        for (const auto &sr : right_singletons) {
            outv.push_back(std::unique_ptr<FocalElement>(new CompositeFocalElement(sl->clone(), sr->clone())));
        }
    }
    return outv;
}

bool CompositeFocalElement::equal_to(FocalElement const &rhs) const {
    auto rhsr = static_cast<const CompositeFocalElement &>(rhs);
    return (*left == *rhsr.getLeft()) && (*right == *rhsr.getRight());
}

const std::unique_ptr<FocalElement> &CompositeFocalElement::getLeft() const {
    return left;
}

const std::unique_ptr<FocalElement> &CompositeFocalElement::getRight() const {
    return right;
}

bool CompositeFocalElement::is_inside(FocalElement const &rhs) const {
    auto rhsr = static_cast<const CompositeFocalElement &>(rhs);
    ConflictFocalElement conf;
    return (left->inside(*rhsr.getLeft()) || *left == conf) && (right->inside(*rhsr.getRight()) || *right == conf);
}

std::unique_ptr<FocalElement> CompositeFocalElement::do_intersection(FocalElement const &rhs) const {
    auto rhsr = static_cast<const CompositeFocalElement &>(rhs);
    std::unique_ptr<FocalElement> empty(new ConflictFocalElement());
    std::unique_ptr<FocalElement> leftinter = (*left == *empty || *rhsr.getLeft() == *empty) ? empty->clone()
                                                                                             : left->intersect(
                    *rhsr.getLeft());
    std::unique_ptr<FocalElement> rightinter = (*right == *empty || *rhsr.getRight() == *empty) ? empty->clone()
                                                                                                : right->intersect(
                    *rhsr.getRight());
    if (*leftinter == *empty && *rightinter == *empty)return std::move(empty);
    return std::unique_ptr<FocalElement>(new CompositeFocalElement(std::move(leftinter), std::move(rightinter)));
}

std::unique_ptr<FocalElement> CompositeFocalElement::do_union(FocalElement const &rhs) const {
    auto rhsr = static_cast<const CompositeFocalElement &>(rhs);
    std::unique_ptr<FocalElement> empty(new ConflictFocalElement());
    std::unique_ptr<FocalElement> leftun =
            *left == *empty ? rhsr.getLeft()->clone() : (*rhsr.getLeft() == *empty ? left->clone() : left->unite(
                    *rhsr.getLeft()));
    std::unique_ptr<FocalElement> rightun =
            *right == *empty ? rhsr.getRight()->clone() : (*rhsr.getRight() == *empty ? right->clone() : right->unite(
                    *rhsr.getRight()));
    return std::unique_ptr<FocalElement>(new CompositeFocalElement(std::move(leftun), std::move(rightun)));
}

std::unique_ptr<FocalElement> CompositeFocalElement::clone() const {
    return std::unique_ptr<FocalElement>(new CompositeFocalElement(left->clone(), right->clone()));
}

void CompositeFocalElement::print(std::ostream &os) const {
    os << "(" << *right << "," << *left << ")";
}

CompositeFocalElement::CompositeFocalElement(const CompositeFocalElement &other) {
    right = other.getRight()->clone();
    left = other.getLeft()->clone();
}

CompositeFocalElement &CompositeFocalElement::operator=(const CompositeFocalElement &other) {
    if (*this != other) {
        right = other.getRight()->clone();
        left = other.getLeft()->clone();
    }
    return *this;
}




