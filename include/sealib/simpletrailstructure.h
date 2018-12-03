#ifndef SEALIB_SIMPLETRAILSTRUCTURE_H_
#define SEALIB_SIMPLETRAILSTRUCTURE_H_

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
    uint32_t degree;
    uint32_t nextUnused;
    uint32_t lastClosed;

    Sealib::Bitset<uint8_t> inAndOut;
    Sealib::Bitset<uint8_t> matched;

    Sealib::Bitset<uint8_t> flags;

    uint32_t married[4];

    std::vector<uint32_t> unused;

    /**
     * Removes nextUnused from the double linked unused array.
     * Updates links and returns nextUnused.
     * @return nextUnused.
     */
    inline uint32_t getNextUnused();

 public:
    /**
     * Creates a trailsture object.
     * @param _degree Degree of the node, equals the number of outgoing arcs.
     */
    explicit SimpleTrailStructure(uint32_t _degree);

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
    uint32_t getLastClosed() const;

    /**
     * Leaves the node, gets arbitrary element from unused,
     * moves it to InAndOut and returns it.
     * If the TrailStructure is black, it returns uint32_t max value.
     * @return 
     */
    uint32_t leave();

    /**
     * @return Starting index of a Trail, or (uint32_t) - 1
     */
    uint32_t getStartingArc() const;

    /**
     * @return true if there is a starting arc (unmatched, outgoing edge)
     */
    bool hasStartingArc() const;

    /**
     * Enters the node at the specified edge/arc, and if there is an unused arc left, 
     * leaves it at the next arc and matches the entering and exiting arcs.
     * Otherwise the entering arc is left unmatched.
     * @param i arc to enter
     * @return arc that was left, or unsiged int max value if no arc.
     */
    uint32_t enter(uint32_t i);

    /**
     * Gets the match for a given matched arc.
     * The inAndOut bit array is interpreted as a dyckword.
     * @param idx 
     * @return 
     */
    uint32_t getMatched(uint32_t idx) const;

    /**
     * Matches the elements i and o.
     * This is done by marking them in the married array.
     * Unmatches all previously matched elements to i and o.
     * @param i first element to be matched
     * @param o second element to be matched
     */
    void marry(uint32_t i, uint32_t o);
};
}  // namespace Sealib
#endif  // SEALIB_SIMPLETRAILSTRUCTURE_H_
