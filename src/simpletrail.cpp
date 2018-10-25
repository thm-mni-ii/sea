#include <src/simpletrail.h>

void Sealib::SimpleTrail::insertSubTrail(const SimpleTrail &subTrail, unsigned long idx) {
    std::vector<std::tuple<unsigned int, unsigned int>> oldTrail = trail;
    trail.resize(trail.size() + subTrail.getTrail().size());
    for (unsigned long i = 0; i < subTrail.getTrail().size(); i++) {
        trail[i + idx] = subTrail.getTrail()[i];
    }
    for (unsigned long i = idx + subTrail.getTrail().size(); i < trail.size(); i++) {
        std::tuple<unsigned int, unsigned int> oldVal = oldTrail[i - subTrail.getTrail().size()];
        trail[i] = oldVal;
    }
}

void Sealib::SimpleTrail::addArc(std::tuple<unsigned int, unsigned int> arc) {
    trail.push_back(arc);
}

const std::vector<std::tuple<unsigned int, unsigned int>> &Sealib::SimpleTrail::getTrail() const {
    return trail;
}

Sealib::SimpleTrail::SimpleTrail() = default;

std::tuple<unsigned int, unsigned int> Sealib::SimpleTrail::getOutgoingFrom(unsigned int u) const {
    for (unsigned int i = 0; i < trail.size(); i++) {
        if (std::get<0>(trail[i]) == u) {
            return trail[i];
        }
    }
    return std::make_tuple((unsigned int) -1, (unsigned int) -1);
}

unsigned long
Sealib::SimpleTrail::getFirstIndexOf(std::tuple<unsigned int, unsigned int> arc) const {
    for (unsigned int i = 0; i < trail.size(); i++) {
        if (trail[i] == arc) {
            return i;
        }
    }
    return (unsigned long) -1;
}

void Sealib::SimpleTrail::pushBackSubTrail(const Sealib::SimpleTrail &subTrail) {
    unsigned long oldSize = trail.size();
    trail.resize(trail.size() + subTrail.getTrail().size());
    for (unsigned long i = oldSize; i < trail.size(); i++) {
        trail[i] = subTrail.getTrail()[i - oldSize];
    }
}
