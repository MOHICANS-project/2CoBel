# 2CoBel
2CoBel is a library for Belief Function Theory (BFT) in 2D (and 1D) continuos spaces. Please cite the referring paper if you use the library in your project:

*@inproceedings{pellicano18ijar,
 author = {Pellican{\`o}, Nicola and Le H{\'e}garat-Mascle, Sylvie  and Aldea, Emanuel },
 title = "2CoBel: A scalable belief function representation for 2D discernment frames",
 journal = "International Journal of Approximate Reasoning",
 volume = "103",
 pages = "320 - 342",
 year = "2018"
}*

## Dependencies
- Python 2.7(for plotting)
- Eigen

The already resolved dependencies are [Clipper](http://www.angusj.com/delphi/clipper.php) and a modified version of [matplotlib-cpp](https://github.com/lava/matplotlib-cpp).

## Supported focal elements types
- UnidimensionalFocalElement (see *demos/unidimensional_demo.cpp*)
- Clipper2DFocalElement (see *demos/clipper_demo.cpp*)
- EigenMat2DFocalElement
- BoxSet2DFocalElement
- CompositeFocalElement (for cross product spaces)

## Example (*demos/clipper_demo.cpp*)
A discernment frame is built as a rectangular focal element, by specifiying its vertexes.
```cpp
  std::vector<Geometry::Point2D> dfp;
  dfp.emplace_back(0, 0);
  dfp.emplace_back(100, 0);
  dfp.emplace_back(100, 100);
  dfp.emplace_back(0, 100); //counterclockwise
  Geometry::ClipperPolygon dfpoly(dfp);
  std::unique_ptr<FocalElement> df(new Clipper2DFocalElement(dfpoly));
```
All the BBAs are built (see the code for reference)
```cpp
  buildEvidence(*df);
```
Creation of BBA D as the disjunctive combination between BBAs A and B.
```cpp
   Evidence evD = evA->disjunctive_rule(*evB);
```
BBAs C and D.

![ex1](https://github.com/MOHICANS-project/2CoBel/blob/master/demos/images/ex1.png)

Discounting of BBA C.
```cpp
  evC->discount(0.1);
```

Conjunctive combination of BBA C and D.
```cpp
  Evidence ev_final = evC->conjunctive_rule(evD);
```
![ex2](https://github.com/MOHICANS-project/2CoBel/blob/master/demos/images/ex2.png)

BetP maximization
```cpp
   std::unique_ptr<FocalElement> argmaxBetP = ev_final.maxBetP_withMaximalIntersections();
```
![ex3](https://github.com/MOHICANS-project/2CoBel/blob/master/demos/images/ex3.png)
