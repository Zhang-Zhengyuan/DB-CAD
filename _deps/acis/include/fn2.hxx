/* ORIGINAL: acis2.1/kernutil/d3_fn2/fn2.hxx */
/* $Id: fn2.hxx,v 1.36 2002/08/09 17:15:29 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*******************************************************************/
// sse24 19-Aug-2019: Added functionality to possibly add G2 discontinuity
//				points as additional help points.
// thm 11/12/06 Add crosses_region and process_required_curve
// DM  13/11/06 remove_next moved to FN2_CURVE (blnd11030)
// ajr 21/02/06 Made merge_points_and_curves virtual and protected.
//              Fix for 81176.
// jb  22/12/05 Additional changes for "Handling of non-G1 surfaces"
// jb  07/09/05 Changes for project "Handling of non-G1 surfaces"
// jb  28/04/05 New member function added. 
// jb  05/06/03 Change function flush_help_points to a virtual one.
//				(Needed for 71727)
// jb  15/01/03 Introduce new virtual function fval_at_discontinuity.
//              Needed for 70354 
/*******************************************************************/

#ifndef FN2H
#define FN2H

#include "dcl_kern.h"
#include "logical.h"
#include "fv2.hxx"
#include "math.hxx"

class HELP_POINT;
class TERMINATOR;
// STL jb 07 Sep 2005: New class for handling non-G1 surfaces.
class DISCONTINUITY_POINT;
class STEP;
class FUNC_2V_BOUNDARY_DATA;
class FUNC_2V_REQUIRED_DATA;
class FUNC_2V_REQUIRED_POINT;
class FUNC_2V_REQUIRED_CURVE;

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

#endif

 
// The range of the function is split into REGIONs, which can be used to store 
// local knowledge about the function. The base class of the REGION (which 
// follows) is trivial, as is the base class FUNC_2V's use of it. 

class DECL_KERN REGION : public ACIS_OBJECT
{
protected:
    REGION*	_next;
    logical	_visited;

public:
	REGION() : _next(0), _visited(FALSE) {}
	virtual ~REGION() {}

    logical	visited() const	{ return _visited; }
    REGION*	next() const { return _next; }

    void set_next( REGION* next ) { _next = next; }
    void tick()	{ _visited = TRUE; }

    virtual	logical	seek_terminators() const;
    virtual	SPApar_box pbox() const;
    virtual	logical	in_region( FVAL_2V& ) const;

	// STL jb 07 Sep 2005: New virtual function needed for handling of non-G1
	// surfaces.
	virtual logical seek_discontinuity_points() const;
    
#if defined D3_STANDALONE || defined D3_DEBUG

    virtual void print( D3_ostream&, int level ) const;
    virtual void input( D3_istream& );

#endif
};

#if defined D3_STANDALONE || defined D3_DEBUG

extern	DECL_KERN D3_ostream& operator<<( D3_ostream& os, const REGION& type );
extern	DECL_KERN D3_istream& operator>>( D3_istream& is, REGION& type );

#endif



// Classification of exploration result (this is identical to the 
// classification of the last step in the exploration). 
// STL jb 07 Sep 2005: Introduce an additional enum. For handling of non-G1 
// surfaces.
enum	EXP_STEP_TYPE	
    { 
    EXP_STEP_TO_START, 			// Stepped back to start (closed loop). 
    EXP_STEP_TO_EXIT, 			// Stepped to exit point. 
    EXP_STEP_TO_TERMINATOR, 		// Stepped to a terminator
    EXP_STEP_TO_ISOLATED_POINT,		// Stepped to an isolated point. This
					// only happens on a tangent curve, if a
					// terminator has been misclassified. 
    EXP_STEP_ORDINARY,			// Not a terminating step (not used 
					// in exploration classification).
	EXP_STEP_TO_DISCONT_POINT, 	// A discontinuity point is in most case an
		// ordinary step point at which the direction of the curve might
		// change. In the case of an implicit offset exploration such a point 
		// might become an end point.
    EXP_STEP_NOT_TANGENT,		// A suspected tangent curve turned 
					// out to be non-tangent.
    EXP_STEP_OFF_REGION,		// Stepped outside SPAparameter range
    EXP_STEP_BOTH_WAYS,			// Stepped in both directions from a
					// terminator 
    EXP_STEP_SPECIAL_TERMINATION,	// Stopped by the special_termination
					// function in the class derived from
					// FUNC_2V	 
    EXP_STEP_NULL,			// Failed to step away from a 
					// terminator (degenerate loop?).
    EXP_STEP_UNSET 			// Initial value (not used in 
					// exploration classification).
    };


