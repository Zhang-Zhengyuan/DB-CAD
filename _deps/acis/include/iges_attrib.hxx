/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef  SPAXIGES_IGESHUSK_ATTRIB__HXX
#define  SPAXIGES_IGESHUSK_ATTRIB__HXX

#include "dcl_kern.h"
#include "attrib.hxx"
#include "entity.hxx"
#include "datamsc.hxx"
#include "interval.hxx"

#define MAX_LAYER_SIZE 10

// knt 20 Nov 2004. Added iop attributes to spakern.

extern DECL_KERN int ATTRIB_GSSL_IGES_TYPE ;
#define ATTRIB_GSSL_IGES_LEVEL (ATTRIB_LEVEL + 1)
MASTER_ATTRIB_DECL( ATTRIB_GSSL_IGES, KERN )

extern DECL_KERN int ATTRIB_GSSL_IGES_LAYER_TYPE;
#define ATTRIB_GSSL_IGES_LAYER_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_LABEL_TYPE;
#define ATTRIB_GSSL_IGES_LABEL_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_COLOR_TYPE;
#define ATTRIB_GSSL_IGES_COLOR_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_LINEWEIGHT_TYPE;
#define ATTRIB_GSSL_IGES_LINEWEIGHT_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_DE_ENTRY_TYPE;
#define ATTRIB_GSSL_IGES_DE_ENTRY_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_LINEFONT_TYPE;
#define ATTRIB_GSSL_IGES_LINEFONT_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_ACIS_SURF_RANGE_TYPE;
#define ATTRIB_GSSL_IGES_ACIS_SURF_RANGE_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_SURF_RANGE_TYPE;
#define ATTRIB_GSSL_IGES_SURF_RANGE_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)

extern DECL_KERN int ATTRIB_GSSL_IGES_SRFMAP_TYPE;
#define ATTRIB_GSSL_IGES_SRFMAP_LEVEL (ATTRIB_GSSL_IGES_LEVEL + 1)


class DECL_KERN ATTRIB_GSSL_IGES_LAYER: public ATTRIB_GSSL_IGES
{
	enum layer_type { SingleValued = 0, MultiValued };

	int layer_sz;
	int layer[MAX_LAYER_SIZE];
	layer_type type;
	public:

		ATTRIB_GSSL_IGES_LAYER( ENTITY *owner = NULL ) ;
		virtual void split_owner( ENTITY *new_ent );
		virtual void merge_owner( ENTITY *other_ent, logical delete_owner );
		virtual void copy_owner( ENTITY *new_ent );
		// some set and get functions
		int get_layer_type(void) const { return (int)type; }
		int get_layer_size(void) const { return layer_sz ; }
		void set_single_layer( int sz ){ layer_sz = sz ; type = SingleValued ; return; }
		int get_mult_layer( int *&out_layer ) const;
		void set_mult_layer( const int &sz, int *layer_val );
		int operator==(	const ATTRIB_GSSL_IGES_LAYER& ) const;

		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_LAYER, KERN )

};

//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_LABEL: public ATTRIB_GSSL_IGES
{
	public:
		logical flag;
		char label[16];

		ATTRIB_GSSL_IGES_LABEL( ENTITY *owner = NULL ) ;

		virtual void split_owner( ENTITY *new_ent );
		virtual void merge_owner( ENTITY *other_ent, logical delete_owner );
		virtual void copy_owner( ENTITY *new_ent );
		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_LABEL, KERN )

};

//-----------------------------------------------------------------------
// This attribute can be used to associate the color of an IGES ENTITY. 
//
//-----------------------------------------------------------------------
class DECL_KERN  ATTRIB_GSSL_IGES_COLOR : public ATTRIB_GSSL_IGES
{
	public:
		int color_data;
		double RGB[3];
		char color_name[32];

		ATTRIB_GSSL_IGES_COLOR( ENTITY *owner = NULL ) ;

		virtual void split_owner( ENTITY *new_ent );
 		virtual void merge_owner( ENTITY *new_ent, logical delete_owner );
 		virtual void copy_owner( ENTITY *new_ent );
		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_COLOR, KERN )

};


