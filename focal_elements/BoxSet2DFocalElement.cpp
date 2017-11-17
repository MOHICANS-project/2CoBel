//
// Created by nicola on 16/11/17.
//

#include <iostream>
#include "BoxSet2DFocalElement.h"


BoxSet2DFocalElement::BoxSet2DFocalElement(const std::vector<Geometry::Rectangle> &boxes) : boxes(boxes) {
    int xmin = INT32_MAX, xmax = INT32_MIN, ymin = INT32_MAX, ymax = INT32_MIN;
    for (const auto &box : boxes) {
        if (box.getXmin() < xmin)xmin = box.getXmin();
        else if (box.getXmax() > xmax)xmax = box.getXmax();
        if (box.getYmin() < ymin)ymin = box.getYmin();
        else if (box.getYmax() > ymax)ymax = box.getYmax();
    }
}


double BoxSet2DFocalElement::cardinality() const {
    double card = 0;
    for (auto &box : boxes) {
        card += box.computeArea();
    }
    return card;
}

bool BoxSet2DFocalElement::equal_to(FocalElement const &rhs) const {
    auto rhsr = static_cast<const BoxSet2DFocalElement &>(rhs);

    if (bounding_box != rhsr.getBounding_box() || cardinality() != rhsr.cardinality())return false;
    for (const auto &box : boxes) {
        Geometry::Point2D to_test[] = {Geometry::Point2D(box.getXmin(), box.getYmin()),
                                       Geometry::Point2D(box.getXmin(), box.getYmax()),
                                       Geometry::Point2D(box.getXmax(), box.getYmin()),
                                       Geometry::Point2D(box.getXmax(), box.getYmax())};
        for (const auto &p : to_test) {
            bool included = false;
            for (const auto &box2 : rhsr.getBoxes()) {
                if (box2.has_inside(p)) {
                    included = true;
                    break;
                }
            }
            if (!included)return false;
        }
    }
    for (const auto &box : rhsr.getBoxes()) {
        Geometry::Point2D to_test[] = {Geometry::Point2D(box.getXmin(), box.getYmin()),
                                       Geometry::Point2D(box.getXmin(), box.getYmax()),
                                       Geometry::Point2D(box.getXmax(), box.getYmin()),
                                       Geometry::Point2D(box.getXmax(), box.getYmax())};
        for (const auto &p : to_test) {
            bool included = false;
            for (const auto &box2 : boxes) {
                if (box2.has_inside(p)) {
                    included = true;
                    break;
                }
            }
            if (!included)return false;
        }
    }
    return true;
}

bool BoxSet2DFocalElement::is_inside(FocalElement const &rhs) const {
    auto rhsr = static_cast<const BoxSet2DFocalElement &>(rhs);
    return bounding_box.intersect(rhsr.getBounding_box())==bounding_box && *intersect(rhsr) == *this;
}

std::unique_ptr<FocalElement> BoxSet2DFocalElement::do_intersection(FocalElement const &rhs) const {
    auto rhsr = static_cast<const BoxSet2DFocalElement &>(rhs);

    std::vector<Geometry::Rectangle> new_boxes;
    for (const auto &box : boxes) {
        for (const auto &box2 : rhsr.getBoxes()) {
            Geometry::Rectangle rect = box.intersect(box2);
            if (rect.computeArea() > 0)new_boxes.push_back(rect);
        }
    }
    return std::unique_ptr<FocalElement>(new BoxSet2DFocalElement(new_boxes));

}

std::unique_ptr<FocalElement> BoxSet2DFocalElement::do_union(FocalElement const &rhs) const {
    auto rhsr = static_cast<const BoxSet2DFocalElement &>(rhs);

    std::vector<Geometry::Rectangle> new_boxes = boxes;
    std::vector<bool> to_delete(boxes.size(), false);
    for (const auto &box : rhsr.getBoxes()) {
        std::vector<Geometry::Rectangle> to_add;
        for (int i = 0; i < new_boxes.size(); ++i) {
            if (!to_delete[i]) {

                const Geometry::Rectangle &box_original = new_boxes[i];
                if (box_original.intersect(box).computeArea() > 0) {
                    //delete original box
                    std::vector<Geometry::Rectangle> diff = box_original.difference(box);
                    to_delete[i] = true;
                    for (const auto &item : diff) {
                        to_add.push_back(item);
                    }
                }
            }
        }
        for (auto &add : to_add) {
            new_boxes.push_back(add);
            to_delete.push_back(false);
        }
    }
    std::vector<Geometry::Rectangle> real_new_boxes;
    for (int j = 0; j < new_boxes.size(); ++j) {
        if (!to_delete[j]) real_new_boxes.push_back(new_boxes[j]);
    }
    for (const auto &box : rhsr.getBoxes()) {
        real_new_boxes.push_back(box);
    }


    return std::unique_ptr<FocalElement>(new BoxSet2DFocalElement(real_new_boxes));

}