// Classification of exploration start point. 

enum 	EXP_START_TYPE
    {
    EXP_START_HELP,	// Start from internal help point, closed loop possible
    EXP_START_RESTART,	// Start from internal help point, direction reversed 
			// during exploration, closed loop no longer possible
    EXP_START_ENTRY,	// Start from entry point, closed loop not possible
    EXP_START_EXIT,	// Start from exit point, closed loop not possible
    EXP_START_FTERM,	// Start from terminator, march forwards
    EXP_START_BTERM,	// Start from terminator, march backwards
    EXP_START_TANGENT	// Start from terminator, possible tangent curve
    };


#if defined D3_STANDALONE || defined D3_DEBUG

extern	DECL_KERN D3_ostream& operator<<( D3_ostream& os, const EXP_STEP_TYPE& type );
extern	DECL_KERN D3_istream& operator>>( D3_istream& is, EXP_STEP_TYPE& type );

#endif


// Reason for end of relaxation iteration. This is independent of the success 
// of relaxation, but will indicate why relaxation failed, if it did so. 

enum 	FN2_RELAX_TYPE
    {
    FN2_RELAX_SMALL_STEP,	// The steplength was negligible. This is the 
				// usual end to a successful relaxation. 
    FN2_RELAX_DEGENERATE,	// No step could be calculated because the 
				// conic is degenerate. 
    FN2_RELAX_STALLED,		// The residue could not be decreased (no 
				// direction could be found for the solution). 
    FN2_RELAX_ABANDONED,	// No close solution was evident 
    FN2_RELAX_ABORTED,		// The maximum number of steps was reached

    FN2_RELAX_TRUNCATED		// The final step was truncated at the pbox
				// boundary. 
    };


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_KERN D3_ostream& operator<<( D3_ostream& os, FN2_RELAX_TYPE type );

#endif

// Level of exploration that is required from ssi.

enum COMPLETENESS_TYPE
    {
    COMPLETENESS_UNKNOWN,	// Completeness doesn't matter.
    SINGLE_CURVE,		// Single curve only required.
    COMPOSITE_CURVE,		// One curve whose bits are separated by
				// terminators.
    INPUT_DATA,			// Curves corresponding to input data only.
    PROCESS_BOUNDARIES,		// Curves corresponding to input data 
                                // and process boundaries, but do
                                // not do subdivision.
    DO_SUBDIVISION,		// Curves corresponding to input data 
                                // and do subdivision, but do not 
                                // process boundaries.
    ALL_CURVES			// All Curves.
	};


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_KERN D3_ostream& operator<<( D3_ostream& os, 
					 COMPLETENESS_TYPE type );

#endif

// The FN2_CURVE_POINT class. The exploration functions build up lists of 
// these to represent curves. Applications may derive their own class. 

class DECL_KERN FN2_CURVE_POINT : public ACIS_OBJECT
    {
    FVAL_2V*	_fval;
    SPApar_dir	_duv;
    FN2_CURVE_POINT* _next;
    friend	class	FN2_CURVE;

public:	
		FN2_CURVE_POINT( const FVAL_2V& fval, const SPApar_dir& duv )
		    :	_fval( fval.copy() ), _duv( duv ), _next( 0 )	{}

    virtual ~FN2_CURVE_POINT();
    
    FVAL_2V&		fval()		{ return *_fval; }
    const SPApar_pos&	uv()	const	{ return _fval->uv(); }
    const SPApar_dir&	duv()	const	{ return _duv; }
    FN2_CURVE_POINT* 	next()	const	{ return _next; }
    };


