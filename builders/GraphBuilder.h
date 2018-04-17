//
// Created by nicola on 17/04/18.
//

#ifndef PROJECT_GRAPHBUILDER_H
#define PROJECT_GRAPHBUILDER_H


#include <cstdio>
#include <vector>
#include <unordered_map>
#include <focal_elements/FocalElement.h>

namespace GraphBuilder {

    void buildGraph(const std::vector<std::unique_ptr<FocalElement>> &focal_elements,
                    std::unordered_map<size_t, std::vector<size_t>> &adj_list, std::vector<size_t> &indices,
                    std::vector<int> &parents, std::vector<int> &oldest_parents);
};


#endif //PROJECT_GRAPHBUILDER_H
