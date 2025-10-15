/* ORIGINAL: abl2.1/blend/kernbool/entent/bl_para.hxx */
/* $Id: bl_para.hxx,v 1.6 2002/08/09 17:28:29 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


//
// Modification---
//	08-Mar-96 bd	Add acis.hxx include.
//

#ifndef BL_PARA
#define BL_PARA

#include "dcl_blnd.h"
#include "acis.hxx"

#include "logical.h"

#include "param.hxx"
#include "surdef.hxx"

class COEDGE;
class SPAposition;
class SPApar_box;

class DECL_BLND bl_sided_par_pos : public SPApar_pos
{
public:

    bl_sided_par_pos( void )
        : SPApar_pos(), _uside( 99 ), _vside( 99 )
    {}

    bl_sided_par_pos( double uval, double vval, int us = 99, int vs = 99 )
        : SPApar_pos( uval, vval ), _uside( us ), _vside( vs )
    {}

    bl_sided_par_pos( const SPApar_pos &uv, int us = 99, int vs = 99 )
        : SPApar_pos( uv ), _uside( us ), _vside( vs )
    {}

	evaluate_surface_quadrant quad( void ) const;

    int _uside;
    int _vside;
};

// A class to represent parametric information about what a particular
// point is on. There may be none, e.g. when on a vertex, or a single
// SPAparameter, when lying on some curve, or two, when in the middle of a
// face.

class DECL_BLND bl_par_info : public ACIS_OBJECT
{

public:

	bl_par_info() 
        : _type( 0 ), _par0( 0 ), _par1( 0 ), _us( 99 ), _vs( 99 ) {}
	bl_par_info( double t, int side = 99 ) 
        : _type( 1 ), _par0( t ), _par1( 0 ), _us( side ), _vs( 99 ) {}
	bl_par_info( SPApar_pos const& uv )
		: _type ( 2 ), _par0( uv.u ), _par1( uv.v ), _us( 99 ), _vs( 99 ) {}
    bl_par_info( bl_sided_par_pos const &uv )
		: _type ( 2 ), _par0( uv.u ), _par1( uv.v ), 
          _us( uv._uside ), _vs( uv._vside )  
    {}

	// Query the type.

	logical none() const { return _type == 0; }
	logical param() const { return _type == 1; }
	logical pp() const { return _type == 2; }

	// Return the information, ASSUMING you know the correct type.

	double t() const { return _par0; }
	SPApar_pos uv() const { return SPApar_pos( _par0, _par1 ); }

    // For parameters on dicontinuities.

    int side() const { return _us; }
    int uside() const { return _us; }
    int vside() const { return _vs; }

    // Debug the information object.

    void debug( FILE *dfp ) const
    {
        char const *type_str = none() ? "None" : 
            ( param() ? "Curve parameter" : "Surface parameter" );
        fprintf( dfp, "\nType: %s", type_str );
        if ( param() )
        {
            fprintf( dfp, "\nParameter: %lf", t() );
            if ( side() != 99 )
                fprintf( dfp, " Evaluate side: %d", side() );
        }
        else if ( pp() )
        {
fprintf( dfp, "\nParameter: %g %g",(double) uv().u,(double) uv().v );
            if ( uside() != 99 )
                fprintf( dfp, " Evaluate u side: %d", uside() );                
            if ( vside() != 99 )
                fprintf( dfp, " Evaluate v side: %d", vside() );
        }
    }

private:

	int _type;		// i.e. the number of parameters, 0, 1 or 2.

	double _par0, _par1;
    int _us, _vs;

};

// Utility to return face parameters of pos on the coedge's face, where
// pos is at edge_par on the coedge's edge (NOT on the coedge). pos may
// be a NULL reference, in which case it is evaluated.

DECL_BLND bl_sided_par_pos
bl_get_face_pars(
		 COEDGE *coed,
		 double edge_par,
		 SPAposition const &pos,
         const bl_sided_par_pos &uv_guess
           = * ( bl_sided_par_pos * ) NULL_REF
		 );

#endif
