/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/**
 * @fullreview CFV kpl 04:01:19
 */

#ifndef  SPAXIGES_VDAHUSK_ATTRIB__HXX
#define  SPAXIGES_VDAHUSK_ATTRIB__HXX

#include "dcl_kern.h"
#include "attrib.hxx"
#include "entity.hxx"
#include "datamsc.hxx"

#define MAX_LAYER_SIZE 10

// knt 20 Nov 2004. Added iop attributes to spakern.

extern DECL_KERN  int ATTRIB_GSSL_VDA_TYPE ;
#define ATTRIB_GSSL_VDA_LEVEL (ATTRIB_LEVEL + 1)
MASTER_ATTRIB_DECL( ATTRIB_GSSL_VDA, KERN)

extern DECL_KERN int ATTRIB_VDA_TYPE ;
#define ATTRIB_VDA_LEVEL (ATTRIB_GSSL_VDA_LEVEL + 1)

//---------------------------------------------------------------------

class DECL_KERN ATTRIB_VDA: public ATTRIB_GSSL_VDA
{
	char	*path ;
	int		sr_num ;

public :

	ATTRIB_VDA(
			ENTITY *owner = NULL,
			char *ele_pathname = NULL,
			int	 serial_number = 0 
			);

	char *pathname(void) const { return path ; }
	int   number(void) const { return sr_num ; }

	ATTRIB_FUNCTIONS( ATTRIB_VDA, NONE )
};

//----------------------------------------------------------------------

#endif //SPAXIGES_VDAHUSK_ATTRIB__HXX
