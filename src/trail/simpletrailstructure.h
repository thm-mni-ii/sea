#ifndef SRC_TRAIL_SIMPLETRAILSTRUCTURE_H_
#define SRC_TRAIL_SIMPLETRAILSTRUCTURE_H_

#include <sealib/collection/bitset.h>
#include <vector>

namespace Sealib {
/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 * @author Johannes Meintrup
 */
class SimpleTrailStructure {
 private:
    uint64_t degree;
    uint64_t nextUnused;
    uint64_t lastClosed;

    Sealib::Bitset<uint8_t> inAndOut;
    Sealib::Bitset<uint8_t> matched;

    Sealib::Bitset<uint8_t> flags;

    uint64_t married[4];

    std::vector<uint64_t> unused;

    /**
     * Removes nextUnused from the double linked unused array.
     * Updates links and returns nextUnused.
     * @return nextUnused.
     */
    inline uint64_t getNextUnused();

 public:
    /**
     * Creates a trailsture object.
     * @param _degree Degree of the node, equals the number of outgoing arcs.
     */
    explicit SimpleTrailStructure(uint64_t _degree);

    /**
     * Checks if the TrailStructure is currently grey.
     * Grey meaning atleast one arc has been used so far.
     * @return true when grey, false otherwise
     */
    bool isGrey() const;

    /**
     * Checks whether a TrailStructure is black.
     * Black meaning all arcs have been traversed.
     * @return true when black, false otherwise
     */
    bool isBlack() const;

    /**
     * Checks whether a TrailStructure is even.
     * Even meaning the number of unused arcs is even..
     * @return true when even, false otherwise
     */
    bool isEven() const;

    /**
     * @return value of lastClosed variable.
     */
    uint64_t getLastClosed() const;

    /**
     * Leaves the node, gets arbitrary element from unused,
     * moves it to InAndOut and returns it.
     * @return arbitrary unused element; if the TrailStructure is black, returns
     * uint64_t max value.
     */
    uint64_t leave();

    /**
     * @return Starting index of a Trail, or (uint64_t) - 1
     */
    uint64_t getStartingArc() const;

    /**
     * @return true if there is a starting arc (unmatched, outgoing edge)
     */
    bool hasStartingArc() const;

    /**
     * Enters the node at the specified edge/arc, and if there is an unused arc
     * left, leaves it at the next arc and matches the entering and exiting
     * arcs. Otherwise the entering arc is left unmatched.
     * @param i arc to enter
     * @return arc that was left, or unsiged int max value if no arc.
     */
    uint64_t enter(uint64_t i);

    /**
     * Gets the match for a given matched arc.
     * The inAndOut bit array is interpreted as a dyckword.
     * @param idx arc
     * @return match
     */
    uint64_t getMatched(uint64_t idx) const;

    /**
     * Matches the elements i and o.
     * This is done by marking them in the married array.
     * Unmatches all previously matched elements to i and o.
     * @param i first element to be matched
     * @param o second element to be matched
     */
    void marry(uint64_t i, uint64_t o);
};
}  // namespace Sealib
#endif  // SRC_TRAIL_SIMPLETRAILSTRUCTURE_H_
