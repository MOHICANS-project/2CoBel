//
// Created by nicola on 20/11/17.
//

#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <cfloat>
#include "focal_elements/CompositeFocalElement.h"
#include "errors/InvalidBBAError.h"
#include "Evidence.h"
#include "errors/IllegalArgumentError.h"

const double EPS = 1e-3;


template<typename T>
void Evidence::dfs(std::unordered_map<size_t, std::vector<size_t>> &adj_list, size_t current_pos, T path,
                   std::unique_ptr<FocalElement> current_intersection,
                   std::vector<std::unique_ptr<FocalElement>> &output_vec, std::vector<T> &check,
                   std::vector<size_t> &indices, std::vector<int> &parents, size_t cur_root) {

    extendPath(path, current_pos);

    bool found_one = false;
    for (auto next_pos : adj_list[current_pos]) {
        const FocalElement &fe = *fecontainer->getFocalElementsArray()[indices[next_pos]];
        if (parents[next_pos] >= 0 && parents[next_pos] < cur_root)continue; //early stopping
        std::unique_ptr<FocalElement> next_inter = current_intersection->intersect(fe);
        if (next_inter->cardinality() == 0)continue;
        found_one = true;
        dfs(adj_list, next_pos, path, std::move(next_inter), output_vec, check, indices, parents, cur_root);
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
            output_vec.push_back(std::move(current_intersection));
            check.push_back(path);
        }
    }
}


Evidence::Evidence(std::unique_ptr<FocalElement> _discernment_frame, double _ignorance) : discernment_frame(
        std::move(_discernment_frame)), ignorance(_ignorance) {
    dispatcher = std::unique_ptr<FocalElementContainerDispatcher>(new DefaultFocalElementContainerDispatcher());
    fecontainer = dispatcher->getContainer(*discernment_frame);
    is_gssf = false;
}

Evidence::Evidence(std::unique_ptr<FocalElementContainerDispatcher> _dispatcher,
                   std::unique_ptr<FocalElement> _discernment_frame, double _ignorance) : dispatcher(
        std::move(_dispatcher)), discernment_frame(std::move(_discernment_frame)), ignorance(_ignorance) {
    fecontainer = dispatcher->getContainer(*discernment_frame);
    is_gssf = false;

}

Evidence::Evidence(std::unique_ptr<FocalElementContainerDispatcher> _dispatcher,
                   std::unique_ptr<FocalElementContainer> &&_fecontainer,
                   std::unique_ptr<FocalElement> _discernment_frame,
                   double _ignorance) : dispatcher(
        std::move(_dispatcher)), fecontainer(std::move(_fecontainer)), discernment_frame(std::move(_discernment_frame)),
                                        ignorance(_ignorance), is_gssf(false) {
}


const std::vector<std::unique_ptr<FocalElement>> &Evidence::getFocal_elements() const {
    return fecontainer->getFocalElementsArray();
}

void Evidence::addFocalElement(std::unique_ptr<FocalElement> elem, double mass) {
    if (typeid(elem) != typeid(discernment_frame))
        throw IncompatibleTypeError("The type of focal elements has to match the one of the discernment frame.");
//    if (mass <= 0 || mass > 1)
//        throw IncompatibleTypeError("Mass value has to be > 0 and <1.");
    if (elem->cardinality() < 0) {
        std::string err = "Focal element cardinality has to be >= 0 [" + std::to_string(elem->cardinality()) + "].";
        throw IncompatibleTypeError(err.c_str());
    }
    if (elem->isEmpty())
        throw IncompatibleTypeError("Cannot add the empty set.");
    fecontainer->push(std::move(elem), mass);
}

const std::vector<double> &Evidence::getMassArray() const {
    return fecontainer->getMassArray();
}

double Evidence::getIgnorance() const {
    return ignorance;
}


double Evidence::conflict() {
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    double conf = 1.0 - std::accumulate(mass_array.begin(), mass_array.end(), 0.0) - ignorance;
    return fabs(conf) < FLT_EPSILON ? 0.0 : conf;
}