// The FN2_CURVE class. This is simply a list of FN2_CURVE_POINTS. 

class DECL_KERN FN2_CURVE : public ACIS_OBJECT
    {
    FN2_CURVE_POINT*	_start;		// Start point
    FN2_CURVE_POINT*	_end;		// End point
    FN2_CURVE*		_next;		// Next curve
    logical 		_tangent;	// Is it a tangent curve?

public:
		FN2_CURVE( FN2_CURVE* next =0 )	
		    : 	_next( next ), _start(0), _end(0), _tangent(FALSE)  {}

    virtual	~FN2_CURVE();

    FN2_CURVE_POINT*	start()		{ return _start; }
    FN2_CURVE_POINT*	end()		{ return _end; }
    void	add_to_start( FN2_CURVE_POINT* cp );
    void	add_to_end( FN2_CURVE_POINT* cp );
    void	insert( FN2_CURVE_POINT* prev, FN2_CURVE_POINT* cp );

    FN2_CURVE*	next()				{ return _next; }
    logical	tangent()			{ return _tangent; }

    void	set_next( FN2_CURVE* next ) 	{ _next = next; }
    void	set_tangent( logical tangent )  { _tangent = tangent; }
    // Remove the next point and update the curve end (if necessary). It is 
    // the caller's responsible to ensure that cp is in the curve point list
    void    remove_next( FN2_CURVE_POINT* cp );
    };


// The FUNC_2V class. 
// 
// A FUNC_2V is a single-valued function of two variables. The class is an 
// abstract base class, so that users have to derive their specific function 
// class from it, and provide virtual functions such as evaulators. Once this 
// is done, various ways of analysing the function behaviour are available. 
//
// The class uses FVAL_2Vs to hold function values and parameters. Functions 
// that return a reference to an FVAL_2V refer to an internally held FVAL_2V 
// (in some class derived from FUNC_2V). The internal FVAL_2V is likely to be 
// reused by another call to a FUNC_2V function, so must be copied if it is 
// required. 
// 

