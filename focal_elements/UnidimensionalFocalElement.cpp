//
// Created by nicola on 14/11/17.
//

#include <cxcore.h>
#include <bitset>
#include "UnidimensionalFocalElement.h"



unsigned long long UnidimensionalFocalElement::getID() const {
    return ID;
}


double UnidimensionalFocalElement::cardinality() const {
    std::bitset<sizeof(unsigned long long) * CHAR_BIT> b(ID);
    return b.count();
}

bool UnidimensionalFocalElement::equal_to(FocalElement const &rhs) const {
    auto rhsr= static_cast<const UnidimensionalFocalElement&>(rhs);
    return ID==rhsr.ID;
}

bool UnidimensionalFocalElement::is_inside(FocalElement const &rhs) const {
    auto rhsr= static_cast<const UnidimensionalFocalElement&>(rhs);
    return ((ID&rhsr.getID())==ID);
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::do_intersection(FocalElement const &rhs) const {

    auto rhsr= static_cast<const UnidimensionalFocalElement&>(rhs);
    unsigned long long newID=ID&rhsr.getID();
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(newID));
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::do_union(FocalElement const &rhs)  const {

    auto rhsr= static_cast<const UnidimensionalFocalElement&>(rhs);
    unsigned long long  newID=ID|rhsr.getID();
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(newID));
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::clone() const {
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(ID));
}

