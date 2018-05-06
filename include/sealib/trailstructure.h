//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_TRAILSTRUCTURE_H
#define SEA_TRAILSTRUCTURE_H

#include <vector>
#include <sealib/dynamicbitsetextension.h>
#include "localdycktable.h"
#include "rankselect.h"

/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 */
namespace Sealib {
    class TrailStructure{
    private:
        unsigned int degree;
        unsigned int nextUnused;
        unsigned int lastClosed;
        unsigned int dyckStart;

    public:
        unsigned int getDegree() const;

        const boost::dynamic_bitset<> &getInAndOut() const;

        const boost::dynamic_bitset<> &getMatched() const;

    private:
        boost::dynamic_bitset<> inAndOut;
        boost::dynamic_bitset<> matched;

        boost::dynamic_bitset<> dyckWord;
        std::vector<unsigned int> pioneerTable;
        boost::dynamic_bitset<> pioneerDyckWord;

        boost::dynamic_bitset<> pioneerRankSelectBitSet;
        RankSelect pioneerRankSelect;

        boost::dynamic_bitset<> flags; //at(0) flipped == grey, at(1) flipped == black, at(2) flipped = uneven

        LocalDyckTable& table;


    private:
        unsigned int *married;

        unsigned int *unused;
        /**
         * Removes nextUnused from the double linked unused array.
         * Updates links and returns nextUnused.
         * @return nextUnused.
         */
        inline unsigned int getNextUnused();

        /**
         * Creates the dyckword formed by in and out arcs once the trailstructure turns black.
         */
        void initDyckStructures();

        static unsigned long findMatchNaive(boost::dynamic_bitset<> &word, unsigned long idx);

    public:
        virtual ~TrailStructure();



        /**
         * Reference to the dyck word formed by in and out arcs.
         * Call only on black trailstructures.
         * @return ref to dyckWord
         */
        const boost::dynamic_bitset<> &getDyckWord() const;

        /**
         * Constructor for the TrailStructure object.
         * @param _degree Degree of the node, equals the number of outgoing arcs.
         */
        explicit TrailStructure(unsigned int _degree, LocalDyckTable& _table);

        /**
         * Inline function to check if the TrailStructure is currently grey.
         * Grey meaning atleast one arc has been used so far.
         * @return true when grey, false otherwise
         */
        bool isGrey();

        /**
         * Inline function to check whether a TrailStructure is black.
         * Black meaning all arcs have been traversed.
         * @return true when black, false otherwise
         */
        bool isBlack();

        /**
         * Inline function to check whether a TrailStructure is even.
         * Even meaning the number of unused arcs is even..
         * @return true when even, false otherwise
         */
        bool isEven();


        /**
         * Leaves the node, gets arbitrary element from unused,
         * moves it to InAndOut and returns it.
         * If the TrailStructure is black, it returns unsigned int max value.
         * @return
         */
        unsigned int leave();

        /**
         * Enters the node at the specified edge/arc, and if there is an unused arc left,
         * leaves it at the next arc and matches the entering and exiting arcs.
         * Otherwise the entering arc is left unmatched.
         * @param i arc to enter
         * @return arc that was left, or unsiged int max value if no arc.
         */
        unsigned int enter(unsigned int i);

        /**
         * Gets the match for a given matched arc.
         * The inAndOut bit array is interpreted as a dyckword.
         * @param idx
         * @return
         */
        unsigned int getMatched(unsigned int idx);

        /**
         * Matches the elements i and o.
         * This is done by marking them in the married array.
         * Unmatches all previously matched elements to i and o.
         * @param i first element to be matched
         * @param o second element to be matched
         */
        void marry(unsigned int i, unsigned int o);

        /**
         * Getter for lastClosed.
         * @return value of lastClosed variable.
         */
        unsigned int getLastClosed(){
            return lastClosed;
        }

        /**
         * Returns the index of the starting arc of a Trail.
         * If the TrailStructure has no starting arc, returns (unsigned int) - 1.
         * @return Starting index of a Trail, or (unsigned int) - 1
         */

        unsigned int getStartingArc();

        /**
         * Checks if the arc at idx i is the end of a trail.
         * The arc is the end of a trail if it's unmatched and is an entering edge.
         * @param i index to check
         * @return true if ending, false otherwise
         */
        bool isEndingArc(unsigned int i);


        /**
         * @return value of dyckStart
         */
        unsigned int getDyckStart();

        unsigned int getMatchedNew(unsigned int idx);

        void initPioneerTable();

        void initDyckWord();

        void initPioneerRankSelect();

        void initPioneerWord();
    };
}

#endif //SEA_TRAILSTRUCTURE_H
