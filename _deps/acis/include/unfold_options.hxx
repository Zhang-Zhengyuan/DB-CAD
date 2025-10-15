/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef UNFOLD_OPTIONS_CLASS
#define UNFOLD_OPTIONS_CLASS

#include "dcl_hlc.h"
#include "mmgr.hxx"
#include "lists.hxx"

class unfold_options_impl;

/**
* @file unfold_options.hxx
* \addtogroup hlc
* \addtogroup
* @{
*/

/**
 * This class contains parameters that govern the behavior of @href api_unfold.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Unfold Sheet Body](https://doc.spatial.com/articles/u/n/f/Unfold_Sheet_Body_c677.html)</i>
 * <br><br>
 * <b>Role:</b> Below are lists of parameters available in the @href unfold_options class.
 * The constructor of @href unfold_options initializes all the parameters to their default value. 
 * You can use the <tt>set</tt> functions to change any of the parameters and the <tt>get</tt> functions
 * to obtain the current value of the parameter set into a @href unfold_options object. <br>
 * <ul>
 * <li> <b>Splitting Edges</b><br> 
 * A list of connected edges along which splitting or cutting should takes place in order to unfold a closed sheet body input.</li><br>
 * <li> <b>Refinement Factor</b><br>
 * This factor directly affects the mesh quality of the algorithm. Increase the refinement factor from default value of 1.0 in order to get finer facets. <br>This is required when there are surfaces with high curvature, no matter whether the surfaces are developable or not. Increasing the Refinement Factor will directly affect the performance of the algorithm so use it only when such surfaces are present in the input.<br>
 * Any positive value between 1.0 and 10.0. There is as such no restriction on the upper bound of this range but increasing it beyond this is not recommended.</li><br>
 * <li> <b>Simplify</b><br>
 * Set this flag to true or false if you want the output EDGEs to be simplified to analytic geometry if possible. By default this flag is set to true.</li><br>
 * <li> <b>Edge Simplification Tolerance</b><br>
 * Edge Simplification Tolerance provides control over the tolerance for edge simplification. This will be honored only if Simplfy is ON. Default tolerance value is SPAresabs.
 * </ul>
 * <br><br>
 * A sample snippet code to illustrate the usage is given below: <br>
 * <pre>
 * unfold_options unfold_opts;
 * unfold_opts.setRefinementFactor(2.0); //Sets Refinement Factor for part with high curvature / non developable surfaces. 
 * unfold_opts.setSimplify(false); // Switchs OFF edge simplification for output unfolded body.
 * outcome result = api_unfold (body, &out_body, &unfold_opts);
 * </pre>
 * <br> 
 *
 * @see api_unfold
 *  
**/
class DECL_HLC unfold_options : public ACIS_OBJECT
{

protected:

	unfold_options_impl* m_impl;
	
	unfold_options ( unfold_options const& input );
	
	unfold_options& operator= ( unfold_options const& input );

public:
	// ********************************************************************
	//Constructor
	unfold_options ();

	//Destructor
	~unfold_options ();
	
	// ********************************************************************
	/** 
	*  Set Splitting Edges for cutting.
	*/
	void setSplittingEdges ( ENTITY_LIST & edges );
	
	/**
	*  Get Splitting Edges for cutting.
	*/
	void getSplittingEdges ( ENTITY_LIST & edges );

	// ********************************************************************
	/**
	*  Set Refinement Factor.
	*/
	void setRefinementFactor(double);
	
	/**
	*  Get Refinement Factor.
	*/
	double getRefinementFactor();

	// ********************************************************************
	/**
	* Set simplify flag.
	*/
	void setSimplify(bool);
	
	/**
	* Get simplify flag.
	*/
	bool getSimplify();

	// ********************************************************************

	/**
	*  Set Edge Simplification Tolerance
	*/
	void setEdgeSimplificationTolerance(double);

	/**
	*  Get Edge Simplification Tolerance
	*/
	double getEdgeSimplificationTolerance();

	// ********************************************************************

};

#endif
