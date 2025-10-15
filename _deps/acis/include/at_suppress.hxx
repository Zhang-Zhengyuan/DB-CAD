/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef AT_SUPPRESS_H
#define AT_SUPPRESS_H

#include "dcl_asm.h"
#include "at_sys.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_SUPPRESS, ASM)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_ASM int ATTRIB_SUPPRESS_TYPE;
#define ATTRIB_SUPPRESS_LEVEL ( ATTRIB_SYS_LEVEL + 1 )

class DECL_ASM ATTRIB_SUPPRESS : public ATTRIB_SYS
{
public:

	ATTRIB_SUPPRESS(ENTITY* owner = NULL);

    virtual logical pattern_compatible() const;
                                    // returns TRUE 

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_SUPPRESS, ASM)

#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// do not doc
/**
 * @nodoc
 */
DECL_ASM ATTRIB_SUPPRESS* find_suppress_attrib(ENTITY* owner);

#endif // AT_SUPPRESS_H

