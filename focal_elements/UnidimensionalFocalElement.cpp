//
// Created by nicola on 14/11/17.
//


#include <bitset>
#include <iostream>
#include <climits>
#include <cmath>
#include "UnidimensionalFocalElement.h"


unsigned long long UnidimensionalFocalElement::getKey() const {
    return ID;
}


double UnidimensionalFocalElement::cardinality() const {
    std::bitset<sizeof(unsigned long long) * CHAR_BIT> b(ID);
    return b.count();
}

bool UnidimensionalFocalElement::equal_to(FocalElement const &rhs) const {
    auto rhsr = static_cast<const UnidimensionalFocalElement &>(rhs);
    return ID == rhsr.getKey();
}

bool UnidimensionalFocalElement::is_inside(FocalElement const &rhs) const {
    auto rhsr = static_cast<const UnidimensionalFocalElement &>(rhs);
    return ((ID & rhsr.getKey()) == ID);
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::do_intersection(FocalElement const &rhs) const {

    auto rhsr = static_cast<const UnidimensionalFocalElement &>(rhs);
    unsigned long long newID = ID & rhsr.getKey();
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(newID));
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::do_union(FocalElement const &rhs) const {

    auto rhsr = static_cast<const UnidimensionalFocalElement &>(rhs);
    unsigned long long newID = ID | rhsr.getKey();
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(newID));
}

std::unique_ptr<FocalElement> UnidimensionalFocalElement::clone() const {
    return std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(ID));
}

std::vector<std::unique_ptr<FocalElement>> UnidimensionalFocalElement::getInnerSingletons(int step_size) const {
    std::vector<std::unique_ptr<FocalElement>> singletons;
    if (isEmpty()) return singletons;
    for (unsigned long long i = 0; i < sizeof(unsigned long long) * 8; i += step_size) {
        unsigned long long mask = 1ull << i;
        if (mask & ID)
            singletons.push_back(std::unique_ptr<FocalElement>(new UnidimensionalFocalElement(mask)));
    }
    return singletons;
}

size_t UnidimensionalFocalElement::hash() const {
    return std::hash<unsigned long long>()(ID);
}

void UnidimensionalFocalElement::print(std::ostream &os) const {
    if (isEmpty()) {
        os << "Empty set";
        return;
    }
    std::bitset<sizeof(unsigned long long) * CHAR_BIT> b(ID);
    const std::string str_bits = b.to_string();
    const auto first_digit = str_bits.find('1');
    if (first_digit != std::string::npos)
        os << str_bits.substr(first_digit);
    else os << "0";
}

std::unique_ptr<UnidimensionalFocalElement> UnidimensionalFocalElement::createDiscernmentFrame(unsigned char card) {
    auto newID = static_cast<unsigned long long>(pow(2.0, card)) - 1;
    return std::unique_ptr<UnidimensionalFocalElement>(new UnidimensionalFocalElement(newID));
}

std::unique_ptr<UnidimensionalFocalElement>
UnidimensionalFocalElement::createSingletonHypotesis(size_t hypotesis_number) {
    auto newID = static_cast<unsigned long long>(pow(2.0, hypotesis_number));
    return std::unique_ptr<UnidimensionalFocalElement>(new UnidimensionalFocalElement(newID));
}

bool UnidimensionalFocalElement::isEmpty() const {
    return ID == 0;
}

void UnidimensionalFocalElement::clear() {
    ID = 0;
}



