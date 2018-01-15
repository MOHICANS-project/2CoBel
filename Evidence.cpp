//
// Created by nicola on 20/11/17.
//

#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <cfloat>
#include <src/evidential/focal_elements/CompositeFocalElement.h>
#include "Evidence.h"
#include "errors/IllegalArgumentError.h"


template<typename T>
void Evidence::dfs(std::unordered_map<size_t, std::vector<size_t>> &adj_list, size_t current_pos, T path,
                   std::unique_ptr<FocalElement> current_intersection,
                   std::vector<std::unique_ptr<FocalElement>> &output_vec, std::vector<T> &check) {

    extendPath(path, current_pos);
    //std::cout << "Current path: " << path << std::endl;

    bool found_one = false;
    for (auto next_pos : adj_list[current_pos]) {
        const FocalElement &fe = *focal_elements[next_pos];
        std::unique_ptr<FocalElement> next_inter = current_intersection->intersect(fe);
        if (next_inter->cardinality() == 0)continue;
        found_one = true;
        dfs(adj_list, next_pos, path, std::move(next_inter), output_vec, check);
    }

    if (!found_one) {
        //check if current path is redundant
        bool red = false;
        for (auto c : check) {
            if ((path & c) == path) {
                red = true;
                break;
            }
        }
        if (!red) {
            //std::cout << "Adding path: " << path << std::endl;
            output_vec.push_back(std::move(current_intersection));
            check.push_back(path);
        }
    }
}


Evidence::Evidence(std::unique_ptr<FocalElement> _discernment_frame, double _ignorance) : discernment_frame(
        std::move(_discernment_frame)), ignorance(_ignorance) {
    dispatcher = std::unique_ptr<FocalElementArrayBuilderDispatcher>(new DefaultFocalElementArrayBuilderDispatcher());
    febuilder = dispatcher->getBuilder(*discernment_frame);
}

Evidence::Evidence(std::unique_ptr<FocalElementArrayBuilderDispatcher> _dispatcher,
                   std::unique_ptr<FocalElement> _discernment_frame, double _ignorance) : dispatcher(
        std::move(_dispatcher)), discernment_frame(std::move(_discernment_frame)), ignorance(_ignorance) {
    febuilder = dispatcher->getBuilder(*discernment_frame);

}


const std::vector<std::unique_ptr<FocalElement>> &Evidence::getFocal_elements() const {
    return focal_elements;
}

void Evidence::addFocalElement(std::unique_ptr<FocalElement> elem, double mass) {
    if (typeid(elem) != typeid(discernment_frame))
        throw IncompatibleTypeError("The type of focal elements has to match the one of the discernment frame.");
    if (mass <= 0)
        throw IncompatibleTypeError("Mass value has to be > 0.");
    if (elem->cardinality() < 0)
        throw IncompatibleTypeError("Focal element cardinality has to be >= 0.");
    if (!elem->inside(*discernment_frame)) {
        std::cout << *elem << std::endl;
        throw IncompatibleTypeError("The focal element has to lie inside the discernment frame.");
    }
    focal_elements.push_back(std::move(elem));
    mass_array.push_back(mass);
}

const std::vector<double> &Evidence::getMassArray() const {
    return mass_array;
}

double Evidence::getIgnorance() const {
    return ignorance;
}


double Evidence::conflict() {
    double conf = 1.0 - std::accumulate(mass_array.begin(), mass_array.end(), 0.0) - ignorance;
    return fabs(conf) < FLT_EPSILON ? 0.0 : conf;
}

double Evidence::plausibility(const FocalElement &elem) {
    double pl = 0;
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (*fe.intersect(elem) != empty_set) pl += mass_array[i];
    }
    pl += ignorance;
    return pl;
}

double Evidence::belief(const FocalElement &elem) {
    double bel = 0;
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (fe.inside(elem)) bel += mass_array[i];
    }
    if (elem == (*discernment_frame))bel += ignorance;
    return bel;
}

double Evidence::q_(const FocalElement &elem) {
    double q = 0;
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (elem.inside(fe)) q += mass_array[i];
    }
    q += ignorance;
    return q;
}

double Evidence::BetP(const FocalElement &w) {
    if (w.cardinality() == 0)return 0;
    double bp = 0;
    double conftot = conflict();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (fe.cardinality() == 0)conftot += mass_array[i];
        else bp += (w.intersect(fe)->cardinality()) * (mass_array[i] / fe.cardinality());
    }
    bp += w.cardinality() * (ignorance / discernment_frame->cardinality());
    bp /= (1 - conftot);
    return bp;
}


void Evidence::setIgnorance(double ignorance) {
    if (ignorance < 0)throw IncompatibleTypeError("Ignorance value has to be >= 0.");
    Evidence::ignorance = ignorance;
}

