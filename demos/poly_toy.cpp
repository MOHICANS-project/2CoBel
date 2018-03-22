//
// Created by nicola on 19/02/18.
//

#include "../Evidence.h"
#include "../geometry/ClipperPolygon.h"
#include "../focal_elements/Clipper2DFocalElement.h"

Evidence *evA;
Evidence *evB;
Evidence *evC;
Evidence *evD;
Evidence *evE;
Evidence *evF;

void buildEvidence() {

    std::vector<Geometry::Point2D> vertices;
    vertices.emplace_back(-2000, -2000);
    vertices.emplace_back(2000, -2000);
    vertices.emplace_back(2000, 2000);
    vertices.emplace_back(-2000, 2000);
    std::vector<Geometry::ClipperPolygon> polys;
    polys.emplace_back(vertices);
    std::unique_ptr<FocalElement> df(new Clipper2DFocalElement(polys));

    //A
    Geometry::ClipperPolygon pola1 = Geometry::ClipperPolygon::approximateDisk(Geometry::Point2D(500, 700), 50, 32);
    std::vector<Geometry::ClipperPolygon> polysa;
    polysa.push_back(pola1);
    std::unique_ptr<FocalElement> a1(new Clipper2DFocalElement(polysa));

    std::vector<Geometry::Point2D> verticesa2;
    verticesa2.emplace_back(550, 550);
    verticesa2.emplace_back(650, 650);
    verticesa2.emplace_back(475, 875);
    verticesa2.emplace_back(325, 725);
    std::vector<Geometry::ClipperPolygon> polysa2;
    polysa2.emplace_back(verticesa2);
    std::unique_ptr<FocalElement> a2(new Clipper2DFocalElement(polysa2));


    std::vector<Geometry::Point2D> verticesa3;
    verticesa3.emplace_back(575, 475);
    verticesa3.emplace_back(725, 625);
    verticesa3.emplace_back(500, 975);
    verticesa3.emplace_back(225, 700);
    std::vector<Geometry::ClipperPolygon> polysa3;
    polysa3.emplace_back(verticesa3);
    std::unique_ptr<FocalElement> a3(new Clipper2DFocalElement(polysa3));

    Clipper2DFocalElementWriter wrt;
    //wrt.writeFocalElement(*a1,1.0/2,"fesp/a1.txt");
    //wrt.writeFocalElement(*a2,1.0/4,"fesp/a2.txt");
    //wrt.writeFocalElement(*a3,1.0/4,"fesp/a3.txt");

    evA = new Evidence(df->clone());
    evA->addFocalElement(std::move(a1), 1.0 / 2);
    evA->addFocalElement(std::move(a2), 1.0 / 4);
    evA->addFocalElement(std::move(a3), 1.0 / 4);

    //C
    Geometry::ClipperPolygon polc1 = Geometry::ClipperPolygon::approximateDisk(Geometry::Point2D(300, 700), 100, 64);
    std::vector<Geometry::ClipperPolygon> polysc1;
    polysc1.push_back(polc1);
    std::unique_ptr<FocalElement> c1(new Clipper2DFocalElement(polysc1));

    Geometry::ClipperPolygon polc2 = Geometry::ClipperPolygon::approximateDisk(Geometry::Point2D(300, 700), 200, 64);
    std::vector<Geometry::ClipperPolygon> polysc2;
    polysc2.push_back(polc2);
    std::unique_ptr<FocalElement> c2(new Clipper2DFocalElement(polysc2));

    Geometry::ClipperPolygon polc3 = Geometry::ClipperPolygon::approximateDisk(Geometry::Point2D(300, 700), 300, 128);
    std::vector<Geometry::ClipperPolygon> polysc3;
    polysc3.push_back(polc3);
    std::unique_ptr<FocalElement> c3(new Clipper2DFocalElement(polysc3));

    //wrt.writeFocalElement(*c1,2.0/3,"fesp/c1.txt");
    //wrt.writeFocalElement(*c2,1.0/3,"fesp/c2.txt");
    //wrt.writeFocalElement(*c3,1.0/3,"fesp/c3.txt");

    evC = new Evidence(df->clone());
    evC->addFocalElement(std::move(c1), 2.0 / 3);
    evC->addFocalElement(std::move(c2), 1.0 / 3);
    //evC->addFocalElement(std::move(c3),1.0/3);


    std::vector<Geometry::Point2D> verticese;
    verticese.emplace_back(0, 0);
    verticese.emplace_back(1000, 0);
    verticese.emplace_back(1000, 1000);
    verticese.emplace_back(250, 1000);
    verticese.emplace_back(250, 500);
    verticese.emplace_back(0, 500);
    std::vector<Geometry::ClipperPolygon> polyse;
    polyse.emplace_back(verticese);
    std::unique_ptr<FocalElement> e(new Clipper2DFocalElement(polyse));

    //wrt.writeFocalElement(*e,1.0,"fesp/e.txt");
    evE = new Evidence(df->clone());
    evE->addFocalElement(std::move(e), 1.0);


    std::vector<Geometry::Point2D> verticesf;
//    verticesf.emplace_back(350,0);
//    verticesf.emplace_back(650,0);
//    verticesf.emplace_back(650,250);
//    verticesf.emplace_back(1000,250);
//    verticesf.emplace_back(1000,500);
//    verticesf.emplace_back(650,500);
//    verticesf.emplace_back(650,1000);
//    verticesf.emplace_back(350,1000);
//    verticesf.emplace_back(350,500);
//    verticesf.emplace_back(0,500);
//    verticesf.emplace_back(0,250);
//    verticesf.emplace_back(350,250);
    verticesf.emplace_back(450, 0);
    verticesf.emplace_back(650, 0);
    verticesf.emplace_back(650, 250);
    verticesf.emplace_back(1000, 250);
    verticesf.emplace_back(1000, 500);
    verticesf.emplace_back(650, 500);
    verticesf.emplace_back(650, 1000);
    verticesf.emplace_back(450, 1000);
    verticesf.emplace_back(450, 500);
    verticesf.emplace_back(0, 500);
    verticesf.emplace_back(0, 250);
    verticesf.emplace_back(450, 250);
    std::vector<Geometry::ClipperPolygon> polysf;
    polysf.emplace_back(verticesf);
    std::unique_ptr<FocalElement> f(new Clipper2DFocalElement(polysf));

    //wrt.writeFocalElement(*f,0.5,"fesp/f.txt");
    evF = new Evidence(df->clone(), 0.5);
    evF->addFocalElement(std::move(f), 0.5);

}


int main(int argc, char **argv) {

//
    buildEvidence();
//    Clipper2DFocalElementWriter wrt;
    Evidence evG = evC->conjunctive_rule(*evE, false).conjunctive_rule(*evF, false);
//    for (int i = 0; i < evG.numFocalElements(); ++i) {
//        wrt.writeFocalElement(*evG.getFocal_elements()[i],evG.getMass(i),"fesp/g"+std::to_string(i)+".txt");
//    }

//
    Evidence ev4A = evG.conjunctive_rule(*evA, false);
//    for (int i = 0; i < ev4A.numFocalElements(); ++i) {
//        wrt.writeFocalElement(*ev4A.getFocal_elements()[i],ev4A.getMass(i),"fesp/4a"+std::to_string(i)+".txt");
//    }
    std::cout << ev4A.conflict() << " " << ev4A.getIgnorance() << std::endl;
    //std::unique_ptr<FocalElement> winner=ev4A.maxBetP_withMaximalIntersections_opt();
    std::unique_ptr<FocalElement> winner = ev4A.maxBetP_withSingletons(10);
//    wrt.writeFocalElement(*winner,0.0,"fesp/win.txt");




    delete evA;
    delete evB;
    delete evC;
    delete evD;
    delete evE;
    delete evF;
}

