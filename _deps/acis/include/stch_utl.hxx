/* ORIGINAL: acis2.1/sg_husk/stitch/stch_utl.hxx */
/* $Id: stch_utl.hxx,v 1.6 2000/12/26 18:26:42 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( STCH_HDR_DEF )
#define STCH_HDR_DEF

/**
 * \defgroup BOOLNMOPTAPI Unstitch Non-Manifold Body Options
 * \ingroup BOOLOPTIONSAPI
 *
 */

/**
* @file stch_utl.hxx
* @CAA2Level L1
* @CAA2Usage U1
*/

#include "dcl_bool.h"
#include "tophdr.hxx"

/*! \addtogroup BOOLNMOPTAPI
 *
 *  @{
 */

/**
*
* Enum controlling whether internally non-manifold entities are unstitched during
* an unstitch operation.
*
* <br><br>
*
* <b>Role: </b>UNSTITCH_NM_OPTION is an enum used to set the options 
* for a particular operation that involves unstitching non-manifold bodies. It controls 
* how internal non-manifold entities are treated.
* 
* <br><br>
* 
* This enum is used by the @href unstitch_nm_options class to control unstitching of
* internally non-manifold entities. 
* 
* <br><br>
*
* @param UNSTITCH_NM_ALL_BUT_INTERNAL_NM_EDGES
* Default. Unstitch all non-manifold entities, including internally nm-vertices but not
* including internally nm-edges.It is the default value of an
* unstitch_nm_options object to preserve backwards compatibility with existing calls
* to @href api_unstitch_nonmani.
* <br>
* @param UNSTITCH_NM_ALL_NMS
* Unstitch all non-manifold entities, including internally nm-vertices and nm-edges.
* This provides consistency to the unstitching of all non-manifold entities.
* <br>
* @param UNSTITCH_NM_ALL_BUT_INTERNAL_NMS
* Unstitch all non-manifold entities, except internally nm-edges and nm-vertices.
* This leads to no duplicate vertices in the resulting lumps, as it does not unstitch internally
* non-manifold entities.
* <br>
* @param UNSTITCH_NM_ALL_NMS_AND_REPORT_INVALIDITY
* Same behaviour as UNSTITCH_NM_ALL_NMS and throw error if lumps contain duplicate vertices. 
*
*/
#if 0
enum UNSTITCH_NM_OPTION
{
    UNSTITCH_NM_ALL_BUT_INTERNAL_NM_EDGES,
    UNSTITCH_NM_ALL_NMS,
    UNSTITCH_NM_ALL_BUT_INTERNAL_NMS,
    UNSTITCH_NM_ALL_NMS_AND_REPORT_INVALIDITY,
};
#endif

/**
 * @nodoc
 * Although these are the enums being used, the documentation shown above hides internal
 * enum values that should not be used externally
 */
enum UNSTITCH_NM_OPTION
{
    UNSTITCH_NM_ALL_BUT_INTERNAL_NM_EDGES,
    UNSTITCH_NM_ALL_NMS,
    UNSTITCH_NM_ALL_BUT_INTERNAL_NMS,
    UNSTITCH_NM_OTHER_THAN_SELF_NMS = 2, // Please do not use. This is for internal use only
    UNSTITCH_NM_ALL_NMS_AND_REPORT_INVALIDITY,
    UNSTITCH_NM_UNKNOWN
};

/**
* Hold information and options for an unstitching of non-manifolds operation.
* 
* <br><br>
* 
* <b>Role:</b> An @href unstitch_nm_options object supplies information to a given
* unstitch non-mani operation.
*
* <br><br>
*
* This class must be used by @href api_unstitch_nonmani in order to control the treatment
* of  internally non-manifold entities. 
*
* Values are set based on the @href UNSTITCH_NM_OPTION enum.
*/
class DECL_BOOL unstitch_nm_options : public ACIS_OBJECT
{
public: 
    /**
    * Default constructor.
    */
    unstitch_nm_options();
    unstitch_nm_options( unstitch_nm_options const &uo );

