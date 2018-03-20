//
// Created by nicola on 20/11/17.
//

#ifndef FUSION_EVIDENCE_H
#define FUSION_EVIDENCE_H


#include <vector>
#include "focal_elements/FocalElement.h"
#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
#include "containers/FocalElementContainerDispatcher.h"
#include "containers/DefaultFocalElementContainerDispatcher.h"

/**
 * @class Evidence
 * Main class encapsulating the logic for the management of a belief assignment.
 */
class Evidence {

    std::unique_ptr<FocalElementContainerDispatcher> dispatcher;
    std::unique_ptr<FocalElementContainer> fecontainer;
    std::unique_ptr<FocalElement> discernment_frame;
    double ignorance;


protected:



    void extendPath(unsigned long long &path, size_t pos);

    void extendPath(boost::dynamic_bitset<> &path, size_t pos);


    template<typename T>
    void dfs(std::unordered_map<size_t, std::vector<size_t>> &adj_list, size_t current_pos, T path,
             std::unique_ptr<FocalElement> current_intersection, std::vector<std::unique_ptr<FocalElement>> &output_vec,
             std::vector<T> &check, std::vector<size_t> &indices, std::vector<int> &parents, size_t cur_root);

    std::unique_ptr<FocalElement> maxBetP(std::vector<std::unique_ptr<FocalElement>> &elems, bool computeInters);


    explicit Evidence(std::unique_ptr<FocalElementContainerDispatcher> dispatcher,
                      std::unique_ptr<FocalElementContainer> &&fecontainer,
                      std::unique_ptr<FocalElement> discernment_frame, double _ignorance = 0);

public:
    /**
     * Constructor.
     * @param discernment_frame The discernment frame.
     * @param _ignorance The ignorance value.
     */
    explicit Evidence(std::unique_ptr<FocalElement> discernment_frame, double _ignorance = 0);

    /**
     * Constructor.
     * @param dispatcher The container dispatcher.
     * @param discernment_frame The discernment frame.
     * @param _ignorance The ignorance value.
     */
    explicit Evidence(std::unique_ptr<FocalElementContainerDispatcher> dispatcher,
                      std::unique_ptr<FocalElement> discernment_frame, double _ignorance = 0);


    Evidence(const Evidence &other);

    Evidence &operator=(const Evidence &other);

    Evidence(Evidence &&other) = default;

    Evidence &operator=(Evidence &&other)= default;

    /**
     * Get the focal elements.
     * @return Array of FocalElement objects.
     */
    const std::vector<std::unique_ptr<FocalElement>> &getFocal_elements() const;

    /**
     * Get the mass values.
     * @return Array of masses.
     */
    const std::vector<double> &getMassArray() const;

    /**
     * Get the number of focal elements.
     * @return Size of the FocalElement array.
     */
    size_t numFocalElements() const;

    /**
     * Get the mass at the given position
     * @param i position
     * @return The related mass
     */
    double getMass(size_t i) const;

    /**
     * Set the mass value at the given position.
     * @param mass The new mass value
     * @param index Position
     */
    void setMass(double mass, int index);

    /**
     * Set the mass value of the given FocalElement.
     * @param mass The new mass value
     * @param fe The target FocalElement
     */
    void setMass(double mass, const FocalElement &fe);

    /**
     * Get the mass value of the given FocalElement.
     * @param fe The target FocalElement
     * @return The related mass value.
     */
    double getMass(const FocalElement &fe);

    /**
     * Delete the FocalElement at the given position
     * @param index The target position
     */
    void deleteFocalElement(int index);

    /**
     * Delete the given FocalElement.
     * @param fe The target FocalElement
     */
    void deleteFocalElement(const FocalElement &fe);

    /**
     * Add a new FocalElement.
     * @param elem The FocalElement.
     * @param mass The related mass value.
     */
    void addFocalElement(std::unique_ptr<FocalElement> elem, double mass);

    /**
     * Calculate the conflict (mass of the empty set).
     * @return Conflict value.
     */
    double conflict();

    /**
     * Calculate the \f$Pl(A)\f$ of the FocalElement A.
     * @param elem The FocalElement.
     * @return Plausibility of elem.
     */
    double plausibility(const FocalElement &elem);

