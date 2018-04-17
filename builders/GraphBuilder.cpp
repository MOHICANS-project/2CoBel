//
// Created by nicola on 17/04/18.
//

#include <algorithm>
#include "GraphBuilder.h"

namespace GraphBuilder {

    void buildGraph(const std::vector<std::unique_ptr<FocalElement>> &focal_elements,
                    std::unordered_map<size_t, std::vector<size_t>> &adj_list, std::vector<size_t> &indices,
                    std::vector<int> &parents, std::vector<int> &oldest_parents) {

        indices.resize(focal_elements.size());
        parents.resize(focal_elements.size());
        oldest_parents.resize(focal_elements.size());

        //Sort by cardinality
        for (size_t l = 0; l < focal_elements.size(); ++l) {
            indices[l] = l;
            parents[l] = -1;
            oldest_parents[l] = -1;
        }

        std::sort(indices.begin(), indices.end(),
                  [&](size_t x, size_t y) {
                      return focal_elements[x]->cardinality() > focal_elements[y]->cardinality();
                  });


        for (size_t i = 0; i < focal_elements.size(); ++i) {
            const FocalElement &fe1 = *focal_elements[indices[i]];
            if (fe1.cardinality() == 0.0)continue;
            adj_list.insert(std::make_pair(i, std::vector<size_t>()));
            for (size_t j = i + 1; j < focal_elements.size(); ++j) {
                const FocalElement &fe2 = *focal_elements[indices[j]];
                std::unique_ptr<FocalElement> interbuf = fe1.intersect(fe2);
                if (interbuf->cardinality() > 0) {
                    if (fabs(interbuf->cardinality() - fe2.cardinality()) < 1e-4) {
                        parents[j] = static_cast<int>(i);
                        if (oldest_parents[j] < 0)oldest_parents[j] = static_cast<int>(i);
                    } else {
                        adj_list[i].push_back(j);
                    }
                }
            }
        }


        for (int m = 0; m < focal_elements.size(); ++m) {
            if (parents[m] >= 0) {
                adj_list[parents[m]].insert(
                        std::upper_bound(adj_list[parents[m]].begin(), adj_list[parents[m]].end(), m),
                        m);
            }
        }
    }
}


