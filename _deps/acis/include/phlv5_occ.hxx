// COPYRIGHT DASSAULT SYSTEMES 1989
// Edge for Hidden Line Removal
#if !defined(PHLV5_OCC_CLASS)
#define PHLV5_OCC_CLASS
#include "dcl_phlv5.h"
#include "logical.h"
#include "curve.hxx"
#include "ent_phlv5.hxx"
#include "phlv5entmc.hxx"
#include "position.hxx"
#include "lists.hxx"
#include "box.hxx"
#include "transfrm.hxx"
#include "usecount.hxx"
/**
 * @file phlv5_occ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup PHLV5API
 *  \brief Declared at <phlv5_occ.hxx>
 *  @{
 */

class CURVE;
class FACE;
class BODY;
class PHLV5_SEGMENT;
class ENTITY_LIST;
class phlv5_segment;

/**
 * Derivation level
 */
#define PHLV5_OCC_LEVEL ( ENTITY_PHLV5_LEVEL + 1 )

extern DECL_PHLV5 int PHLV5_OCC_TYPE;

/**
 * Defines an PHLV5 occurrence.
 * <br>
 * <b>Role:</b> The <tt>PHLV5_OCC</tt> class acts as the primary place holder for <tt>TRANSFORM<tt>s of the differnet occurrences. Each PHLV5 edge points
 * one such occurrence. Many <tt>PHLV5_EDGE<tt> points to the same PHLV5_OCC becuase they are the result of the same occurrence and governed by the same <tt>TRANSFORM<tt> 
  * @see PHLV5_EDGE,BODY,
 */
class DECL_PHLV5 PHLV5_OCC : public ENTITY_PHLV5
{
	BODY * MyBody;
	TRANSFORM * MyTransform;
	void * MyUserData;
	public:

// Main constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param Body
 * Body.
 * @param Transform
 * Transform
 */
PHLV5_OCC(	BODY* Body=0,
			TRANSFORM * Trans=0,
			void * UserData = 0);


// Get the body of the edge
/**
 * Gets the <tt>BODY</tt> of the underlying modeling <tt>EDGE</tt>.
 */
BODY* GetBody() const {return (MyBody);};

void * GetUserData() const { return (MyUserData);};

TRANSFORM* GetTransform() const {return (MyTransform);};


// Copy Constructor
/**
 * C++ copy constructor.
 * <br><br>
 * @param e
 * PHLV5_OCC to copy.
 */
PHLV5_OCC(PHLV5_OCC const& e);
/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
 void clean();

/**
  * @nodoc
 */
ENTITY_FUNCTIONS ( PHLV5_OCC, PHLV5 )
/**
  * @nodoc
 */
 USE_COUNTED_DECL
virtual logical deletable() const;
virtual logical savable() const;
/**
  * Set the TRANSFORM for this occurrence.
 */
void SetTransform(TRANSFORM* transform);

/**
*Set the BODY pointer for this occurrence.
*/
void SetBody(BODY* body);

};
/*! @} */
#endif