double Evidence::plausibility(const FocalElement &elem) {
    double pl = 0;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (!fe.intersect(elem)->isEmpty()) pl += mass_array[i];
    }
    pl += ignorance;
    return pl;
}

double Evidence::belief(const FocalElement &elem) {
    double bel = 0;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (fe.inside(elem)) bel += mass_array[i];
    }
    if (*discernment_frame == elem)bel += ignorance;
    return bel;
}

double Evidence::q_(const FocalElement &elem) {
    double q = 0;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
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
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
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


    std::vector<int> indices_max;
    double maxbp = 0;
    for (int j = 0; j < elems.size(); ++j) {
        const FocalElement &singleton = *elems[j];
        double card = singleton.cardinality();
        double bp = BetP(singleton) / card;
        //std::cout << singleton << " " << bp << std::endl;
        if (bp > maxbp) {
            maxbp = bp;
            indices_max.clear();
            indices_max.push_back(j);
        } else if (bp == maxbp) {
            indices_max.push_back(j);
        }
    }

    std::unique_ptr<FocalElement> winner = elems[indices_max[0]]->clone();
    for (int k = 1; k < indices_max.size(); ++k) {
        winner = winner->unite(*elems[indices_max[k]]);
    }

    if (!computeInters) return winner;

    std::vector<int> index_intersecting;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe = *focal_elements[i];
        if (winner->inside(fe)) index_intersecting.push_back(i);
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
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    std::vector<std::unique_ptr<FocalElement>> all_singletons;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
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
    //std::cout << all_singletons.size() << std::endl;
    return maxBetP(all_singletons, true);
}


std::unique_ptr<FocalElement> Evidence::maxBetP_withMaximalIntersections() {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }

    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();

    //Sort by cardinality
    std::vector<size_t> indices(focal_elements.size());
    for (size_t l = 0; l < focal_elements.size(); ++l) {
        indices[l] = l;
    }

    std::sort(indices.begin(), indices.end(),
              [&](size_t x, size_t y) { return focal_elements[x]->cardinality() > focal_elements[y]->cardinality(); });


    std::vector<int> parents(focal_elements.size(), -1);
    std::vector<int> oldest_parents(focal_elements.size(), -1);

    std::unordered_map<size_t, std::vector<size_t>> adj_list;

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
            adj_list[parents[m]].insert(std::upper_bound(adj_list[parents[m]].begin(), adj_list[parents[m]].end(), m),
                                        m);
        }
    }

    std::vector<std::unique_ptr<FocalElement>> inters;

    if (focal_elements.size() <= sizeof(unsigned long long) * 4) {
        std::vector<unsigned long long> checks;
        for (size_t k = 0; k < focal_elements.size(); ++k) {
            if (focal_elements[indices[k]]->cardinality() > 0 && parents[k] < 0) {
                dfs(adj_list, k, 0ull, focal_elements[indices[k]]->clone(), inters, checks, indices, oldest_parents, k);
            }
        }
    } else {
        std::vector<boost::dynamic_bitset<>> checks;
        for (size_t k = 0; k < focal_elements.size(); ++k) {
            if (focal_elements[indices[k]]->cardinality() > 0 && parents[k] < 0) {
                boost::dynamic_bitset<> root(focal_elements.size());
                dfs(adj_list, k, root, focal_elements[indices[k]]->clone(), inters, checks, indices, oldest_parents, k);
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

bool Evidence::isValidBBA() const {

    double cum = ignorance;
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (auto mass : mass_array) {
        if (!is_gssf && mass < 0.0 && fabs(mass) > EPS)return false;
        if (!is_gssf && mass > 1.0 && fabs(mass - 1.0) > EPS)return false;
        cum += mass;
    }

    return !(cum > 1.0 && fabs(cum - 1.0) > EPS);
}


Evidence Evidence::conjunctive_rule(const Evidence &other, bool normalizeDempster) {
    if (!isValidBBA() || !other.isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }

    if ((*other.getDiscernment_frame()) != (*discernment_frame))
        throw IncompatibleTypeError("Conjunctive rule can be applied only on the same discernment frame");

    auto *df = dynamic_cast<const CompositeFocalElement *>(discernment_frame.get());
    if (df != nullptr) {
        Evidence ev_left = marginalization().conjunctive_rule(other.marginalization(), false);
        Evidence ev_right = marginalization(false).conjunctive_rule(other.marginalization(false), false);
        Evidence res = ev_left.vacuous_extension_and_conjuction(ev_right);
        if (normalizeDempster)res.normalize();
        return res;
    }

    double conflict_new = 0.0;

    std::unique_ptr<FocalElementContainer> new_fecontainer = dispatcher->getContainer(*discernment_frame);

    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe1 = *focal_elements[i];
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> inters = fe1.intersect(fe2);
            double mm = mass_array[i] * other.getMassArray()[j];
            if (inters->isEmpty()) conflict_new += mm;
            else new_fecontainer->push(std::move(inters), mm);
        }
        if (other.getIgnorance() > 0) {
            std::unique_ptr<FocalElement> inters = fe1.clone();
            double mm = mass_array[i] * other.getIgnorance();
            new_fecontainer->push(std::move(inters), mm);
        }
    }
    if (ignorance > 0) {
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> inters = fe2.clone();
            double mm = ignorance * other.getMassArray()[j];
            new_fecontainer->push(std::move(inters), mm);
        }
    }

    if (normalizeDempster) {
        const std::vector<double> &new_masses = new_fecontainer->getMassArray();
        for (int i = 0; i < new_masses.size(); ++i) {
            new_fecontainer->set(i, new_masses[i] / (1 - conflict_new));
        }
    }

    Evidence outev(dispatcher->clone(), std::move(new_fecontainer), discernment_frame->clone(),
                   ignorance * other.getIgnorance() / (normalizeDempster ? (1 - conflict_new) : 1));

    return outev;
}


