/* ORIGINAL: acis2.1/sg_husk/sweep/at_path.hxx */
// $Id: at_srel.hxx,v 1.6 2001/05/22 15:42:17 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined ( AT_REL_HXX )
#define AT_REL_HXX

#include "dcl_kern.h"
#include "entity.hxx"
#include "at_sg.hxx"

class COEDGE;
class VERTEX;

extern DECL_KERN int ATTRIB_STI_REL_ATTR_TYPE;

#define ATTRIB_STI_REL_ATTR_LEVEL ( ATTRIB_SG_LEVEL + 1 )
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_STI_REL_ATTR, NONE )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

class DECL_KERN ATTRIB_STI_REL_ATTR : public ATTRIB_SG
{
	COEDGE *from_cd;
	COEDGE *to_cd;
	VERTEX *from_vt;
	VERTEX *to_vt;

public:

	ATTRIB_STI_REL_ATTR(
					ENTITY* = NULL,
					COEDGE* = NULL,
					COEDGE* = NULL,
					VERTEX* = NULL,
					VERTEX* = NULL );

    virtual logical pattern_compatible() const;
                                    // returns TRUE 

	COEDGE *from_coed() const { return from_cd; }
	COEDGE *to_coed() const { return to_cd; }
	VERTEX *from_vertex() const { return from_vt; }
	VERTEX *to_vertex() const { return to_vt; }

	
	// Functions called to aid attribute migration during 
	// modelling operations.

	// Implment the split_owner, which is basically to copy the
	// attribute onto the new entity.  The reason for implementing
	// this is because this attribute is now being used in skin
	// to save the relation information, and the entity which has
	// this attribute may get split some-times.

	virtual void split_owner( ENTITY *);

//	virtual logical copyable() const { return duplicatable(); }

//	virtual logical duplicatable() const { return TRUE; }

	// We do not need any of these, as this is a temporary attribute 
	// used for passing data between the various  functions in sweep.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_STI_REL_ATTR, NONE )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

#endif
