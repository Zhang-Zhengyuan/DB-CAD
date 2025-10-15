/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Main definition file for offset error information return object.
/*******************************************************************/
#if !defined( OFFS_ERROR_INFO )
#define OFFS_ERROR_INFO
/**
 * @file offserr_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup OFSTAPI
 *  @{
 */
#include <stdio.h>
#include "dcl_ofst.h"
#include "param.hxx"
#include "logical.h"
#include "err_info.hxx"
#include "errorsys.hxx"
class ENTITY;
class surface;
/**
 * Error information from an offset operation
 */
class DECL_OFST offset_error_info: public error_info 
{
protected:
    // no instances of offset_error_info to be made
    // constructor and destructor are protected
    offset_error_info();
    virtual ~offset_error_info(); 
public:
	/**
	 * Error ID
	 */
    LOCAL_PROC int id();
	/**
	 * Type ID for this object
	 */
    virtual int type() const; 
};
/**
 * Error information from an offset surface operation.
 */
class DECL_OFST curvature_error_info: public offset_error_info {
	/**
	 *
	 */
    SPApar_pos* _uv_param;
    /**
	 * number of parameters 
	 */
	int _num_par;               
	/**
	 * the offset distance (with respect to the sense of the face)
	 */
    double _offset_distance;
    /**
	 * the smaller of the two principal radii of curvature
	 */                            
    double _rad_crv;
    /**
	 * Is entity dead
	 */
	logical _entity_dead; 

	virtual unsigned int get_special_aux_data_types(SPAstr const *&aux_data_types) const;

	virtual unsigned int query_special_aux_data(SPAstr const &aux_data_type,
		VOID_LIST &aux_data) const;

public:
	/**
	 * Constructor
	 * @param uv_param
	 * Parameters at which error occurs
	 * @num_par
	 * number of uv parameters
	 * @param offset_distance
	 * offset distance
	 * @param face
	 * face where error took place
	 * @rad_crv
	 * radius of curvature
	 */
    curvature_error_info(SPApar_pos* uv_param, 
						int num_par, 
						double offset_distance, 
						ENTITY* face = NULL, 
						double rad_crv = 0);
	/**
	 * Destructor
	 */
    virtual ~curvature_error_info(); 
	/**
	 * Error ID
	 */
    LOCAL_PROC int id();
	/**
	 * Type ID of this object
	 */ 
    virtual int type() const; 

	/**
	 * Set error ENTITY
	 */
    void set_entity(ENTITY * ent); 
	/**
	 * Get error ENTITY
	 */
    ENTITY* get_entity();
	/**
	 * Is error ENTITY dead.
	 */
    logical is_entity_dead() { return _entity_dead; }
	/**
	 * Array of UV error parameters
	 */
    const SPApar_pos* get_uv_param() { return _uv_param; }
	/**
	 * Size of error uv parameters array
	 */
    int get_num_param() { return _num_par; }
	/**
	 * Returns if offset is positive.
	 */
    logical get_positive_offset() { return (_offset_distance > 0.0); }
	/**
	 * Returns the offset distance.
	 */
    double get_offset_distance() { return _offset_distance; }
	/**
	 * Return radius of curvature.
	 */
    double get_rad_crv() { return _rad_crv; }
};

/*! @} */
#endif

