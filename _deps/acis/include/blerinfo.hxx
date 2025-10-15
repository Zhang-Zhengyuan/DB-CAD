/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( BL_ERR_INFO )
#define BL_ERR_INFO
 
#include "acis.hxx"
#include "dcl_blnd.h"

#include "lists.hxx"
#include "position.hxx"
#include "errorsys.hxx"
#include "err_info.hxx"

class ENTITY;
class ENTITY_LIST;

class DECL_BLND blend_error_info : public error_info 
{
	public:

		// data

		SPAposition	point;
		logical		point_set;

		// constructor and destructor

		blend_error_info( 
				ENTITY_LIST const &ent_list,
				SPAposition const &pos = 
				* (SPAposition const *) NULL_REF 
				);

		blend_error_info( 
				ENTITY *e0 = NULL, 
				ENTITY *e1 = NULL,
				ENTITY *e2 = NULL,
				SPAposition const &pos = 
				* (SPAposition const *) NULL_REF 
				);

		~blend_error_info();

		LOCAL_VAR int id();
		virtual int type() const;
};

// The modified sys error.

void 
bl_sys_error( 
        err_mess_type err_num, 
        ENTITY *e1 = NULL,
		ENTITY *e2 = NULL, 
		ENTITY *e3 = NULL, 
        SPAposition const &pos = 
            * (SPAposition const *) NULL_REF  
        );

void
bl_sys_error( 
        err_mess_type err_num, 
        ENTITY_LIST const &e_list,
        SPAposition const &pos =
			*( SPAposition const * ) NULL_REF
        );
#endif
