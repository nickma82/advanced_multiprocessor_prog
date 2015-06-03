/*
 * AMPSet.h
 *
 * Advanced Multiprocessor Programming
 * Author: kpozniak
 *
 */

#ifndef AMP_SET
#define AMP_SET

/*
Basic interface for list based sets.
A set may contain each item at most once.
Only for long datatype for now, genericity  will be added later
*/
class AMPSet
{

public:
	//adds an item to the set
	//returns true if successfull, false if the item already was in the set
	virtual bool add(long item) = 0;
	
	//removes an item from the set
	//returns true if successfull, false if the item was not in the set
	virtual bool remove(long item) = 0;
	
	//checks if an item is contained in a set
	//return true if the item is contained, false if not
	virtual bool contains(long item) = 0;
};



#endif  // AMP_SET
