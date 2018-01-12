//
// Created by nicola on 20/11/17.
//

#ifndef FUSION_EVIDENCE_H
#define FUSION_EVIDENCE_H


#include <vector>
#include <src/evidential/focal_elements/FocalElement.h>
#include <src/evidential/focal_elements/ConflictFocalElement.h>
#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
#include <src/evidential/builders/FocalElementArrayBuilderDispatcher.h>
#include <src/evidential/builders/DefaultFocalElementArrayBuilderDispatcher.h>

class Evidence {

protected:
    std::unique_ptr<FocalElement> discernment_frame;
    double ignorance;

    ConflictFocalElement empty_set;

    std::vector<std::unique_ptr<FocalElement>> focal_elements;
    std::vector<double> mass_array;

    std::unique_ptr<FocalElementArrayBuilderDispatcher> dispatcher;
    std::unique_ptr<FocalElementArrayBuilder> febuilder;

    void extendPath(unsigned long long &path, size_t pos);

    void extendPath(boost::dynamic_bitset<> &path, size_t pos);

    template<typename T>
    void dfs(std::unordered_map<size_t, std::vector<size_t>> &adj_list, size_t current_pos, T path,
             std::unique_ptr<FocalElement> current_intersection, std::vector<std::unique_ptr<FocalElement>> &output_vec,
             std::vector<T> &check);

    std::unique_ptr<FocalElement> maxBetP(std::vector<std::unique_ptr<FocalElement>> &elems, bool computeInters);


public:

    explicit Evidence(std::unique_ptr<FocalElement> discernment_frame, double _ignorance = 0);

    explicit Evidence(std::unique_ptr<FocalElementArrayBuilderDispatcher> dispatcher,
                      std::unique_ptr<FocalElement> discernment_frame, double _ignorance = 0);

    Evidence(const Evidence &other);

    Evidence &operator=(const Evidence &other);

    Evidence(Evidence &&other) = default;

    Evidence &operator=(Evidence &&other)= default;

    const std::vector<std::unique_ptr<FocalElement>> &getFocal_elements() const;

    const std::vector<double> &getMassArray() const;

    void addFocalElement(std::unique_ptr<FocalElement> elem, double mass);

    double conflict();

    double plausibility(const FocalElement &elem);

    double belief(const FocalElement &elem);

    double q_(const FocalElement &elem);

    double BetP(const FocalElement &w);

    std::unique_ptr<FocalElement> maxBetP_withSingletons(int approx_step_size = 1);

    std::unique_ptr<FocalElement> maxBetP_withMaximalIntersections();

    bool isValidBBA();

    Evidence conjunctive_rule(const Evidence &other, bool normalizeDempster = true);

    Evidence disjunctive_rule(const Evidence &other);

    Evidence vacuous_extension(std::unique_ptr<FocalElement> discernment_frame_2, bool extend_right = true);

    Evidence marginalization(bool marginalize_right = true);

    Evidence vacuous_extension_and_conjuction(const Evidence &other);

    void discount(double alpha);


    double getIgnorance() const;

    void setIgnorance(double ignorance);

    const std::unique_ptr<FocalElement> &getDiscernment_frame() const;


};


#endif //FUSION_EVIDENCE_H
