/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//   Define a class for creating dynamic arrays of positions.  There is
//   an operator to cast a position_array to a SPAposition*, so it can be
//   used when a SPAposition* is needed, but it will automatically grow as
//   you add positions to it.
//----------------------------------------------------------------------

#ifndef position_array_hxx
#define position_array_hxx

//======================================================================
/**
* @file posarray.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */
#include "dcl_kern.h"
#include "position.hxx"

//======================================================================
/**
 * Creates dynamic arrays of positions.
 * <br>
 * <b>Role</b>:This class creates dynamic arrays of positions. There is an operator to cast a
 * position_array to a SPAposition* so it can be used when a SPAposition* is needed, but it
 * automatically grows as positions are added to it.
 * @see SPAposition
 */
class DECL_KERN position_array : public ACIS_OBJECT
{

protected:
	SPAposition* m_pPositions;
	int m_nNumPositions;
	int m_nArraySize;

	// Set the size of the array so that it contains at least size positions.
	// If the array is already big enough, the size will not be changed.
	void SetMinSize(int size);

public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	position_array();
	position_array(const char* gme);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * @param initialSize
 * initial size
 */
	position_array(int initialSize);
	position_array(const char* gme, int initialSize);
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * @param pos_arr
 * position_array
 */
	position_array(const position_array& pos_arr);
	position_array(const char* gme, const position_array& pos_arr);
/**
 * C++ destructor, deleting a position_array.
 */
	virtual ~position_array();

	// Return the number of positions in the array.
/**
 * Returns the size of the position_array.
 */
	int GetSize() const { return m_nNumPositions; }

	// Set the size of the array.  This will expand the array if needed.
	// If you set the size to be smaller than the current size, this
	// will set the number of positions, but not change the amount of
	// memory allocated. (see also: Shrink)
/**
 * Sets the size of the array, which expands the array, if needed.
 * <b>Role</b>: If size is smaller than the current size, this sets the number of positions, but
 * it does not change the amount of allocated memory.
 * @param size
 * size
 */
	void SetSize(int size);
	void gme_SetSize(int size);

	// Empty the array.  This will set the number of positions in the
	// array to zero, but will not actually free up the storage which
	// has been allocated.
/**
 * Empties the array.
 * <b>Role</b>: This sets the number of positions in the array to 0, but it does not free up
 * allocated storage.
 */
	void Empty() { SetSize(0); }
	void gme_Empty();

	// Truncate the size of the internal SPAposition buffer so that it is
	// exactly big enough to hold the specified number of positions
/**
 * Truncates the size of the internal position buffer so that it is exactly big enough to hold the specified number of positions.
 */
	void Shrink();
	void gme_Shrink();

	// Get the element at a given index in the array.  This will expand
	// the size of the array if needed.  The element is returned as a
	// reference so that it can be used on the left side of an
	// assignment.
/**
 * Gets the element at a given index in the array, which expands the size of the array if needed.
 * <b>Role</b>: The element returns as a reference so that it can be used on the left side of an
 * assignment.
 * @param nIndex
 * index value
 */
	SPAposition& ElementAt(int nIndex);
	SPAposition& gme_ElementAt(int nIndex);

	// This gives you a copy of the SPAposition at a given index.  It
	// provides access to a const position_array.  The index must be
	// within range.

/**
 * Provides a copy of the position at a given index.
 * <b>Role</b>: This method also provides access to a constant position_array. The index must be
 * within the range.
 * @param nIndex
 * index value
 */
	SPAposition PositionAt(int nIndex) const;
	SPAposition gme_PositionAt(int nIndex) const;

	// Get the last SPAposition in the array that is set.
/**
 * Gets the last position in the array that is set.
 */
	SPAposition GetLast() const;
	SPAposition gme_GetLast() const;

	// Add a SPAposition as the last SPAposition in the array.  The array will
	// be expanded as needed.  The Add and RemoveLast methods are for
	// convenience.  They let you add positions to the array without
	// having to keep track of the current index.

/**
 * Adds a SPAposition as the last position in the array, which expands the array as needed.
 * <b>Role</b>: Use the Add and RemoveLast methods for convenience to add positions to the array
 * without keeping track of the current index.
 * @param pos
 * position
 */
	int Add(const SPAposition& pos);
	int gme_Add(const SPAposition& pos);

	// Remove the last SPAposition in the array.  It is expected that this
	// will be used in conjunction withh Add.
/**
 * Removes the last position in the array. Use this with the Add method.
 */
	int RemoveLast();
	int gme_RemoveLast();

	// Make it look like an array.  Since this returns a SPAposition&, it can
	// be used on the left side of an assignment.  The array will be expanded
	// if needed because the index is too big.

/**
 * Makes the operator look like an array.
 * <b>Role</b>: Because this method returns a position, it can be used on the left side of an
 * assignment. The array is expanded, if needed, if the index is too large.
 * @param nIndex
 * index value
 */
	SPAposition& operator[](int nIndex) { return ElementAt(nIndex); }
	SPAposition& gme_operator_index(int nIndex);

	// This method lets you cast a position_array to a SPAposition* so that it
	// can be used as an argument in procedures which want a SPAposition*.
	// The pointer returned is only valid as long as the size of the
	// array is not changed.
/**
 * Returns a copy of the position.
 * <b>Role</b>: The user must must delete the returned buffer when it is no longer needed.
 */
	operator SPAposition*() const { return m_pPositions; }

/**
 * Casts a position_array to a SPAposition* so that it can be used as an argument in procedures that require a SPAposition*.
 * <b>Role</b>: The returned pointer is only valid as long as the size of the array is not changed.
 */
	SPAposition* GetBuffer() const { return m_pPositions; }

	// This returns a copy of the SPAposition* buffer.  You must delete the
	// buffer returned when you are through with it.

/**
 * Copies the position_array.
 * <b>Role</b>: The user must delete the returned buffer when it is no longer needed.
 */
	SPAposition* CopyBuffer() const;
	SPAposition* gme_CopyBuffer() const;
};

//======================================================================
/** @} */
#endif