std::unique_ptr<FocalElement> Evidence::maxBetP(std::vector<std::unique_ptr<FocalElement>> &elems, bool computeInters) {

    if (elems.empty())return discernment_frame->clone();


    int indexmax = -1;
    double maxbp = 0;
    for (int j = 0; j < elems.size(); ++j) {
        const FocalElement &singleton = *elems[j];
        double card = singleton.cardinality();
        double bp = BetP(singleton) / card;
        if (bp > maxbp) {
            maxbp = bp;
            indexmax = j;
        }
    }
    const FocalElement &winner = *elems[indexmax];

    if (!computeInters) return winner.clone();

    std::vector<int> index_intersecting;
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (winner.inside(fe)) index_intersecting.push_back(i);
    }


    if (index_intersecting.size() == 1)
        return focal_elements[index_intersecting[0]]->clone();

    std::unique_ptr<FocalElement> feout = focal_elements[index_intersecting[0]]->intersect(
            *focal_elements[index_intersecting[1]]);
    for (int i = 2; i < index_intersecting.size(); ++i) {
        feout = feout->intersect(*focal_elements[index_intersecting[i]]);
    }


    return feout;
}

std::unique_ptr<FocalElement> Evidence::maxBetP_withSingletons(int approx_step_size) {
    std::vector<std::unique_ptr<FocalElement>> all_singletons;
    for (const auto &fe : focal_elements) {
        if (all_singletons.size() >= discernment_frame->cardinality())break;
        std::vector<std::unique_ptr<FocalElement>> singletons = fe->getInnerSingletons(approx_step_size);
        for (int i = 0; i < singletons.size(); ++i) {
            const FocalElement &singleton = *singletons[i];
            auto results = std::find_if(
                    all_singletons.begin(),
                    all_singletons.end(),
                    [&](std::unique_ptr<FocalElement> const &object) {
                        return *object == singleton;
                    });
            if (results == all_singletons.end())all_singletons.push_back(std::move(singletons[i]));
        }
    }
    return maxBetP(all_singletons, true);
}


std::unique_ptr<FocalElement> Evidence::maxBetP_withMaximalIntersections() {

    std::unordered_map<size_t, std::vector<size_t>> adj_list;
    for (size_t i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe1 = *focal_elements[i];
        if (fe1.cardinality() == 0.0)continue;
        adj_list.insert(std::make_pair(i, std::vector<size_t>()));
        for (size_t j = i + 1; j < focal_elements.size(); ++j) {
            const FocalElement &fe2 = *focal_elements[j];
            if (fe1.intersect(fe2)->cardinality() > 0) {
                adj_list[i].push_back(j);
            }
        }
    }

    std::vector<std::unique_ptr<FocalElement>> inters;

    if (focal_elements.size() <= sizeof(unsigned long long) * 4) {
        std::vector<unsigned long long> checks;
        for (size_t k = 0; k < focal_elements.size() - 1; ++k) {
            if (focal_elements[k]->cardinality() > 0)
                dfs(adj_list, k, 0ull, focal_elements[k]->clone(), inters, checks);
        }
    } else {
        std::vector<boost::dynamic_bitset<>> checks;
        for (size_t k = 0; k < focal_elements.size() - 1; ++k) {
            if (focal_elements[k]->cardinality() > 0) {
                boost::dynamic_bitset<> root(focal_elements.size());
                dfs(adj_list, k, root, focal_elements[k]->clone(), inters, checks);
            }
        }
    }

    if (focal_elements.size() == 1 && focal_elements[0]->cardinality() > 0)inters.push_back(focal_elements[0]->clone());

    return maxBetP(inters, false);
}

void Evidence::extendPath(unsigned long long &path, size_t pos) {
    path = path | (1ull << pos);
}

void Evidence::extendPath(boost::dynamic_bitset<> &path, size_t pos) {
    path[pos] = 1;
}

bool Evidence::isValidBBA() {
    double cum = 0.0;
    for (auto mass : mass_array) {
        if (mass < 0.0 && fabs(mass) > FLT_EPSILON)return false;
        if (mass > 1.0 && fabs(mass - 1.0) > FLT_EPSILON)return false;
        cum += mass;
    }
    return !(cum > 1.0 && fabs(cum - 1.0) > FLT_EPSILON);
}