const std::unique_ptr<FocalElement> &Evidence::getDiscernment_frame() const {
    return discernment_frame;
}

Evidence Evidence::disjunctive_rule(const Evidence &other) {
    if (!isValidBBA() || !other.isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    if ((*other.getDiscernment_frame()) != (*discernment_frame))
        throw IncompatibleTypeError("Disjunctive rule can be applied only on the same discernment frame");

    std::unique_ptr<FocalElementContainer> new_fecontainer = dispatcher->getContainer(*discernment_frame);

    double ignorance_new = 0.0;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        const FocalElement &fe1 = *focal_elements[i];
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            const FocalElement &fe2 = *other.getFocal_elements()[j];
            std::unique_ptr<FocalElement> uni = fe1.unite(fe2);
            double mm = mass_array[i] * other.getMassArray()[j];
            if (*uni == *discernment_frame)ignorance_new += mm;
            else new_fecontainer->push(std::move(uni), mm);

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

    Evidence outev(dispatcher->clone(), std::move(new_fecontainer), discernment_frame->clone(), ignorance_new);


    return outev;
}

void Evidence::discount(double alpha) {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    if (alpha < 0 || alpha > 1) throw IllegalArgumentError("The discounting factor has to be in the interval [0,1].");
    //const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < mass_array.size(); ++i) {
        fecontainer->set(i, mass_array[i] * (1 - alpha));
    }
    ignorance *= (1 - alpha);
    ignorance += alpha;
}

Evidence Evidence::vacuous_extension(std::unique_ptr<FocalElement> discernment_frame_2, bool extend_right) {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    std::unique_ptr<FocalElement> disc_fr_copy = discernment_frame_2->clone();
    std::unique_ptr<FocalElement> new_discernment_frame(
            new CompositeFocalElement(extend_right ? discernment_frame->clone() : std::move(disc_fr_copy),
                                      extend_right ? std::move(disc_fr_copy) : discernment_frame->clone()));

    Evidence outev(dispatcher->clone(), std::move(new_discernment_frame), ignorance);

    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        outev.addFocalElement(std::unique_ptr<FocalElement>(
                new CompositeFocalElement(extend_right ? focal_elements[i]->clone() : discernment_frame_2->clone(),
                                          extend_right ? discernment_frame_2->clone() : focal_elements[i]->clone())),
                              mass_array[i]);
    }
    double conf = conflict();
    if (conf > 0) {
        std::unique_ptr<FocalElement> empty_set = discernment_frame->clone();
        empty_set->clear();
        outev.addFocalElement(std::unique_ptr<FocalElement>(
                new CompositeFocalElement(extend_right ? std::move(empty_set) : discernment_frame_2->clone(),
                                          extend_right ? discernment_frame_2->clone() : std::move(empty_set))),
                              conf);
    }

    return outev;
}

