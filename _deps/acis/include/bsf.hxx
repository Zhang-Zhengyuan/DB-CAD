/* ORIGINAL: acis2.1/kerngeom/d3_srf/bsf.hxx */
/* $Id: bsf.hxx,v 1.21 2002/08/09 17:15:23 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BSFH
#define BSFH

#include "dcl_kern.h"
#include "logical.h"
#include "bcu.hxx"
#include "math.hxx"
#include "surdef.hxx"

class SPAposition;
class BISPAN;
class SVEC;
class VBL_SURF;
class VBL_SING_LIST;
class CONVEX_POLYGON;
class SPAbox;
class curve;
class BOUNDARY;
class DEGENERATE_BOUNDARY;
class GEOMETRIC_BOUNDARY;
class INTERNAL_BOUNDARY;
class check_status_list;
class SPApar_transf;
class SPApar_bilinear_transf;

// STI ROLL
class SizeAccumulator;
// STI ROLL

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

#endif


enum BSF_enclosure
    {
    BSF_internal, 
    BSF_external, 
    BSF_on_boundary
    };

enum BSF_null_range
    {
    BSF_non_null,
    BSF_null_u_range, 
    BSF_null_v_range, 
    BSF_null_u_and_v_ranges 
    };

#if defined D3_STANDALONE || defined D3_DEBUG

DECL_KERN D3_ostream &operator<<( D3_ostream &os, BSF_enclosure );

DECL_KERN D3_ostream &operator<<( D3_ostream &os, BSF_null_range );

#endif

// A bounded surface is a portion of a surface bounded in SPAparameter space. Each
// parametric boundary has a corresponding BOUNDARY which is owned by the
// bounded surface, even if the boundary is a periodic or degenerate one. These
// boundaries are evaluated lazily by, and owned by, the bounded surface.
//
// Currently, normal bounded surfaces have 4 parametric boundaries: bounded
// vertex blends may have any number greater than or equal to 4. These
// parametric boundaries are numbered in the following way for a 4-sided patch
// and a 8-sided patch: 
//
//              "HI V" = 1                        2             
//              _______                         ______                       
//             |       |                    3  /      \ 1           
//             |       |                      /        \
//             |       |                      |        |
// "LO_U = 2"  |       |  "HI U" = 0        4 |        | 0          
//             |       |                      |        |
//             |_______|                      \        /            
//                                           5 \______/ 7            
//             "LO V" = 3                          6                
//                                                  


class   DECL_KERN BOUNDED_SURFACE : public ACIS_OBJECT
    {
protected:

    const surface* _sf;

    SPApar_box     _range;         // The region of the surface we are 
                                   // interested in: this controls what root
                                   // bispan the bounded surface makes and what
                                   // the surface boundaries are. 

    SPApar_box	   _unsubset_range;

    int            _periodicity;   // Periodicity flag: 
                                   //      0 => neither u or v are periodic
                                   //      1 => Only u is periodic
                                   //      2 => Only v is periodic
                                   //      3 => Both u and v are periodic

    BISPAN*        _root;          // Root bispan, evaluated lazily.

    int            _n_para_bdry;   // The number of parametric boundaries is 
                                   // set by constructor and is 4 for all except
                                   // VBL surfaces at present. 

	const SPApar_bilinear_transf * _transf;		// The transformation to be applied to the 
	                               // axis-aligned box and points before evaluation.
 
    BOUNDARY**     _boundaries;    // The boundaries, evaluated lazily.
    INTERNAL_BOUNDARY**	   _internal_u_boundaries;
    INTERNAL_BOUNDARY**	   _internal_v_boundaries;

    double         _typical_Pu_x_Pv; 
                                   // Typical length of Pu * Pv across surface

    int            _acc_derivs;    // Number of accurate surface derivatives
                                   // is cached here for efficiency.

    // The following members provide lazy evaluation of some of the above
    // quantities. 

    void initialize_boundary_pointers();

    virtual BOUNDARY* make_boundary( int i );

    virtual void make_root() = 0;        

    // Is either or both of the SPAparameter ranges zero in length?

    virtual BSF_null_range null_range() const;

    // Cache the typical size of Pu * Pv on this surface:

    void get_typical_Pu_x_Pv();

    // This flag controls what happens if an illegal SVEC is found on the bounded 
    // surface.  By default, processing continues, but if this flag is set a 
    // sys_error is generated.  The surface checker sets this flag. 

    logical _abort_on_illegal_svec;

	// R3L (Sep 30, 2011): abort_on_illegal_svec() modifies the 'abort_on_failure' flag
	// of 'off_spl_sur'. Ideally, it should not be doing this in the first place since, 
	// the same 'off_spl_sur' is shared by multiple entities. In order to reset this value
	// of 'abort_on_failure' we now need to introduce this additional data member _ss_abort[].
	//
private:
	// _ss_abort[0]: Flag to denote if 'abort_on_failure' was modified
	// _ss_abort[1]: Original value of 'abort_on_failure'
	//
	bool _ss_abort[2];

public:

    BOUNDED_SURFACE( 
                    const surface* sf, 
                    const SPApar_box& pbox,
                    int _n_para_bdry,
					const SPApar_bilinear_transf * transf = NULL
                    );

    virtual ~BOUNDED_SURFACE();

    void strip();

    void strip_bispan();

    // This enum is used to index the boundaries:

    enum { HI_U = 0, HI_V = 1, LO_U = 2, LO_V = 3 };

    virtual BOUNDED_SURFACE& operator=( const BOUNDED_SURFACE& );

    const surface& sf()    const { return *_sf; }

    const SPApar_box& range() const { return _range; }

	const SPApar_bilinear_transf * transf() const { return _transf; }

    SPApar_box&	   unsubset_range();

    double         lo_u()        { return _range.u_range().start_pt(); }
    double         hi_u()        { return _range.u_range().end_pt(); }
    double         lo_v()        { return _range.v_range().start_pt(); }
    double         hi_v()        { return _range.v_range().end_pt(); }

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
 
    virtual BOUNDED_SURFACE* trim_back_from_singularity( 
                                                        double dist, 
                                                        DEGENERATE_BOUNDARY& 
                                                        ) = 0;

    BISPAN* root() { if ( _root == 0 ) make_root(); return _root; }

    double typical_Pu_x_Pv() 
	{
	if ( _typical_Pu_x_Pv == SPAnull ) get_typical_Pu_x_Pv();
	return _typical_Pu_x_Pv;
	}

    int accurate_derivs()
	{ 
	if ( _acc_derivs < 0 ) _acc_derivs = _sf->accurate_derivs();
	return  _acc_derivs; 
	}
    
    // Boundary access functions: all boundaries - whether they are periodic,
    // degenerate or normal geometric boundaries - are indexed as described
    // above in a counter-clockwise manner. They can be accessed using this
    // indexing scheme using the boundary( index ) function: this returns an
    // object of unknown type - it must be interrogated to find out if it's a
    // degenerate, periodic or normal boundary if its processing depends on its
    // type. 

    int number_of_parametric_boundaries() const { return _n_para_bdry; }

    BOUNDARY& boundary( int i )
        {
        if ( ! _boundaries ) initialize_boundary_pointers();
        if ( ! _boundaries[ i ] ) _boundaries[ i ] = make_boundary( i );
        return *_boundaries[ i ];
        } 

	void reset_boundaries();
	
    // The geometric boundaries of a surface -- that is, the non-degenerate
    // non-periodic ones which correspond to real boundaries in 3-space -- are
    // accessed by the following functions. The argument to geometric_boundary (
    // int ) is *NOT* the boundary index as described above but the ordinal
    // number of the geometric boundary counted anticlockwise. To find out which
    // the index of this boundary use the index() boundary member function. 

    int number_of_geometric_boundaries();

    GEOMETRIC_BOUNDARY& geometric_boundary( int );

    // Similarly, the degenerate boundaries are accessed in order using these
    // functions: 

    int number_of_degenerate_boundaries();

    DEGENERATE_BOUNDARY& degenerate_boundary( int );


    // Now internal boundaries.  The default versions of these functions
    // claim no internal boundaries. 

    virtual int number_of_internal_u_boundaries( int order );
    virtual int number_of_internal_v_boundaries( int order );

    virtual INTERNAL_BOUNDARY& internal_u_boundary( int index, int order );
    virtual INTERNAL_BOUNDARY& internal_v_boundary( int index, int order );


    // Return a curve plus its SPAinterval which corresponds to a SPAparameter line
    // shfted from the SPAparameter line corresponding to the given boundary  (
    // referred to by its index ) shifted by dparam into the SPAparameter space of
    // the surface. Also returned are the start and end points of the curve in
    // the SPAparameter space of the surface. This is a virtual function.

    virtual void offset_boundary_curve( const int    index, 
				        const double dparam, 
				        curve*&      cu,
				        SPAinterval&    ivl,
				        SPApar_pos&     uv0,
				        SPApar_pos&     uv1 );
    
    // This function returns SPAparameter-space steps to the two nearest 
    // boundaries, in SPAparameter space. 

    virtual void    distance_from_boundary( SPApar_vec&, SPApar_vec&, SVEC& );

    // These functions are used to test the positions of SVECs against the range
    // of a bounded surface, taking into account periodicities and degeneracies. 
    // Only exterior is virtual to allow VBLs to handle their polygons; the
    // others are implemented from the BOUNDARY::svec_on_boundary function.
    // 
    // eps is the 3-space resolutiion distance.
    //
    // exterior: true if SVEC parameters lie outside surface range (no
    //           resolution test is done);
    // 
    // out_of_bounds: true is SVEC outside patch boundaries by more than eps 
    //
    // internal: true is SVEC inside patch boundaries by more than eps
    //
    // on_nth_boundary: true if SVEC within eps of the nth boundary
    // 
    // on_boundary: returns the indices of any boundaries the svec is on 
    //              (up to a maximum of three):
    //
    // 1. If the svec is on no boundaries, then the function returns FALSE and 
    //	  the indices 'first, 'second' and 'third' are set to -1. 
    // 2. If an svec is on the corner of the BOUNDED_SURFACE then the 'first' 
    //	  and 'second' indices will be set to the corresponding boundary 
    //	  indices.
    //	  'third' will be set to -1. 
    // 3. The svec can only be on three boundaries if it is within 'eps' of 
    //	  a degenerate point, and then 'first' and 'second' will indicate the 
    //	  non-degenerate boundaries, and 'third' will indicate the degenerate 
    //	  one. It is possible that 'first' and 'second' will be set to -1 in 
    //    this case, if they correspond to periodic boundaries.  

    virtual logical exterior( SVEC& sv );

    BSF_enclosure on_boundary( SVEC&, int& b1, int& b2, int& b3, double eps = SPAresabs );

    logical on_nth_boundary( SVEC&, const int, double eps = SPAresabs );

    logical internal( SVEC&, double eps = SPAresabs  );

    logical out_of_bounds( SVEC&, double eps = SPAresabs );


    virtual void closest_parametric_boundaries( const SPApar_pos&, int&, int& );

    // Range checking flags: BSFs can have empty ranges due to boxing.
    
    logical valid_ranges() { return null_range() == BSF_non_null; }

    logical null_u_and_v_ranges() 
                           { return null_range() == BSF_null_u_and_v_ranges; }

    logical null_u_range() { return null_range() == BSF_null_u_range || 
                                    null_range() == BSF_null_u_and_v_ranges; }

    logical null_v_range() { return null_range() == BSF_null_v_range || 
                                    null_range() == BSF_null_u_and_v_ranges; }


    // Singularity functions: only BOUNDED_SURFACEs need to know about the
    // sing_lo_u type functions. 

    logical regular() { return number_of_degenerate_boundaries() == 0; }

	void 	set_range( SPAinterval u, SPAinterval v )
	{
		_range = SPApar_box( u, v );
	}

private:

    logical sing_lo_u();
    logical sing_hi_u();
    logical sing_lo_v();
    logical sing_hi_v();

public:

    // Is the point at one of the surface singularities to within resabs?

    logical point_is_singular( const SPAposition& );

    // Ask the surface whether a given uv point is a special point where the
    // surface is non-C2.

    virtual logical non_C2_point( const SPApar_pos& ) const;

    // Query the periodicity of the bounded surface. The actual period is 
    // the same as the period of the underlying surface. 

    logical     periodic_u() const { return _periodicity % 2 == 1; }
    logical     periodic_v() const { return _periodicity > 1; }

	// Access function to set the periodicity of a bounded surface. This is intended
	// for use when it has been marked as periodic but is non_G1 at its "seam", or 
	// when a G2 discontinuity exists at the seam for an implicit offset SSI.
	void		set_periodicity(int i) { if ( i > -1 && i < 4 ) _periodicity = i;}

    // Return a subset of the surface range that excludes some or all
    // singularities by a small amount (this function is for performance tuning
    // only). 

    SPApar_box	safe_range();

    // Query or set the abort_on_illegal_svec flag. 

    logical	abort_on_illegal_svec() { return _abort_on_illegal_svec; }

    void	set_abort_on_illegal_svec( logical flag );


#if defined D3_STANDALONE || defined D3_DEBUG

    virtual     void    print( D3_ostream &os ) const;

#endif

#if defined D3_DEBUG 

    virtual     void    plot( FILE* fp, int nu = 0, int nv = 0 );

    virtual	void	save( FILE* fp, logical iter = FALSE );

#endif
    };


class   DECL_KERN BOUNDED_IMPL_SURFACE : public BOUNDED_SURFACE
    {
	const bool _equal_split;
protected:
    virtual     void    make_root();
    
public:
    
    BOUNDED_IMPL_SURFACE(
		const surface*, 
		const SPApar_box&, 
		int n_para_bdry = 4,
		const SPApar_bilinear_transf * transf = nullptr,
		bool equal_split = false);

    virtual ~BOUNDED_IMPL_SURFACE();

    virtual BOUNDED_SURFACE* trim_back_from_singularity( 
                                                        double dist, 
                                                        DEGENERATE_BOUNDARY& 
                                                        );
    };


class   DECL_KERN BOUNDED_PARA_SURFACE : public BOUNDED_SURFACE
    {
protected:
    virtual     void    make_root();

public:

    BOUNDED_PARA_SURFACE( 
                         const surface*, 
                         const SPApar_box&,
                         int n_para_bdry = 4,
						 const SPApar_bilinear_transf * transf = NULL
                         );

    virtual ~BOUNDED_PARA_SURFACE();

    virtual BOUNDED_SURFACE* trim_back_from_singularity( 
                                                        double dist, 
                                                        DEGENERATE_BOUNDARY& 
                                                        );

    virtual int number_of_internal_u_boundaries( int order );
    virtual int number_of_internal_v_boundaries( int order );

    virtual INTERNAL_BOUNDARY& internal_u_boundary( int index, int order );
    virtual INTERNAL_BOUNDARY& internal_v_boundary( int index, int order );


    };


class DECL_KERN BOUNDED_VBL_SURFACE : public BOUNDED_PARA_SURFACE 
    { 
    // These are for storing the singularity information list, used in deciding
    // where the singularities are, marking when they've been trimmed, storing
    // the newly created boundaries etc.

    int                 _init_singularities;
    VBL_SING_LIST*      _singularities;
    VBL_SING_LIST*      singularities();

    // Here we keep the general convex polygon representing the bounded surface.
    // When we trim back from a degenerate edge the bounded surface no longer
    // represents the entire regular polygon. (_other_poly is an implementation
    // detail.)

    CONVEX_POLYGON*     _other_poly;
    CONVEX_POLYGON*     _poly;
    CONVEX_POLYGON*     polygon();

    virtual BOUNDARY* make_boundary( int );

    virtual void make_root();

public:

	BOUNDED_VBL_SURFACE( const surface* sf, int n_para_bdry, 
		SPApar_box const& pbox = SPApar_box(), const SPApar_bilinear_transf *transf = NULL );
    
    virtual ~BOUNDED_VBL_SURFACE();
    
    const VBL_SURF&     vbl()   const;
    
    virtual void distance_from_boundary( SPApar_vec&, SPApar_vec&, SVEC& );

    virtual logical exterior( SVEC& sv );

    virtual void closest_parametric_boundaries( const SPApar_pos&, int&, int& );

    virtual void offset_boundary_curve( const int    index, 
				        const double dparam, 
				        curve*&      cu,
				        SPAinterval&    ivl,
				        SPApar_pos&     uv0,
				        SPApar_pos&     uv1 );

    virtual BOUNDED_SURFACE* trim_back_from_singularity( 
                                                        double dist, 
                                                        DEGENERATE_BOUNDARY& 
                                                        );
    
    // This needs overriding for blend surfaces: vertices where the twists don't
    // match are non-C2 points.

    virtual logical non_C2_point( const SPApar_pos& ) const;

#if defined D3_DEBUG 
    
    virtual     void    plot( FILE* fp, int nu = 0, int nv = 0 );
    
#endif
    };


// Make a bounded surface of the appropriate type on the heap. 

// uq2 12/28/17 - I added the last argument "equal_split" to this function, a boolean flag (with false as default value)
//                whose job it is to determine the bahavior of IMPL_BISPAN::split_u. The default behavior is to return
//                false if underlying geometry is a cone, which is the desired behavior for intersectors. 
//                If "equal_split" is true, IMPL_BISPAN::split_u only depends on u- and v-range lengths,
//                which is the desired behavior if equal patches are expected, such as for functionality implemented
//                in SPAbool\PrivateInterfaces\near_coin_priv.hxx.
DECL_KERN BOUNDED_SURFACE* BSF_make_bounded_surface( const surface*, 
						const SPApar_box&, const SPApar_bilinear_transf*  transf = nullptr, bool equal_split = false );

#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_KERN D3_ostream &operator<<( D3_ostream &os, const BOUNDED_SURFACE& );
extern	DECL_KERN BOUNDED_SURFACE*	BSF_input( D3_istream&, SPAbox& b, surface*& );

#endif

#endif

