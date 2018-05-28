//
// Created by nicola on 30/11/17.
//

#include <iostream>
#include "../errors/IllegalArgumentError.h"
#include "GenericFocalElementContainer.h"


void GenericFocalElementContainer::push(std::unique_ptr<FocalElement> elem, double mass) {
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (*elem == fek) {
            mms[k] += mass;
            return;
        }
    }
    fes.push_back(std::move(elem));
    mms.push_back(mass);
}


const std::vector<std::unique_ptr<FocalElement>> &GenericFocalElementContainer::getFocalElementsArray() {
    return fes;
}

const std::vector<double> &GenericFocalElementContainer::getMassArray() {
    return mms;
}


void GenericFocalElementContainer::clear() {
    std::vector<std::unique_ptr<FocalElement>>().swap(fes);
    std::vector<double>().swap(mms);
}

void GenericFocalElementContainer::set(int index, double mass) {
    if (index < 0 || index > mms.size()) throw IllegalArgumentError("Out of bounds");
    mms[index] = mass;
}

void GenericFocalElementContainer::set(const FocalElement &fe, double mass) {
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (fek == fe) {
            mms[k] = mass;
            return;
        }
    }
}

double GenericFocalElementContainer::get(const FocalElement &fe) {
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (fek == fe) {
            return mms[k];
        }
    }
    return 0;
}

void GenericFocalElementContainer::erase(int index) {
    if (index < 0 || index >= mms.size()) throw IllegalArgumentError("Index out of bounds.");
    fes[index].reset();
    fes.erase(fes.begin() + index);
    mms.erase(mms.begin() + index);
}

void GenericFocalElementContainer::erase(const FocalElement &fe) {
    int index = -1;
    for (int i = 0; i < fes.size(); ++i) {
        if (*fes[i] == fe) {
            index = i;
            break;
        }
    }
    if (index < 0)return;
    erase(index);
}

bool GenericFocalElementContainer::contains(const FocalElement &fe) {
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (fek == fe) {
            return true;
        }
    }
    return false;
}

size_t GenericFocalElementContainer::size() const {
    return fes.size();
}


void GenericFocalElementContainer::push(std::unique_ptr<FocalElement> elem, double mass,
                                        const std::function<double(double, double)> &acc) {
    //alternative push: code duplication is not avoided for performance reasons.
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (*elem == fek) {
            mms[k] = acc(mms[k], mass);
            return;
        }
    }
    fes.push_back(std::move(elem));
    mms.push_back(mass);
}

int GenericFocalElementContainer::indexOf(const FocalElement &fe) {
    for (int i = 0; i < fes.size(); ++i) {
        if (*fes[i] == fe) {
            return i;
        }
    }
    return -1;
}