Evidence Evidence::conjunctive_rule(const Evidence &other, bool normalizeDempster = true) {
    if ((*other.getDiscernment_frame()) != (*discernment_frame))
        throw IncompatibleTypeError("Conjunctive rule can be applied only on the same discernment frame");
    double conflict_new = 0.0;


    febuilder->clear();

    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe1 = *focal_elements[i];
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> inters = fe1.intersect(fe2);
            double mm = mass_array[i] * other.getMassArray()[j];
            if (*inters == empty_set) conflict_new += mm;
            else febuilder->push(std::move(inters), mm);
        }
        if (other.getIgnorance() > 0) {
            std::unique_ptr<FocalElement> inters = fe1.clone();
            double mm = mass_array[i] * other.getIgnorance();
            febuilder->push(std::move(inters), mm);
        }
    }
    if (ignorance > 0) {
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> inters = fe2.clone();
            double mm = ignorance * other.getMassArray()[j];
            febuilder->push(std::move(inters), mm);
        }
    }
    std::vector<std::unique_ptr<FocalElement>> outfes = febuilder->getFocalElementsArray();
    std::vector<double> outmasses = febuilder->getMassArray();


    Evidence outev(dispatcher->clone(), discernment_frame->clone(), ignorance * other.getIgnorance());
    for (int k = 0; k < outfes.size(); ++k) {
        outev.addFocalElement(std::move(outfes[k]), outmasses[k] / (normalizeDempster ? (1 - conflict_new) : 1));
    }

    febuilder->clear();

    return outev;
}


const std::unique_ptr<FocalElement> &Evidence::getDiscernment_frame() const {
    return discernment_frame;
}

Evidence Evidence::disjunctive_rule(const Evidence &other) {
    if ((*other.getDiscernment_frame()) != (*discernment_frame))
        throw IncompatibleTypeError("Disjunctive rule can be applied only on the same discernment frame");

    double ignorance_new = 0.0;

    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe1 = *focal_elements[i];
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> uni = fe1.unite(fe2);
            double mm = mass_array[i] * other.getMassArray()[j];
            if (*uni == *discernment_frame)ignorance_new += mm;
            else febuilder->push(std::move(uni), mm);

        }
        if (other.getIgnorance() > 0) {
            double mm = mass_array[i] * other.getIgnorance();
            ignorance_new += mm;
        }
    }
    if (ignorance > 0) {
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            double mm = ignorance * other.getMassArray()[j];
            ignorance_new += mm;
        }
    }
    ignorance_new += ignorance * other.getIgnorance();

    std::vector<std::unique_ptr<FocalElement>> outfes = febuilder->getFocalElementsArray();
    std::vector<double> outmasses = febuilder->getMassArray();


    Evidence outev(dispatcher->clone(), discernment_frame->clone(), ignorance_new);
    for (int k = 0; k < outfes.size(); ++k) {
        outev.addFocalElement(std::move(outfes[k]), outmasses[k]);
    }

    febuilder->clear();

    return outev;
}

void Evidence::discount(double alpha) {
    if (alpha < 0 || alpha > 1) throw IllegalArgumentError("The discounting factor has to be in the interval [0,1].");
    for (int i = 0; i < mass_array.size(); ++i) {
        mass_array[i] *= (1 - alpha);
    }
    ignorance *= (1 - alpha);
    ignorance += alpha;
}

Evidence Evidence::vacuous_extension(std::unique_ptr<FocalElement> discernment_frame_2, bool extend_right = true) {

    std::unique_ptr<FocalElement> disc_fr_copy = discernment_frame_2->clone();
    std::unique_ptr<FocalElement> new_discernment_frame(
            new CompositeFocalElement(extend_right ? discernment_frame->clone() : std::move(disc_fr_copy),
                                      extend_right ? std::move(disc_fr_copy) : discernment_frame->clone()));
    Evidence outev(dispatcher->clone(), std::move(new_discernment_frame), ignorance);
    for (int i = 0; i < focal_elements.size(); ++i) {
        outev.addFocalElement(std::unique_ptr<FocalElement>(
                new CompositeFocalElement(extend_right ? focal_elements[i]->clone() : discernment_frame_2->clone(),
                                          extend_right ? discernment_frame_2->clone() : focal_elements[i]->clone())),
                              mass_array[i]);
    }

    return outev;
}

Evidence Evidence::marginalization(bool marginalize_right = true) {

    auto *df = dynamic_cast<const CompositeFocalElement *>(discernment_frame.get());
    if (df == nullptr)return *this;

    double ignorance_new = 0;

    for (int i = 0; i < focal_elements.size(); ++i) {
        auto *fe = dynamic_cast<const CompositeFocalElement *>(focal_elements[i].get());
        if ((marginalize_right && *fe->getLeft() == *df->getLeft()) ||
            (!marginalize_right && *fe->getRight() == *df->getRight()))
            ignorance_new += mass_array[i];
        if ((marginalize_right && *fe->getLeft() == empty_set) ||
            (!marginalize_right && *fe->getRight() == empty_set))
            continue;
        febuilder->push(marginalize_right ? fe->getLeft()->clone() : fe->getRight()->clone(), mass_array[i]);
    }


    std::vector<std::unique_ptr<FocalElement>> outfes = febuilder->getFocalElementsArray();
    std::vector<double> outmasses = febuilder->getMassArray();


    Evidence outev(dispatcher->clone(), marginalize_right ? df->getLeft()->clone() : df->getRight()->clone(),
                   ignorance_new);
    for (int k = 0; k < outfes.size(); ++k) {
        outev.addFocalElement(std::move(outfes[k]), outmasses[k]);
    }

    febuilder->clear();
    return outev;
}


