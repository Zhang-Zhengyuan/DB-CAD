/* $Id: cvty_att.hxx,v 1.5 2001/05/22 15:43:35 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file defining attributes for local ops.

#if !defined( LOPT_CVTY_ATTRIB )
#define LOPT_CVTY_ATTRIB

#include "acis.hxx"
#include "dcl_lopt.h"

#include "tophdr.hxx"
#include "attrib.hxx"
#include "at_sys.hxx"
#include "lists.hxx"
#include "vlists.hxx"

#include "position.hxx"
#include "intcusf.hxx"

// handle use-counted entities correctly in rollback
#include "usecount.hxx"

#include "cvty.hxx"

class ENTITY;
class SURFACE;
class FACE;
class EDGE;
class APOINT;
class VERTEX;
class CURVE;

class curve;
class surface;

class PCURVE;
class pcurve;

#if defined D3_STANDALONE || defined D3_DEBUG

class   D3_ostream;
class   D3_istream;

#endif

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOPT_EDGE_CVTY, LOPT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// ******************************************
// now define the convexity attribute

extern DECL_LOPT int ATTRIB_LOPT_EDGE_CVTY_TYPE;
#define ATTRIB_LOPT_EDGE_CVTY_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOPT ATTRIB_LOPT_EDGE_CVTY: public ATTRIB_SYS {
private:
    // data
	LOPT_EDGE_cvty _cvty;       // original convexity of edge

	// set data value
	void set_convexity();

public:
	// Simple read functions for the data members.
	// original data
    EDGE   				*edge() const { return ( EDGE * ) entity(); }
    LOPT_EDGE_cvty 		convexity() const { return _cvty; }

	// Constructors
	ATTRIB_LOPT_EDGE_CVTY( ENTITY *own = NULL );

	ATTRIB_LOPT_EDGE_CVTY( ENTITY *own, LOPT_EDGE_cvty const cvty );

	// Make a copy of a lop cvty attrib ( provokes error )
	ATTRIB_LOPT_EDGE_CVTY(  ATTRIB_LOPT_EDGE_CVTY const * );
	virtual ATTRIB_LOPT_EDGE_CVTY* copy() const;

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	// merge_owner: the owner of this attribute is about to be merged 
	// with the "other_entity".  After the merge, this owner will be 
	// deleted if the logical "deleting_owner" is TRUE, otherwise it 
	// will be retained and "other_entity" will be deleted.
	void merge_owner(
			 ENTITY *,	// "other entity"
			 logical		// TRUE if owner of this attribute
			 // will be deleted
			 );
									
	// Test two LOPT_EDGE__CVTY_ATTRIBS for equality.
	virtual logical operator==( ATTRIB_LOPT_EDGE_CVTY const& ) const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_LOPT_EDGE_CVTY, LOPT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
};

#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_LOPT D3_ostream& operator<<( D3_ostream& os, 
								 const ATTRIB_LOPT_EDGE_CVTY& );

#endif

extern DECL_LOPT ATTRIB_LOPT_EDGE_CVTY *find_lopt_cvty_attrib( EDGE *ed );


#endif



