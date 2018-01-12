//
// Created by nicola on 30/11/17.
//

#include <iostream>
#include "GenericFocalElementArrayBuilder.h"


void GenericFocalElementArrayBuilder::push(std::unique_ptr<FocalElement> elem, double mass) {
    //std::cout <<" I AM THE ONE AHAA" << std::endl;
    for (int k = 0; k < fes.size(); ++k) {
        const FocalElement &fek = *fes[k];
        if (fek == *elem) {
            //std::cout <<" EXISTS " << std::endl;
            mms[k] += mass;
            return;
        }
    }
    //std::cout <<"NEW " << std::endl;
    fes.push_back(std::move(elem));
    mms.push_back(mass);

}

std::vector<std::unique_ptr<FocalElement>> GenericFocalElementArrayBuilder::getFocalElementsArray() {
    std::vector<std::unique_ptr<FocalElement>> arr;
    for (int i = 0; i < fes.size(); ++i) {
        arr.push_back(std::move(fes[i]));
    }
    return arr;
}

std::vector<double> GenericFocalElementArrayBuilder::getMassArray() {
    return mms;
}


void GenericFocalElementArrayBuilder::clear() {
    std::vector<std::unique_ptr<FocalElement>>().swap(fes);
    std::vector<double>().swap(mms);
}