    /**
     * Calculate the \f$Bel(A)\f$ of the FocalElement A.
     * @param elem The FocalElement.
     * @return Belief of elem.
     */
    double belief(const FocalElement &elem);

    /**
     * Calculate the \f$q(A)\f$ of the FocalElement A.
     * @param elem The FocalElement.
     * @return q of elem.
     */
    double q_(const FocalElement &elem);

    /**
     * Calculate the \f$BetP(A)\f$ of the FocalElement A.
     * @param elem The FocalElement.
     * @return BetP of elem.
     */
    double BetP(const FocalElement &w);

    /**
    * Calculate the \f$BetP(A)\f$ of the ith FocalElement A.
    * @param i Position of A.
    * @return BetP of A.
    */
    double BetP(size_t i);

    /**
     * Find the FocalElement with maximum BetP by inspecting all the singleton FocalElements included in the set.
     * If the approx_step_size is greater than 1, the singletons are uniformly sampled,
     * and an approximate solution is computed (useful for performance).
     * @param approx_step_size Sampling step of the singleton set
     * @return The FocalELemt with maximum BetP
     */
    std::unique_ptr<FocalElement> maxBetP_withSingletons(int approx_step_size = 1);

    /**
     * Find the FocalElement with maximum BetP by inspecting all the maximal intersections of the current set.
     * Maximal intersections are all the FocalElement intersections which are not contained into others (implying a lower BetP).
     * @return The FocalELemt with maximum BetP
     */
    std::unique_ptr<FocalElement> maxBetP_withMaximalIntersections();

    /**
     * Check if the BBA is valid. All the masses must be >0 and the conflict must be >=0.
     * @return True if the current BBA is valid.
     */
    bool isValidBBA() const;

    /**
     * Conjunctive combination rule on two sources.
     * @param other The other BBA to combine with.
     * @param normalizeDempster True if the Dempster Rule has to be applied for normalization.
     * @return The BBA result of the conjuctive combination.
     */
    Evidence conjunctive_rule(const Evidence &other, bool normalizeDempster = true);

    /**
     * Disjunctive combination rule on two sources.
     * @param other The other BBA to combine with.
     * @return The BBA result of the disjunctive combination.
     */
    Evidence disjunctive_rule(const Evidence &other);

    /**
     * Perform vacuous extension of the current BBA from discernment frame \f$\Omega_{1}\f$ to \f$\Omega_{1} \times \Omega_{2}\f$
     * @param discernment_frame_2 \f$\Omega_{2}\f$
     * @param extend_right If true, extend to\f$\Omega_{1} \times \Omega_{2}\f$, otherwise to \f$\Omega_{2} \times \Omega_{1}\f$
     * @return The BBA result of vacuous extension.
     */
    Evidence vacuous_extension(std::unique_ptr<FocalElement> discernment_frame_2, bool extend_right = true);


    /**
     * Perform marginalization of the current BBA from \f$\Omega_{1} \times \Omega_{2}\f$ to \f$\Omega_{1}\f$
     * @param marginalize_right If true marginalize to \f$\Omega_{1}\f$, otherwise to \f$\Omega_{2}\f$
     * @return The BBA result of marginalization
     */
    Evidence marginalization(bool marginalize_right = true) const;

    /**
     * Perform jointly vacuous extension and conjunction of two sources defined on two independent discernment frames.
     * @param other The other BBA
     * @return The BBA result of vacuous extension and conjunction.
     */
    Evidence vacuous_extension_and_conjuction(const Evidence &other);

    /**
     * Perform conditioning operation on the current BBA.
     * @param C The condition FocalElement.
     * @return The BBA result of conditioning.
     */
    Evidence conditioning(const FocalElement &C);

    /**
     * Perform discounting operator on the masses values.
     * @param alpha The discounting factor.
     */
    void discount(double alpha);

    /**
     * Normalize the current BBA so that the conflict is zero.
     */
    void normalize();

    /**
     * Get the ignorance (mass of the discernment frame).
     * @return Ignorance value.
     */
    double getIgnorance() const;

    /**
     * Set the ignorance (mass of discernment frame).
     * @param ignorance The new ignorance value.
     */
    void setIgnorance(double ignorance);

    /**
     * Get the discernment frame.
     * @return Discernment frame.
     */
    const std::unique_ptr<FocalElement> &getDiscernment_frame() const;


};


#endif //FUSION_EVIDENCE_H