class DECL_KERN FUNC_2V  : public ACIS_OBJECT
    {
protected:
    FVAL_2V*	_fval;			// A workspace fval used in calculations
					// and returned by various functions,
					// such as the relaxation functions. 
    int		_periodicity;
    double 	_epsilon;		// Value to be used in resolution tests

    FN2_RELAX_TYPE	_relax_termination;


    // The following pointers to FN2_CURVEs are used by terminators (and help 
    // points) to indicate that a particular branch does not correspond to a 
    // proper fn2_curve, but to a conventional one. 

    // This dummy curve is pointed to by a help point if the corresponding 
    // branch corresponds to a curve, but not to an FN2_CURVE. 

    const FN2_CURVE*	_dummy;

    // This dummy curve is pointed to by a help point if the corresponding 
    // branch has been pruned (i.e it goes out of the SPAparameter domain). A
    // branch is semi-pruned if it is known that one of a pair of branches can
    // be pruned. 

    const FN2_CURVE*	_pruned;
    const FN2_CURVE*	_semi_pruned;

	FUNC_2V_REQUIRED_DATA* _required_data;

public:
    const FN2_CURVE*	dummy()		const 	{ return _dummy; }
    const FN2_CURVE*	pruned()	const 	{ return _pruned; }
    const FN2_CURVE*	semi_pruned()	const 	{ return _semi_pruned; }

protected:
    
    // Error handling - the number of minor errors is recorded, and if it 
    // reaches a certain level, processing is aborted. 

    int		_error;


    // Completeness. See enum with comments above.

    COMPLETENESS_TYPE		_completeness;


    // No seeking. Usually when exploration is underway, it looks out for
    // terminators, turning points and 'farpoints' on other pieces of
    // intersection track. This is expensive, and may be turned off if it is
    // known that there are no such points, or that they have already been
    // found. The 'safe_area' flag is set in the latter case. 

    logical	_safe_area;


    // Store the range for the function over which we may expect evaluation to
    // work. Normally this will be the unit square, but if we have a function
    // with singularities then it mat be slightly smaller. 
    // NOTE: this range is only used to improve performance, by keeping the
    // relaxation methods away from singularities. 

    SPApar_box	_safe_range;


    // Exploration (i.e. the process of tracking curves of zero function 
    // value) starts from help points, and maintains lists of terminators and 
    // explored points. The following lists and functions are solely for the 
    // use of the exploration functions. 

    HELP_POINT*	_help_points;
    HELP_POINT*	_entry_points;
    HELP_POINT*	_exit_points;
    HELP_POINT*	_isol_points;
    HELP_POINT*	_turning_points;
    TERMINATOR*	_terminators;

    // STL jb 07 Sep 2005: Add a list of points on surface's discontinuities.
	// For handling of non-G1 surfaces.
	DISCONTINUITY_POINT* _discontinuity_points;

	// STL jb 05 Jun 03: Change this function to a virtual function.
    virtual void flush_help_points( FN2_CURVE&, HELP_POINT*& list );

    double	_exploration_control_angle;
    double	_sin_control_angle;
    double	_cos_control_angle;

    // jb Sep 2001 - add exploration_step_limit
    int         _exploration_step_limit;

    int 	check_termination( STEP&, HELP_POINT& start, int& );
    logical	tangent_explore( TERMINATOR&, int branch0, int branch1 );

	void mark_artefact_help_point(HELP_POINT* hpt);

    virtual	logical	extra_help_points();

	void explore(logical seek_extra_help_points);

    void	free_list( HELP_POINT*& );
    void	free_list( TERMINATOR*& );
    void	free_list( FN2_CURVE*& );
    void	free_list( REGION*& );
	// STL jb 07 Sep 2005: Needed for cleanup. For handling of non-G1 surfaces.
	void    free_list( DISCONTINUITY_POINT*& );

    // Given two HELP_POINTS, this function is asked to decide whether the two 
    // are actually the same. 
    // The function may return:
    //  1 - the two HELP_POINTs are mergeable
    // -1 - the two HELP_POINTs are distinct
    //  0 - the two HELP_POINTs should be made partners
    //
    // The default function returns 1 if the points are within SPAresnor in 
    // SPAparameter space, otherwise it returns -1. 
    
public:
    virtual	int	mergeable( HELP_POINT& first, HELP_POINT& second );

    TERMINATOR*	merge_terminators( TERMINATOR* tm0, TERMINATOR* tm1 );
    void	replace_terminator( TERMINATOR* tm_old, TERMINATOR* tm_new ); 

protected:

    // Allow the application to stop exploration - curve extension uses this
    // facility. 

    virtual	logical special_termination( STEP& );

    // The constructor is protected because this is an abstract base class. 

	FUNC_2V( double epsilon = SPAresabs, 
	 COMPLETENESS_TYPE completeness = ALL_CURVES, 
	 logical safe_area = FALSE,
	 FUNC_2V_REQUIRED_DATA* required_data = 0);

    //FUNC_2V&	operator=( FUNC_2V& );

    virtual	~FUNC_2V();

    FN2_CURVE*	_curves;

    REGION*	_regions;
    REGION*	_current_region;

    // STL ajr 21Feb06. Made this virtual and protected
    void virtual merge_points_and_curves( HELP_POINT*& ) {};

	void virtual process_required_curves() {};
public:

    virtual	void	strip();


    // Set the periodicity:	0 => neither u or v are periodic
    //				1 => Only u is periodic
    //				2 => Only v is periodic
    //				3 => Both u and v are periodic
    // The period in each case, is always 1.0

    void	set_periodicity( int periodicity );

    logical	periodic_in_u()		{ return _periodicity % 2 == 1; }
    logical	periodic_in_v()		{ return _periodicity > 1; }

    double	epsilon()	const	{ return _epsilon; }

    const SPApar_box&	safe_range()	const	{ return _safe_range; }

    void set_epsilon( double new_eps ) { _epsilon = new_eps; }

    // Test whether a SPApar_pos is in a SPApar_box, allowing for periodicity. 
    // Note that the SPApar_pos is overwritten with the periodic equivalent 
    // which is inside the pbox. 

    logical	in_pbox( SPApar_pos&, const SPApar_box& );


    // Information about regions. 

    REGION*	regions()	 const	{ return _regions; }
    REGION&	current_region() const	{ return *_current_region; }
    void	set_current_region( FVAL_2V& );
    void	tick_regions( FVAL_2V& );


    // Only a FUNC_2V is allowed to make an FVAL_2V, and this is the
    // routine that does it. Function values, derivatives etc. are all 
    // obtained through FVAL_2Vs. Note that the FVAL_2V obtained by 
    // current_fval() occupies reusable workspace and will be overwritten by 
    // the next call. Use the copy function to get an fval owned by the 
    // application. This must be deleted subsequently. 
    //
    // This base class administers the current fval which is an FVAL_2V by
    // default. If the application has derived a class from FVAL_2V, then it
    // must delete the stored fval and create one of the correct type. 

    FVAL_2V& current_fval()		{ return *_fval; }

	virtual int evaluate(FVAL_2V&,int derivs = 2) =0;

	// Overload fval() rather than use a default argument to avoid having to
	// specify a NULL_REF for dir
    FVAL_2V& fval( const SPApar_pos& uv, const SPApar_vec& dir = *(SPApar_vec*) NULL_REF )
	{ return fval(uv,2,dir); }

    FVAL_2V& fval( const SPApar_pos& uv, int n_deriv, const SPApar_vec& dir = *(SPApar_vec*) NULL_REF )
	{
		// Return the fval if at least two derivatives, or the number requested (<2)
		// have been successfully evaluated
		int derivs_evaluated = _fval->overwrite( uv, dir ,n_deriv);
		return (derivs_evaluated>=2 || derivs_evaluated==n_deriv) ? *_fval : *(FVAL_2V*) NULL_REF;
	}

    // Functions allowing derived classes to store and recover extra data - 
    // such as parameters on the second surface, in the case of IVECs. 
    // The default is to do nothing. 

    virtual	void	save_state();
    virtual	void	restore_state();
    virtual	void	reset_state( FVAL_2V& fv );


    // The turning-point function returns an fval at which both partial 
    // derivatives are zero, if one can be found in the supplied SPApar_box, 
    // otherwise it returns 0. 
    // The fval supplies an initial estimate of the solution. 

    FVAL_2V&	turning_point( FVAL_2V& fval, const SPApar_box& uv_box );


    // The second_order-point function returns an fval at which all three second 
    // derivatives are zero, if one can be found in the supplied SPApar_box, 
    // otherwise it returns 0. 
    // The fval supplies an initial estimate of the solution. 

    FVAL_2V&	second_order_point( FVAL_2V& fval, const SPApar_box& uv_box );


    // The axispoint function returns an fval on the axis of the conix closest
    // to the supplied direction, if one can be found in the supplied SPApar_box, 
    // otherwise it returns 0. 
    // The fval supplies an initial estimate of the solution. 

    FVAL_2V&	axispoint( FVAL_2V& fval, const SPApar_dir& ref, 
					  const SPApar_box& uv_box );


    // The nearpoint function returns an fval with a zero function value, if 
    // one can be found in the supplied SPApar_box, otherwise it returns 0. 
    // The fval supplies an initial estimate of the solution. 
    // 
    // The returned fval is not in general the closest to the original one, 
    // but it is in the neighbourhood of the closest. 

    FVAL_2V&	nearpoint( FVAL_2V& fval, const SPApar_box& uv_box );


    // The extreme_point function returns an fval with zero function
    // first derivatives (but not a saddle point), if one can be found in the
    // supplied SPApar_box, otherwise it returns 0. The fval supplies an initial
    // estimate of the solution.  If an fval is found whose absolute value
    // exceeds the supplied maximum, the function makes a quick exit and returns
    // that maximum.  The relaxation condition is set to FN2_RELAX_ABANDONED in
    // this case. 

    FVAL_2V&	extreme_point( FVAL_2V& fval, const SPApar_box& pbox, double max );


    // The steppoint function returns an fval with a zero function value, 
    // which makes an angle of a prescribed value with the given tangent. 
    // If no such fval can be found in the supplied SPApar_box then it returns 0. 
    // The fval supplies an initial estimate of the solution. 
    // 

    FVAL_2V&	steppoint( FVAL_2V& fval, const SPApar_dir& tangent, 
			   double cos_ang, const SPApar_box& uv_box );


    // The farpoint function takes an fval which already has a zero function 
    // value, and returns another one with a zero function value, but which 
    // is on a different part of the curve, if one can be found in the 
    // supplied SPApar_box. If the curve has two distinct branches in the 
    // neighbourhood, then this function returns a point on the other branch. 

    FVAL_2V&	farpoint( FVAL_2V& fval, const SPApar_box& uv_box );


    // The boundary_point function takes two fvals which span a boundary, (the
    // second, B, being 'out-of-bounds') and relaxes to an fval exactly on the
    // boundary. If the relaxed point is still out of bounds (i.e. it relaxed to
    // the wrong boundary) then the function calls itself recursively. 

    virtual	FVAL_2V&	boundary_point( FVAL_2V& A, FVAL_2V& B );


    // Ask why the last relaxation operation terminated. This is set whether 
    // the relaxation was successful or not. 

    FN2_RELAX_TYPE relax_termination()	const	{ return _relax_termination; }


    // set_termination is intended for use by the relaxation functions. 

    void	set_termination( FN2_RELAX_TYPE type ) 
			{ _relax_termination = type; }


    // This is intended to calculate the safe_point

    virtual double terminator_safe_distance( TERMINATOR& );

    // Exploration. The first function explores all help points; these must 
    // previously have been stored using add_help_point. The second explores 
    // from a given help point, returning the final step type. 

    void	explore();

    EXP_STEP_TYPE	explore_help_point( HELP_POINT& start, int start_type, 
				    	    const SPApar_dir& tangent, 
					    FVAL_2V* final_step = 0 );

    void	set_control_angle( double angle );

    double	exploration_control_angle()	const
		    { return _exploration_control_angle; }

    // jb Sep 2001 - methods for exploration_step_limits
    void 	set_exploration_step_limit( int limit );

    int		exploration_step_limit()	const
	            { return _exploration_step_limit; }

    double	sin_control_angle()	const 	{ return _sin_control_angle; }
    double	cos_control_angle()	const 	{ return _cos_control_angle; }

    COMPLETENESS_TYPE	completeness()	const 	{ return _completeness; }

    logical	single_curve()		const
	{ return _completeness == SINGLE_CURVE; }
    logical	composite_curve()	const
	{ return _completeness == COMPOSITE_CURVE; }
    logical	do_boundaries()		const
	{ return _completeness == PROCESS_BOUNDARIES
	      || _completeness == ALL_CURVES; }
    logical	do_subdivision()	const
	{ return _completeness == DO_SUBDIVISION
	      || _completeness == ALL_CURVES; }

    void	set_completeness( COMPLETENESS_TYPE complete  )
	{ _completeness = complete; }

    logical	safe_area()		const 	{ return _safe_area; }
    void	set_safe_area( logical sf_area ) { _safe_area = sf_area; }


    // Estimate a suitable length for the current step (this will contain a 
    // start but no end). The steplength is measured in SPAparameter space. 
    // The second argument is set to TRUE if the curvature was used to 
    // estimate a steplength that will satisfy the angular control. 

    virtual	double	steplength( STEP& step, logical& curv_control );


    // Judge the current step. Return a double indicating how close the step is 
    // to achieving its target. 1.0 is perfect; less than one is within the 
    // constraints, but there is room for improvement; greater than 1 is 
    // unacceptable. 
    // The default is to calculate the total turning angle of the step and 
    // compare it with the exploration control angle. 

    virtual	double	judge_step( STEP& step, logical accept_step = FALSE );

    
    HELP_POINT*	help_points()		const	{ return _help_points; }
    HELP_POINT*	entry_points()		const	{ return _entry_points; }
    HELP_POINT*	exit_points()		const	{ return _exit_points; }
    HELP_POINT*	isolated_points()	const	{ return _isol_points; }
    HELP_POINT*	turning_points()	const	{ return _turning_points; }
    TERMINATOR*	terminators()		const	{ return _terminators; }
	// STL jb 07 Sep 2005: This will return the list of discontinuity points.
	// For handling of non-G1 surfaces.
    DISCONTINUITY_POINT* discontinuity_points()		const	
	{ return _discontinuity_points; }

	// STL jb 15 Dec 2005: This function will return the discontinuity point
	// at the given parameters.
	DISCONTINUITY_POINT* get_discontinuity_point_at_uv( FVAL_2V& );

    FN2_CURVE*	curves()		const	{ return _curves; }

    // Enquiry functions. An fval_2v is internal if it is inside the FUNC_2V
    // SPAparameter space, by a small distance (i.e. it does not include the
    // boundary). It is out of bounds if it is at least eps outside the
    // boundary. If eps is set to null, then the default value of SPAresnor is used
    // (it cannot be initialised to SPAresnor because this is a virtual function,
    // and other manifestations of it use different defaults). 

    virtual	logical	internal( FVAL_2V&, double eps = SPAnull );
    virtual	logical	out_of_bounds( FVAL_2V&, double eps = SPAnull );


    // Add a new help point to the appropriate list, returning a pointer to 
    // the new point or 0 if the point corresponds to one already on the list. 
    // The 'new_term' argument of add_terminator is TRUE if the returned pointer
    // is to a new terminator on the heap, and FALSE if it is to an existing
    // (matching) terminator. Tangents may be supplied to add_terminator; this 
    // is only necessary if the terminator is a high order one or at a surface 
    // singularity, because if it is not then they can be calculated easily 
    // anyway. Even then, it is not compulsary to give tangents, but if this 
    // is done then exploration and curve fitting will work much better. If 
    // tangents are given, then the correct number must be given; for a four-
    // branch terminator two directions must be given. 

    HELP_POINT* add_help_point( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0, FVAL_2V* orig_fval = 0 );
    HELP_POINT*	add_entry_point( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0, FVAL_2V* orig_fval = 0 ); 
    HELP_POINT*	add_exit_point( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0, FVAL_2V* orig_fval = 0 ); 
    HELP_POINT*	add_isolated_point( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0,
		logical& new_ipt = *( logical* ) NULL_REF );
    HELP_POINT*	add_turning_point( FVAL_2V& );

    TERMINATOR*	add_terminator( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0, 
				logical& new_term = *(logical*)NULL_REF,
				int ndir = 0, SPApar_dir* tangents = 0,
				logical force_terminator = FALSE );
	// STL jb 07 Sep 2005: Adding of discontinuity points. For handling of 
	// non-G1 surfaces.
    DISCONTINUITY_POINT* add_discontinuity_point( FVAL_2V&, SPApar_dir, 
										  FVAL_2V&, SPApar_dir, double, double,
										  FUNC_2V_BOUNDARY_DATA* b = 0 );

	// Virtual function to allow derived classes to do more work when a
	// terminator is added (the default is just to call add_terminator)
    virtual TERMINATOR*	add_terminator_and_extras( FVAL_2V&, FUNC_2V_BOUNDARY_DATA* b = 0, 
				logical& new_term = *(logical*)NULL_REF);



    virtual	FUNC_2V_BOUNDARY_DATA* 	find_boundary( FVAL_2V& );

	virtual	logical on_boundary( FVAL_2V& );

    void	remove_next_from_list( HELP_POINT*& list, HELP_POINT* prev );
    HELP_POINT*	unlink_next_from_list( HELP_POINT*& list, HELP_POINT* prev );

    void	remove_next_help_point( HELP_POINT* prev )
			{ remove_next_from_list( _help_points, prev ); }

    HELP_POINT*	unlink_next_help_point( HELP_POINT* prev )
			{ return unlink_next_from_list( _help_points, prev ); }

    TERMINATOR*	unlink_next_terminator( TERMINATOR* prev )
		{ return (TERMINATOR*) unlink_next_from_list( 
			(HELP_POINT*&) _terminators, (HELP_POINT*) prev ); }

    void	remove_next_terminator( TERMINATOR* prev );

    void	remove_next_entry_point( HELP_POINT* prev );

    void	remove_next_exit_point( HELP_POINT* prev );

    HELP_POINT*	unlink_next_entry_point( HELP_POINT* prev )
		{ return unlink_next_from_list( _entry_points, prev ); }
    HELP_POINT*	unlink_next_exit_point( HELP_POINT* prev )
		{ return unlink_next_from_list( _exit_points, prev ); }

    
    // Make a new curve point. Functions derived from FUNC_2V would usually 
    // supply their own version of this function. 

    virtual	FN2_CURVE_POINT* make_curve_point( FVAL_2V&, const SPApar_dir& );
	// STL jb 07 Sep 2005: New overwrite of this function needed to allow
	// points to be created with two different directions at discontinuities.
	// For handling of non-G1 surfaces.
	virtual FN2_CURVE_POINT* make_curve_point( FVAL_2V&, 
											const SPApar_dir&,
											logical, const SPApar_dir&, 
									        FVAL_2V& );

    FN2_CURVE_POINT* add_curve_point( FN2_CURVE_POINT*, logical end =TRUE );


    // Called from add_curve_point, this function allows the aplication to make
    // changes to a CURVE_POINT once it is in a list. The default version of the
    // function does nothing. 

    virtual	void	edit_curve_point( FN2_CURVE_POINT&, logical end );


    // Start a new curve, and add subsequent new points to it. 
 
    virtual	void	start_new_curve();


    // Remove the latest curve and all points on it. 

    void	abort_curve()		
		{ FN2_CURVE* cu = _curves; _curves = cu->next(); ACIS_DELETE cu; }


    // Equality tests for help points and terminators. The functions return 
    // TRUE if the two points are certainly equal, FALSE if they are not equal 
    // or it is non-trivial to decide. The test for a terminator looks at the 
    // area of flatness of the terminator. terminators_equal returns 1 if the 
    // terminators are equal, 2 if they are equal and are also known to have 
    // compatible tangent directions (Returning 1 does not imply that the 
    // tangent directions are incompatible). 

    virtual	logical	fvals_equal( FVAL_2V&, FVAL_2V& );
    virtual int	terminators_equal( FVAL_2V&, FVAL_2V&,
		logical both_on_boundary = FALSE );
    logical	help_point_and_terminator_equal( FVAL_2V&, FVAL_2V& );


    // Error handling. Increment the error count and allow it to be examined. 

    void	increment_error()	{ _error++; }
    logical	error();
    void	reset_error()		{ _error = 0; }

	// STL jb 15 Jan 03: Introduce a function which allows to test if a fval is
	// at a u or v discontinuity.
	virtual logical fval_at_discontinuity( FVAL_2V& );
	
	// RCW: 22-Mar-11, This is a new member function to test if a fval is located
	// at a G2 discontinuity.
	// 
	virtual logical fval_at_G2_discontinuity(FVAL_2V&, logical&, logical& );
	
	// STL jb 28 Apr 05: Query function to identify cases of singular splines 
	virtual logical is_singular_spline_case();

	virtual logical terminator_at_discontinuity(const TERMINATOR&);

	// STL jb 07 Sep 2005: New function to find the direction at discontinuity
	// point. For handling of non-G1 surfaces.
	virtual void determine_discontinuity_points( ) {}
 
	// 19-Aug-2019 sse24
	// To determine if surfaces have G2 discontinuities which can be added as help points
	virtual void determine_G2_discontinuity_points() {}
	// Returns an array with the G2 discontinuities and their count (implemented in SSI)
	virtual const double* get_G2_discontinuities_u(int&) { return nullptr; }

#if defined D3_STANDALONE || defined D3_DEBUG

    // These functions may be provided by classes derived from the FUNC_2V 
    // base class to print out any function-specific data, and to read it in.

    virtual void print( D3_ostream&, int level )	const;
    virtual void input( D3_istream& );

#endif
    };

#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_KERN D3_ostream& operator<<( D3_ostream&, const FUNC_2V& );
extern DECL_KERN D3_istream& operator>>( D3_istream&, FUNC_2V& );

#endif
#endif