//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_LINEWEIGHT : public ATTRIB_GSSL_IGES
{
	public:
		double		line_weight;

		ATTRIB_GSSL_IGES_LINEWEIGHT( ENTITY *owner = NULL ) ;
 
		virtual void split_owner( ENTITY *new_ent );
 		virtual void merge_owner( ENTITY *other_ent, logical delete_owner );
 		virtual void copy_owner( ENTITY *new_ent );
		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_LINEWEIGHT, KERN )

};


//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_DE_ENTRY: public ATTRIB_GSSL_IGES
{
	int de_entry;
	public:

		ATTRIB_GSSL_IGES_DE_ENTRY( ENTITY *owner = NULL, int id = 0 ) ;
                                    // returns TRUE 
		virtual void split_owner( ENTITY *new_ent );
		virtual void merge_owner( ENTITY *new_ent, logical delete_owner );
		virtual void copy_owner( ENTITY *new_ent );
		void set_de_entry( int id ){ de_entry = id ; return ; }
		int get_de_entry(void){ return de_entry ; }
		void debug( FILE * );

		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_DE_ENTRY, KERN )

};
//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_LINEFONT : public ATTRIB_GSSL_IGES
{
	public:
		int line_font;

		ATTRIB_GSSL_IGES_LINEFONT( ENTITY *owner = NULL ) ;
                                    // returns TRUE 
 		virtual void split_owner( ENTITY *new_ent );
 		virtual void copy_owner( ENTITY *new_ent );
 		virtual void merge_owner( ENTITY *new_ent, logical delete_owner );
		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_LINEFONT, KERN )

};
//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_ACIS_SURF_RANGE: public ATTRIB_GSSL_IGES
{
	public:
		SPAinterval u ;
		SPAinterval v ;
		ATTRIB_GSSL_IGES_ACIS_SURF_RANGE( ENTITY *owner = NULL ) ;
                                    // returns TRUE 
        void merge_owner( ENTITY *other_ent , logical delete_owner);
		void copy_owner( ENTITY *other_ent);
		void split_owner( ENTITY *new_ent );


		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_ACIS_SURF_RANGE, KERN )
};


//-----------------------------------------------------------------------
class DECL_KERN ATTRIB_GSSL_IGES_SURF_RANGE: public ATTRIB_GSSL_IGES
{
	public:
		SPAinterval u ;
		SPAinterval v ;
		ATTRIB_GSSL_IGES_SURF_RANGE( ENTITY *owner = NULL ) ;
                                    // returns TRUE 
        void merge_owner( ENTITY *other_ent , logical delete_owner);
		void copy_owner( ENTITY *other_ent);
		void split_owner( ENTITY *new_ent );

		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_SURF_RANGE, KERN )
};
//-----------------------------------------------------------------------
// This attribute is used to specify the mapping between an IGES surface
// and an ACIS surface.
//
//-----------------------------------------------------------------------
//
// Define the various types of mappings between an IGES representation
// for a surface and the ACIS representation.
//
//		APPROXIMATION			the ACIS surface is an approximation 
//		GEOMETRIC_EQUIV			the ACIS surface repressents the IGES
//								surface exactly
//		PARAMETRIC_EQUIV		the ACIS surface represents the IGES 
//								surface exactly and also has the same
//								parametrization except perhaps for a
//								linear transformation
//
enum IGES_SRF_MAPPING {
	APPROXIMATION,
	GEOMETRIC_EQUIV,
	PARAMETRIC_EQUIV
	};

class DECL_KERN ATTRIB_GSSL_IGES_SRFMAP: public ATTRIB_GSSL_IGES
{
	IGES_SRF_MAPPING srf_mapping;

	public:

		ATTRIB_GSSL_IGES_SRFMAP( 
			ENTITY *owner = NULL, 
			IGES_SRF_MAPPING srf_mapping = APPROXIMATION
		) ;

		void set_srf_mapping( IGES_SRF_MAPPING new_srf_mapping )
			{ srf_mapping = new_srf_mapping; return; }

		IGES_SRF_MAPPING get_srf_mapping(void){ return srf_mapping; }

		void debug( FILE * );



		ATTRIB_FUNCTIONS( ATTRIB_GSSL_IGES_SRFMAP, KERN )
};
//-----------------------------------------------------------------------
#endif 
