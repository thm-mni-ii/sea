#ifndef SEALIB_TRAILSTRUCTURE_H_
#define SEALIB_TRAILSTRUCTURE_H_

#include <sealib/localdycktable.h>
#include <sealib/dictionary/rankselect.h>
#include <sealib/dictionary/simplerankselect.h>
#include <sealib/dyckmatchingstructure.h>
#include <sealib/largedoublelinkedlist.h>
#include <vector>

/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 */
namespace Sealib {
class TrailStructure {
 private:
    uint32_t lastClosed;
    uint32_t dyckStart;

    Sealib::Bitset<uint8_t> inAndOut;
    Sealib::Bitset<uint8_t> matched;
    Sealib::Bitset<uint8_t> flags;

    std::unique_ptr<DyckMatchingStructure> dyckMatchingStructure;
    std::unique_ptr<std::vector<uint32_t>> married;
    std::unique_ptr<DoubleLinkedList> unused;

    /**
     * Removes nextUnused from the double linked unused array.
     * Updates links and returns nextUnused.
     * @return nextUnused.
     */
    inline uint32_t getNextUnused();

    /**
     * initializes the DyckMatchingStructure used by getMatched
     */
    inline void initDyckStructure();

 public:
    /**
     * @return value of degree
     */
    uint32_t getDegree() const;

    /**
     * @return ref to inAndOut bitset
     */
    const Sealib::Bitset<uint8_t> &getInAndOut() const;

    /**
     * @return ref to matched bitset
     */
    const Sealib::Bitset<uint8_t> &getMatchedBitset() const;

    /**
     * @return ref to dyckWord
     */
    const Sealib::Bitset<uint8_t> &getDyckWord() const;

    /**
     * Constructor for the TrailStructure object.
     * @param _degree Degree of the node, equals the number of outgoing arcs.
     */
    explicit TrailStructure(uint32_t _degree);

    /**
     * check if the TrailStructure is currently grey.
     * Grey meaning atleast one arc has been used so far.
     * @return true when grey, false otherwise
     */
    bool isGrey() const;

    /**
     * check whether a TrailStructure is black.
     * Black meaning all arcs have been traversed.
     * @return true when black, false otherwise
     */
    bool isBlack() const;

    /**
     * check whether a TrailStructure is even.
     * Even meaning the number of unused arcs is even..
     * @return true when even, false otherwise
     */
    bool isEven() const;

    /**
     * Leaves the node, gets arbitrary element from unused,
     * moves it to InAndOut and returns it.
     * If the TrailStructure is black, it returns uint32_t max value.
     * @return
     */
    uint32_t leave();

    /**
     * Enters the node at the specified edge/arc, and if there is an unused arc left,
     * leaves it at the next arc and matches the entering and exiting arcs.
     * Otherwise the entering arc is left unmatched.
     * @param i arc to enter
     * @return arc that was left, or unsiged int max value if no arc.
     */
    uint32_t enter(uint32_t i);

    /**
     * Matches the elements i and o.
     * This is done by marking them in the married array.
     * Unmatches all previously matched elements to i and o.
     * @param i first element to be matched
     * @param o second element to be matched
     */
    void marry(uint32_t i, uint32_t o);

    /**
     * @return value of lastClosed variable.
     */
    uint32_t getLastClosed() const;

    /**
     * @return Starting index of a Trail, or (uint32_t) - 1
     */
    uint32_t getStartingArc() const;

    /**
     * @return true if there is a starting arc (unmatched, outgoing edge)
     */
    bool hasStartingArc() const;

    /**
     * Checks if the arc at idx i is the end of a trail.
     * The arc is the end of a trail if it's unmatched and is an entering edge.
     * @param i index to check
     * @return true if ending, false otherwise
     */
    bool isEndingArc(uint32_t i) const;

    /**
     * @return value of dyckStart
     */
    uint32_t getDyckStart() const;

    /**
     * Gets the match for a given matched arc.
     * The inAndOut bit array is interpreted as a dyckword.
     * @param idx
     * @return
     */
    uint32_t getMatched(uint32_t idx) const;

    /**
     * Used for debugging
     * @param idx
     * @return
     */
    uint32_t getMatchedNaive(uint32_t idx);
};
}  // namespace Sealib
#endif  // SEALIB_TRAILSTRUCTURE_H_
