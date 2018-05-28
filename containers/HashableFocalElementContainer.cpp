//
// Created by nicola on 30/11/17.
//

#include "../focal_elements/HashableFocalElement.h"
#include "../errors/IllegalArgumentError.h"
#include "HashableFocalElementContainer.h"

void HashableFocalElementContainer::push(std::unique_ptr<FocalElement> elem, double mass) {
    auto *tmp = static_cast<HashableFocalElement *>(elem.get());
    if (mms.find(tmp) != mms.end()) {
        masses[mms.at(tmp)] += mass;
        return;
    }

    fes.push_back(std::move(elem));
    masses.push_back(mass);
    mms.insert(std::make_pair(tmp, fes.size() - 1));
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
    auto *tmp = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(&fe));//safe for comparison only
    if (mms.find(tmp) != mms.end()) {
        masses[mms.at(tmp)] = mass;
        return;
    }
}

double HashableFocalElementContainer::get(const FocalElement &fe) {
    auto *tmp = const_cast<HashableFocalElement *>(dynamic_cast<const HashableFocalElement *>(&fe));//safe for comparison only
    if (mms.find(tmp) != mms.end()) {
        return masses[mms.at(tmp)];
    }
    return 0;
}

void HashableFocalElementContainer::erase(int index) {
    if (index < 0 || index > mms.size()) throw IllegalArgumentError("Out of bounds");
    auto *tmp = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(fes[index].get()));//safe for comparison only
    mms.erase(tmp);
//    for (int i = index + 1; i < fes.size(); ++i) {
//        auto *x = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(fes[i].get()));
//        mms.at(x) -= 1;
//    }
    for (auto &pair: mms) {
        if (pair.second > index) {
            pair.second--;
        }
    }
    fes[index].reset();
    fes.erase(fes.begin() + index);
    masses.erase(masses.begin() + index);
}

void HashableFocalElementContainer::erase(const FocalElement &fe) {
    auto *tmp = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(&fe));//safe for comparison only
    if (mms.find(tmp) != mms.end()) {
        size_t index = mms.at(tmp);
        mms.erase(tmp);
//        for (size_t i = index + 1; i < fes.size(); ++i) {
//            auto *x = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(fes[i].get()));
//            mms.at(x) -= 1;
//        }
        for (auto &pair: mms) {
            if (pair.second > index) {
                pair.second--;
            }
        }
        fes[index].reset();
        fes.erase(fes.begin() + index);
        masses.erase(masses.begin() + index);
    }
}

bool HashableFocalElementContainer::contains(const FocalElement &fe) {
    auto *tmp = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(&fe));//safe for comparison only
    return mms.find(tmp) != mms.end();
}

size_t HashableFocalElementContainer::size() const {
    return fes.size();
}


void HashableFocalElementContainer::push(std::unique_ptr<FocalElement> elem, double mass,
                                         const std::function<double(double, double)> &acc) {
    //alternative push: code duplication is not avoided for performance reasons.
    auto *tmp = static_cast<HashableFocalElement *>(elem.get());
    if (mms.find(tmp) != mms.end()) {
        size_t idx = mms.at(tmp);
        masses[idx] = acc(masses[idx], mass);
        return;
    }

    fes.push_back(std::move(elem));
    masses.push_back(mass);
    mms.insert(std::make_pair(tmp, fes.size() - 1));
}

int HashableFocalElementContainer::indexOf(const FocalElement &fe) {
    auto *tmp = const_cast<HashableFocalElement *>(static_cast<const HashableFocalElement *>(&fe));//safe for comparison only
    if (mms.find(tmp) != mms.end()) {
        return static_cast<int>(mms.at(tmp));
    }
    return -1;
}

