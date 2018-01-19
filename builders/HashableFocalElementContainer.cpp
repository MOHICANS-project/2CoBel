//
// Created by nicola on 30/11/17.
//

#include <src/evidential/focal_elements/HashableFocalElement.h>
#include <src/evidential/errors/IllegalArgumentError.h>
#include "HashableFocalElementContainer.h"

void HashableFocalElementContainer::push(std::unique_ptr<FocalElement> elem, double mass) {
    auto *tmp = dynamic_cast<HashableFocalElement *>(elem.get());
    if (tmp == nullptr) throw IncompatibleTypeError("Expecting an HashableFocalElement");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    std::unique_ptr<FocalElement> cc = elem->clone();
    elem.release();
    derivedPointer.reset(tmp);
    if (mms.find(derivedPointer) != mms.end()) {
        masses[mms.at(derivedPointer)] += mass;
        return;
    }

    fes.push_back(std::move(cc));
    masses.push_back(mass);
    mms.insert(std::make_pair(std::move(derivedPointer), masses.size() - 1));
}


const std::vector<std::unique_ptr<FocalElement>> &HashableFocalElementContainer::getFocalElementsArray() {
    return fes;
}


const std::vector<double> &HashableFocalElementContainer::getMassArray() {
    return masses;
}


void HashableFocalElementContainer::clear() {
    mms.clear();
    fes.clear();
    masses.clear();
}

void HashableFocalElementContainer::set(int index, double mass) {
    if (index < 0 || index > mms.size()) throw IllegalArgumentError("Out of bounds");
    masses[index] = mass;
}

void HashableFocalElementContainer::set(const FocalElement &fe, double mass) {
    std::unique_ptr<FocalElement> elem = fe.clone();
    auto *tmp = dynamic_cast<HashableFocalElement *>(elem.get());
    if (tmp == nullptr) throw IncompatibleTypeError("Expecting an HashableFocalElement");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    elem.release();
    derivedPointer.reset(tmp);
    if (mms.find(derivedPointer) != mms.end()) {
        masses[mms.at(derivedPointer)] += mass;
        return;
    }
}

double HashableFocalElementContainer::get(const FocalElement &fe) {
    std::unique_ptr<FocalElement> elem = fe.clone();
    auto *tmp = dynamic_cast<HashableFocalElement *>(elem.get());
    if (tmp == nullptr) throw IncompatibleTypeError("Expecting an HashableFocalElement");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    elem.release();
    derivedPointer.reset(tmp);
    if (mms.find(derivedPointer) != mms.end()) {
        return masses[mms.at(derivedPointer)];
    }
}

void HashableFocalElementContainer::erase(int index) {
    if (index < 0 || index > mms.size()) throw IllegalArgumentError("Out of bounds");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    auto *tmp = dynamic_cast<HashableFocalElement *>(fes[index].get());
    derivedPointer.reset(tmp);
    mms.erase(derivedPointer);
    derivedPointer.reset();
    fes[index].reset();
    fes.erase(fes.begin() + index);
    masses.erase(masses.begin() + index);
}

void HashableFocalElementContainer::erase(const FocalElement &fe) {
    auto *tmp = dynamic_cast<const HashableFocalElement *>(&fe);
    if (tmp == nullptr) throw IncompatibleTypeError("Expecting an HashableFocalElement");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    derivedPointer.reset(tmp);
    if (mms.find(derivedPointer) != mms.end()) {
        int index = mms.at(derivedPointer);
        mms.erase(derivedPointer);
        fes[index].reset();
        fes.erase(fes.begin() + index);
        masses.erase(masses.begin() + index);
    }
}


