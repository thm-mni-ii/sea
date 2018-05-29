//
// Created by jmeintrup on 27.05.18.
//
#include <sealib/simpletrail.h>

void Sealib::SimpleTrail::insertSubTrail(const SimpleTrail &subTrail, unsigned int idx) {
    std::vector<unsigned int> oldTrail = trail;
    trail.resize(trail.size() + subTrail.getTrail().size());
    for(unsigned long i = 0; i < subTrail.getTrail().size(); i++) {
        trail[i+idx] = subTrail.getTrail()[i];
    }
    for(unsigned long i = idx + subTrail.getTrail().size(); i < trail.size(); i++) {
        unsigned int oldVal = oldTrail[i - subTrail.getTrail().size()];
        trail[i] = oldVal;
    }
}

void Sealib::SimpleTrail::addEdge(unsigned int edge) {
    trail.push_back(edge);
}

const std::vector<unsigned int> &Sealib::SimpleTrail::getTrail() const {
    return trail;
}

Sealib::SimpleTrail::SimpleTrail() = default;

Sealib::SimpleTrail::SimpleTrail(unsigned int toReserve) : trail(toReserve){

}
