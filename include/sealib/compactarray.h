#ifndef COMPACTARRAY_H
#define COMPACTARRAY_H
#include <math.h>
//#define COMPACTARRAY_DEBUG
#define COMPACTARRAY_DATA 100
#define COMPACTARRAY_GROUP 10
#define COMPACTARRAY_VALUE 1
#define COMPACTARRAY_FAULT (unsigned)-99

/*
	This compact array tries to use the space of an unsigned int array as well as possible.
	
	The compact array consists of _data_ elements (the source array slots),
	which are split into a number of _groups_,
	which contain a number of _values_ (the slots of the compact array).
*/
class CompactArray {
	private:
		unsigned int *data;
		double e;
		/*
			data width: no. bits in unsigned int
			group width: no. bits a group occupies
			value width: no. bits a value needs (e.g. ld(3) for 3 possible states)
		*/
		unsigned int valueWidth,valueCount,
			groupWidth,groupCount,
			dataWidth,dataCount;
	
	public:
		/* Create a new compact array.
			@param count number of values this compact array can hold
			@param epsilon width parameter: 3/e must divide n
		 * If you don't know what to use for epsilon: 1.5 if count is even, 3.0 if count is odd.
		*/
		CompactArray(unsigned int count,double epsilon);
				
		~CompactArray();
		
		/* Insert a value to the given index.
			@param i the destination index
			@param p the value to insert
		*/
		void insert(unsigned int i,unsigned int p);
		
		/* Get an element from the specified compartment.
			@param selector level of the compact array to take the result from <br>
		 		COMPACTARRAY_DATA, COMPACTARRAY_GROUP or COMPACTARRAY_VALUE
			@param i index (depending on the selector, this will specify a data element,
				a group or a value index)
			@return the retrieved number
		 */
		unsigned int get(unsigned int selector,unsigned int i);
		/* Get a value from the compact array.
			@param i value index to get the value from
			@return the found value
		 */
		unsigned int get(unsigned int i);
};
#endif