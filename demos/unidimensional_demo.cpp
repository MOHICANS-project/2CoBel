//
// Created by nicola on 28/03/18.
//
#include "../focal_elements/FocalElement.h"
#include "../focal_elements/UnidimensionalFocalElement.h"
#include "../Evidence.h"

/*
 * Example of 2CoBel usage for 1D frames.
 *
 */
int main(int argc, char **argv) {

    //Create discernment frame
    std::unique_ptr<FocalElement> df = UnidimensionalFocalElement::createDiscernmentFrame(3);

    //Create singleton hypothesis
    std::unique_ptr<FocalElement> h0 = UnidimensionalFocalElement::createSingletonHypotesis(0);//001
    std::unique_ptr<FocalElement> h1 = UnidimensionalFocalElement::createSingletonHypotesis(1);//010
    std::unique_ptr<FocalElement> h2 = UnidimensionalFocalElement::createSingletonHypotesis(2);//100

    //Build Evidence
    Evidence evidence(std::move(df));
    evidence.addFocalElement(h0->unite(*h1), 0.2);
    evidence.addFocalElement(h0->unite(*h2), 0.3);
    evidence.addFocalElement(h1->unite(*h2), 0.5);

    //Decision making
    std::unique_ptr<FocalElement> argmaxBetP = evidence.maxBetP_withSingletons(); //small df, ok to use singletons

    std::cout << *argmaxBetP << " " << evidence.BetP(*argmaxBetP) << std::endl;

    return 0;
}

