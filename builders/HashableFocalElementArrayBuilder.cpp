//
// Created by nicola on 30/11/17.
//

#include <src/evidential/focal_elements/HashableFocalElement.h>
#include "HashableFocalElementArrayBuilder.h"

void HashableFocalElementArrayBuilder::push(std::unique_ptr<FocalElement> elem, double mass) {
    // std::cout << "Adding " << *elem << std::endl;
    auto *tmp = dynamic_cast<HashableFocalElement *>(elem.get());
    if (tmp == nullptr) throw IncompatibleTypeError("Expecting an HashableFocalElement");
    std::unique_ptr<HashableFocalElement> derivedPointer;
    elem.release();
    derivedPointer.reset(tmp);
    if (mms.find(derivedPointer) != mms.end()) {
        //std::cout << "Already present " << std::endl;
        mms.at(derivedPointer) += mass;
        return;
    }

    //std::cout << "New " << std::endl;
    mms.insert(std::make_pair(std::move(derivedPointer), mass));
}


std::vector<std::unique_ptr<FocalElement>> HashableFocalElementArrayBuilder::getFocalElementsArray() {
    if (fes.size() != mms.size())storeResults();
    std::vector<std::unique_ptr<FocalElement>> arr;
    for (int i = 0; i < fes.size(); ++i) {
        arr.push_back(std::move(fes[i]));
    }
    return arr;
}


std::vector<double> HashableFocalElementArrayBuilder::getMassArray() {
    if (masses.size() != mms.size())storeResults();
    return masses;
}


void HashableFocalElementArrayBuilder::storeResults() {
    fes.clear();
    masses.clear();
    // std::cout << "Storing results" << std::endl;
    int i = 0;
    for (auto &item : mms) {
        fes.push_back(item.first->clone());
        masses.push_back(item.second);
        //std::cout << *fes[i] << " " << masses[i] <<std::endl;
        i++;
    }
}

void HashableFocalElementArrayBuilder::clear() {
    mms.clear();
    fes.clear();
    masses.clear();
}