Evidence Evidence::marginalization(bool marginalize_right) const {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    auto *df = dynamic_cast<const CompositeFocalElement *>(discernment_frame.get());
    if (df == nullptr)return *this;

    std::unique_ptr<FocalElementContainer> new_fecontainer = dispatcher->getContainer(*discernment_frame);

    double ignorance_new = ignorance;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        auto *fe = dynamic_cast<const CompositeFocalElement *>(focal_elements[i].get());
        if ((marginalize_right && *fe->getLeft() == *df->getLeft()) ||
            (!marginalize_right && *fe->getRight() == *df->getRight()))
            ignorance_new += mass_array[i];
        else if ((marginalize_right && fe->getLeft()->isEmpty()) ||
                 (!marginalize_right && fe->getRight()->isEmpty()))
            continue;
        else new_fecontainer->push(marginalize_right ? fe->getLeft()->clone() : fe->getRight()->clone(), mass_array[i]);
    }


    Evidence outev(dispatcher->clone(), std::move(new_fecontainer),
                   marginalize_right ? df->getLeft()->clone() : df->getRight()->clone(),
                   ignorance_new);


    return outev;
}


Evidence Evidence::vacuous_extension_and_conjuction(const Evidence &other) {
    if (!isValidBBA() || !other.isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    Evidence outev(dispatcher->clone(), std::unique_ptr<FocalElement>(
            new CompositeFocalElement(discernment_frame->clone(), other.getDiscernment_frame()->clone())),
                   ignorance * other.getIgnorance());
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < focal_elements.size(); ++i) {
        for (int j = 0; j < other.getFocal_elements().size(); ++j) {
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(focal_elements[i]->clone(), other.getFocal_elements()[j]->clone())),
                                  mass_array[i] * other.getMassArray()[j]);
        }
        if (other.getIgnorance() > 0)
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(focal_elements[i]->clone(), other.getDiscernment_frame()->clone())),
                                  mass_array[i] * other.getIgnorance());
    }
    if (ignorance > 0) {
        for (int k = 0; k < other.getFocal_elements().size(); ++k) {
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(discernment_frame->clone(), other.getFocal_elements()[k]->clone())),
                                  ignorance * other.getMassArray()[k]);
        }
    }
    double conf = conflict();
    if (conf > 0) {
        std::unique_ptr<FocalElement> empty_set = discernment_frame->clone();
        empty_set->clear();
        for (int k = 0; k < other.getFocal_elements().size(); ++k) {
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(empty_set->clone(), other.getFocal_elements()[k]->clone())),
                                  conf * other.getMassArray()[k]);
        }
        if (other.getIgnorance() > 0)
            outev.addFocalElement(std::unique_ptr<FocalElement>(
                    new CompositeFocalElement(empty_set->clone(), other.getDiscernment_frame()->clone())),
                                  conf * other.getIgnorance());
    }

    return outev;
}

void Evidence::setMass(double mass, int index) {
    /*if (mass == 0)
        throw IncompatibleTypeError(
                "Mass value has to be > 0. Delete the corresponding focal element to have the same behavior.");*/
    if (mass < 0)throw IncompatibleTypeError("Mass value has to be > 0.");
    fecontainer->set(index, mass);
}

void Evidence::setMass(double mass, const FocalElement &fe) {
    /*if (mass == 0)
        throw IncompatibleTypeError(
                "Mass value has to be > 0. Delete the corresponding focal element to have the same behavior.");*/
    if (mass < 0)throw IncompatibleTypeError("Mass value has to be > 0.");
    fecontainer->set(fe, mass);
}