Evidence Evidence::vacuous_extension_and_conjuction(const Evidence &other) {
    Evidence outev(dispatcher->clone(), std::unique_ptr<FocalElement>(
            new CompositeFocalElement(discernment_frame->clone(), other.getDiscernment_frame()->clone())),
                   ignorance * other.getIgnorance());

    for (int i = 0; i < focal_elements.size(); ++i) {
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(focal_elements[i]->clone(), other.getFocal_elements()[j]->clone())),
                                  mass_array[i] * other.getMassArray()[j]);
        }
        outev.addFocalElement(std::unique_ptr<FocalElement>(
                new CompositeFocalElement(focal_elements[i]->clone(), other.getDiscernment_frame()->clone())),
                              mass_array[i] * other.getIgnorance());
    }

    for (int k = 0; k < other.getFocal_elements().size(); ++k) {
        outev.addFocalElement(std::unique_ptr<FocalElement>(
                new CompositeFocalElement(discernment_frame->clone(), other.getFocal_elements()[k]->clone())),
                              ignorance * other.getMassArray()[k]);
    }

    return outev;
}

void Evidence::setMass(double mass, int index) {
    if (mass == 0)
        throw IncompatibleTypeError(
                "Mass value has to be > 0. Delete the corresponding focal element to have the same behavior.");
    if (mass < 0)throw IncompatibleTypeError("Mass value has to be > 0.");
    if (index < 0 || index >= mass_array.size()) throw IllegalArgumentError("Index out of bounds.");

    mass_array[index] = mass;
}

void Evidence::setMass(double mass, const FocalElement &fe) {
    if (mass == 0)
        throw IncompatibleTypeError(
                "Mass value has to be > 0. Delete the corresponding focal element to have the same behavior.");
    if (mass < 0)throw IncompatibleTypeError("Mass value has to be > 0.");

    bool found = false;
    for (int i = 0; i < focal_elements.size(); ++i) {
        if (fe == *focal_elements[i]) {
            mass_array[i] = mass;
            found = true;
            break;
        }
    }
    if (!found)throw IllegalArgumentError("The focal element does not exist.");
}

void Evidence::deleteFocalElement(int index) {
    if (index < 0 || index >= mass_array.size()) throw IllegalArgumentError("Index out of bounds.");
    focal_elements[index].reset();
    focal_elements.erase(focal_elements.begin() + index);
    mass_array.erase(mass_array.begin() + index);
}

void Evidence::deleteFocalElement(const FocalElement &fe) {
    int index = -1;
    for (int i = 0; i < focal_elements.size(); ++i) {
        if (fe == *focal_elements[i]) {
            index = i;
            break;
        }
    }
    if (index < 0)throw IllegalArgumentError("The focal element does not exist.");
    focal_elements[index].reset();
    focal_elements.erase(focal_elements.begin() + index);
    mass_array.erase(mass_array.begin() + index);
}


Evidence::Evidence(const Evidence &other) {
    discernment_frame = other.getDiscernment_frame()->clone();
    ignorance = other.getIgnorance();
    const std::vector<std::unique_ptr<FocalElement>> &fe_other = other.getFocal_elements();
    const std::vector<double> &ms_other = other.getMassArray();
    for (int i = 0; i < ms_other.size(); ++i) {
        focal_elements.push_back(fe_other[i]->clone());
        mass_array.push_back(ms_other[i]);
    }
    dispatcher = other.dispatcher->clone();
    febuilder = dispatcher->getBuilder(*discernment_frame);
}

Evidence &Evidence::operator=(const Evidence &other) {

    discernment_frame = other.getDiscernment_frame()->clone();
    ignorance = other.getIgnorance();
    const std::vector<std::unique_ptr<FocalElement>> &fe_other = other.getFocal_elements();
    const std::vector<double> &ms_other = other.getMassArray();
    for (int i = 0; i < ms_other.size(); ++i) {
        focal_elements.push_back(fe_other[i]->clone());
        mass_array.push_back(ms_other[i]);
    }
    dispatcher = other.dispatcher->clone();
    febuilder = dispatcher->getBuilder(*discernment_frame);
    return *this;
}

double Evidence::getMass(const FocalElement &fe) {
    for (int i = 0; i < focal_elements.size(); ++i) {
        if (*focal_elements[i] == fe) {
            return mass_array[i];
        }
    }
}



















