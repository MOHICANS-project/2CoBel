//
// Created by nicola on 25/04/18.
//

#include <errors/IllegalArgumentError.h>
#include "EvidentialAlgorithms.h"

void
dfs(int depth, size_t cur_node, double cur_mass, const std::vector<Evidence> &evs, int q, std::vector<size_t> &path,
    std::unique_ptr<FocalElementContainer> &output_elements, double &ignorance) {
    if (depth >= 0) {
        if (cur_node < evs[depth].numFocalElements())cur_mass *= evs[depth].getMass(cur_node);
        else if (cur_node == evs[depth].numFocalElements())cur_mass *= evs[depth].getIgnorance();
        else cur_mass *= evs[depth].conflict();
        path.push_back(cur_node);
    }
    if (depth < static_cast<int>(evs.size() - 1)) {
        for (size_t i = 0; i < evs[depth + 1].numFocalElements(); ++i)
            dfs(depth + 1, i, cur_mass, evs, q, path, output_elements, ignorance);
        if (evs[depth + 1].getIgnorance() > 0)
            dfs(depth + 1, evs[depth + 1].numFocalElements(), cur_mass, evs, q, path, output_elements, ignorance);
        if (evs[depth + 1].conflict() > 0)
            dfs(depth + 1, evs[depth + 1].numFocalElements() + 1, cur_mass, evs, q, path, output_elements, ignorance);

    } else {
        std::vector<bool> v(path.size());
        std::fill(v.end() - (path.size() - q), v.end(), true);

        std::unique_ptr<FocalElement> final_uni = evs[0].getDiscernment_frame()->clone();
        final_uni->clear();
        do {
            std::unique_ptr<FocalElement> inters = evs[0].getDiscernment_frame()->clone();
            bool danni = false;
            for (int i = 0; i < path.size(); ++i) {
                if (v[i] && path[i] != evs[i].numFocalElements()) {
                    if (path[i] == evs[i].numFocalElements() + 1) {
                        danni = true;
                        break;
                    }
                    inters = inters->intersect(*evs[i].getFocal_elements()[path[i]]);
                }
            }
            if (!danni)final_uni = final_uni->unite(*inters);
        } while (std::next_permutation(v.begin(), v.end()));
        if (*final_uni == *evs[0].getDiscernment_frame()) ignorance += cur_mass;
        else if (!final_uni->isEmpty()) output_elements->push(std::move(final_uni), cur_mass);
    }
    path.erase(path.end() - 1);
}


namespace EvidentialAlgorithms {
    Evidence qrelaxation(const std::vector<Evidence> &evs, int q) {
        if (evs.empty())throw IllegalArgumentError("At least one source needed for q-relaxation.");
        if (evs.size() == q)throw IllegalArgumentError("Cannot relax all the sources.");
        std::unique_ptr<FocalElementContainer> output_elements = evs[0].getDispatcher().getContainer(
                *evs[0].getDiscernment_frame());

        std::vector<size_t> path;
        double ignorance = 0;
        dfs(-1, 0, 1, evs, q, path, output_elements, ignorance);

        Evidence outev(evs[0].getDispatcher().clone(), std::move(output_elements),
                       evs[0].getDiscernment_frame()->clone(),
                       ignorance);
        return outev;
    }
}