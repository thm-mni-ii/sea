//
// Created by jmeintrup on 20.03.18.
//

#ifndef SEA_DYCKWORD_H
#define SEA_DYCKWORD_H


#include <vector>
/**
 * Container Class for a DyckWord.
 * A DyckWord is a word created by the Rule
 * S -> '(' S ')' | ϵ
 * '(' is being represented as a set bit, and ')' as an unset bit.
 */
class DyckWord {
private:
    /**
     * vector<bool> is internally represented as a bitset of variable length.
     */
    std::vector<bool>  bits;
public:

    /**
     * Flips the bit at position idx in the bit vector
     * @param idx index of the bit to be flipped
     */
    void flip(size_t idx);

    /**
    * Returns the value at position idx
    * @param idx index of the bit to be returned
    */
    std::_Bit_reference at(size_t idx);

    /**
     * Constructor of a DyckWord of length size.
     * All bits are initially set to false, which is equal to '('.
     * @param size size of the DyckWord
     */
    explicit DyckWord(size_t size);

    /**
     * Finds the matching closing or opening parantheses to the parentheses at index idx.
     * @param idx index of the parantheses to find it's given match
     * @return index of the match, or idx if there was no match found.
     */
    size_t findMatch(size_t idx);
};
#endif //SEA_DYCKWORD_H
