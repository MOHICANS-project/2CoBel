//
// Created by nicola on 28/03/18.
//


#include "../focal_elements/FocalElement.h"
#include "../geometry/Point2D.h"
#include "../geometry/ClipperPolygon.h"
#include "../focal_elements/Clipper2DFocalElement.h"
#include "../Evidence.h"
#include "../plotters/ClipperPolygonPlotter.h"

Evidence *evA;
Evidence *evB;
Evidence *evC;


void buildEvidence(const FocalElement &df) {
    evA = new Evidence(df.clone());
    std::vector<Geometry::Point2D> elema1p;
    elema1p.emplace_back(0, 10);
    elema1p.emplace_back(10, 10);
    elema1p.emplace_back(10, 20);
    elema1p.emplace_back(0, 20);
    Geometry::ClipperPolygon elema1poly(elema1p);
    std::unique_ptr<FocalElement> elema1(new Clipper2DFocalElement(elema1poly));
    evA->addFocalElement(std::move(elema1), 1.0);

    evB = new Evidence(df.clone());
    std::vector<Geometry::Point2D> elemb1p;
    elemb1p.emplace_back(7, 12);
    elemb1p.emplace_back(30, 12);
    elemb1p.emplace_back(30, 30);
    elemb1p.emplace_back(7, 30);
    Geometry::ClipperPolygon elemb1poly(elemb1p);
    std::unique_ptr<FocalElement> elemb1(new Clipper2DFocalElement(elemb1poly));
    std::vector<Geometry::Point2D> elemb2p;
    elemb2p.emplace_back(9, 15);
    elemb2p.emplace_back(20, 15);
    elemb2p.emplace_back(20, 25);
    elemb2p.emplace_back(9, 25);
    Geometry::ClipperPolygon elemb2poly(elemb2p);
    std::unique_ptr<FocalElement> elemb2(new Clipper2DFocalElement(elemb2poly));
    evB->addFocalElement(std::move(elemb2), 0.6);
    evB->addFocalElement(std::move(elemb1), 0.4);

    evC = new Evidence(df.clone());
    std::vector<Geometry::Point2D> elemc1p;
    elemc1p.emplace_back(15, 22);
    elemc1p.emplace_back(35, 14);
    elemc1p.emplace_back(35, 30);
    Geometry::ClipperPolygon elemc1poly(elemc1p);
    std::unique_ptr<FocalElement> elemc1(new Clipper2DFocalElement(elemc1poly));
    std::vector<Geometry::Point2D> elemc2p;
    elemc2p.emplace_back(23, 22);
    elemc2p.emplace_back(33, 18);
    elemc2p.emplace_back(33, 26);
    Geometry::ClipperPolygon elemc2poly(elemc2p);
    std::unique_ptr<FocalElement> elemc2(new Clipper2DFocalElement(elemc2poly));
    evC->addFocalElement(std::move(elemc2), 0.6);
    evC->addFocalElement(std::move(elemc1), 0.4);


}

void plotEvidences(std::vector<Evidence> &evs) {
    ClipperPolygonPlotter plotter;
    std::vector<std::reference_wrapper<FocalElement>> v;
    for (auto &ev : evs) {
        for (int i = 0; i < ev.numFocalElements(); ++i) {
            v.emplace_back(*ev.getFocal_elements()[i]);
        }
    }
    plotter.plotClipperFocalElements(v);
}

void plotEvidence(Evidence &ev) {
    std::vector<Evidence> evs;
    evs.push_back(ev);
    plotEvidences(evs);
}


int main(int argc, char **argv) {

    //Discernment frame construction
    std::vector<Geometry::Point2D> dfp;
    dfp.emplace_back(0, 0);
    dfp.emplace_back(100, 0);
    dfp.emplace_back(100, 100);
    dfp.emplace_back(0, 100); //counterclockwise
    Geometry::ClipperPolygon dfpoly(dfp);
    std::unique_ptr<FocalElement> df(new Clipper2DFocalElement(dfpoly));

    //Evidences' construction
    buildEvidence(*df);


    //Disjunctive combination
    Evidence evD = evA->disjunctive_rule(*evB);
    std::vector<Evidence> evs;
    evs.push_back(evD);
    evs.push_back(*evC);
    plotEvidences(evs);

    //Discounting
    evC->discount(0.1);


    //Conjunctive combination
    Evidence ev_final = evC->conjunctive_rule(evD);
    plotEvidence(ev_final);

    //Decision making
    std::unique_ptr<FocalElement> argmaxBetP = ev_final.maxBetP_withMaximalIntersections();


    //Plot BetP argmax
    ClipperPolygonPlotter plotter;
    std::vector<std::reference_wrapper<FocalElement>> v;
    v.emplace_back(*argmaxBetP);
    plotter.plotClipperFocalElements(v);


    delete evA;
    delete evB;
    delete evC;
    return 0;
}