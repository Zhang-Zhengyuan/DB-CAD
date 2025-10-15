/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmVolumeAssoc_hxx
#define SPA3dPmVolumeAssoc_hxx

#include "cell.hxx"
#include "SPA3dPmACIS.h"

/**
* @file SPA3dPmVolumeAssoc.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

struct Cell_VolumeElem_Assoc_Impl;

/**
* The <tt>Cell_VolumeElem_Assoc</tt> class can be used to find 3D volume mesh elements for an ACIS model CELL or
* a CELL for a volume mesh element. Objects of the class are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>Cell_VolumeElem_Assoc</tt> object does not duplicate the underlying association object implementation, it only increases
* the number of light objects referencing the actual association data. The default constructor <tt>Cell_VolumeElem_Assoc()</tt> creates
* an object that points to no underlying data, and thus it is considered an uninitialized object. To create an initialized object from a mesh
* call the API function @href <tt>api_3dpm_create_volume_assoc</tt>.
*/
class DECL_SPA3dpm Cell_VolumeElem_Assoc
{
public:

	/**
	* Default constructor.
	* Creates an uninitialized object. It refers to no actual association data. Use the assignment operator or the API function @href <tt>api_3dpm_create_volume_assoc</tt> to make the object refer to some existing data.
	*/
	Cell_VolumeElem_Assoc();

	/**
	* Copy constructor. The new object will reference the same association data as the original object.
	*/
	Cell_VolumeElem_Assoc(const Cell_VolumeElem_Assoc &other);

	/**
	* Assignment operator. The "this" object will reference the same association data as the "other" object.
	*/
	const Cell_VolumeElem_Assoc &operator=(const Cell_VolumeElem_Assoc &other);

	/**
	* Destructor. It only deletes the underlying data structure if this object was the last reference to it.
	*/
	~Cell_VolumeElem_Assoc();

	/**
	* Returns true if the object refers to actual association data.
	*/
	bool Initialized() const;

	/**
	* 3D volume element type.
	* @param Tetra
	* @param Hexa
	* @param Pyramid
	* @param Prism
	*/
	enum VolumeElemType { Tetra, Hexa, Pyramid, Prism };

	/**
	* <tt>VolumeElem</tt> identifies a 3D volume mesh element in a mesh.
	*/
	struct DECL_SPA3dpm VolumeElem
	{
		/**
		* Type of the 3D volume element.
		*/
		VolumeElemType elemType;

		/**
		* Index of element from 1 to the number of elements of type <tt>elemType</tt>.
		*/
		int elemIndex;

		/**
		* Construct an object with initial values.
		*/
		VolumeElem(VolumeElemType inElemType, int inElemIndex);

		/**
		* Default constructor, class members are uninitialized.
		*/
		VolumeElem();
	};

	/**
	* A read-only array of <tt>VolumeElem</tt>'s.
	*/
	class DECL_SPA3dpm VolumeElemArray
	{
	public:

		/**
		* Default constructor creates an empty array.
		*/
		VolumeElemArray();

		/**
		* Copy constructor.
		*/
		VolumeElemArray(const VolumeElemArray &other);

		/**
		* Assignment operator.
		*/
		const VolumeElemArray &operator=(const VolumeElemArray &other);

		/**
		* Move constructor.
		*/
		VolumeElemArray(VolumeElemArray &&other);

		/**
		* Move assignment operator.
		*/
		const VolumeElemArray &operator=(VolumeElemArray &&other);

		/**
		* Destructor.
		*/
		~VolumeElemArray();

		/**
		* Get the size of the array.
		* @return The number of elements in the array.
		*/
		size_t Size() const;

		/**
		* Get an element of the array.
		* @param[in] index : the index of the element, starting from 0.
		* @return Reference to the element of the given index.
		*/
		const VolumeElem &operator[](size_t index) const;

		/**
		* Delete all elements of the array and make it empty.
		*/
		void Clear();

	private:
		void *impl;
		friend struct II;
	};

	/**
	* Find the ACIS model CELL that contains a given 3D volume mesh element.
	* @param[in] volElem : the volume element (tetrahedron, hexahedron, pyramid or prism) that we are trying to locate
	* @return The CELL containing volElem, or NULL in case of error.
	*/
	CELL *FindCell(const VolumeElem &volElem) const;

	/**
	* Find all 3D volume mesh elements that are contained within an ACIS model CELL.
	* @param[in] cell : the CELL whose 3D volume elements (tetrahedra, hexahedra, pyramids and prisms) we are trying to obtain
	* @param[out] volElements: the array of elements inside the cell.
	*/
	void FindVolumeElements(CELL *cell, VolumeElemArray &volElements) const;

private:
	Cell_VolumeElem_Assoc_Impl *impl;
	friend struct II;
};

}

/** @} */
#endif