void BoxSet2DFocalElement::simplify_contigous() {
    std::vector<bool> box_valid(boxes.size(), true);
    for (int i = 0; i < boxes.size(); ++i) {
        if (box_valid[i]) {
            int l0 = boxes[i].getXmin();
            int c0 = boxes[i].getYmin();
            int l2 = boxes[i].getXmax();
            int c2 = boxes[i].getYmax();
            for (int j = i + 1; j < boxes.size(); ++j) {
                if (box_valid[j]) {
                    int ll0 = boxes[j].getXmin();
                    int cc0 = boxes[j].getYmin();
                    int ll2 = boxes[j].getXmax();
                    int cc2 = boxes[j].getYmax();
                    if ((ll0 == l0 && ll2 == l2 && (cc0 == c2 || cc0 == c2 + 1 || cc2 == c0 || cc2 == c0 - 1)) ||
                        (cc0 == c0 && cc2 == c2 && (ll0 == l2 || ll0 == l2 + 1 || ll2 == l0 || ll2 == l0 - 1))) {
                        l0 = std::min(l0, ll0);
                        l2 = std::max(l2, ll2);
                        c0 = std::min(c0, cc0);
                        c2 = std::max(c2, cc2);
                        box_valid[j] = false;
                    }
                }
            }
            boxes[i].setXmin(l0);
            boxes[i].setXmax(l2);
            boxes[i].setYmin(c0);
            boxes[i].setYmax(c2);
        }
    }
    std::vector<Geometry::Rectangle> new_set;
    for (int k = 0; k < boxes.size(); ++k) {
        if (box_valid[k])new_set.push_back(boxes[k]);
    }
    if (new_set.size() != boxes.size())boxes = new_set;

}


void BoxSet2DFocalElement::simplify_included() {
    std::vector<bool> supp(boxes.size(), false);
    int numsupp = 0;
    for (int i = 0; i < boxes.size(); ++i) {
        if (!supp[i]) {
            Geometry::Rectangle &box = boxes[i];
            Geometry::Point2D to_test[] = {Geometry::Point2D(box.getXmin(), box.getYmin()),
                                           Geometry::Point2D(box.getXmin(), box.getYmax()),
                                           Geometry::Point2D(box.getXmax(), box.getYmin()),
                                           Geometry::Point2D(box.getXmax(), box.getYmax())};
            for (int j = 0; j < boxes.size(); ++j) {
                if (j != i) {
                    bool included = true;
                    for (auto &p : to_test) {
                        if (!boxes[j].has_inside(p)) {
                            included = false;
                            break;
                        }
                    }
                    if (included) {
                        supp[i] = true;
                        numsupp++;
                    }
                }
            }
        }
    }
    std::vector<bool> verif(boxes.size(), false);
    for (int i = 0; i < boxes.size(); ++i) {
        if (supp[i] && !verif[i]) {
            verif[i] = true;
            for (int j = i + 1; j < boxes.size(); ++j) {
                if (supp[j] && !verif[j] && boxes[i] == boxes[j]) {
                    verif[j] = true;
                    supp[i] = false;
                    numsupp--;
                }
            }
        }
    }
    if (numsupp > 0) {
        std::vector<Geometry::Rectangle> new_set;
        for (int i = 0; i < boxes.size(); ++i)
            if (!supp[i])new_set.push_back(boxes[i]);
        boxes = new_set;
    }

}


const std::vector<Geometry::Rectangle> &BoxSet2DFocalElement::getBoxes() const {
    return boxes;
}

const Geometry::Rectangle &BoxSet2DFocalElement::getBounding_box() const {
    return bounding_box;
}

size_t BoxSet2DFocalElement::getNumBoxes() {
    return boxes.size();
}

std::unique_ptr<FocalElement> BoxSet2DFocalElement::clone() const {
    return std::unique_ptr<FocalElement>(new BoxSet2DFocalElement(boxes));
}