void Evidence::deleteFocalElement(int index) {
    fecontainer->erase(index);
}

void Evidence::deleteFocalElement(const FocalElement &fe) {
    fecontainer->erase(fe);
}


Evidence::Evidence(const Evidence &other) {
    discernment_frame = other.getDiscernment_frame()->clone();
    ignorance = other.getIgnorance();
    dispatcher = other.dispatcher->clone();
    fecontainer = dispatcher->getContainer(*discernment_frame);
    const std::vector<std::unique_ptr<FocalElement>> &fe_other = other.getFocal_elements();
    const std::vector<double> &ms_other = other.getMassArray();
    for (int i = 0; i < ms_other.size(); ++i) {
        fecontainer->push(fe_other[i]->clone(), ms_other[i]);
    }

}

Evidence &Evidence::operator=(const Evidence &other) {

    discernment_frame = other.getDiscernment_frame()->clone();
    ignorance = other.getIgnorance();
    dispatcher = other.dispatcher->clone();
    fecontainer = dispatcher->getContainer(*discernment_frame);
    const std::vector<std::unique_ptr<FocalElement>> &fe_other = other.getFocal_elements();
    const std::vector<double> &ms_other = other.getMassArray();
    for (int i = 0; i < ms_other.size(); ++i) {
        fecontainer->push(fe_other[i]->clone(), ms_other[i]);
    }

    return *this;
}

double Evidence::getMass(const FocalElement &fe) {
    return fecontainer->get(fe);
}

void Evidence::normalize() {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    double conf = conflict();
    if (conf == 0.0)return;
    const std::vector<double> &mass_array = fecontainer->getMassArray();
    for (int i = 0; i < mass_array.size(); ++i) {
        fecontainer->set(i, mass_array[i] / (1 - conf));
    }
    ignorance /= (1 - conf);
}

Evidence Evidence::conditioning(const FocalElement &C) {
    if (!isValidBBA()) {
        throw InvalidBBAError("Cannot apply method to an invalid BBA.");
    }
    if (*discernment_frame == C) return *this;
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();
    const std::vector<double> &mass_array = fecontainer->getMassArray();

    std::unique_ptr<FocalElementContainer> new_fecontainer = dispatcher->getContainer(*discernment_frame);

    for (int i = 0; i < focal_elements.size(); ++i) {
        new_fecontainer->push(std::move(focal_elements[i]->intersect(C)), mass_array[i]);
    }

    Evidence outev(dispatcher->clone(), std::move(new_fecontainer), discernment_frame->clone(), 0);
    return outev;
}

size_t Evidence::numFocalElements() const {
    return fecontainer->getFocalElementsArray().size();
}

double Evidence::getMass(size_t i) const {
    if (i < 0 || i > fecontainer->getMassArray().size()) throw IllegalArgumentError("Out of bounds");
    return fecontainer->getMassArray()[i];
}

double Evidence::BetP(size_t i) {
    if (i < 0 || i > fecontainer->getMassArray().size()) throw IllegalArgumentError("Out of bounds");
    const std::unique_ptr<FocalElement> &elem = fecontainer->getFocalElementsArray()[i];
    return BetP(*elem);
}

void Evidence::setGSSF() {
    is_gssf = true;
}

bool Evidence::isConsonant() {
    const std::vector<std::unique_ptr<FocalElement>> &focal_elements = fecontainer->getFocalElementsArray();

    //Sort by cardinality
    std::vector<size_t> indices(focal_elements.size());
    for (size_t l = 0; l < focal_elements.size(); ++l) {
        indices[l] = l;
    }
    std::sort(indices.begin(), indices.end(),
              [&](size_t x, size_t y) { return focal_elements[x]->cardinality() < focal_elements[y]->cardinality(); });

    for (int i = 0; i < indices.size() - 1; ++i) {
        if (!focal_elements[indices[i]]->inside(*focal_elements[indices[i + 1]]))return false;
    }

    return true;
}






