    /**
    * Destructor.
    */
    virtual ~unstitch_nm_options ()  { };

    /**
    * Accessor. Gets current value of unstitch nm option, based on enum @href UNSTITCH_NM_OPTION.
    */
    UNSTITCH_NM_OPTION get_unstitch_nm_option() const;

    /**
    * Accessor. Gets current value of unstitch nm option, based on enum @href UNSTITCH_NM_OPTION.
    */
    const char* get_unstitch_nm_option_string( UNSTITCH_NM_OPTION new_option ) const;

    /**
    * Accessor. Sets value of unstitch nm option, based on enum @href UNSTITCH_NM_OPTION.
    * @param new_option
    * Option value.
    */
    void set_unstitch_nm_option( UNSTITCH_NM_OPTION new_option );

    /**
    * @nodoc
    * Equals operator for copy constructor.
    * @param rhs
    * unstitch_nm_options object.
    */
    unstitch_nm_options &operator= ( unstitch_nm_options const &rhs );

    /**
    * @nodoc
    * Always returns FALSE.
    * @param rhs
    * unstitch_nm_options object.
    */
    logical operator== ( unstitch_nm_options const &rhs ) const
    {
        SPAUNUSED( rhs );
        return FALSE;
    }
    /**
    * @nodoc
    * Always returns TRUE.
    * @param rhs
    * unstitch_nm_options object.
    */
    logical operator!= ( unstitch_nm_options const &rhs ) const
    {
        SPAUNUSED( rhs );
        return TRUE;
    }

private:
    /**
    * @nodoc
    * Data member - Keep a record of the current option based on given enum
    */
    UNSTITCH_NM_OPTION m_CurrOption;

    /**
    * @nodoc
    * Data member - Used to keep a record and printout the enum values as strings.
    */
    static const char* const unstitch_nm_opt_string[];  
};

/*! @} */

/////////////////////////////////////////////////////////////////
//	Utility routines broken out from stitch.cxx
/**
 * @nodoc
 */
DECL_BOOL LUMP *sg_last_lump( const BODY * );
/**
 * @nodoc
 */
DECL_BOOL SHELL *sg_last_shell( const LUMP * );
/**
 * @nodoc
 */
DECL_BOOL FACE *sg_last_face( const SHELL * );
/**
 * @nodoc
 */
DECL_BOOL void sg_transfer_faces( SHELL *, SHELL * );
/**
 * @nodoc
 */
DECL_BOOL void sg_transfer_lumps( BODY *, BODY * );
/**
 * @nodoc
 */
DECL_BOOL void sg_transfer_wires( BODY *, BODY * );
/**
 * @nodoc
 */
DECL_BOOL void sg_remove_lump( LUMP * );
/**
 * @nodoc
 */
DECL_BOOL void sg_remove_shell( SHELL * );
/**
 * @nodoc
 */
DECL_BOOL void sg_merge_shells( SHELL *, SHELL * );
/**
 * @nodoc
 */
DECL_BOOL void sg_merge_bodies( BODY *, BODY * );
/**
 * @nodoc
 */
DECL_BOOL void sg_merge_lumps( LUMP *, LUMP * );
/**
 * @nodoc
 */
DECL_BOOL void sg_put_pcurves_in_intcurve( COEDGE *, double );
/**
 * @nodoc
 */
DECL_BOOL void sg_make_pcurves_private( COEDGE * & );
/**
 * @nodoc
 */
// STI let (2/97): Moved to kern2.2/kernel/sg_husk/query/q_vert.hxx
//logical same_edge_group		( VERTEX *, COEDGE *, COEDGE * );
DECL_BOOL void sg_remove_edge_group( VERTEX *, COEDGE * );
/**
 * @nodoc
 */
DECL_BOOL void sg_get_fixed_params( EDGE *, double &, double & );
#endif
