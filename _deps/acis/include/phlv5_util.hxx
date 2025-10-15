// COPYRIGHT DASSAULT SYSTEMES 1989
 
#if !defined( Phlv5_Utils_CLASS )
#define Phlv5_Utils_CLASS

#include "dcl_phlv5.h"
#include "logical.h"
#include "api.hxx"  

class ENTITY_LIST;
class ENTITY;
class BODY;
class FACE;
class WIRE;
class EDGE;
class LOOP;

//
// Very simple class to organize phlv5 pre-processing code. No data members or anything.
//

enum phlv5_topology_type {PHLV5_OPEN_FACE, PHLV5_TORUS_TYPE, PHLV5_SPHERE_TYPE_V, 
						  PHLV5_SPHERE_TYPE_U, PHLV5_CONE_TYPE_U, PHLV5_CONE_TYPE_V, PHLV5_UNKNOWN_TYPE};
 
class DECL_PHLV5 Phlv5_Utils
{ 
	public:	

		// Replace this with get owner next chance
        static BODY* BodyOfEntity( const ENTITY* entity );
        static BODY* BodyOfEdge( const EDGE* edge );
				
		// Main preprocessor function
		static outcome PreProcessBody (BODY* body);

		// Main splitting function
		static void split_phlv5_faces(BODY* body);
		
		// Hidden attribute functions
		static logical edge_is_hidden(EDGE* edge);
		static void clear_seam_lines();

		// Debugging functions
		static logical draw_face_pcurves(FACE*, FILE*);

		// Returns all the split faces from a given face - only goes one level deep.
		static void sg_get_all_split_faces(FACE*, ENTITY_LIST&);

		// removes bad faces
		static logical remove_bad_faces(BODY* body);

		// Split utility functions
		static phlv5_topology_type phlv5_get_topology_type(FACE *inface);
		static logical split_cone_v_closed(FACE* this_face, LOOP* periphery_loop);
		static logical split_cone_u_closed(FACE* this_face, LOOP* periphery_loop);
		static logical split_sphere_v_closed(FACE* bface);
		static logical split_sphere_u_closed(FACE* bface);
		static logical split_torus(FACE* bface, ENTITY_LIST& new_faces);
	
		// P-curve repair utility functions 
		static logical examine_face_pcurves(FACE *this_face, logical fix);
		static logical examine_analytic_face_pcurves(FACE*, logical);
		static logical add_pcurves_to_face(FACE* inFace);
		static logical has_no_pcurves(FACE* inFace);		

		static void mark_split_edges_as_hidden( ENTITY_LIST& new_faces, ENTITY_LIST& original_edges );

};

DECL_PHLV5 void CATHLREntity_Check_PHLV5();

#endif
