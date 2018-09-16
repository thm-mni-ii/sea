#include <sealib/graphalgorithm.h>

std::unique_ptr<std::vector<Sealib::SimpleTrailStructure>> Sealib::GraphAlgorithm::hierholzerSimple(Sealib::BasicGraph &graph) {
    return std::unique_ptr<std::vector<Sealib::SimpleTrailStructure>>();
}
std::unique_ptr<std::vector<Sealib::TrailStructure>> Sealib::GraphAlgorithm::hierholzer(Sealib::BasicGraph &graph) {
    return std::unique_ptr<std::vector<Sealib::TrailStructure>>();
}
std::unique_ptr<Sealib::NaiveTrail> Sealib::GraphAlgorithm::hierholzerNaive(Sealib::BasicGraph &graph) {
    return std::unique_ptr<Sealib::NaiveTrail> ();
}
