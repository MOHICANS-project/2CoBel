//
// Created by nicola on 14/11/17.
//

#ifndef FUSION_UNIDIMENSIONALFOCALELEMENT_H
#define FUSION_UNIDIMENSIONALFOCALELEMENT_H


#include <type_traits>
#include <vector>
#include <set>
#include "HashableFocalElement.h"

/**
 * @class UnidimensionalFocalElement
 * 1D FocalElement using an integer ID as source of information.
 * The ID binary representation encodes the singleton hypotesis which are included in the current set.
 * E.g. ID=5 -> 101 -> {h1,h3} focal element.
 */
class UnidimensionalFocalElement : public HashableFocalElement {

    unsigned long long ID;


public:
    /**
     * Constructor.
     * @param ID Identifier of the focal element.
     */
    explicit UnidimensionalFocalElement(unsigned long long ID) : ID(ID) {}

    ~UnidimensionalFocalElement() override = default;

    double cardinality() const override;

    unsigned long long getKey() const;

    size_t hash() const override;

    std::vector<std::unique_ptr<FocalElement>> getInnerSingletons(int step_size = 1) const override;

    std::unique_ptr<FocalElement> clone() const override;

    /**
     * Create a discernment frame of cardinality.
     * @param card Cardinality of the discernment frame.
     * @return FocalElement with ID corresponding to a card number of ones in binary represenation.
     */
    static std::unique_ptr<UnidimensionalFocalElement> createDiscernmentFrame(unsigned char card);

    /**
     * Create a singleton hypotesis of the given number.
     * @param hypotesis_number The hypotesis progressive number
     * @return FocalElement with ID \f$2^{hypotesis_number}\f$
     */
    static std::unique_ptr<UnidimensionalFocalElement> createSingletonHypotesis(size_t hypotesis_number);

    bool isEmpty() const override;

    void clear() override;

private:
    bool equal_to(FocalElement const &rhs) const override;

    bool is_inside(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_intersection(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_union(FocalElement const &rhs) const override;

    std::unique_ptr<FocalElement> do_difference(FocalElement const &rhs) const override;

    void print(std::ostream &os) const override;


};


#endif //FUSION_UNIDIMENSIONALFOCALELEMENT_H
