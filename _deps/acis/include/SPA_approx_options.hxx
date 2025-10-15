/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
 * @file SPA_approx_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BSGEOMETRY
 *
 * @{
 */
#if !defined(SPA_APPROX_OPTIONS_HXX)
#define SPA_APPROX_OPTIONS_HXX

#include "dcl_kern.h"
#include "spa_approx_enum.hxx"

//=============================================================================
// classes associated with approximation

class SPApar_box;
class SPAinterval;
class SPA_internal_approx_options;
/**
 * This class is used as a base for the @href SPA_EDGE_approx_options and @href SPA_COEDGE_approx_options classes.
 * As such, it provides parameters and functionality common to both. 
 **/
class DECL_KERN SPA_approx_options: public ACIS_OBJECT
{

public:
    // methods to set values to be stored inside the class instance

	/**
	* Sets the tolerance that <tt>api_approx</tt> will attempt to fit to.
	* <br><br>
	* @param tol
	* the position tolerance value
	**/
    void set_requested_tol(double tol);

	/**
	* Sets the parameterization mode for the output of <tt>api_approx</tt>.
	* <br><br>
	* @param pm
	* the parameterization mode
	**/
    virtual void set_param_mode(SPA_approx_param_mode pm);

	/**
	* Sets the degree for the output of <tt>api_approx</tt>.
	* <br><br>
	* @param ad
	* the degree of the approximating curve
	**/
	void set_degree(SPA_approx_degree ad);

    //query methods

	/**
	* Returns the tolerance that <tt>api_approx</tt> will attempt to fit to.
	**/
    double get_requested_tol() const;

	/**
	* Returns the parameterization mode that will be used for the output of <tt>api_approx</tt>.
	**/
    SPA_approx_param_mode get_param_mode() const;

	/**
	* Returns the degree for the output of <tt>api_approx</tt>.
	**/
	SPA_approx_degree get_degree() const;

    // methods for returning various informational aspects from the approximation step

	/**
	* Returns the actual tolerance for the approximation produced.
	* For edges, this is at least the maximum gap between edge curve and its approximation.
	* For coedges, this is at least as large as the maximum gap between the 
	* approximation and the projection of the edge curve on the face the coedge bounds.
	**/
    double get_actual_tol() const;

	/**
	* @nodoc
	**/
	SPA_internal_approx_options* get_impl() const;

	/**
    * Determines if <tt>set_range()</tt> was invoked on a derived class instance.
	**/
    logical get_is_domain_set() const;
    
protected:
    // base class constructors, destructor are all protected to
    // prevent construction/destruction except via derived class construction/destruction

	/**
	* @nodoc
	**/
    SPA_approx_options();

	/**
	* @nodoc
	**/
    SPA_approx_options(SPA_approx_options const &);

	/**
	* @nodoc
	**/
    virtual ~SPA_approx_options();

	/**
	* @nodoc
	**/
    SPA_approx_options &operator=(SPA_approx_options const &);

	/**
	* @nodoc
	**/
    SPA_internal_approx_options *m_pApproxOpts;  
};
                                                                    
/**
 * The <tt>SPA_EDGE_approx_options</tt> object is used to control the behavior of
 * the signature of @href api_approx that approximates an edge.
 */
class DECL_KERN SPA_EDGE_approx_options: public SPA_approx_options
{
public:
    SPA_EDGE_approx_options();
    SPA_EDGE_approx_options(SPA_EDGE_approx_options const &);
    SPA_EDGE_approx_options &operator=(SPA_EDGE_approx_options const &);
    virtual ~SPA_EDGE_approx_options();
    
    // methods to set and get local data

	/**
	* Sets the desired domain for the edge approximation.
	* <br><br>
	* @param desired_domain
	* the domain of the edge approximation
	**/
    void set_domain(SPAinterval const &desired_domain);

	/**
	* Gets the domain of the approximation.
	**/
    SPAinterval const &get_domain() const;    
};

/**
 * The <tt>SPA_COEDGE_approx_options</tt> object is used to control the behavior of
 * the signature of @href api_approx that approximates a coedge.
 */
class DECL_KERN SPA_COEDGE_approx_options: public SPA_approx_options
{
public:
    SPA_COEDGE_approx_options();
    SPA_COEDGE_approx_options(SPA_COEDGE_approx_options const &);
    SPA_COEDGE_approx_options &operator=(SPA_COEDGE_approx_options const &);
    virtual ~SPA_COEDGE_approx_options();

    // methods to set local data

	/**
	* Sets the parameterization mode for the output of <tt>api_approx</tt>. 
	* NOTE: <tt>SPA_approx_param_mode_APPROX_ARC_LENGTH</tt> is not available 
	* for coedge approximation.
	* <br><br>
	* @param pm
	* the parameterization mode
	**/
    virtual void set_param_mode(SPA_approx_param_mode pm);

	/**
	* Sets the desired domain for the edge approximation.
	* <br><br>
	* @param desired_domain
	* the domain of the edge approximation
	**/
    void set_domain(SPAinterval const &desired_domain);

	/**
	* Sets whether to extend the surface for tolerant edge projection.
	* <br><br>
	* @param do_extend
	* specifies whether or not to extend the surface
	**/
	void set_extend_surface(logical do_extend);
    
    // query methods

	/**
	* Gets the domain.
	**/
    SPAinterval const& get_domain() const;

	/**
	* Gets the logical indicating whether to extend the surface for tolerant edge projection.
	**/
	logical get_do_extend_surface() const;

};

/** @} */
#endif
