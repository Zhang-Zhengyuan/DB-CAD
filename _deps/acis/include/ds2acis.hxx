/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : ds2acis.hxx
// created  : April 5, 1995
// author   : GWCelniker
// contains : classes ATTRIB_DM2ACIS
//                    ATTRIB_DS2ACIS
//                    ATTRIB_DC2ACIS
//
//            Functions for debug and convenience - not intended for general use
//            DS_dmod_to_face()
//            DS_remove_boundary_attributes()
//
// synopsis : Glue code to connect the dmod functions to the
//            Acis modeler.  Given an ACIS Entity build a deformable
//            model.  Save the user inputs on the ACIS model.
//            This object will be stored as an attribute attached
//            to the ATTRIB_DSGROUP which is attached to the ENTITY being
//            sculpted to automatically handle deletion and garbage
//            collection problems. Derived classes support deformable
//            surfaces and deformable curves.
//attribute : The contained dmod and its associated rendering data are
//            temporary.  They can be completely generated from
//            the geometry and the information stored in an
//            ATTRIB_DSMODEL attribute on an ATTRIB_DSGROUP attached
//            to an ACIS FACE or EDGE.  These objects will be generated at
//            the start of an editing session and deleted at the end.
//            Modifications to the dynamic model during an editing
//            session are accumulated only in the DS_dmod model.  At
//            the user's discretion those changes can be committed
//            to the ATTRIB_DSMODEL.  Commits can be rolled forward
//            and back.  Edits to the DS_dmod model can not be rolled
//            back.  ATTRIB_DM2ACIS is an attribute associated to
//            the ACIS ENTITY being edited that will hold pointers to
//            all the different models used during a single edit
//            session.  At the end of an edit session the attribute
//            and all the data it points to may be deleted with a
//            call to the API function
//            api_remove_attrib_ds2acis_from_owner()
//

#ifndef ATTRIB_DM2ACIS_ATTRB_H
#define ATTRIB_DM2ACIS_ATTRB_H

#include "api.hxx"
#include "agcatt.hxx"    // organization class attribute    // organization class attribute

/**
* @file ds2acis.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

// forward declarations
class FACE ;
class surface ;
class DS_dmod ;

class EDGE ;
class curve ;
class DS_dcurv ;

class ATTRIB_DSMODEL ;
class ATTRIB_DSGROUP ;
class SPApar_pos;
class SPApar_vec;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class pcurve ;
class intcurve ;
class INDEXED_MESH ;
#ifndef DS_USE_6x_RENDERING
class ADM_regobj;
#else
class deformable_figure ;
#endif /*DS_USE_6x_RENDERING*/
class SPAtransf;
class outcome ;
class ATTRIB_CRV_CSTRN ;
class COEDGE ;

class DS_pfunc ;
class DS_crv2acis ;
class DS_load ;
class DS_cstrn ;
class DM_tag_array;
class DM_dbl_array;

class adm_options;
class AcisOptions;

#include "bs3surf.hxx"   // typedef bs3_surface   // typedef bs3_surface
#include "bs3curve.hxx"  // typedef bs3_curve  // typedef bs3_curve
#include "param.hxx"    // SPApar_vec, SPApar_pos    // SPApar_vec, SPApar_pos

#include "ds2acis.err"     // sys_error #defines     // sys_error #defines
#include "ds2std.hxx"      // DS_acis_assert(),      // DS_acis_assert(),
                                          // DS_sys_error()
#include "dmapi.hxx"      // DM_DRAW_... defines      // DM_DRAW_... defines
// for ATTRIB_DS2ACIS
#define D2A_UPDATE_MESH      ( 1 << 0 )     // recalc mesh vertices
#define D2A_REBUILD_MESH     ( 1 << 1 )     // rebuild mesh

#include "dcl_adm.h"            // DECL_ADM            // DECL_ADM

#ifndef DS_USE_6x_RENDERING
class DS_face_model_array;
#else
#include "dsfmarr.hxx" // class DS_face_model_array // class DS_face_model_array
#endif
class ATTRIB_DS2ACIS;

/*
// Helper function to walk hierarchy and create a (dmod,tag)
// pair.
void DS_find_hierarchy_dmods
  (ATTRIB_DM2ACIS *hierarchy,       // in : ATTRIB_DS2ACIS to be processed
   DS_int_block & tag_array);       // out: array of dmod tags
*/                                    //
// Global variable governing choice of link contraint/load
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_ADM int DS_use_link_loads;
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_ADM int DS_auto_elevate;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_ADM outcome api_dm_get_attrib_dm2acis(ENTITY *entity, ATTRIB_DM2ACIS *&attrib_dm2acis,
                                           adm_options *admO, AcisOptions *acisO);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_ADM outcome api_dm_rm_multi_face( ATTRIB_DM2ACIS *attrib_dm2acis,
                                       int tag, adm_options *adm_o, AcisOptions *ao);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_ADM outcome api_dm_add_link( ATTRIB_DM2ACIS *attrib_dm2acis, const ENTITY *ent,
                                  int behavior, double gain, int& tag, adm_options *adm_o, AcisOptions *ao);
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_DC2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_DM2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_DS2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif



// During sculpting one ATTRIB_DM2ACIS may point to hierarchy of
// DS_dmod deformable objects each of which may contain any number
// of loads and constraints.  Every DS_dmod, load, and constraint
// is assigned a unique tag number.  Tag number 2 is assigned to the
// root DS_dmod, and all subsequent tag objects are assigned tag
// values in the order in which they are created.  Tag values
// are guaranteed to remain the same before and after a Save and
// Restore cycle.  Tag values of deleted objects are not reused.
//
// The active DS_dmod:  While sculpting a patch hierarchy of
// DS_dmods, the ATTRIB_DM2ACIS saves an active DS_dmod pointer.
// The function Make_active(), and any of the
// methods that operate on or find a constraint or load, make one of the
// members of the hierarchy active.  All I/O methods that act on
// the values of a DS_Dmod or its loads and constraints
// act on the active member of the hierarchy by default.
// The tag number of the active member can be found by using the
// Active() method.
// The ATTRIB_DM2ACIS active feature helps simplify the user interface
// by not requiring that the end_user always specify a DS_dmod
// object by name for every method call.
// Graphics are supported to distinguish the active DS_dmod
// for the end-user during a sculpting session.

// d2a_editing: Some DShusk functions cause the split, merge, and lose
//              callbacks on DShusk attributes to be called.  These
//              same functions are also launched by other ACIS
//              functions, like the booleans.  The behavior of those
//              callbacks may differ depending on who causes
//              the callback to be executed.  All DShusk commands
//              that edit the ATTRIB_DM2ACIS object and which may
//              cause the callbacks to fire, will set the d2a_editing
//              value to 1 when starting and clear the value back to
//              0 when completing.
//

// 26-Apr-99 erik: Removed obsolete is_<class> documentation template.

//=============================================================
// class ATTRIB_DM2ACIS Derived from ATTRIB_AGC
//=============================================================
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_ADM int ATTRIB_DM2ACIS_TYPE;
#define    ATTRIB_DM2ACIS_LEVEL (ATTRIB_AGC_LEVEL + 1)

/**
 * Connects the deformable model functions to the ACIS modeler.
 * <br>
 * <b>Role:</b> This is the glue code to connect the deformable modeler functions to the
 * ACIS modeler. Given an ACIS <tt>ENTITY</tt> it builds a deformable model, and saves the user
 * inputs on the ACIS model. This object will be stored as an attribute attached to the
 * <tt>ATTRIB_DSGROUP</tt> which is attached to the <tt>ENTITY</tt> being sculpted to automatically handle
 * deletion and garbage collection problems. Derived classes support deformable surfaces
 * and deformable curves.
 * <br><br>
 * The contained <tt>dmod</tt> and its associated rendering data are temporary. They can be
 * completely generated from the geometry and the information stored in an <tt>ATTRIB_DSMODEL</tt>
 * attribute on an <tt>ATTRIB_DSGROUP</tt> attached to an ACIS <tt>FACE</tt> or <tt>EDGE</tt>. These objects will be
 * generated at the start of an editing session and deleted at the end.
 * <br><br>
 * Modifications to the dynamic model during an editing session are accumulated only
 * in the <tt>DS_dmod</tt> model. At the user's discretion those changes can be committed to the
 * <tt>ATTRIB_DSMODEL</tt>. Commits can be rolled forward and back. Edits to the <tt>DS_dmod</tt> model can
 * not be rolled back. <tt>ATTRIB_DM2ACIS</tt> is an attribute associated to the ACIS <tt>ENTITY</tt> being
 * edited that will hold pointers to all the different models used during a single edit
 * session. At the end of an edit session the attribute and all the data it points to may
 * be deleted with a call to the API function <tt>api_remove_attrib_ds2acis_from_owner</tt>.
 * <br><br>
 * During sculpting one <tt>ATTRIB_DM2ACIS</tt> may point to hierarchy of <tt>DS_dmod</tt> deformable
 * objects each of which may contain any number of loads and constraints. Every <tt>DS_dmod</tt>,
 * load, and constraint is assigned a unique tag number. Tag number 2 is assigned to the
 * root <tt>DS_dmod</tt>, and all subsequent tag objects are assigned tag values in the order in
 * which they are created. Tag values are guaranteed to remain the same before and after
 * a save and restore cycle. Tag values of deleted objects are not reused.
 * <br><br>
 * The active <tt>DS_dmod</tt>: While sculpting a patch hierarchy of <tt>DS_dmod</tt>, the
 * <tt>ATTRIB_DM2ACIS</tt> saves an active <tt>DS_dmod</tt> pointer. The function <tt>Make_active</tt>, and any of
 * the methods that operate on or find a constraint or load, make one of the members of
 * the hierarchy active. All I/O methods that act on the values of a <tt>DS_Dmod</tt> or its loads
 * and constraints act on the active member of the hierarchy by default. The tag number
 * of the active member can be found by using the <tt>Active</tt> method. The <tt>ATTRIB_DM2ACIS</tt> active
 * feature helps simplify the user interface by not requiring that the end-user always
 * specify a <tt>DS_dmod</tt> object by name for every method call. Graphics are supported to
 * distinguish the active <tt>DS_dmod</tt> for the end-user during a sculpting session.
 * <br><br>
 * <tt>d2a_editing</tt>: Some ADM functions cause the split, merge, and lose callbacks on
 * ADM attributes to be called. These same functions are also launched by other ACIS
 * functions, like the Booleans. The behavior of those callbacks may differ depending on
 * who causes the callback to be executed. All ADM commands that edit the <tt>ATTRIB_DM2ACIS</tt>
 * object and which may cause the callbacks to fire, will set the <tt>d2a_editing</tt> value to 1
 * when starting and clear the value back to 0 when completing.
 * @see ADM_regobj, AcisVersion, DS_dmod
 */
 class DECL_ADM ATTRIB_DM2ACIS: public ATTRIB_AGC  // identifer = "d2a"
{
  private:
    virtual                  // bas: ATTRIB_DM2AICS pure virtual function
    void
     Rm_multi_face           // eff: Disconnect face from quilt
      (int )                 // in : tag = Face id to remove from hierarchy
                             { DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
                             }
  protected:

    virtual void
    Add_link(const ENTITY* , int /*behavior*/, double /*gain*/, int& /*tag*/)
    {
      DS_sys_error(DS_INTERNAL_ERROR);
    }
    // the owner entity pointer is in the parent class ATTRIB
    // the derived class info is available through
    // identity(ATTRIB_DC2ACIS_LEVEL) == ATTRIB_DC2ACIS_TYPE and
    // identity(ATTRIB_DS2ACIS_LEVEL) == ATTRIB_DS2ACIS_TYPE

#ifndef DS_USE_6x_RENDERING
    // Methods for managing the rendering registration object during backup and destruction
    ADM_regobj* Make_regobj_backupcopy() const;
    void Set_regobj(ADM_regobj*);
    void Clear_regobj();
#endif // DS_USE_6x_RENDERING

    AcisVersion d2a_av;         // created with this algorithmic version - req'd to repopulate the ATTRIB_DM2ACIS after roll
                                // AcisVersion CTOR will create using current global version
    int d2a_trim_faces;         // override versioning for trim faces
    int d2a_use_bdy_loads;      // override versioning for using bdy loads
    // execution state
    int d2a_editing ;           // 0 = not being edited by a DShusk command
                                // 1 = being edited by DShusk command

    // commit state
    int d2a_becommited ;         // 0 = NO commit since last solve
                                // 1 = has been commited since last solve
    // the deformable model
    DS_dmod *d2a_dmod ;         // the root deformable model or NULL
//    double   d2a_domain_scale ; // dmod_domain = domain_scale * acis_domain

    // graphics for tags (dimension independent)
#ifndef DS_USE_6x_RENDERING
    ADM_regobj* d2a_regobj;
#else
  deformable_figure
            *d2a_tags_fig ;     // rendering of dmod tag data
#endif /* DS_USE_6x_RENDERING */


#ifndef DS_USE_6x_RENDERING
#else
    void   **d2a_tags_handle ;  // an array of tag_fig handles
    int      d2a_handle_count;  // Size of d2a_tags_handle
#endif /*DS_USE_6x_RENDERING*/


#ifndef DS_USE_6x_RENDERING
#else
  deformable_figure
            *d2a_shape_fig ;    // rendering of tmp_surf data
#endif /* DS_USE_6x_RENDERING */

#ifndef DS_USE_6x_RENDERING
#else
    void   **d2a_shape_handle ; // an array to shape_fig handles
    int      d2a_shape_count ;  // Size of d2a_shape_handle
#endif /*DS_USE_6x_RENDERING*/
    int      d2a_mesh_state ;   // controls remeshing
                                // oneof: 0,D2A_UPDATE_MESH
#ifndef DS_USE_6x_RENDERING
#else
    DS_face_model_array d2a_faces;
#endif
    // sculpting session rendering colors
    double d2a_surfC[3] ;       // surface rendering color
    double d2a_cptC[3] ;        // control point rendering color
    double d2a_positionC[3] ;   // enabled pos_cstrn rendering color
    double d2a_tangentC[3] ;    // enabled tan_cstrn rendering color
    double d2a_curvatureC[3] ;  // enabled C2 rendering color
    double d2a_pos_linkC[3] ;   // enabled pos_linked rendering color
    double d2a_tan_linkC[3] ;   // enabled tan_linked rendering color
    double d2a_C2_linkC[3] ;    // enabled C2 rendering color
    double d2a_cstrn_offC[3] ;  // disabled cstrn rendering color
    double d2a_loadC[3] ;       // load rendering color

    // tag administration - each load, cstrn, parent, and child gets a
    //  unique tag number shared between its dmod and its ATTRIB_DSMODEL
    //  representations

  public:

// internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
     void set_algorithmic_version(const AcisVersion&);
    // internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
    const AcisVersion& get_algorithmic_version() const;
    // internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
    void set_use_bdy_loads(int);
    // internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
    int use_bdy_loads() const;
    // internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
    void set_trim_faces(int);
    // internal use only
/*
// tbrv
*/
/**
 * @nodoc
 */
    int trim_faces() const;

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
     virtual logical pattern_compatible() const;
                                    // returns TRUE
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_ADM outcome api_dm_get_attrib_dm2acis(ENTITY *entity, ATTRIB_DM2ACIS *&attrib_dm2acis,
                                                      adm_options *admO, AcisOptions *acisO);
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_ADM outcome api_dm_rm_multi_face( ATTRIB_DM2ACIS *attrib_dm2acis,
                                         int tag, adm_options *adm_o, AcisOptions *ao);

/*
// tbrv
*/
/**
 * @nodoc
 */
 	friend DECL_ADM outcome api_dm_add_link( ATTRIB_DM2ACIS *attrib_dm2acis, const ENTITY *ent,
                                             int behavior, double gain, int& tag, adm_options *adm_o, AcisOptions *ao);

/**
 * Returns <tt>TRUE</tt> if this is savable.
 */
     virtual logical savable() const
     {
		 return FALSE;
     }
#ifndef DS_USE_6x_RENDERING
/**
 * Returns <tt>d2a_regobj</tt>.
 */
     ADM_regobj* Get_regobj() const;
/**
 * Returns the specified icon.
 * <br><br>
 * @param tag
 * tag object ID.
 */
    DM_icon* Get_icon(int tag);
/*
// tbrv
*/
/**
 * @nodoc
 */
    void   Activate_regobj() ;
/*
// tbrv
*/
/**
 * @nodoc
 */
    void   Deactivate_regobj() ;
/**
 * Sets the icon for a tag object.
 * <br><br>
 * <b>Role:</b> This call should be followed with a call to <tt>Set_icon_owner</tt> to
 * allow the icon to initialize itself.
 * <br><br>
 * @param tag
 * tag object ID.
 * @param icon
 * new icon.
 */
     void   Set_icon(int tag, DM_icon* icon);
/**
 * Sets the icon's owner.
 * <br><br>
 * <b>Role:</b> Serves as notification to the icon that the owning tag object is complete,
 * and the icon can be initialized.
 * <br><br>
 * @param tag
 * tag object ID.
 */
    void    Set_icon_owner(int tag );
/**
 * Sets the icon's owner.
 * <br><br>
 * <b>Role:</b> Serves as notification to the icon that the owning tag object is complete,
 * and the icon can be initialized.
 * <br><br>
 * @param tags
 * array of tags.
 * @param ntags
 * number of tags.
 */
    void    Set_icon_owner(const int* tags, int ntags);
/**
 * Broadcasts the command encapsulated by the <tt>cmd_args</tt> to all tag objects in the <tt>ATTRIB_DM2ACIS</tt> using the data encapsulated by the <tt>draw_args</tt>.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Setstate</tt> method. The data encapsulate by the <tt>cmd_args</tt> is
 * passed through this method.
 * <br><br>
 * @param args
 * command passed.
 */
    void    Setstate_all_icons(const DM_icon_cmd_args& args);
/**
 * Passes the command encapsulated by the <tt>query_args</tt> to the icon to query the icon state.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Query</tt> method. The data encapsulate by the <tt>query_args</tt> is passed through this method.
 * <br><br>
 * @param args
 * args passed.
 * @param tag
 * tag ID.
 */
    void    Query_icon(DM_icon_query_args& args, int tag);
/**
 * Passes the command encapsulated by the <tt>cmd_args</tt> to the icon to change the icon's state.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Setstate</tt> method. The data encapsulated by the <tt>cmd_args</tt> is passed through this method.
 * <br><br>
 * @param args
 * args passed.
 * @param tag
 * tag ID.
 */
    void    Setstate_icon(const DM_icon_cmd_args& args, int tag);
/**
 * Broadcasts the command encapsulated by the <tt>cmd_args</tt> to the icon to change the icon's state.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Setstate</tt> method. The data encapsulated by the <tt>cmd_args</tt> is passed through this method.
 * <br><br>
 * @param args
 * args passed.
 * @param tags
 * array of tags.
 * @param ntags
 * number of tags.
 */
    void    Setstate_icon(const DM_icon_cmd_args& args, const int* tags, int ntags);
/**
 * Broadcasts the command encapsulated by the <tt>cmd_args</tt> to the all icons on the <tt>DS_dmod</tt> specified by the tag to change the icon's state.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Setstate</tt> method. The data encapsulated by the <tt>cmd_args</tt> is passed
 * through this method.
 * <br><br>
 * @param args
 * args passed.
 * @param tag
 * tag ID.
 */
    void    Setstate_dmod_icons(const DM_icon_cmd_args& args, int tag);
/**
 * Draws icons for all tag object in the <tt>ATTRIB_DM2ACIS</tt> using the data encapsulated by the <tt>draw_args</tt>.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Draw</tt> method.The data encapsulate by the <tt>draw_args</tt> is passed through
 * this method.
 * <br><br>
 * @param args
 * args passed.
 */
    void    Draw_all_icons(const DM_icon_draw_args& args);
/**
 * Draws icons using the data encapsulated by the <tt>draw_args</tt>.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Draw</tt> method. The data encapsulate by the <tt>draw_args</tt> is passed through.
 * <br><br>
 * @param args
 * args passed.
 * @param tag
 * tag ID.
 */
    void    Draw_icon(const DM_icon_draw_args& args, int tag);
/**
 * Draws this icon using the data encapsulated by the <tt>draw_args</tt>.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Draw</tt> method. The data encapsulate by the <tt>draw_args</tt> is passed through.
 * <br><br>
 * @param args
 * args passed.
 * @param tags
 * array of tags.
 * @param ntags
 * number of tags.
 */
    void    Draw_icon(const DM_icon_draw_args& args, const int* tags, int ntags);
/**
 * Draws all icons on the <tt>DS_dmod</tt> specified by tag using the data encapsulated by the <tt>draw_args</tt>.
 * <br><br>
 * <b>Role:</b> Calls the icon's <tt>Draw</tt> method. The data encapsulate by the <tt>draw_args</tt> is passed
 * through this method.
 * <br><br>
 * @param args
 * args passed.
 * @param tag
 * tag ID.
 */
    void    Draw_dmod_icons(const DM_icon_draw_args& args, int tag);
/*
    void
    Dist_tag_iline( int tag, const double* pi0, const double* pi1, int& pt_index, double& dist, double& zdepth );
    void
    Iline_pick_tags(const int* tags, int ntags, const double* pi0, const double* pi1, double max_dist, DS_int_block& tags_hit,
                    DS_int_block& pt_indices, DS_dbl_block& dists, DS_dbl_block& zdepths, int& nhits );
    void
    Iline_pick_cpts(const int* tags, int ntags, const double* pi0, const double* pi1, double max_dist, DS_int_block& tags_hit,
                    DS_int_block& pt_indices, DS_dbl_block& dists, DS_dbl_block& zdepths, int& nhits );
*/


#endif

/**
 * Returns a <tt>DM_tag_array</tt> containing the tags of all tag objects embedded in the <tt>ATTRIB_DM2ACIS</tt>.
 * <br><br>
 * @param ntags
 * number of tags.
 * @param tags
 * array of tags found.
 */
    void    Get_tags(int& ntags, DM_tag_array& tags );
/**
 * This method returns a <tt>DM_tag_array</tt> containing the tags of all <tt>DS_dmods</tt> in the <tt>DS_dmod</tt> hierarchy.
 * <br><br>
 * @param ntags
 * number of tags.
 * @param tags
 * array of tags found.
 */
     void    Get_dmods(int& ntags, DM_tag_array& tags);
/**
 * Returns a <tt>DM_tag_array</tt> containing the tags of all tag objects embedded in the specified <tt>DS_dmod</tt>.
 * <br><br>
 * @param dmod_tag
 * DS_dmod ID.
 * @param ntags
 * number of tags.
 * @param tags
 * array of tags found.
 */
    void    Get_dmod_tags(int dmod_tag, int& ntags, DM_tag_array& tags );
/**
 * Returns the ACIS <tt>ENTITY</tt> pointer (<tt>FACE</tt> or <tt>EDGE</tt>) associated with the <tt>DS_dmod</tt> specified by tag.
 * <br><br>
 * @param tag
 * tag object ID.
 */
    ENTITY*    Get_entity(int tag);
/**
 * Evaluates an embedded source curve for the given tag.
 * <br><br>
 * <b>Role:</b> Returns the embedding surface <i>uv</i> (domain) points corresponding to the input
 * curve parameter points. See <tt>ATTRIB_DM2ACIS::Get_xyz</tt> for evaluating the embedding surface.
 * <br><br>
 * @param tag
 * tag object ID.
 * @param s
 * array of domain points.
 * @param npt
 * number of eval points.
 * @param C
 * array of uv points.
 */
    void    Eval_crv_src_domain(int tag, const double* s, int npt, DM_dbl_array& C);
/**
 * Evaluates a target curve for the given tag.
 * <br><br>
 * <b>Role:</b> Returns the <i>xyz</i> points corresponding to the input curve parameter points.
 * <br><br>
 * @param tag
 * tag object ID.
 * @param uv
 * array of domain points.
 * @param npt
 * number of eval points.
 * @param W
 * array of xyz points.
 */
    void    Eval_crv_tgt(int tag, const double* uv, int npt, DM_dbl_array& W);
/**
 * Gets the tag object minimum parameterization value.
 * <br><br>
 * <b>Role:</b> Many tag objects are parameterized by either a continuous (e.g., curve load)
 * or discrete parameter (e.g., point spring set). This function returns the lower corner of
 * the parameterization bounding box. For example, for a curve load, this is a single value,
 * and for an area constraint this a pair (<i>uv</i>) of values.
 * <br><br>
 * @param tag
 * tag object ID.
 * @param s
 * param value.
 */
    void    Param_min(int tag, DM_dbl_array &s);
/*
// tbrv
*/
/**
 * @nodoc
 */
    void    Get_minimal_corners(int tag,DM_dbl_array &us,DM_dbl_array &vs);
/**
 * Gets the tag object maximum parameterization value.
 * <br><br>
 * <b>Role:</b> Many tag objects are parameterized by either a continuous (e.g., curve load)
 * or discrete parameter (e.g., point spring set). This function returns the lower corner of
 * the parameterization bounding box. For example, for a curve load, this is a single value,
 * and for an area constraint this a pair (<i>uv</i>) of values.
 * <br><br>
 * @param tag
 * tag object ID.
 * @param s
 * param value.
 */
    void    Param_max(int tag, DM_dbl_array &s);
/*
// tbrv
*/
/**
 * @nodoc
 */
    void    Find_link_total_gap( int tag, int behavior, double *total_gap);

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
 * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DM2ACIS(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll back and
 * history management.
 * <br><br>
 * @param entity_owner
 * owner.
 */
   ATTRIB_DM2ACIS              // Default Constructor
   (ENTITY *entity_owner=NULL);// in : the future owner of this attribute
                               //      The root parent's FACE or EDGE
/*
  ATTRIB_DM2ACIS             // Constructor
  (DS_dmod * the_dmod) ;       // in : root dmod
*/

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
 * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DM2ACIS(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll back and
 * history management.
 * <br><br>
 * @param d2a
 * deformable model to acis.
 * @param owner
 * owner.
 */
   ATTRIB_DM2ACIS               // Copy Constructor
   (ATTRIB_DM2ACIS &d2a,       // in : attribute being copied
    ENTITY *owner) ;           // in : owner for new attribute
/**
 * Implements the assignment operator.
 * <br><br>
 * @param d2a
 * object with values to copy.
 */
   ATTRIB_DM2ACIS &operator=    // assignment operator
   (const ATTRIB_DM2ACIS &d2a);// in : attribute with values to copy
/**
 * Copies the attribute information from this class instance to the supplied entity.
 * <br><br>
 * @param owner
 * owner for new attribute.
 */
   virtual ATTRIB_AGC*          // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)    // in : owner for new attribute
                               { return(ACIS_NEW ATTRIB_DM2ACIS(*this,owner)); }

  // ATTRIB_DM2ACIS DShusk command editing state functions
/**
 * Command for editing state functions and is related to the <tt>Set_editing</tt> and <tt>Clear_editing</tt> methods.
 */
   int Editing()                { return d2a_editing ; }
/**
 * Sets the editing state functions and is related to the <tt>Editing</tt> and <tt>Clear_editing</tt> methods.
 */
   void Set_editing()           { d2a_editing = 1 ; }
/**
 * Command for editing state functions and is related to the <tt>Editing</tt> and <tt>Set_editing</tt> methods.
 */
  void Clear_editing()         { d2a_editing = 0 ; }

  // ATTRIB_DM2ACIS access functions for mesh rebuild state
/**
 * Access functions for mesh rebuild state.
 */
  int Get_mesh_state()       { return d2a_mesh_state; }
/**
 * Access functions for mesh rebuild state.
 */
  void Set_mesh_rebuild()    { d2a_mesh_state |= D2A_REBUILD_MESH ; }
/**
 * Access functions for mesh rebuild state.
 */
   void Clear_mesh_rebuild()  { d2a_mesh_state &= ~D2A_UPDATE_MESH ; }
/**
 * Access functions for mesh rebuild state.
 */
   void Clear_mesh_state()    { d2a_mesh_state = 0 ; }

  // ATTRIB_DM2ACIS multi-surface management
/**
 * Connects quad faces into a quilt, returning the tag for the new root sibling or -1 if none.
 * <br><br>
 * @param face1
 * in : face = Face to connect to hierarchy.
 * @param tag
 * out: tag_shift = amount face tag numbers have shifted,new_tag_id = old_tag_id + tag_shift.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  int                      // rtn: tag for new root sibling or -1 for none
   Add_multi_face          // eff: Connect quad faces into a quilt
    (FACE *face1,               // in : face = Face to connect to hierarchy
      int &tag)               // out: tag_shift = amount face tag numbers have shifted
                           //      new_tag_id = old_tag_id + tag_shift
                           // note: sets active patch
                           {
								SPAUNUSED(face1)
								SPAUNUSED(tag)
								DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
								return (-1) ;
                           }

  // ATTRIB_DM2ACIS curve tracking management
/**
 * Builds curve dmod.
 * <br><br>
 * @param tag
 * for this crv or link_cstrn.
 * @param tgt
 * for link_cstrns, tgt= 1 or 2.
 */
   int                      // rtn: tag for new dmod or -1 for none
   Make_tracking_curve     // eff: build curve dmod
    (int tag,              // in : for this crv or link_cstrn
     int tgt=1) ;          // in : for link_cstrns, tgt= 1 or 2

/**
 * Changes a pfunc target for tracking curve.
 * <br><br>
 * @param tag
 * for this crv or link_cstrn.
 * @param tgt
 * for link_cstrns, tgt= 1 or 2.
 * @param target
 * image space target curve.
 * @param behavior
 * one of DM_POSITN, DM_TANG, DM_CURVATURE.
 */
 void
 Set_tracking_curve_target  // eff: change pfunc target for tracking curve
  (int tag,                // in : for this crv or link_cstrn
   int tgt,                // in : for link_cstrns, tgt= 1 or 2
   bs3_curve target,       // in : image space target curve
   int behavior);          // in : one of DM_POSITN, DM_TANG, DM_CURVATURE


  // ATTRIB_DM2ACIS patch hierarchy management
/**
 * Creates and adds a patch to the patch hierarchy.
 * <br><br>
 * <b>Role:</b> Returns the tag for the new patch dmod or -1 for none. This should always
 * be called between <tt>API_BEGIN</tt> and <tt>API_END</tt> and between <tt>Set_editing</tt> and <tt>Clear_editing</tt>.
 * <br><br>
 * @param shape_flag
 * 0=use seams, 1=rectangle, 2=ellipse.
 * @param pt1
 * rect=min-corner,ellipse=center-pt.
 * @param pt2
 * rect=max-corner,ellipse=1st-axis-end-pt.
 * @param pt3
 * rect=not_used,  ellipse=2nd-axis-end-pt.
 * @param refinement
 * parent_knot_spacing/child_knot_spacing.
 * @param seam_count
 * number of curves in seam (may be 0).
 * @param seam
 * seam crvs (NULL=add a square (min/max) seam).
 * @param tag_flag
 * 1 =  specify parent tgt_dmod to get new patch 1 = active dmod,2= root dmod,else = member dmod with tag==tag_flag.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  int                      // rtn: tag for new patch dmod or -1 for none
   Add_patch               // eff: make and add a patch to patch hierarchy
   (int shape_flag,                   // in : shape_flag = 0=use seams, 1=rectangle, 2=ellipse
    SPApar_pos pt1,               // in : pt1 = rect=min-corner,ellipse=center-pt
    SPApar_pos pt2,               // in : pt2 = rect=max-corner,ellipse=1st-axis-end-pt
    SPApar_pos pt3,               // in : pt3 = rect=not_used,  ellipse=2nd-axis-end-pt
    int refinement,                   // in : refinement = parent_knot_spacing/child_knot_spacing
    int seam_count,                   // in : seam_count = number of curves in seam (may be 0)
    DS_pfunc **seam,           // in : seam = seam crvs (NULL=add a square (min/max) seam)
    int tag_flag)                   // in : tag_flag=1 =  specify parent tgt_dmod to get new patch
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // NOTE: always call between API_BEGIN/API_END
                           //       and between Set_editing()/Clear_editing()
                           {
								SPAUNUSED(shape_flag)
								SPAUNUSED(pt1)
								SPAUNUSED(pt2)
								SPAUNUSED(pt3)
								SPAUNUSED(refinement)
								SPAUNUSED(seam_count)
								SPAUNUSED(seam)
								SPAUNUSED(tag_flag)
								DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
								return (-1) ;
                           }
/**
 * Removes and deletes a patch from the hierarchy.
 * <br><br>
 * <b>Role:</b> This should always be called between <tt>API_BEGIN</tt> and <tt>API_END</tt>.
 * <br><br>
 * @param tag_flag
 * specify the tgt_dmod to be copied.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  void Rm_patch            // eff: remove and delete patch from hierarchy
   (int tag_flag )                   // in : tag_flag=1 = specify the tgt_dmod to be copied
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: deletes all offspring as well.
                           // NOTE: always call between API_BEGIN/API_END
                           //       and between Set_editing()/Clear_editing()
                           {
								SPAUNUSED(tag_flag)
								DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
                           }
/**
 * Changes the entity pointer for a <tt>DS_dmod</tt>.
 * <br><br>
 * @param from_entity
 * old entity ptr.
 * @param in_entity
 * new entity ptr.
 */
   void Change_entity       // eff: change the entity ptr for a DS_dmod
    (ENTITY *from_entity,  // in : old entity ptr value
     ENTITY *in_entity) ;  // in : new entity ptr value
/**
 * Returns a tag for input <tt>tag_flag</tt>.
 * <br><br>
 * @param tag_flag
 * tag_flag to parse.
 */
   int Tag_from_tag_flag    // rtn: tag for input tag_flag
    (int tag_flag=1) ;     // in : tag_flag to parse
/**
 * Gets the parent tag id, or -1 if none.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   int Parent_tag           // rtn: parent tag id or -1 for none
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Gets the parent dmod pointer, or <tt>NULL</tt>.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   DS_dmod *Parent          // rtn: parent dmod ptr or NULL
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Gets the child tag id, or -1 if none.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   int Child_tag           // rtn: child tag id or -1 for none
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Gets the child <tt>dmod</tt> pointer, or <tt>NULL</tt>.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   DS_dmod *Child           // rtn: child dmod ptr or NULL
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Gets the sibling tag, or -1 if none.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   int Sibling_tag          // rtn: sibling tag id or -1 for none
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Gets the sibling dmod pointer or <tt>NULL</tt>.
 * <br><br>
 * @param tag_flag
 * specify target dmod.
 */
   DS_dmod *Sibling         // rtn: sibling dmod prt or NULL
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag

  // ATTRIB_DM2ACIS simple data access functions
/**
 * Returns the commit state.
 */
  int
  Get_commited()           // return commited state
                           { return d2a_becommited; }
/**
 * Sets the commit state to <tt>TRUE</tt>.
 */
  void
  Set_commited()           // set commited state to TRUE
                           { d2a_becommited = 1; }
/**
 * Sets the commit state to <tt>FALSE</tt>.
 */
  void
  Set_uncommited()         // set commited state to FALSE
                           { d2a_becommited = 0; }
/**
 * Returns the root <tt>DS_dmod</tt> pointer.
 */
   DS_dmod *Dmod()          // rtn: root DS_dmod pointer
                           { return d2a_dmod ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   void                     // eff: should not be used except by
   Set_dmod                //      internal DM kernel functions
   (DS_dmod *dmod)         // in : new value for d2a_dmod
                           // note: make sure to delete current
                           //       d2a_dmod before making this call
                           { d2a_dmod = dmod ; }

  // DS_dmod iterator
/**
 * Returns a sequence of dmods for recursion, or <tt>NULL</tt>.
 * <br><br>
 * @param tgt_dmod
 * dmod for current recursion level.
 * @param walk_flag
 * specify how deep to go.
 * @param next_cstrn
 * cstrn to start src_W_dmod search.
 * @param dmod_flag
 * dmod to start child,sibling search.
 */

  DS_dmod *Next_dmod       // rtn: sequence of dmods for recursion or NULL
   (DS_dmod *tgt_dmod,     // in : dmod for current recursion level
    int walk_flag,         // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           //      2=this,sibling, and offspring
    DS_cstrn *&next_cstrn, // i/o: cstrn to start src_W_dmod search
                           //      Set to NULL to skip src_W_dmod search
    int &dmod_flag) ;      // i/o: dmod to start child,sibling search
                           //      1=rtn sibling next if appropriate
                           //      2=rtn child next if appropriate
                           //      0=all done (rtn NULL on next call)

/**
 * Returns the active member <tt>DS_dmod</tt> pointer.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the
 * corresponding Standalone Deformable Modeling Component interface function
 * @href DM_get_active_patch.
 */
  DS_dmod *Active() const ;// rtn: Active member DS_dmod pointer
/**
 * Returns the tag for active <tt>DS_dmod</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the
 * corresponding Standalone Deformable Modeling Component interface function
 * @href DM_get_active_patch and @href DM_get_dmod_tag.
 */
   int                      // rtn: tag for active DS_dmod
   Active_tag() ;          // eff: if active, rtn tag, else rtn
                           //      DS_NO_ACTIVE_DMOD
/**
 * Returns the patch that contains the given tag or -1, and makes the returned <tt>dmod</tt> the <tt>active_dmod</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the
 * corresponding Standalone Deformable Modeling Component interface function @href DM_find_patch_by_tag.
 * <br><br>
 * @param tag
 * tag id for object to be classified.
 */
   int Find_patch_by_tag    // rtn: tag for patch that contains tag or -1
    (int tag) ;            // in : tag id for object to be classified
                           // note: make returned dmod the active_dmod
/**
 * Makes the child <tt>DS_dmod</tt> with tag active.
 * <br><br>
 * <b>Role:</b> An error is returned when tag is not in <tt>DS_dmod</tt> hierarchy.
 * <br><br>
 * @param tag
 * unique id for target child DS_dmod.
 */
   void Make_active         // eff: make the child DS_dmod with tag active
    (int tag) ;            // in : unique id for target child DS_dmod
                           // err: when tag is not in DS_dmod hierarchy

#ifndef DS_USE_6x_RENDERING

#else
/*
// tbrv
*/
/**
 * @nodoc
 */
  deformable_figure *      // rtn: ptr to the tags deformable figure
   Tags_figure()           { return d2a_tags_fig ; }
#endif

#ifndef DS_USE_6x_RENDERING
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
  deformable_figure *      // rtn: ptr to the tags deformable figure
   Shape_figure()          { return d2a_shape_fig ; }
#endif /*DS_USE_6x_RENDERING*/
/**
 * Returns a pointer to the ACIS representation surface object for debugging purposes.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod.
 */
  surface *Tmp_surf        // rtn: ptr to ACIS rep of tgt_dmod shape or NULL
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag

/**
 * Returns a pointer to the deformable surface's surface object for debugging purposes.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod.
 */
   curve *Tmp_curv          // rtn: ptr to ACIS rep of tgt_dmod shape or NULL
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Returns a 1 if from a solid or 0 if from a sheet or an edge.
 * <br><br>
 * <b>Role:</b> Searches <tt>d2a_dmod</tt> for type <tt>ds_solid_cstrn crv_cstrns</tt>.
 */
   int                      // rtn: 1=from solid,0=from sheet or an edge
   Is_shape_from_solid() ; // eff: srch d2a_dmod for type ds_solid_cstrn
                           //      crv_cstrns.
/**
 * Searches for <tt>dmod</tt> by tag or uses the active <tt>dmod</tt>.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod.
 */
   int                      // rtn: draw_state value for target DS_dmod
   Draw_state              // eff: search for dmod by tag or use active dmod
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the deformable surface's drawing state.
 * <br><br>
 * <b>Role:</b> <tt>draw_state</tt> is a bit array. When a bit is on, the associated data will
 * be displayed. When a bit is off, the data is not displayed. <tt>draw_state</tt> is a bit array
 * (logical OR) of the following:
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_draw_state.
 * <br><br>
 * @param draw_state
 * drawing state.
 * @param tag_flag
 * specify tgt dmod.
 */
   void Set_draw_state      // set: target member's draw_state
    (int draw_state,       // in : orof the DM_DRAW_... bit constants
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Sets the appearance of comb plots.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the
 * corresponding Standalone Deformable Modeling Component interface function
 * @href DM_set_comb_graphics.
 * <br><br>
 * @param elem_pt_count
 * number of vecs shown per elem.
 * @param gain
 * scaling applied to comb vecs.
 * @param tag_flag
 * specify tgt dmod and how deep to go.
 */
   void Set_comb_graphics   // set: active member's comb plot appearance
    (int elem_pt_count,    // in : number of vecs shown per elem
     double gain,          // in : scaling applied to comb vecs
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns current comb graphic's values.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the
 * corresponding Standalone Deformable Modeling Component interface function
 * @href DM_get_comb_graphics.
 * <br><br>
 * @param elem_pt_count
 * number of vecs shown per elem.
 * @param gain
 * scaling applied to comb vecs.
 * @param tag_flag
 * specify tgt dmod and how deep to go.
 */
   void Get_comb_graphics   // get: active member's comb graphic's vals
    (int &elem_pt_count,   // out: number of vecs shown per elem
     double &gain,         // out: scaling applied to comb vecs
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Returns the degree of the polynomials accurately integrated when making deformable and constraint equations.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_integral_degree.
 * <br><br>
 * @param integral_degree
 * accuracy of integration.
 * @param tag_flag
 * specify tgt dmod.
 */
   void Get_integral_degree // get: polynomial degree accurately integrated
                           //      in deformable and constraint equations
    (int &integral_degree, // out: the accuracy of integration
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Returns the target's domain dimension: 1 is for a 1d curve; 2 is for a 2d surface.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_domain_dim.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod.
 */
   int                      // rtn: target's domain dimension
   Domain_dim              //      1=1d curve, 2=2d surface
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
#ifndef DS_USE_6x_RENDERING
#else
  // ATTRIB_DM2ACIS rendering colors
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Surf_color()           // rtn: surface rendering color
                                   { return d2a_surfC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Cpt_color()            // rtn: control point rendering color
                                   { return d2a_cptC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Cstrn_position_color() // rtn: SPAposition cstrn rendering color
                                   { return d2a_positionC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Cstrn_tangent_color()  // rtn: tangent cstrn rendering color
                                   { return d2a_tangentC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Cstrn_curvatur3_color()// rtn: curvature cstrn rendering color
                                   { return d2a_curvatureC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Cstrn_off_color()      // rtn: disabled cstrn rendering color
                                   { return d2a_cstrn_offC ; }
/*
// tbrv
*/
/**
 * @nodoc
 */
   double* Load_color()           // rtn: load rendering color
                                   { return d2a_loadC ; }
#endif
  // ATTRIB_DM2ACIS pointer chasing functions
/**
 * Returns a <tt>FACE</tt> or <tt>EDGE</tt> pointer to the owner root entity of this attribute.
 */
  ENTITY *Root_owner () ;   // rtn: FACE/EDGE owner ptr for hierarchy root

/**
 * Returns the owner pointer for target <tt>dmod DS_group</tt>.
 * <br><br>
 * @param dmod
 * target dmod.
 */
  ENTITY *Base_owner       // rtn: owner ptr for tgt_dmod DS_group's base
   (DS_dmod *dmod) ;       // in : the target dmod
/**
 * Gets the <tt>attrib_dsgroup</tt> for this hierarchy.
 */
   ATTRIB_DSGROUP *         // rtn: attrib_dsgroup for this hierarchy
   Attrib_dsgroup() ;      // eff: depends on the owner being
                           //      of type ATTRIB_DSGROUP.

  // ATTRIB_DM2ACIS glue functions
/**
 * Forces all deformable surface data to be recreated from the data stored in the ACIS geometry and attribute structures.
 * <br><br>
 * <b>Role:</b> The <tt>ATTRIB_DS2ACIS</tt> object will be reset to the state at the time of the
 * last call to <tt>Commit_to_face</tt> or to the starting default state if <tt>Commit_to_face</tt> has never
 * been called. Used to re-synchronize with ACIS modeler.
 * <br><br>
 * @param pt_icon_rad
 * size of pt icon radius.
 */
  void                     // use: to re-sync DS with ACIS modelers
   Refresh_data            // eff: get tmp data from ACIS persistent data
  (float pt_icon_rad= -1.0); // in : size of pt icon radius (-1=leave as is)
/**
 * Moves data to face and <tt>ATTRIB_DSMODEL</tt>.
 */
    virtual                 // bas: ATTRIB_DM2ACIS virtual function
    void                   // use: called by api_commit_attrib_ds2acis_to_face()
     Commit_to_owner()     // eff: move data to face and ATTRIB_DSMODEL
                           { DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
                           }
/**
 * Updates the <tt>tmp_shape</tt> from <tt>DS_dmod</tt> data.
 * <br><br>
 * <b>Role:</b> Called after Solve and <tt>Update_tmp_shape</tt>.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go.
 */
    void                    // use: call after Solve()
    Update_tmp_shape       // eff: Update the tmp_shape from DS_dmod data
    (int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)

                           // use: call after Solve(),Update_tmp_shape()
#ifndef DS_USE_6x_RENDERING
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
  void                    // eff: recomputes the vertex positions
    Update_tags_figure     //      of d2a_tags_fig from values in
     (float icon_radius) ; // in : size of pt icon radius display

                           // use: call after Solve(),Update_tmp_shape()
/*
// tbrv
*/
/**
 * @nodoc
 */
  void                    // eff: recomputes the rendering figure
    Update_shape_figure    //      for the shape data stored in
     () ;                  //      each derived class.
#endif

#ifndef DS_USE_6x_RENDERING
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
    void                    // use: call after Update_shape_figure()
    Repaint                //      and/or Update_tags_figure()
     () ;                  // eff: passes the rw context a
                           //      refresh_all
#endif
  // ATTRIB_DM2ACIS action functions
                           // use: after any changes in dmod object
/**
 * Solves for a new deformed shape using the current values for surface parameters, loads, and constraints.
 * <br><br>
 * <b>Role:</b> Every time the deformable surface state is modified by changing tag objects
 * or surface parameters, use this method to find a new position. When dynamic effects are
 * large, multiple solves will produce an animation effect. If this is not desired, call
 * <tt>Solve_dmod</tt> with a large iteration count. <tt>Solve_dmod</tt> will integrate for iteration time
 * steps before returning. No graphics of the intermediate time steps are shown. To save
 * computation time, increase delta time, reduce or eliminate mass, and choose a modest
 * value for damping. See <tt>Set_integral_data</tt>. This will cause the system to move from one
 * equilibrium position to the next without dynamic effects.
 * <br><br>
 * Use this after any changes to <tt>dmod</tt> object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_solve.
 * <br><br>
 * @param iter_count
 * in: number of time-steps.
 * @param tag_flag
 * in: specify tgt dmod, 1 for active dmod and offspring, 2 for root dmod and offspring else member dmod with tag==tag_flag.
 */
  void                     // eff: compute a dmod solution & change
   Solve_dmod              //      d2a_surface control-point pos's
    (int iter_count,       // in : number of time-steps or -1 to converge
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod and offspring
                           //      2   = root   dmod and offspring
                           //     else = member dmod with tag==tag_flag
                           //                        and offspring
/**
 * Splits <tt>dmod</tt>'s domain description, making more elements per <tt>dmod</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_split_dmod.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param split_pt_count
 * number of split locations.
 * @param split_pts
 * locations at which to split domain.
 * @param tag_flag
 * in: specify tgt dmod, 1 for active dmod and offspring, 2 for root dmod and offspring else member dmod with tag==tag_flag.
 */
   void                     // eff: Splits dmod's domain description
   Split_dmod              //      making more elements per dmod.
    (int domain_dim,       // in : dmod domain_dim (for error checking)
     int split_pt_count,   // in : number of split locations
     double *split_pts,    // in : locs at which to split domain
                           //      Sized:[pt_count*domain_dim]
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod and offspring
                           //      2   = root   dmod and offspring
                           //     else = member dmod with tag==tag_flag
                           //                        and offspring
                           // warning: multiple pt splits with split_flag == 0
                           //      yield difficult to predict splits.  Splitting
                           //      a surface at an existing internal knot
                           //      decreases the surface's internal continuity
                           //      across that knot.
/**
 * Increases the deformable model's polynomial degree, adding more degrees of freedom per element.
 * <br><br>
 * <b>Role:</b> Valid values for <tt>tag_flag</tt> are:
 * <br><br>
 * 1 = active model and offspring <br>
 * 2 = root model and offspring <br>
 * else = member model with <tt>tag==tag_flag</tt> and offspring
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go.
 * @param new_degree_u
 * new degree in 1st domain dim.
 * @param new_degree_v
 * new degree in 2nd domain dim.
 */
 void                       // eff: Increase dmod's polynomial degree
Set_dmod_degree            // adding more dofs per element
  (int tag_flag,           // in : specify tgt dmod and how deep to go
                           //      1   = active dmod and offspring
                           //      2   = root   dmod and offspring
                           //     else = member dmod with tag==tag_flag
                           //                        and offspring
   int new_degree_u,       // in : new degree in 1st domain dim
   int new_degree_v) ;     // in : new degree in 2nd domain dim
/**
 * Increases the deformable model's polynomial degree, adding more degrees of freedom per element.
 * <br><br>
 * <b>Role:</b> Valid values for <tt>tag_flag</tt> are:
 * <br><br>
 * 1 = active model and offspring <br>
 * 2 = root model and offspring <br>
 * else = member model with <tt>tag==tag_flag</tt> and offspring
 * <br><br>
 * The <tt>new_degree</tt> argument is applied to all domain dimensions.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go.
 * @param new_degree
 * new degree; applied to all domain dim's.
 */
 void                       // eff: Increase dmod's polynomial degree
Set_dmod_degree // adding more dofs per element
  (int tag_flag,           // in : specify tgt dmod and how deep to go
                           //      1   = active dmod and offspring
                           //      2   = root   dmod and offspring
                           //     else = member dmod with tag==tag_flag
                           //                        and offspring
   int new_degree) ;       // in : new degree; applied to all domain dim's
/**
 * Increases the <tt>dmod</tt>'s polynomial degree by adding more degrees of freedom per element.
 * <br><br>
 * <b>Role:</b> This preserves the continuity and allows the new shape to form exactly the old.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_elevate_dmod_degree.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod and offspring, 2 = root dmod and offspring else = member dmod with tag==tag_flag and offspring.
 * @param cont_flag
 * 0 preserves current elem continuity and 1 increments elem continuity.
 */
   void                     // eff: Increase dmod's polynomial degree
   Elevate_dmod_degree     //      adding more dofs per element.
   (int tag_flag=1,        // in : specify tgt dmod and how deep to go
                           //      1   = active dmod and offspring
                           //      2   = root   dmod and offspring
                           //     else = member dmod with tag==tag_flag
                           //                        and offspring
   int cont_flag=0) ;      // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old
/**
 * Increases the <tt>dmod</tt>'s domain size by 5% across each edge.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_extrapolate_dmod.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void                     // eff: Increse the dmod's domain size
   Extrapolate_dmod        //      by 5% across each edge.
    (int tag_flag) ;       // in : specify tgt dmod and how deep to go
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
 /**
  * Outputs a <tt>dmod</tt> and related data summary.
  * <br><br>
  * <b>Role:</b> Derived functions must call the parent. For information purposes only, refer to the description of the corresponding
  * Standalone Deformable Modeling Component interface function @href DM_print_dmod.
  * <br><br>
  * @param tag_flag
  * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
  */
   void
   Print_dmod              // eff: output a dmod and related data summary
    (int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
                           // note: Derived funtions must call the parent.

  // ATTRIB_DM2ACIS modeling functions
  //    We have an Add_, a Rm_, and a group of Set_ functions for
  //    each load and constraint type known to the deformable model.
  //
  //    Changes made by these functions (and Solve_dmod() ) are
  //    recorded only in the DS_dmod structure and may have
  //    cascading affects into the shape of the tmp_surf and the
  //    state of the rendering_figures.  These changes are not
  //    placed on the
  //    buletin board and may not be undone.  When desired, the
  //    function Commit_to_FACE() is used to write the accumulated
  //    changes caused by a sequence of Add_, Rm_, Set_, and
  //    Solve_dmod() calls to the FACE.  The changes made by
  //    Commit_to_FACE() are written to the bulletin board
  //    and may be undone with roll-back.
  //
  //    Each load and constraint is associated with a unique
  //    identifier tag which is returned whenever a load or
  //    constraint is created or selected by some other criterion.
  //    Identifier tags are used as input arguments in the Rm_ and
  //    Set_ functions.

  // ATTRIB_DM2ACIS set the def-model parameters
/**
 * Sets the deformable surface's stretch resistance parameters.
 * <br><br>
 * @param domain_dim
 * 1=1d curve, 2=2d surface.
 * @param alpha
 * 1d=[alpha], 2d=[au,av,theta(degrees)].
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void Set_alpha           // set: d2a_dmod resistance to stretch
    (int domain_dim,       // in : 1=1d curve, 2=2d surface
     double *alpha,        // in : 1d=[alpha], 2d=[au,av,theta(degrees)]
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns <tt>d2a_dmod</tt> resistance to stretch.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_alpha.
 * <br><br>
 * @param alpha
 * 1d=[alpha], 2d=[au,av,theta(degrees)].
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_alpha           // get: d2a_dmod resistance to stretch
    (double *alpha,        // out: 1d=[alpha], 2d=[au,av,theta(degrees)]
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the deformable surface's bending resistance parameters.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_beta.
 * <br><br>
 * @param domain_dim
 * 1=1d curve, 2=2d surface.
 * @param beta
 * 1d=[beta],2d=[bu,bv,theta(degrees)].
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void Set_beta            // set: d2a_dmod resistance to bending
    (int domain_dim,       // in : 1=1d curve, 2=2d surface
     double *beta,         // in : 1d=[beta], 2d=[bu,bv,theta(degrees)]
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns <tt>d2a_dmod</tt> resistance to bending.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_beta.
 * <br><br>
 * @param domain_dim
 * 1=1d curve, 2=2d surface.
 * @param beta
 * 1d=[beta],2d=[bu,bv,theta(degrees)].
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_beta            // get: d2a_dmod resistance to bending
    (int domain_dim,       // in : 1=1d curve, 2=2d surface
     double *beta,         // out: 1d=[beta], 2d=[bu,bv,theta(degrees)]
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Epsilon regulates a shape fairing (energy minimization) term that is used to dampen control point oscillations in high degree splines (degree>8).
 * <br><br>
 * <b>Role:</b> Like the primary fairing terms, alpha and beta, epsilon should be 0 or positive.
 * Epsilon is considered a supplement to the alpha and beta shape fairing terms, and should be
 * relatively small compared to beta, the chief shape fairing term.
 * <br><br>
 * @param eps
 * epsilon.
 * @param tag
 * tag object identifier.
 */
   void Set_epsilon         //
     (double eps,          // in : epsilon
     int tag) ;            // in : tag of dmod to set
/**
 * Epsilon regulates a shape fairing (energy minimization) term that is used to dampen control point oscillations in high degree splines (degree>8).
 * <br><br>
 * <b>Role:</b> Like the primary fairing terms, alpha and beta, epsilon should be 0 or positive.
 * Epsilon is considered a supplement to the alpha and beta shape fairing terms, and should be
 * relatively small compared to beta, the chief shape fairing term.
 * <br><br>
 * @param eps
 * epsilon.
 * @param tag
 * tag object identifier.
 */
   void Get_epsilon         //
    (double& eps,          // out: epsilon
     int tag)const ;       // in : tag of dmod to get
/*
// tbrv
*/
/**
 * @nodoc
 */
   void Set_area_load_target
     (int tag,
      bs3_surface bs3);
/**
 * Sets the deformable surface's resistance to deviating from the default shape.
 * <br><br>
 * <b>Role:</b> Setting this parameter to 0.0 turns off an effective spring that pulls the
 * deformable surface back to the default shape. Use the <tt>Set_default_shape</tt> method to turn off
 * the default shape behavior.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_delta.
 * <br><br>
 * @param delta
 * resist move from default shape val.
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void Set_delta           //      from default shape
    (double delta,         // in : resist move from default shape val
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns the <tt>d2a_dmod</tt> resistance to bending.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_gamma.
 * <br><br>
 * @param gamma
 * resist bending change rate value.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_gamma           // get: d2a_dmod resistance to displacement
    (double &gamma,        // out: resist bending change rate value
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the deformable surface's resistance to bending from the default shape.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_gamma.
 * <br><br>
 * @param gamma
 * resist bending change rate value.
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void Set_gamma           //      from default shape
    (double gamma,         // in : resist bending change rate value
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns the <tt>d2a_dmod</tt> resistance to displacement.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_delta.
 * <br><br>
 * @param d
 * displacement resistance value.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_delta           // get: d2a_dmod resistance to displacement
    (double &d,            // out: displacement resistance value
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets default shape state to flag value.
 * <br><br>
 * <b>Role:</b> When <tt>flag = 1</tt>, the current deformable shape is copied into the default shape.
 * From then on, the deformable surface will seek to resume the default shape
 * when not acted upon by constraints and loads. When <tt>flag = 0</tt>, the default shape is cleared
 * allowing the surface to seek the fairest shape possible for the current set of constraints
 * and loads.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_default_shape.
 * <br><br>
 * @param shape_flag
 * 0=no shape, 1=save current_shape.
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
  void Set_default_shape    // set: default shape state to flag value
    (int shape_flag,       // in : 0=no shape, 1=save current_shape
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns the default shape state.
 * <br><br>
 * <b>Role:</b> 0 is no default shape; 1 default shape in use.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_default_shape.
 * <br><br>
 * @param shape_flag
 * 0=no default shape,1=default shape in use.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
  void Get_default_state    // get: default shape state
    (int &shape_flag,      // out: 0=no default shape,1=default shape in use
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the deformable surface's interior state value.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_interior_state.
 * <br><br>
 * @param interior_state
 *  0 = allow C0 between elements,1 = enforce C1 between elements.
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void
   Set_interior_state      // set: tgt dmod's interior state value
    (int interior_state,   // in : 0 = allow C0 between elements
                           //      1 = enforce C1 between elements
     int tag_flag) ;       // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns the target model's interior state value.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_integral_degree.
 * <br><br>
 * @param interior_state
 *  0 = allow C0 between elements,1 = enforce C1 between elements.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
  void
   Get_interior_state      // get: tgt dmod's interior state
    (int &interior_state,  // in : 0 = allow C0 between elements
                           //      1 = enforce C1 between elements
     int tag_flag) ;       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the d2a_dmod <tt>dyn_load</tt> parameter values.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_dynamics.
 * <br><br>
 * @param dt
 * size of time steps between solves.
 * @param mass
 * amount of mass added at each dof.
 * @param damp
 * amount of damp added at each dof.
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void Set_dynamics        // set: d2a_dmod dyn_load effects
    (double dt,            // in : size of time steps between solves
     double mass,          // in : amount of mass added at each dof
     double damp,          // in : amount of damp added at each dof
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
/**
 * Returns the <tt>d2a_dmod dyn_load</tt> parameter values.
 * <br><br>
 * @param dt
 * size of time steps between solves.
 * @param mass
 * amount of mass added at each dof.
 * @param damp
 * amount of damp added at each dof.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_dynamics        // get: d2a_dmod dyn_load param values
    (double &dt,           // in : size of time steps between solves
     double &mass,         // in : amount of mass added at each dof
     double &damp,         // in : amount of damp added at each dof
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Returns the degree of freedom of the given <tt>dmod</tt> state.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_dof_state.
 * <br><br>
 * @param dof_count
 * total surface dof count.
 * @param free_count
 * number of free dofs to deform.
 * @param cstrn_count
 * total number of constraint equations.
 * @param fixed_count
 * independent constraint equations cnt.
 * @param lambda_count
 * lambda constraint eqn count.
 * @param lfixed_count
 * independent lambda constraint count.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_dof_state       // eff: get dof counts for dmod
    (int &dof_count,       // out: total surface dof count
     int &free_count,      // out: number of free dofs to deform
     int &cstrn_count,     // out: total number of constraint equations
     int &fixed_count,     // out: independent constraint equations cnt
     int &lambda_count,    // out: lambda constraint eqn count
     int &lfixed_count,    // out: independent lambda constraint count
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Returns the number elements for the deformable model.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_elem_count.
 * <br><br>
 * @param elem_count
 * total number of elems in dmod.
 * @param base_count
 * number of elems in each basis.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   int                      // rtn: domain dimension
    Get_elem_count         // get: element state for def model
    (int &elem_count,      // out: total number of elems in dmod
     int *base_count,      // out: number of elems in each basis
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag

// TODO:??? this function not yet implemented
/**
 * This method is not yet implemented.
 * <br><br>
 * @param elem_u_count
 * u dir elem-count.
 * @param elem_v_count
 * v dir elem-count.
 * @param tol
 * pecifies tol requirement to satisfy.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
  int                      // rtn: 1=success,0=no change
   Set_elem_count          // eff: change pfunc elem-mesh
    (int elem_u_count, //elem_u_count,     // in : u dir elem-count
     int elem_v_count, //elem_v_count=-1,  // in : v dir elem-count (not used by curves)
     int tol, //fit_flag=0,       // in : specifies tol requirement to satisfy
          //                  //      0 = shapes match to SPAresabs everywhere
          //                  //      1 = shapes match to SPAresabs at cstrns
          //                  //      2 = shapes match to best tol possible
     int tag_flag); //tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: offspring are modified as well
/**
 * Returns the basis polynomial degree, 1d:[u_dir, not_used]. 2d:[u_dir, v_dir].
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_dmod_degree.
 * <br><br>
 * @param degrees
 * 1d:[u_dir,not_used],2d:[u_dir,v_dir].
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void Get_degree          // eff: get basis polynomial degrees
    (int degrees[2],       // out: 1d:[u_dir,not_used],2d:[u_dir,v_dir]
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * For information purposes only.
 * <br><br>
 * <b>Role:</b> Refer to the description of the corresponding Standalone Deformable Modeling
 * Component interface function @href DM_get_pfunc_dofs.
 * <br><br>
 * @param dof_count
 * number of degrees-of-freedom.
 * @param image_dim
 * number of coordinates in each dof.
 * @param dof_count_u
 * u_dir dof count.
 * @param dof_count_v
 * v_dir dof count.
 * @param dof_vec
 * ptr to degrees-of-freedom loc array.
 * @param dof_def
 * ptr to default dof loc array.
 * @param weights
 * ptr to weight array (NURBS only).
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   int                      // rtn: 0=success -1=inputs not NULL on entry
                           // eff: for NURB and NUB deformable models
   Get_dofs                //      gather list of dof values
    (int &dof_count,       // out: number of degrees-of-freedom
     int &image_dim,       // out: number of coordinates in each dof
     int &dof_count_u,     // out: u_dir dof count
     int &dof_count_v,     // out: v_dir dof count
     double *&dof_vec,     // out: ptr to degrees-of-freedom loc array
     double *&dof_def,     // out: ptr to default dof loc array
     double *&weights,     // out: ptr to weight array (NURBS only)
     int tag_flag) ;       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // mallocs: No - do not call free on
                           //          u_knots or v_knots
/**
 * Returns the degree of the polynomials accurately integrated when making deformable and constraint equations.
 * <br><br>
 * <b>Role:</b> In case of mallocs: do not call <tt>free</tt> on <tt>u_knots</tt> or <tt>v_knots</tt>.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_get_pfunc_knots.
 * <br><br>
 * @param u_knot_count
 * number of unique u_knot values.
 * @param v_knot_count
 * number of unique v_knot values.
 * @param u_knots
 * ptr to u_knot array.
 * @param u_knot_index
 * ptr to u_knot_index array.
 * @param v_knots
 * ptr to v_knot array.
 * @param v_knot_index
 * ptr to v_knot_index array.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   int                      // rtn: 0=success -1=inputs not NULL on entry
                           // eff: for NURB and NUB deformable models
   Get_knots               //      gather list of knot values
    (int &u_knot_count,    // out: number of unique u_knot values
     int &v_knot_count,    // out: number of unique v_knot values
     double *&u_knots,     // out: ptr to u_knot array
     int    *&u_knot_index,// out: ptr to u_knot_index array
     double *&v_knots,     // out: ptr to v_knot array
     int    *&v_knot_index,// out: ptr to v_knot_index array
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // mallocs: No - do not call free on
                           //          u_knots or v_knots
/**
 * Returns the number of surface figures render points.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_mesh_count.
 * <br><br>
 * @param grid
 * 1d:[u_dir,not used], 2d:[u_dir,v_dir].
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void
   Get_mesh_grid           // get: number of shape_fig render pts
    (int grid[2],          // out: 1d:[u_dir,not used], 2d:[u_dir,v_dir]
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Sets the number of surface figures render points.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_mesh_count.
 * <br><br>
 * @param grid
 * 1d:[nu,not_used],2d:[nu,nv].
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2 = and siblings and offspring), else = member dmod with tag==tag_flag (neg = and offspring).
 */
   void
   Set_mesh_grid           // set: pt_counts used for shape rendering
    (int grid[2],          // in : 1d:[nu,not_used],2d:[nu,nv]
     int tag_flag=1) ;     // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)

/*
// TODO: ??? Implement after creating a
//       pfunc->Set_ntgrl_degree() func which modifies all
//       the stored data in the pfunc
  void                     // set: d2a_dmod spatial integration
   Set_ntgrl_degree        //      precision
   (int ntgrl_degree) ;    // in : degree polynomial exactly ntgrtd
*/


  // ATTRIB_DM2ACIS add, rm, and set optional values for tags
/**
 * Adds a point constraint to the deformable surface at the domain location specified by <tt>dpt</tt> and returns a <tt>tag</tt> identifier.
 * <br><br>
 * <b>Role:</b> While the point constraint is enabled, all future deformations are guaranteed to interpolate
 * the point constraint's associated image point location. If <tt>tag</tt> is -1, the next <tt>tag</tt> number is assigned by
 * the system; otherwise, the specified tag number is used. The user must ensure that all tag numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone Deformable Modeling Component
 * interface function @href DM_add_pt_cstrn.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim.
 * @param dpt
 * domain_pt location of constraint.
 * @param ipt
 * cstrn pt location or NULL.
 * @param behavior
 * orof DM_POS_FIXED = pos constrained at pt, DM_TAN_FIXED = tan constrained at pt, DM_CURV_FIXED = curvature at pt.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
  int                      // rtn : tag identifier
   Add_pt_cstrn            // eff : add pt constraint to d2a_dmod
    (int domain_dim,       // in  : dmod domain_dim (for error checking)
     SPApar_pos &dpt,         // in  : domain_pt location of constraint
     SPAposition *ipt= NULL,  // in  : cstrn pt location or NULL
     int behavior =        // orof: SPA_DM_POS_FIXED  = pos constrained at pt
            SPA_DM_POS_FIXED,  //   SPA_DM_TAN_FIXED  = tan constrained at pt
                           //       SPA_DM_CURV_FIXED = curvature cstrn at pt
     int tag_flag  =1,     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag      =-1) ;   // in  : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Adds curve constraints along the parameter-space curve and returns a tag identifier.
 * <br><br>
 * <b>Role:</b> Each segment of the parameter-space curve is integrated to <tt>ntrgl_degree</tt> accuracy
 * to formulate the constraint equations. <tt>src_type</tt> identifies the origin of the curve constraint
 * which limits the system in allowing it to be deleted or disabled, as shown in the following
 * table. <tt>src_struct</tt> is an optional unused pointer saved for the application in case it needs to
 * associate this curve constraint with some other data object. ACIS uses this parameter to
 * associate constraint curves with topology edges within a solid model. If <tt>tag</tt> is -1, the next
 * tag number is assigned by the system; otherwise, the specified tag number is used. The user
 * must ensure that all tag numbers are unique.
 * <br><br>
 * Current ACIS <tt>crv_cstrn</tt> sources, privileges, and <tt>src_structs</tt>:
 * <pre>
 * DS_CRV_SRC       delete  stop    src_       comment
 * ds_solid_coedge    NO     NO    coedge   crv from solid model bndry
 *                                          any number of faces may
 *                                          connect to crv
 * ds_sheet_coedge    NO    YES    coedge   crv from sheet model
 *                                          bndry only face being
 *                                          deformed connects to crv
 * ds_user_curve     YES    YES     NULL    crv from user interface
 *                                          and not from topology
 *                                          boundaries</pre>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_add_crv_cstrn.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim.
 * @param pcur
 * uv domain curve description.
 * @param behavior
 * orof: DM_POS_FIXED (constrain dmod pos), DM_TAN_FIXED (constrain dmod tan), DM_CURV_FIXED (constrain dmod curv).
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_crv_cstrn           // eff: adds crv_cstrn along pcurve
    (int domain_dim,       // in : dmod domain_dim (for error checking)
     pcurve *pcur,         // in : uv domain curve description
                           //      NOT stored/deleted with crv_cstrn
     int behavior    =     // orof: SPA_DM_POS_FIXED  (constrain dmod pos)
            SPA_DM_POS_FIXED,  //   SPA_DM_TAN_FIXED  (constrain dmod tan)
                           //       SPA_DM_CURV_FIXED (constrain dmod curv)
     int tag_flag    =1,   // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag        =-1) ; // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Returns a tag identifier for the <tt>area_cstrn</tt> added to the model.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_add_area_cstrn.
 * <br><br>
 * @param domain_min
 * low bound if NULL load all domain.
 * @param domain_max
 * top bound if NULL load all domain.
 * @param zone_flag
 * 0 = area within the square moves compliment area is fixed, 1 = area with the square is fixed area compliment moves.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_area_cstrn          // eff: adds area_cstrn to dmod
    (SPApar_pos &domain_min,  // in : low bound if NULL load all domain
     SPApar_pos &domain_max,  // in : top bound if NULL load all domain
     int zone_flag = 0,    // in : 0=area within the square moves
                           //        compliment area is fixed
                           //      1=area with the square is fixed
                           //        area compliment moves
     int tag_flag =  1,    // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag      = -1) ;  // in : when == -1 assign next tag number
                           //      else use this tag number
/*
// tbrv
*/
/**
 * @nodoc
 */
 int
Add_area_C0_load // eff: adds area_cstrn to dmod
  (SPApar_pos const * corners,    // in : ordered list of corners of region
   int ncorners,    // in : number of corners of region
   int zone_flag,          // in : 0=area within the boundary moves
                           //        compliment area is fixed
                           //      1=area within the boundary is fixed
                           //        area compliment moves
   bs3_surface target,     // if null use current shape
   double gain,            // strength of the load
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int tag);               // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Searches the entire hierarchy for the <tt>cstrn</tt> that matches the tag identifier.
 * <br><br>
 * <b>Role:</b> Returns the tag of the patch containing <tt>cstrn</tt> or -1. Sets the <tt>active_dmod</tt>
 * to be the owner of the return object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_find_cstrn_by_tag.
 * <br><br>
 * @param tag
 * unique tag identifier to match.
 * @param patch_tag1
 * tag of patch containing cstrn or -1.
 * @param patch_tag2
 * linked patch or -1.
 */
   DS_cstrn *               // rtn: cstrn with matching tag value or NULL
   Find_cstrn_by_tag       // eff: search entire hierarchy for cstrn
    (int tag,              // in : unique tag identifier to match
     int &patch_tag1,      // out: tag of patch containing cstrn or -1
     int &patch_tag2) ;    // out: linked patch or -1
                           // note: sets active_dmod to owner of rtn object
/**
 * Sets the <tt>active_dmod</tt> to owner of the tag object.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_cstrn.
 * <br><br>
 * @param tag
 * tag id for constraint to query.
 * @param type_id
 * cstrn type id.
 * @param src_type
 * ds_solid_cstrn: from solid model, ds_bound_cstrn: from sheet model, ds_user_cstrn: from end user, ds_seam_cstrn: from patch hierarcy, ds_link_cstrn: for multi-surf.
 * @param patch1_tag
 * tag of patch containing tag or -1.
 * @param patch2_tag
 * link_cstrn 2nd patch id or -1.
 * @param shape1
 * shape type, sized.
 * @param shape2
 * link_cstrn 2nd shape type or "none".
 * @param behavior
 * or ofDM_POS_FREE DM_TAN_FREE DM_POS_FIXED DM_TAN_FIXED DM_POS_LINKED DM_TAN_LINKED DM_POS_2_FREE DM_TAN_2_FREE DM_POS_2_FIXED DM_TAN_2_FIXED DM_POS_2_LINKED DM_TAN_2_LINKED.
 * @param state
 * 0=disabled,1=enabled.
 * @param rights
 * or of DM_STOPABLE DM_DELETABLE.
 * @param src1_data
 * src_data stored with cstrn.
 * @param src2_data
 * link_cstrn 2nd src_data.
 * @param domain_flag
 * 0=return uv_pts in pfunc_space, 1=return uv_pts in unit_space..
 * @param uv1_pts_count
 * number of pts loaded into uv1_pts.
 * @param uv1_pts
 * defining locs for shapes.
 * @param uv2_pts_count
 * link_cstrn 2nd shape pt set.
 * @param uv2_pts
 * magnitude scaling for crv_cstrn tangent.
 * @param tang_gain
 * magnitude scaling for crv_cstrn tangent.
 * @param ntgrl_degree
 * crv_cstrn numerical integration accuracy.
 */
   int                      // rtn: 0=tag is cstrn in tgt_dmod else rtn -1
   Get_cstrn               // eff: load cstrn data into rtn args
  (int tag,                // in : tag id for constraint to query
   DS_TAGS &type_id,       // out: cstrn type id
   DS_CSTRN_SRC &src_type, // out: ds_solid_cstrn: from solid model
                           //      ds_bound_cstrn: from sheet model
                           //      ds_user_cstrn:  from end user
                           //      ds_seam_cstrn:  from patch hierarcy
                           //      ds_link_cstrn:  for multi-surf
   int &patch1_tag,        // out: tag of patch containing tag or -1
   int &patch2_tag,        // out: link_cstrn 2nd patch id or -1
   char *shape1,           // out: shape type,       sized:[20]
   char *shape2,           // out: link_cstrn 2nd shape type or "none"
   int &behavior,          // out: orof DM_POS_FREE     DM_TAN_FREE
                           //           DM_POS_FIXED    DM_TAN_FIXED
                           //           DM_POS_LINKED   DM_TAN_LINKED
                           //
                           //           DM_POS_2_FREE   DM_TAN_2_FREE
                           //           DM_POS_2_FIXED  DM_TAN_2_IXED
                           //           DM_POS_2_LINKED DM_TAN_2_LINKED
   int &state,             // out: 0=disabled,1=enabled
   int &rights,            // out: orof DM_STOPABLE,DM_DELETABLE
   void *&src1_data,       // out: src_data stored with cstrn
   void *&src2_data,       // out: link_cstrn 2nd src_data
   int domain_flag,        // in : 0=return uv_pts in pfunc_space,
                           //      1=return uv_pts in unit_space.
   int &uv1_pts_count,     // out: number of pts loaded into uv1_pts
   SPApar_pos uv1_pts[3],     // out: defining locs for shapes
   int &uv2_pts_count,     // out: number of pts loaded into uv2_pts
   SPApar_pos uv2_pts[3],     // out: link_cstrn 2nd shape pt set
   double &tang_gain,      // out: magnitude scaling for crv_cstrn tangent
   int &ntgrl_degree) ;    // out: crv_cstrn numerical integration accuracy
                           // note: sets active dmod
/**
 * Returns 0 when a change is allowed, else it returns 01.
 * <br><br>
 * <b>Role:</b> Changes the behavior of a <tt>cstrn</tt>. Sets the <tt>active_dmod</tt> to the owner of the tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_cstrn_behavior.
 * <br><br>
 * @param tag
 * cstrn identifier.
 * @param behavior
 * or of DM_POS_FREE DM_TAN_FREE DM_POS_FIXED DM_TAN_FIXED DM_POS_LINKED DM_TAN_LINKED DM_POS_2_FREE DM_TAN_2_FREE DM_POS_2_FIXED DM_TAN_2_FIXED DM_POS_2_LINKED DM_TAN_2_LINKED.
 */
   int                      // rtn: 0 when change allowed, else -1
   Set_cstrn_behavior      // eff: change behavior for a cstrn
    (int tag,              // in : cstrn identifier
     int behavior) ;       // in : orof DM_POS_FREE     DM_TAN_FREE
                           //           DM_POS_FIXED    DM_TAN_FIXED
                           //           DM_POS_LINKED   DM_TAN_LINKED
                           //
                           //           DM_POS_2_FREE   DM_TAN_2_FREE
                           //           DM_POS_2_FIXED  DM_TAN_2_IXED
                           //           DM_POS_2_LINKED DM_TAN_2_LINKED
                           // note: sets active_dmod to owner of tag object
/**
 * Returns 0 when a change is allowed, otherwise it returns -1.
 * <br><br>
 * <b>Role:</b> Changes the behavior of a <tt>cstrn</tt>. Sets the <tt>active_dmod</tt> to the owner of
 * the tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_cstrn_pttan_uv_dir.
 * <br><br>
 * @param tag
 * point cstrn identifier.
 * @param domain_dir
 * uv direction for directional derivative (tangent).
 * @param which_dir
 * one of: DM_TANG1_LEG, DM_TANG2_LEG, DM_CURV1_LEG, DM_CURV2_LEG.
 */
   int                      // rtn: 0 when change allowed, else -1
   Set_cstrn_domain_dir    // eff: change behavior for a cstrn
    (int tag,              // in : point cstrn identifier
     SPApar_vec & domain_dir, // in : uv direction for directional derivative (tangent)
     int which_dir) ;      // oneof: DM_TANG1_LEG
                           //        DM_TANG2_LEG
                           //        DM_CURV1_LEG
                           //        DM_CURV2_LEG
                           // note: sets active dmod
/**
 * Gets the tight state for the constraint.
 * <br><br>
 * @param tag
 * cstrn identifier.
 */
   int                      // ret: tight state
  Get_tight_state          // eff: gets tight state for cstrn
    (int tag);             // in : cstrn identifier
/**
 * Sets the tight state for the constraint.
 * <br><br>
 * @param tag
 * cstrn identifier.
 * @param state
 * 0=turn tight off, 1=turn tight on.
 */
   int
  Set_tight_state          // eff: change state for cstrn
   (int tag,               // in : cstrn identifier
    int state);            // in : 0=turn tight off,1=turn tight on
/**
 * Returns 0 when change is allowed, else it returns -1.
 * <br><br>
 * <b>Role:</b> Changes on/off state for <tt>cstrn</tt>. Sets the <tt>active_dmod</tt> to owner of tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_cstrn_state.
 * <br><br>
 * @param tag
 * cstrn identifier.
 * @param state
 * 0=turn cstrn off, 1=turn cstrn on.
 */
   int                      // rtn: 0 when change allowed, else -1
   Set_cstrn_state         // eff: change state for cstrn
    (int tag,              // in : cstrn identifier
     int state) ;          // in : 0=turn cstrn off,1=turn cstrn on
                           // note: sets active_dmod to owner of tag object
/**
 * Gets the <tt>zone_flag</tt> value for the <tt>area_cstrn</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_area_cstrn_flag.
 * <br><br>
 * @param tag
 * cstrn identifier.
 * @param zone_flag
 * 0=zone area moves zone compliment fixed, 1=zone area fixed zone compliment moves, -1=toggle current zone_flag value.
 */
   void
   Get_area_cstrn_flag     // get: area_cstrn's zone_flag value
    (int tag,              // in : cstrn identifier
     int &zone_flag) ;     // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
                           //     -1=toggle current
                           //        zone_flag value
                           // note: gets active dmod
/**
 * Changes the zone flag for the deformable surface's <tt>area_cstrn</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_area_cstrn_flag.
 * <br><br>
 * @param tag
 * cstrn identifier.
 * @param zone_flag
 * 0=zone area moves zone compliment fixed, 1=zone area fixed zone compliment moves, -1=toggle current zone_flag value.
 */
   void
   Set_area_cstrn_flag     // eff: change zone_flag for area_cstrn
    (int tag,              // in : cstrn identifier
     int zone_flag) ;      // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
                           //     -1=toggle current
                           //        zone_flag value
                           // note: sets active dmod

/**
 * Adds a point pressure to the deformable surface at the domain location specified by dpt and returns a tag identifier.
 * <br><br>
 * <b>Role:</b> The point pressure is similar to a distributed pressure, but acts on only one
 * location of the surface at a time. The magnitude of the pressure is specified by gain.
 * If <tt>tag</tt> is -1, the next tag number is assigned by the system; otherwise, the specified tag
 * number is used. The user must ensure that all tag numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_add_pt_press.
 * <br><br>
 * @param dpt
 * domain_pt location of constraint.
 * @param gain
 * amplitude of the pressure.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
  int                      // rtn: tag identifier
   Add_pt_press            // eff: add pt pressure to d2a_dmod
    (SPApar_pos &dpt,         // in : domain_pt location of constraint
     double gain,          // in : amplitude of the pressure
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Adds a vector load to a <tt>d2a_dmod</tt>.
 * <br><br>
 * <b>Role:</b>  This sets the active <tt>dmod</tt>. For information purposes only, refer to the description
 * of the corresponding Standalone Deformable Modeling Component interface function @href DM_add_vector_load.
 * <br><br>
 * @param ivec
 * direction of vector_load.
 * @param gain
 * magnitude of vector_load.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int
   Add_vector_load         // eff: adds a vector_load to d2a_dmod
    (SPAvector &ivec,         // in : direction of vector_load
     double gain,          // in : magnitude of vector_load
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
                           // note: sets active dmod
/**
 * Adds an attractor to <tt>d2a_dmod</tt>.
 * <br><br>
 * <b>Role:</b>  This also sets the active <tt>dmod</tt>. For information purposes only, refer
 * to the description of the corresponding Standalone Deformable Modeling Component interface
 * function @href DM_add_attractor.
 * <br><br>
 * @param ipt
 * location of attractor.
 * @param power
 * measure of load's locality.
 * @param gain
 * magnitude of attractor.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int
   Add_attractor           // eff: adds a attractor to d2a_dmod
    (SPAposition &ipt,        // in : location of attractor
     int power,            // in : measure of load's locality
                           //      0,1=global: 2,3.. =more local
     double gain,          // in : magnitude of attractor
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
                           // note: sets active dmod
/**
 * Adds a spring load to the deformable surface connecting a point on the surface to a point in image space.
 * <br><br>
 * <b>Role:</b> The load pulls surface point towards the image space point. The surface point
 * is specified by the domain point dpt. When <tt>ipt_flag=0</tt>, the image point is calculated as the
 * projection of dpt into image space. This guarantees that the spring load's image space and
 * surface points start out at the same location. When <tt>ipt_flag=1</tt>, the image space point is set
 * equal to the ipt position. The stiffness of the spring load is specified by gain. If <tt>tag</tt> is
 * -1, the next tag number is assigned by the system; otherwise, the specified tag number is used.
 * The user must ensure that all tag numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_add_spring.
 * <br><br>
 * @param dpt
 * domain_pt loc of surf spring end.
 * @param ipt
 * image_pt loc of free spring end.
 * @param ipt_flag
 * 0 to let free_pt = image_loc of dpt, 1 to let free_pt = ipt.
 * @param gain
 * stiffness of the spring.
 * @param slide_state
 * 0=fixed,1=sliding.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_spring              // eff: add pt spring to d2a_dmod
    (SPApar_pos &dpt,         // in : domain_pt loc of surf spring end
     SPAposition &ipt,        // in : image_pt loc of free spring end
     int ipt_flag,         // in : 0: let free_pt = image_loc of dpt
                           //      1: let free_pt = ipt
     double gain,          // in : stiffness of the spring
     int slide_state=0,    // in : 0=fixed,1=sliding
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Adds a distributed pressure load to a region of the deformable surface and returns a tag identifier.
 * <br><br>
 * <b>Role:</b> Pressures act in the direction normal to the surface and act to puff out
 * or suck in the shape of the surface. Larger gain values have larger effects. Very large gain
 * values may cause the system of equations to become marginally stable. The solution will
 * oscillate and never come to an equilibrium position. If this happens, restore stability
 * of the solution by reducing the gain value. To create very large deformations with pressure,
 * use a sequence of mild pressure values intermixed with the the default shape feature supported
 * by the <tt>Set_default_shape</tt> method. The region to which the pressure is applied can be limited
 * by <tt>domain_min</tt> and <tt>domain_max</tt>. When <tt>domain_min</tt> and <tt>domain_max</tt> are
 * <tt>NULL</tt>, the pressure is applied to the entire surface. Otherwise, <tt>domain_min</tt> and
 * <tt>domain_max</tt> point to 2D arrays which specify the corners of a domain box to which the pressure
 * is applied. If <tt>tag</tt> is -1, the next tag number is assigned by the system; otherwise, the specified
 * tag number is used. The user must ensure that all tag numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_add_dist_press.
 * <br><br>
 * @param gain
 * gain of the pressure.
 * @param domain_min
 * low bound if NULL load all domain.
 * @param domain_max
 * top bound if NULL load all domain.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_dist_press          // eff: add a pressure to whole surf
    (double gain,          // in : gain of the pressure
     double *domain_min,   // in : low bound if NULL load all domain
     double *domain_max,   // in : top bound if NULL load all domain
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Adds a load, which is a set of spring loads all sharing a common gain.
 * <br><br>
 * <b>Role:</b> Each spring in the set connects a point on the surface to a point in image space.
 * The load simultaneously pulls all the surface points towards their associated image space
 * points. This load can be used to extract a shape from a set of scattered data points.
 * The surface points are specified by the array <tt>domain_pts</tt>, where each pair of entries
 * identifies one domain point (<i>u</i>,<i>v</i>):
 * <br><br>
 * <tt>domain_pts = [u0,v0, u1,v1,...]</tt>
 * <br><br>
 * The image space points are specified by the array <tt>free_pts</tt>:
 * <br><br>
 * <tt>free_pts = [x0,y0,z0, x1,y1,z1, ...]</tt>
 * <br><br>
 * <tt>gain</tt> is the stiffness for all the springs. If <tt>tag</tt> is -1, the next tag number is assigned
 * by the system; otherwise, the specified tag number is used. The user must ensure that all tag
 * numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_add_spring_set.
 * <br><br>
 * @param pt_count
 * number of springs in spring_set.
 * @param domain_pts
 * Spring surf end pts [uv0..,uvN].
 * @param free_pts
 * Spring free end pts [xyz0..,xyzN].
 * @param gain
 * stiffness of the spring.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_spring_set          // eff: add a set of springs to surf
    (int pt_count,         // in : number of springs in spring_set
     double *domain_pts,   // in : Spring surf end pts [uv0..,uvN]
                           //      Sized:[pt_count*domain_dim]
     double *free_pts,     // in : Spring free end pts [xyz0..,xyzN]
                           //      Sized:[pt_count*image_dim]
                           //      if NULL free_pts default to surf
     double gain,          // in : stiffness of the spring
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
/*
// tbrv
*/
/**
 * @nodoc
 */
   virtual int              // rtn: tag identifier
   Add_spring_set          // eff: add a set of springs to surf
    (int pt_count,         // in : number of springs in spring_set
     const SPAposition *targets,     // in : Spring free end pts [xyz0..,xyzN]
     double gain,          // in : stiffness of the spring
     int tag_flag,         // in : specify tgt dmod
     const SPApar_pos *guesses=NULL) // in : optional guesses for parameters corresponding to targets
	{
		SPAUNUSED(pt_count)
		SPAUNUSED(targets)
		SPAUNUSED(gain)
		SPAUNUSED(tag_flag)
		SPAUNUSED(guesses)
		DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL);
		return 0;
   }

/**
 * Adds a distributed curve spring along <tt>domain_pcur</tt> and returns a tag identifier.
 * <br><br>
 * <b>Role:</b> This load pulls each surface point identified by <tt>domain_pcur</tt> towards an
 * associated point in the load's image space curve. The image space curve can be specified
 * in 3 different manners, depending on the value of syntax.
 * <pre>
 * Syn  Image Space Curve Specification
 * 1    free_pcur
 * 2    a straight line from p0 to p1
 * 3    an ellipse given by the equation:
 * Curve(u) = p0 + p1 * cos(u) + p2 *sin(u)
 * where 0 &lt;= u &lt;= 2 pi</pre>
 * Make sure that <tt>p1</tt> is not in the direction of <tt>p2</tt>, or the ellipse will collapse into a
 * line segment, yielding strange results. <tt>gain</tt> specifies the stiffness between the image and
 * domain curves. Larger gain values draw the surface points closer to the image space curve
 * points. If <tt>tag</tt> is -1, the next tag number is assigned by the system; otherwise, the specified
 * tag number is used. The user must ensure that all tag numbers are unique.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_add_crv_load.
 * <br><br>
 * @param domain_pcur
 * uv or u domain curve description.
 * @param free_pcur
 * image space curve (if NULL project domain_crv to make image free_crv) used when syntax is 1.
 * @param p0
 * used when syntax is 2 or 3.
 * @param p1
 * used when syntax is 2 or 3.
 * @param p2
 * used when syntax is 3.
 * @param syntax
 * specifies free_pcur construction, 1=use free_pcur, 2=free is a line from p0 to p1, 3=free is a round ctr=p0,pa=p1,pb=p2, round = ctr + cos(s)*pa + sin(s)*pb.
 * @param gain
 * stiffness connecting the 2 curves.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_crv_load            // eff: add a distributed curve spring
    (pcurve *domain_pcur,  // in : uv or u domain curve description
                           //      NOT stored/deleted with crv_load
     bs3_curve free_pcur,  // in : image space curve (if NULL project
                           //      domain_crv to make image free_crv)
                           //      used when syntax is 1
     SPAposition &p0,         // in : used when syntax is 2 or 3
     SPAposition &p1,         // in : used when syntax is 2 or 3
     SPAposition &p2,         // in : used when syntax is 3
     int syntax,           // in : specifies free_pcur construction
                           //   1=use free_pcur
                           //   2=free is a line from p0 to p1
                           //   3=free is a round ctr=p0,pa=p1,pb=p2
                           //   round = ctr + cos(s)*pa + sin(s)*pb
     double gain,          // in : stiffness connecting the 2 curves
     int tag_flag,         // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag) ;            // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Returns a tag identifier; Adds a <tt>DS_circ domain_crv</tt> tag object.
 * <br><br>
 * <b>Role:</b> When <tt>flag=0</tt>, adds a curve constraint. When <tt>flag=1</tt>, adds a curve load. The shape
 * of the domain curve is an ellipse specified by a center point and two vectors defining the
 * minimum and maximum axis. The equation of the domain curve is:
 * <pre>
 * D-curve(u) = ctr + pa * cos(u) + pb * sin(u)
 * where 0 &lt;= u &lt;= 2*pi </PRE>
 * The circle is divided into <tt>elem_count</tt> pieces for numerical integration. The shape of
 * each piece is integrated to <tt>ntgrl_degree</tt> accuracy. More pieces and higher integration
 * accuracies decrease errors and increase the computation time of creating the constraint
 * equations, but does not increase the computation time of solving for new deformations.
 * The gain value is used for load curves to specify the stiffness of the sheet spring,
 * which connects the load curve's image space curve to its domain space curve. The image
 * space curve of a load curve is generated by projecting the circle through the surface shape.
 * This projection is represented with a deformable spline curve. If <tt>tag</tt> is -1, the next tag
 * number is assigned by the system; otherwise, the specified tag number is used. The user
 * must ensure that all tag numbers are unique.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param flag
 * 0=add cstrn, 1=add load.
 * @param behavior
 * orof DM_POS_FIXED (make pos cstrn), DM_TAN_FIXED (make tan cstrn), DM_CURV_FIXED (make curvature cstrn).
 * @param ctr
 * circ = ctr + pa*cos + pb*sin.
 * @param pa
 * pa of circ definition.
 * @param pb
 * pb of circ definition.
 * @param gain
 * stiffness betwee the 2 crv_loads.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 * @param tag
 * when == -1 assign next tag number else use this tag number.
 */
   int                      // rtn: tag identifier
   Add_circ_tag            // eff: add a DS_circ domain_crv tag obj
    (int domain_dim,       // in : dmod domain_dim (for error checking)
     int flag,             // in : 0=add cstrn, 1=add load
     int behavior,         //orof: DM_POS_FIXED  (make pos cstrn)
                           //      DM_TAN_FIXED  (make tan cstrn)
                           //      DM_CURV_FIXED (make curvature cstrn)
     SPApar_pos &ctr,         // in : circ = ctr + pa*cos + pb*sin
     SPApar_pos &pa,          // in : pa of circ definition
     SPApar_pos &pb,          // in : pb of circ definition
     double gain=100.0,    // in : stiffness betwee the 2 crv_loads
     int tag_flag=1,       // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
     int tag=-1) ;         // in : when == -1 assign next tag number
                           //      else use this tag number
/**
 * Searches entire hierarchy for <tt>load</tt> that matches the tag identifier.
 * <br><br>
 * <b>Role:</b> It returns the tag for the patch containing the <tt>load</tt> or -1. This sets
 * <tt>active_dmod</tt> to owner of the return object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_find_load_by_tag.
 * <br><br>
 * @param tag
 * unique tag identifier to match.
 * @param patch_tag
 * tag of patch containing load or -1.
 */
   DS_load *                // rtn: load with matching tag value or NULL
   Find_load_by_tag        // eff: search entire hierarchy for load
    (int tag,              // in : unique tag identifier to match
     int &patch_tag) ;     // out: tag of patch containing load or -1
                           // note: sets active_dmod to owner of rtn object
/**
 * Returns the each tag object's tag id and tag type.
 * <br><br>
 * <b>Role:</b> The return size is two times the tag count. [mallocs: Caller must free return object.]
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_get_tag_summary.
 * <br><br>
 * @param tag_count
 * number of tag objects.
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   int *                    // rtn: each tag-object's (tag-id tag-type)
   Get_tag_summary         // eff: build and return int list
    (int &tag_count,       // out: number of tag objects
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: rtn_size:[2*tag_count]
                           // mallocs: caller must free rtn obj
/**
 * Removes a tag object identified by <tt>tag</tt> and returns the tag type of the object deleted.
 * <br><br>
 * <b>Role:</b> When tag is an invalid number or identifies a curve constraint which is
 * undeletable, the value <tt>DS_tag_none</tt> is returned. This sets the <tt>active_dmod</tt> to the owner of
 * the tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_rm_tag_object.
 * <br><br>
 * @param tag
 * identifying tag for the target.
 */
   DS_TAGS                  // rtn: rm_obj's tag_type or DS_tag_none
   Rm_tag_object           // eff: remove a dmodm, load, or cstrn by tag
    (int tag) ;            // in : identifying tag for the target
                           // note: sets active_dmod to owner of tag object
/**
 * When tag identifies a constraint that can be disabled.
 * <br><br>
 * <b>Role:</b> Toggles the constraint state between <tt>DS_CST_ON</tt> and <tt>DS_CST_OFF</tt> and returns the
 * tag type of the modified constraint. When <tt>tag</tt> is an invalid number or identifies a constraint
 * that can not be toggled, the value <tt>DS_tag_none</tt> is returned.
 * <br><br>
 * @param tag
 * identifying tag for the target.
 */
   DS_TAGS                  // rtn: toggle's type or DS_tag_none
   Toggle_cstrn            // eff: toggle cstrn on and off
   (int tag) ;             // in : identifying tag for tgt
                           // note: sets active_dmod to owner of tag object
/**
 * Toggles patch's C0/C1/C2 connectivity.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_patch_continuity.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod, 1 = active dmod, 2 = root dmod, else = member dmod with tag==tag_flag.
 */
   void
   Toggle_seam             // eff: toggle patch's C0/C1/C2 connectivity
    (int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Converts curve constraints into curve loads and converts curve loads into curve constraints.
 * <br><br>
 * <b>Role:</b> When <tt>tag</tt> identifies a curve load, its domain curve is used to create a new
 * curve constraint. The curve load is removed from the deformable surface and the tag for
 * the new curve constraint is returned. When the <tt>tag</tt> identifies a curve constraint, a new
 * curve load is created from the curve constraint's domain curve. The curve load's image
 * space curve is made by projecting the domain curve into image space using the current
 * deformable surface shape.
 * <br><br>
 * When the curve constraint is deletable, it is deleted. When it can be disabled,
 * it is disabled and the tag of the newly created curve load is returned. When the curve
 * constraint can't be deleted or disabled, no curve load is created and the value -1 is returned.
 * <br><br>
 * @param tag
 * identifying tag for tgt load/cstrn.
 * @param flag
 * one of DM_remember_target, DM_forget_target.
 */
   int                        // rtn: new tag id or -1
   Convert_loads_and_cstrns  // eff: convert between cstrns into loads
    (int tag,                // in : identifying tag for tgt load/cstrn
     SPA_DM_target_memory flag   // in : oneof SPA_DM_remember_target, SPA_DM_forget_target
       = SPA_DM_remember_target);
                           // note: sets active_dmod to owner of tag object
/**
 * Sets the domain point in a constraint or load, returning the modified <tt>mv_obj</tt>'s tag type or <tt>DS_tag_none</tt>.
 * <br><br>
 * <b>Role:</b> If the input tag number is invalid or if the identified tag object has no
 * associated domain point to change, <tt>DS_tag_none</tt> is returned. Sets the <tt>active_dmod</tt> to owner
 * of tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_pt_uv.
 * <br><br>
 * @param tag
 * identifying tag for the target.
 * @param dpt
 * new domain_pt for the target.
 */
   DS_TAGS                  // rtn: mv_obj's tag_type or DS_tag_none
   Set_pt_uv               // eff: set domain_pt in a cstrn or load
    (int tag,              // in : identifying tag for the target
     SPApar_pos &dpt) ;       // in : new domain_pt for the target
                           // note: sets active_dmod to owner of tag object
/**
 * Returns the current <i>uv</i> location of the tag.
 * <br><br>
 * <b>Role:</b>  Sets <tt>active_dmod</tt> to owner of tag object. For information purposes only,
 * refer to the description of the corresponding Standalone Deformable Modeling Component interface
 * function @href DM_get_pt_uv.
 * <br><br>
 * @param tag
 * identifying tag for the target.
 * @param dpt
 * tag's current uv location.
 */
   void Get_pt_uv           // get: tag's current uv location
    (int tag,              // in : identifying tag for the target
     SPApar_pos &dpt) ;       // out: tag's current uv location
                           // note: sets active_dmod to owner of tag object
/**
 * Sets the image point in a constraint or load and returns the tag type of the modified object.
 * <br><br>
 * <b>Role:</b> When <tt>p0</tt> is specified and <tt>p1</tt> is set to the <tt>NULL_REF</tt>,
 * the target image point is set to the location in <tt>p0</tt>. When both <tt>p0</tt> and <tt>p1</tt>
 * are specified, the target image point is projected onto a line running through positions <tt>p0</tt>
 * and <tt>p1</tt>. The projection direction is controlled by the <tt>dir</tt> flag. When <tt>dir=1</tt>,
 * the <i>x</i> and <i>y</i>-components of the image point are changed to place it on the image line.
 * When <tt>dir=2</tt>, the <i>z</i>-component of the image point is changed to bring the point as close
 * to the image space line as possible. The projection feature is intended to allow mouse based
 * click-events (projected into lines) to move tag object image space locations in 3-dimensions.
 * If the tag number is invalid or if the identified tag object has no associated image point, the value
 * <tt>DS_tag_none</tt> is returned. Sets the <tt>active_dmod</tt> to owner of tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_pt_xyz.
 * <br><br>
 * @param tag
 * identifying tag for the target.
 * @param pt_index
 * index of tag's image_pt to update.
 * @param p0
 * new ipt or p0 of iline=p0+u*p1-p0.
 * @param p1
 * NULL_REF or p1 of iline=p0+u*p1-p0.
 * @param dir_flag
 * specifies how to compute new_xyz P1=NULL_REF xyz=p0, 1="xy" xyz=xsect (iline, tag(xy_plane)), 2="z" xyz = proj(iline, tag(z_line)).
 * @param cascade_flag
 * 2=move tangent vecs independently but update normal vecs, 1=update related pt_cstrn values and image_pts, 0=don't.

 */
   DS_TAGS                  // rtn: mv_obj's tag_type or DS_tag_none
  Set_pt_xyz               // eff: set image_pt in cstrn,load,or cpt
  (int tag,                // in : identifying tag for the target
   int pt_index,           // in : index of tag's image_pt to update
                           //      for pt-cstrns and SPAvector-loads
                           //      one of DM_BASE_PT
                           //             DM_END_LEG
                           //             DM_TANG_LEG
                           //             DM_TANG1_LEG
                           //             DM_TANG2_LEG
                           //             DM_NORM_LEG
                           //             DM_CURV_LEG
                           //             DM_CURV1_LEG
                           //             DM_CURV2_LEG
                           //             DM_BINORM_LEG
                           //      or a control point tag index,
                           //      or an id number in a spring_set.
     SPAposition &p0,         // in : new ipt or p0 of iline=p0+u*p1-p0
     SPAposition &p1=         // in : NULL_REF or p1 of iline=p0+u*p1-p0
     *(SPAposition *)NULL_REF,
   int dir_flag=0,         // in : oneof
                           //      0=let image_pt = P0
                           //      for pt_index == DM_BASE_PT
                           //        1=move parallel to xy plane,
                           //        2=move parallel to z axis
                           //      else
                           //        1=rotate current SPAvector
                           //        2=change SPAvector magnitude
   int cascade_flag=1) ;   // in : 2=move tangent vecs independently
                           //        but update normal vecs
                           //      1=update related pt_cstrn
                           //        values and image_pts.
                           //      0=don't.
/**
 * Returns the current <i>xyz</i> position of the <tt>tag</tt>.
 * <br><br>
 * <b>Role:</b> Sets <tt>active_dmod</tt> to owner of <tt>tag</tt> object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_pt_xyz.
 * <br><br>
 * @param tag
 * identifying tag to be queried.
 * @param pt_index
 * index of tag's image_pt of interest for pt-cstrns and SPAvector-loads 0 = update base_pt 1 = update tang_pt for spring_sets get spring[pt_index] free_pt.
 * @param p0
 * tag's current xyz SPAposition.
 */
   int                      // rtn: 1=found pt to load,0=no point for tag
    Get_pt_xyz             // get: tag's current xyz location
    (int tag,              // in : identifying tag to be queried
     int pt_index,         // in : index of tag's image_pt of interest
                           //      for pt-cstrns and SPAvector-loads
                           //        0 = update base_pt
                           //        1 = update tang_pt
                           //      for spring_sets get
                           //      spring[pt_index] free_pt
     SPAposition &p0) ;       // out: tag's current xyz SPAposition
                           // note: sets active_dmod to owner of tag object
/**
 * Calculates the <tt>dmod</tt> <i>xyz</i> position for given <tt>uv</tt> location.
 * <br><br>
 * <b>Role:</b> Returns 0 if successful or -1 if bad <tt>tag_flag</tt> value.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_eval_dmod.
 * <br><br>
 * @param uv
 * domain point for evaluation.
 * @param xyz
 * projected xyz SPAposition for uv location.
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   int                      // rtn: 0=success, -1=bad tag_flag value
   Get_xyz                 // eff: calc dmod xyz SPAposition for uv location
    (SPApar_pos uv,           // in : domain point for evaluation
     SPAposition &xyz,        // out: projected xyz SPAposition for uv location
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/*
  int                      // rtn: 1=found crv to change,0=no curve for tag
   Set_tang_gain           // eff: vary a tangent constraint's magnitude
    (int tag,              // in : identifying tag to be modified
     double tang_gain) ;   // in : new value for tang cstrn's magnitude
                           // note: sets active_dmod to owner of tag object

  double                   // rtn: target's tang_gain or -1.0
   Get_tang_gain           // eff: vary a tangent constraint's magnitude
    (int tag) ;            // in : identifying tag to be modified
                           // note: sets active_dmod to owner of tag object
*/
/**
 * Varies a tangent's display magnitude.
 * <br><br>
 * <b>Role:</b>  Returns 0 if successful, otherwise returns a negative error code.
 * This sets <tt>active_dmod</tt> to owner of the tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_tan_display_gain.
 * <br><br>
 * @param tan_display_gain
 * display scaling for tang_vec cstrns.
 * @param tag_flag
 * specify tgt dmod and how deep to go 1 = active dmod (-1 = and offspring) 2 = root dmod (-2 = and siblings and offspring) else = member dmod with tag==tag_flag (neg = and offspring).
 */
  int                      // rtn: 0=success, else negative err code
   Set_tan_display_gain    // eff: vary a tangent's display magnitude
   (double tan_display_gain,// in : display scaling for tang_vec cstrns
    int tag_flag) ;        // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)
                           // note: sets active dmod
/**
 * Returns the target model's <tt>tan_display_gain</tt> value, which indicates a tangent's display magnitude.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_tan_display_gain.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   double                   // rtn: tgt dmod's tan_display_gain value
   Get_tan_display_gain    // eff: vary a tangent's display magnitude
   (int tag_flag) ;        // in : specify tgt dmod and how deep to go
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: gets active dmod
/**
 * When <tt>type = DS_tag_none</tt>, it searches for the load which matches the input tag number and updates its gain.
 * <br><br>
 * <b>Role:</b> Otherwise, it searches the entire load list for all loads of the input type
 * and updates their gains. When <tt>inc_flag</tt> is zero, the tgt gains are set equal to the to the
 * input gain. When <tt>inc_flag</tt> is nonzero, the <tt>tgt</tt> gains are incremented by the input gain.
 * Sets active_dmod to owner of tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_load_gain.
 * <br><br>
 * @param type
 * if type==DS_tag_none then tgt = tag else tgt = all loads of type.
 * @param tag
 * tgt load when type = DS_tag_none.
 * @param gain
 * new gain or gain change.
 * @param inc_flag
 * 0:tag(gain) =gain, 1:tag(gain)+=gain.
 */
   DS_TAGS                  // rtn: set_obj's tag_type or DS_tag_none
   Set_load_gain           // eff: sets gains in d2a_dmod loads
    (DS_TAGS type,         // in : if type==DS_tag_none then tgt = tag
                           //      else tgt = all loads of type
     int tag,              // in : tgt load when type = DS_tag_none
     double gain,          // in : new gain or gain change
     int inc_flag=0) ;     // in : 0:tag(gain) =gain
                           //      1:tag(gain)+=gain
                           // note: sets active_dmod to owner of tag object
/**
 * Returns the current gain value for the <tt>tag</tt>.
 * <br><br>
 * <b>Role:</b>  Sets <tt>active_dmod</tt> to owner of tag object.
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_load_gain.
 * <br><br>
 * @param tag
 * tgt load to examine.
 * @param gain
 * tag's current gain.
 */
   void Get_load_gain       // get: tag's current gain value
    (int tag,              // in : tgt load to examine
     double &gain) ;       // out: tag's current gain
                           // note: sets active_dmod to owner of tag object
/**
 * Returns 1 if found load was changed, otherwise it returns 0.
 * <br><br>
 * <b>Role:</b> This sets the <tt>DS_attractor</tt> power. It also sets the active dmod.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_attractor_power.
 * <br><br>
 * @param tag
 * load identifier.
 * @param power
 * measure of load's locality 0,1=global: 2,3.. =more local.
 */
   int                      // rtn: 1=found load to change,0=didn't
   Set_attractor_power     // eff: sets a DS_attractor power
   (int tag,               // in : load identifier
    int power=2) ;         // in : measure of load's locality
                           //      0,1=global: 2,3.. =more local
                           // note: sets active dmod

// TODO:??? function not yet implemented
/**
 * Sets the spring's slide state.
 * <br><br>
 * <b>Role:</b> This sets <tt>active_dmod</tt> to owner of the tag object.
 * <br><br>
 * @param tag
 * tgt load to examine.
 * @param slide_state
 * 0=fixed, 1=sliding.
 */
  int                      // rtn: 0=success,-1=tag not a spring
   Set_slide_state         // eff: set spring's slide_state
    (int tag, //tag,              // in : tgt load to examine
     int slide_state) ;    // in : 0=fixed, 1=sliding
                           // note: sets active_dmod to owner of tag object
/**
 * Returns the spring's slide state.
 * <br><br>
 * <b>Role:</b> This sets <tt>active_dmod</tt> to owner of tag object.
 * <br><br>
 * @param tag
 * tgt load to examine.
 * @param slide_state
 * 0=fixed, 1=sliding, -1=not a spring.
 */
   void Get_slide_state     // eff: get spring's slide state
    (int tag,              // in : tgt load to examine
     int &slide_state) ;   // out: 0=fixed,1=sliding,-1=not a spring
                           // note: sets active_dmod to owner of tag object

                           // use: for test - not for ACIS faces, its
/**
 * Changes end states for a <tt>d2a_dmod</tt>.
 * <br><br>
 * <b>Role:</b> Used for testing, not for ACIS faces. Its effect won't pass through <tt>commit_to_face</tt>.
 * The end condition tells how the boundary curves of a square spline can be connected. Open edges are not connected.
 * Closed edges are forced to superimpose one another. A square spline with one closed edge
 * will look like a cylinder with a crease along one side. Periodic is a closed surface where
 * the edges are made to connect smoothly. A square spline surface with one periodic edge will
 * be a smooth cylinder. Singular edges are edges which have been collapsed to a single point.
 * Any combination of the four bounding edges of a square spline surface may be singular.
 * Currently, deformable surfaces which have changed their end conditions will not be able to
 * be committed back to their owning faces.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_set_end_conds.
 * <br><br>
 * @param domain_dim
 * 1=curve, 2=surface (for error checking).
 * @param end_cond_u
 * oneof 0=open|1=closed|2=periodic.
 * @param singular_u
 * oneof 0=none|1=low|2=high|3=both.
 * @param end_cond_v
 * oneof 0=open|1=closed|2=periodic.
 * @param singular_v
 * oneof 0=none|1=low|2=high|3=both.
 */
  void                     // affect won't pass thru commit_to_face()
   Set_end_conds           // eff: changes end-states for d2a_dmod
    (int domain_dim,       // in : 1=curve, 2=surface (for error checking)
     int end_cond_u,       // in : oneof 0=open|1=closed|2=periodic
     int singular_u,       // in : oneof 0=none|1=low|2=high|3=both
     int end_cond_v,       // in : oneof 0=open|1=closed|2=periodic
     int singular_v) ;     // in : oneof 0=none|1=low|2=high|3=both
                           // note: only end_cond_u used for 1d curves
                           // note: only root_dmod may be closed or periodic
/**
 * Returns end states for a <tt>d2a_dmod</tt>.
 * <br><br>
 * <b>Role:</b> Only <tt>end_cond_u</tt> set by 1d curves are affected. All other values are set to -1.
 * <br><br>
 * The end condition tells how the boundary curves of a square spline can be connected.
 * Open edges are not connected. Closed edges are forced to superimpose one another. A
 * square spline with one closed edge will look like a cylinder with a crease along one side.
 * Periodic is a closed surface where the edges are made to connect smoothly. A square spline
 * surface with one periodic edge will be a smooth cylinder. Singular edges are edges which
 * have been collapsed to a single point. Any combination of the four bounding edges of a
 * square spline surface may be singular.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_end_conds.
 * <br><br>
 * @param end_cond_u
 * oneof 0=open|1=closed|2=periodic.
 * @param singular_u
 * oneof 0=none|1=low|2=high|3=both.
 * @param end_cond_v
 * oneof 0=open|1=closed|2=periodic.
 * @param singular_v
 * oneof 0=none|1=low|2=high|3=both.
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   void Get_end_conds       // eff: get end-states for d2a_dmod
    (int &end_cond_u,      // in : oneof 0=open|1=closed|2=periodic
     int &singular_u,      // in : oneof 0=none|1=low|2=high|3=both
     int &end_cond_v,      // in : oneof 0=open|1=closed|2=periodic
     int &singular_v,      // in : oneof 0=none|1=low|2=high|3=both
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: only end_cond_u set by 1d curves
                           //       all other values set to -1.
/**
 * Returns the length for springs and spring sets.
 * <br><br>
 * <b>Role:</b> This sets <tt>active_dmod</tt> to the owner of the tag object.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_get_spring_length.
 * <br><br>
 * @param tag
 * tag number of object to be queried.
 */
   double                   // rtn: spring (or longest spring) length or -1
   Get_spring_length       // eff: get length for springs and spring-sets
    (int tag) ;            // in : tag number of object to be queried
                           // note: sets active_dmod to owner of tag object
/**
 * Returns the tag number (or -1) of the point closest to the line specified by <tt>p0</tt> and <tt>p1</tt>.
 * <br><br>
 * <b>Role:</b> Constraints and loads have positive tags, and control points have negative tags.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_find_tag_by_image_line.
 * <br><br>
 * @param p0
 * p0 of iline = p0 + u*(p1 - p0).
 * @param p1
 * p1 of iline = p0 + u*(p1 - p0).
 * @param max_dist
 * width of ray in which to search.
 * @param max_dist_units
 * specify the max_dist units 0=max_dist in image_space units 1=max_dist in active_view pixels.
 * @param pt_index
 * image_pt index in tag for closest pt.
 */
   int                      // rtn: tag (or -1) to pt closest to iline
   Find_tag_by_iline       //      cstrns and loads have pos tags
                           //      control pts have negative tags
    (SPAposition &p0,         // in : p0 of iline = p0 + u*(p1 - p0)
     SPAposition &p1,         // in : p1 of iline = p0 + u*(p1 - p0)
     double max_dist,      // in : width of ray in which to search
     int max_dist_units,   // in : specify the max_dist units
                           //      0=max_dist in image_space units
                           //      1=max_dist in active_view pixels
     int    &pt_index) ;   // out: image_pt index in tag for closest pt
                           // note: sets active_dmod to owner of rtn object
/**
 * Scales the parametric position from 0 to 1 into the surface's parameter range.
 * <br><br>
 * <b>Role:</b> Returns the <i>uv</i>-coordinate values to fit in the current surface.
 * Projects the <i>u</i> and <i>v</i>-coordinate values of uv from the unit square into the deformable
 * surface's actual parameter range. Within the methods of <tt>ATTRIB_DS2ACIS</tt>, all parametric
 * parametric position input positions are given in the range of 0.0 to 1.0. This unit
 * square is mapped to the actual parametric range of the deformable surface. It is
 * expected that this function will only be called by other <tt>ATTRIB_DS2ACIS</tt> methods, but
 * is exposed as a convenience routine for applications.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_set_alpha.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param uv
 * change uv values from unit-square to current surface domain..
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   void                     // eff: scale SPApar_pos from 0 to 1
   Scale_par_pos_to_dmod   //      into the shapes's SPAparameter range
    (int domain_dim,       // in : dmod domain_dim (for error checking)
     SPApar_pos &uv,          // i/o: change uv values from unit-square
                           //      to current surface domain.
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Scales a surface's parametric position to 0 to 1.
 * <br><br>
 * <b>Role:</b> Returns the <i>uv</i>-coordinate values to fit in a unit square. Projects the
 * <i>u</i> and <i>v</i>-coordinate values of uv from the unit square into the deformable surface's
 * actual parameter range. Within the methods of <tt>ATTRIB_DS2ACIS</tt>, all parametric parametric
 * position input positions are given in the range of 0.0 to 1.0. This unit square is mapped
 * to the actual parametric range of the deformable surface. It is expected that this function
 * will only be called by other <tt>ATTRIB_DS2ACIS</tt> methods, but is exposed as a convenience routine
 * for applications.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param uv
 * change uv values current surface domain to the unit-square.
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   void  Scale_par_pos_from_dmod (int domain_dim,       // in : dmod domain_dim (for error checking)
                                 SPApar_pos &uv,          // i/o: change uv values current surface domain
                                                       //      to the unit-square.
                                 int tag_flag=1) ;     // in : specify tgt dmod
                                                       //      1   = active dmod
                                                       //      2   = root   dmod
                                                       //     else = member dmod with tag==tag_flag
/**
 * Scales an array of dpts from 0 to 1 into the surface's parameter range.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param pt_count
 * number of uv points in dpt.
 * @param dpt
 * change uv coord values to fit in the current surface..
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   void Scale_dpt_array (int domain_dim,       // in : dmod domain_dim (for error checking)
                         int pt_count,         // in : number of uv points in dpt
                         double *dpt,          // i/o: change uv coord values to fit in
                                               //      the current surface.
                                               //      1d=[u0,u1,..un], 2d=[uv0,uv1,..uvn]
                                               //      Sized:[domain_dim*pt_count]
                         int tag_flag=1) ;     // in : specify tgt dmod
                                               //      1   = active dmod
                                               //      2   = root   dmod
                                               //     else = member dmod with tag==tag_flag
/**
 * Returns the <tt>DS_tag_type</tt> for the tag or <tt>DS_tag_none</tt>.
 * <br><br>
 * <b>Role:</b> For information purposes only, refer to the description of the corresponding
 * Standalone Deformable Modeling Component interface function @href DM_classify_tag.
 * <br><br>
 * @param tag
 * src tag number.
 */
   DS_TAGS Classify_tag   (int tag) ;            // in : src tag number
/**
 * Test the input <tt>DS_TAGS</tt> value.
 * <br><br>
 * @param ds_tags
 * DS_TAGS value to test.
 */
   int  Is_type_tracking_curve  (DS_TAGS ds_tags) ;     // in : DS_TAGS value to test
/**
 * Evaluates <tt>tmp_surface</tt> at <tt>dpt</tt>.
 * <br><br>
 * <b>Role:</b> Takes a parametric position as input, and outputs the position,
 * the first derivative as the two-element array <tt>ipt_d1 =  [du, dv]</tt> and the second
 * derivative as the three-element array <tt>ipt_d2 = [duu, duv, dvv]</tt>.
 * <br><br>
 * @param domain_dim
 * dmod domain_dim (for error checking).
 * @param dpt
 * parametric position.
 * @param ipt
 * position.
 * @param ipt_d1
 * 1st derivs.
 * @param ipt_d2
 * 2nd derivs.
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
  void Tmp_shape_eval  (int domain_dim,       // in : dmod domain_dim (for error checking)
     SPApar_pos &dpt,         // in : parametric SPAposition
     SPAposition &ipt,        // out: SPAposition
     SPAvector *ipt_d1,       // out: 1st derivs
                           //      case 1d:[du]          sized:1
                           //           2d:[du,dv]       sized:2
     SPAvector *ipt_d2,       // out: 2nd derivs
                           //      case 1d:[duu]         sized:1
                           //           2d:[duu,duv,dvv] sized:3
     int tag_flag=1) ;     // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Builds and stores in <tt>d2a_clump</tt> a clump built from the faceting of a <tt>d2a_tmp_surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns 1 for <tt>pt_on_curve</tt>, 2 for <tt>srf_pt</tt>, -1 for <tt>no_pt</tt>, and -2 for <tt>fuzzy</tt>.
 * <br><br>
 * For information purposes only, refer to the description of the corresponding Standalone
 * Deformable Modeling Component interface function @href DM_xsect_dmod_by_image_line.
 * <br><br>
 * @param pt
 * pt of ray = pt + u * vc.
 * @param vc
 * vc of ray = pt + u * vc.
 * @param max_dist
 * width of ray in which to search for crvs.
 * @param max_dist_units
 * specify the max_dist units, 0=max_dist in image_space units, 1=max_dist in active_view pixels.
 * @param domain_pt
 * shape domain pt of intersection.
 * @param image_pt
 * shape image pt of intersection.
 * @param tag_flag
 * specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
 */
   int Shape_ray_xsect   (SPAposition &pt,          // in : pt of ray = pt + u * vc
                        SPAunit_vector &vc,       // in : vc of ray = pt + u * vc
                        double max_dist,       // in : width of ray in which to search for crvs
                        int max_dist_units,    // in : specify the max_dist units
                                               //      0=max_dist in image_space units
                                               //      1=max_dist in active_view pixels
                        SPApar_pos &domain_pt,    // out: shape domain pt of intersection
                        SPAposition &image_pt,    // out: shape image pt of intersection
                        int tag_flag=2) ;      // in : specify tgt dmod
                                               //      1   = active dmod
                                               //      2   = root   dmod
                                               //     else = member dmod with tag==tag_flag
                                               // note: sets active_dmod to owner of xsect pt

  // ATTRIB_DM2ACIS implementations of ATTRIB virtual functions
/*
// tbrv
*/
/**
 * @nodoc
 */
  virtual void roll_notify // use: callback for rollback notify
#ifndef DS_USE_6x_RENDERING
   (BULLETIN_TYPE type,        // eff: free internal data structs
    ENTITY * other)            //      going into the bulletin board.
#else
   (BULLETIN_TYPE ,        // eff: free internal data structs
    ENTITY *)              //      going into the bulletin board.
#endif /*DS_USE_6x_RENDERING*/

#ifndef DS_USE_6x_RENDERING
        ;

#else
                           { Free_data() ; }
#endif /*DS_USE_6x_RENDERING*/
/**
 * Notifies the <tt>class_name</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
 * The default action is to do nothing. This function is supplied by the application whenever
 * it defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param entity
 * entity being created.
 */
  virtual void  split_owner (ENTITY* entity)              // in : the new entity being created
                           {
								SPAUNUSED(entity)
								Free_data() ;
								lose() ;
                           }
/**
 * Notifies the <tt>class_name</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the attribute.
 * After the merge, this owner will be deleted if the logical deleting owner is <tt>TRUE</tt>, otherwise
 * it will be retained and other entity will be deleted. The default action is to do nothing.
 * This function is supplied by the application whenever it defines a new attribute, and is
 * called when a merge occurs.
 * <br><br>
 * @param other
 * the merge's 2nd topology entity.
 * @param deleted
 * True=this_entity to be deleted , False=other_entity to be deleted.
 */
  virtual void  merge_owner (ENTITY *other ,       // in : the merge's 2nd topology entity
                               logical deleted) ;   // in : True=this_entity to be deleted
                                                    // False=other_entity to be deleted
/**
 * Notifies the <tt>ATTRIB</tt> that its owner is about to be transformed.
 * <br><br>
 * <b>Role:</b> The application has the chance to transform the attribute. The default action
 * is to do nothing. This function is supplied by the application whenever it defines a new
 * attribute, and is called when a transformation occurs.
 * <br><br>
 * @param trans
 * the affine transformation.
 */
   virtual void   trans_owner (SPAtransf const& trans ); 

  // methods to be used by other ATTRIB_DM2ACIS methods
  protected:

   void Size_arrays        // eff: internal array memory management
    (int handle_count=0,   // in : desired size of d2a_tags_handle
     int shape_count=0) ;  // in : desired size of d2a_shape_handle
                           //      use Calc_shape_handle_count()
                           // note: graphic funcs make all size array calls
                           //       since handle counts is a graphic feature

   void Parse_tag_flag     // eff: turn tag_flag into dmod
    (int tag_flag,         // in : specify tgt dmod and how deep to go
                           //   1=active, 2=root, else=member(tag==tag_flag)
                           //   pos=tgt only, neg=tgt + offspring
     DS_dmod *&dmod,       // out: ptr to target dmod
     int &walk_flag) ;     // out: 0=tgt_only, 1=tgt and offspring

  /**
   * Increment and return the
   * hierarchy's tag id value. Call
   * this function to assign a tag
   * value for every tag obj being
   * created to make all
   * hierarchy tag values unique.
   */
   int Assign_next_tag() ;

   /**
    * hierarchy's currently assigned tag id value.
    */
   int Tag_count() const ; // rtn: 

   void Set_tag_count      // eff: set the hierarchy tag_count
     (int tag_count) ;     // in : desired tag number start value
                           // note: take care when changing this
                           //       number not to duplicate tag
                           //       values in the hierarchy.
   void
    Calc_tag_handle_count  // eff: sum required graphic handles
     (int &handle_count,   // out: non-tag graphic count in patch hierarchy
      int &shape_count) ;  // out: surf mesh count in patch hierarchy

   // ATTRIB_DM2ACIS methods to support ACIS_2DS glue code

  public:                  // made public for debug only - no general use
/**
 * Frees any memory in d2a_dmod, d2a_clump, d2a_tmp_surface, and d2a_dmod_clump.
 */
  void Free_data();

  protected:               // back to protected fucntions

  virtual void  Add_solid_model_cstrns (DS_dmod *,              // in : dmod = tgt dmod to update
                                       ATTRIB_DSMODEL *,       // in : dsmodel = src attrib_dsmodel for data
                                       DS_dmod *)              // in : future_parent = parent of dmod (just not set yet)

                           { DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
                           }
   /**
   * Make sure that nested
   * data within copies of the origninal
   *  attribute will not cause double 'frees'
   * when a copied attribute is actually
   *  deleted.  Derived functions must call
   * the parent function
    */
   void   Forget_data()  
   { 
       d2a_dmod         = NULL ;

#ifndef DS_USE_6x_RENDERING

#else
                             d2a_tags_fig     = NULL ;
                             d2a_shape_fig    = NULL ;

                             d2a_tags_handle  = NULL ;
                             d2a_handle_count = 0 ;
#endif /*DS_USE_6x_RENDERING*/
#ifndef DS_USE_6x_RENDERING
#else
                             d2a_shape_handle = NULL ;
                             d2a_shape_count  = 0 ;
#endif /*DS_USE_6x_RENDERING*/
                             d2a_mesh_state   = 0 ;
                           } // end ATTRIB_DM2ACIS::Forget_data

                           // use: run by attrib_ds2acis constructor
  virtual  DS_dmod * Owner_2DS_dmod  (ATTRIB_DSMODEL *,       // in : dsmodel = current shape model in patch_hierarchy
                                       DS_dmod *)              // in : parent_dmod = dmod acting as this new dmod's parent
                                   { DS_sys_error(DS_BAD_PURE_VIRTUAL_CALL) ;
                                     return NULL ;
                                   }

   public:
/**
 * Stores ACIS shape data for <tt>tgt_dmod</tt>.
 * <br><br>
 * @param tag_flag
 * specify tgt dmod and how deep to go, 1 = active dmod (-1 = and offspring), 2 = root dmod (-2  = and siblings and offspring), else = member dmod with tag==tag_flag.
 */
  void   Dmod_2tmp_shape  (int tag_flag=1) ;    // in : specify tgt dmod and how deep to go
                           //      1   = active dmod (-1  = and offspring)
                           //      2   = root   dmod (-2  = and siblings and offspring)
                           //     else = member dmod with tag==tag_flag
                           //                        (neg = and offspring)

   protected:
   void    Tmp_shape_2figure  () ;                  //      from stored ACIS shape reps.

   void   Tags_2figure  (float icon_radius) ; // in : size of pt icon radius display

   public:
#ifndef DS_USE_6x_RENDERING
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
  void                    // use: called by Tags_2figure
    Tags_fig_adds          // eff: add d2a_dmod tag data graphics
     () ;                  //      to d2a_tags_fig
/*
// tbrv
*/
/**
 * @nodoc
 */
  void                    // use: called by Tmp_shape_2figure
    Shape_fig_adds         // eff: add dmod->surf_mesh data as graphics
     () ;                  //      to d2a_shape_fig
#endif /*DS_USE_6x_RENDERING*/

  protected:
#ifndef DS_USE_6x_RENDERING
#else
   void
    Remove_tags_figure() ; // eff: remove and free tags figure data
   void
    Remove_shape_figure(); // eff: remove and free shape figure data
#endif
  // ATTRIB_DM2ACIS Internal commands
  public:
/**
 * Finds matching ACIS and dmod <tt>crv_cstrn</tt> pointers.
 * <br><br>
 * @param coedge
 * coedge to query.
 * @param attrib_crv_cstrn
 * attrib_crv_cstrn ptr.
 * @param crv_cstrn
 * matching crv_cstrn ptr.
 * @param src_data
 * matching crv_cstrn src_data ptr.
 * @param tag_id
 * matching crv_cstrn tag_id.
 * @param dmod
 * containing dmod.
 */
  void   Find_cstrn_for_coedge   (COEDGE *coedge,       // in : coedge to query
     ATTRIB_CRV_CSTRN
       *&attrib_crv_cstrn, // out: attrib_crv_cstrn ptr
     DS_cstrn *&crv_cstrn, // out: matching crv_cstrn ptr
     void *&src_data,      // out: matching crv_cstrn src_data ptr
     int &tag_id,          // out: matching crv_cstrn tag_id
     DS_dmod *&dmod) ;     // out: containing dmod


 // header declarations for all ACIS attribute standard functions
 /**
  * @nodoc
  */
 ATTRIB_FUNCTIONS(ATTRIB_DM2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_DM2ACIS


/**
 * @nodoc
 * affine transformation callback
 */
inline void ATTRIB_DM2ACIS::trans_owner (SPAtransf const& /* trans */ ) 
{
    Free_data() ;
    lose() ;
}



// 26-Apr-99 erik: Removed obsolete is_<class> documentation template.

//=============================================================
// class ATTRIB_DS2ACIS Derived from ATTRIB_AGC
//=============================================================
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_ADM int ATTRIB_DS2ACIS_TYPE;
#define    ATTRIB_DS2ACIS_LEVEL (ATTRIB_DM2ACIS_LEVEL + 1)

/**
 * Connects the deformable surface functions to the ACIS modeler.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_DS2ACIS</tt> class encapsulates the programmable interface to deformable
 * modeling. Using its methods, application programs can create, sculpt, and save deformable
 * models. In addition, its methods support the API functions for managing the movement of data
 * between faces and deformable modeling objects.
 * <br><br>
 * When a sculpting session begins, an <tt>ATTRIB_DS2ACIS</tt> object is created and associated to a face.
 * This object contains a copy of the face's geometry and of a deformable modeling object, both of
 * which are updated by sculpting commands. It contains an <tt>INDEXED_MESH</tt> and a <tt>deformable_figure</tt>
 * that are used for rendering the shape of the deformable model. It also contains the set of tag
 * objects being used to sculpt its shape.
 * <br><br>
 * Sculpting results can can be committed back to the face. At that time, the geometry of the
 * face is replaced by the geometry in the <tt>ATTRIB_DS2ACIS</tt> and the tag object data is stored as a
 * set of attributes attached to the face.
 * <br><br>
 * The <tt>ATTRIB_DS2ACIS</tt> object and its contained data are nonpersistent. When a sculpting session
 * ends, the <tt>ATTRIB_DS2ACIS</tt> object and all its data are removed.
 * <br><br>
 * When the next sculpting session begins, a new <tt>ATTRIB_DS2ACIS</tt> is created and loaded with data
 * representing the state of the <tt>ATTRIB_DS2ACIS</tt> object at the time of the last commit.
 */
class DECL_ADM ATTRIB_DS2ACIS: public ATTRIB_DM2ACIS  // identifer = "s2a"
{
  protected:

   virtual void
    Add_link
    (const ENTITY *ent,
     int behavior,
     double gain,
     int& tag);

  public:

/*
// tbrv
*/
/**
 * @nodoc
 */
  friend DECL_ADM outcome api_dm_add_link( ATTRIB_DM2ACIS *attrib_dm2acis, const ENTITY *ent, int behavior,
                                           double gain, int& tag, adm_options *adm_o, AcisOptions *ao);

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DS2ACIS(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param face_owner
 * new attrib owner.
 * @param pt_icon_rad
 * pt icon radius size.
 */
   ATTRIB_DS2ACIS              // Default Constructor
    (ENTITY *face_owner=NULL,  // in : new attrib owner(root parent's FACE)
     float pt_icon_rad= -1.0);  // in : pt icon radius size  (-1=leave as is)
/*
  ATTRIB_DS2ACIS             // Constructor
  (DS_dmod * the_dmod) ;       // in : root dmod
*/

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DS2ACIS(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param s2a
 * attrib to copy.
 * @param owner
 * owner for new attrib.
 */
  ATTRIB_DS2ACIS               // Copy Constructor
   (ATTRIB_DS2ACIS &s2a,       // in : attribute being copied
    ENTITY *owner) ;           // in : owner for new attribute

/**
 * Assignment operator.
 * <br><br>
 * @param s2a
 * in: attribute with values to copy.
 */
  ATTRIB_DS2ACIS &operator=    // assignment operator
   (const ATTRIB_DS2ACIS &s2a);// in : attribute with values to copy

/**
 * This is a virtual <tt>ATTRIB_AGC</tt> copy function.
 * <br><br>
 * @param s2a
 * in: owner for new attrib.
 */
  virtual ATTRIB_AGC*          // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)    // in : owner for new attribute
                               { return(ACIS_NEW ATTRIB_DS2ACIS(*this,owner));
                               }

/**
 * Returns  <tt>TRUE</tt>.
 */
  virtual logical pattern_compatible() const;
                                    // returns TRUE
  // ATTRIB_DS2ACIS multi-surface management
/**
 * Connects quad faces into a quilt.
 * <br><br>
 * @param face
 * Face to connect to hierarchy.
 * @param tag_shift
 * amount face tag numbers have shifted.
 */
  virtual                  // bas: ATTRIB_DM2AICS pure virtual function
  int                      // rtn: tag for new root sibling or -1 for none
   Add_multi_face          // eff: Connect quad faces into a quilt
    (FACE *face,           // in : Face to connect to hierarchy
      int &tag_shift) ;    // out: amount face tag numbers have shifted
                           //      new_tag_id = old_tag_id + tag_shift
                           // note: sets active patch

/**
 * Disconnects the face from the quilt.
 * <br><br>
 * @param tag
 * Face id to remove from hierarchy.
 */
  virtual                  // bas: ATTRIB_DM2AICS pure virtual function
  void
   Rm_multi_face           // eff: Disconnect face from quilt
    (int tag) ;            // in : Face id to remove from hierarchy

  // ATTRIB_DS2ACIS Patch function support
/**
 * Creates and adds a patch to the patch hierarchy.
 * <br><br>
 * <b>Role:</b> Always call between <tt>API_BEGIN</tt> and <tt>API_END</tt>.
 * <br><br>
 * @param shape_flag
 * in: 0 use seams, 1 rectangle,2 ellipse.
 * @param pt1
 * in: rect = min corner ellipse = center pt.
 * @param pt2
 * in: rect = max corner ellipse = 1st axix end.
 * @param pt3
 * in: rect = not used ellipse = 2nd axis end.
 * @param refinement
 * in: parent know spacing - child knot spacing.
 * @param seam_count
 * in: curve number in seam may be 0.
 * @param seam
 * in: seam curves NULL = add a square min/max seam.
 * @param tag_flag
 * in: specify tgt dmod to get new patch 1 active dmod 2 root dmod else member dmod with tag==tag_flag.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  int                      // rtn: tag for new patch dmod or -1 for none
   Add_patch               // eff: make and add a patch to patch hierarchy
   (int shape_flag,        // in : 0=use seams, 1=rectangle, 2=ellipse
                           //      3=fillet_square
    SPApar_pos pt1,           // in : rect=min-corner,ellipse=center-pt
    SPApar_pos pt2,           // in : rect=max-corner,ellipse=1st-axis-end-pt
    SPApar_pos pt3,           // in : rect=not_used,  ellipse=2nd-axis-end-pt
    int refinement,        // in : parent_knot_spacing/child_knot_spacing
    int seam_count,        // in : number of curves in seam (may be 0)
    DS_pfunc **seam,       // in : seam crvs (NULL=add a square (min/max) seam)
    int tag_flag=1) ;      // in : specify parent tgt_dmod to get new patch
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // NOTE: always call between API_BEGIN/API_END

/**
 * Removes and deletes this patch from the hierarchy.
 * <br><br>
 * <b>Role:</b> Deletes all offspring, as well. Always call this between <tt>API_BEGIN</tt> and <tt>API_END</tt>.
 * <br><br>
 * @param tag_flag
 * in: specify tgt dmod to be copied 1 active dmod 2 root dmod else member dmod with tag==tag_flag.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  void Rm_patch            // eff: remove and delete patch from hierarchy
   (int tag_flag=1) ;      // in : specify the tgt_dmod to be copied
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: deletes all offspring as well.
                           // NOTE: always call between API_BEGIN/API_END


  // ATTRIB_DS2ACIS API_commit function sequence
/**
 * Moves data to face and <tt>ATTRIB_DSMODEL</tt>.
 */
  virtual                  // bas: ATTRIB_DM2ACIS virtual function
   void                    // use: called by api_commit_attrib_ds2acis_to_face()
    Commit_to_owner() ;    // eff: move data to face and ATTRIB_DSMODEL

/**
 * Recursive tail of <tt>Commit_to_owner</tt>.
 * <br><br>
 * <b>Role:</b> Writes the current deformable surface geometry and dynamic model data into the
 * owner face's surface and <tt>ATTRIB_DSMODEL</tt> structures. These changes are spooled to the bulletin
 * board and can be undone with roll back.
 * <br><br>
 * @param dmod
 * current target shape to commit.
 */
  void                     // use: called by ATTRIB_DS2ACIS::Commit_to_owner
   Commit_to_faceR         // eff: recursive tail of Commit_to_owner()
    (DS_dmod *dmod) ;      // in : current target shape to commit

/**
 * Rebuilds the <tt>ds_bound_cstrn</tt> and <tt>ds_seam_cstrn</tt> curve geometry.
 * <br><br>
 * @param face
 * in: face being modified.
 * @param dmod
 * dmod being committed.
 * @param curve_count
 * size of following arrays.
 * @param ds_curve
 * in: ordered list of seams and sheet curves.
 * @param acis_curve
 * in: ordered list of ACIS intcurves.
 * @param scale_vec
 * orig_dmod_space= scale_vec* internal_pfunc_space.
 */
  void                        // eff: Rebuild ds_bound_cstrn and
   Update_face_bndry_geometry //      ds_seam_cstrn curve geom
    (FACE *face,              // in : Face being modified
     DS_dmod *dmod,           // in : dmod being committed
     int curve_count,         // in : size of following arrays
     DS_cstrn **ds_curve,     // in : ds curve cstrn ordered list
     intcurve **acis_curve,   // in : acis intcurves ordered list
     double *scale_vec) ;     // in : orig_dmod_space=scale_vec*internal_pfunc_space

  // ATTRIB_DS2ACIS API_get function sequence

/**
 * Builds a <tt>DS_dmod</tt> from the owner's shape.
 * <br><br>
 * <b>Role:</b> This is run by <tt>Refresh_data</tt>.
 * <br><br>
 * @param dsmodel
 * in: current shape model in patch hierarchy.
 * @param parent_dmod
 * in: dmod acting as this new dmod's parent.
 */                           // use: run by Refresh_data
  virtual                  // bas: ATTRIB_DM2DACIS virtual function
  DS_dmod *                // rtn: DS_dmod for input dsmodel
  Owner_2DS_dmod           // eff: build a DS_dmod from owner's shape
  (ATTRIB_DSMODEL *dsmodel,// in : current shape model in patch_hierarchy
   DS_dmod *parent_dmod) ; // in : dmod acting as this new dmod's parent

/**
 * Adds model edges as curve constraints.
 * <br><br>
 * @param dmod
 * in: tgt dmod to update.
 * @param dsmodel
 * in: src attrib_dsmodel for data.
 * @param future_parent
 * out: parent.
 */
  virtual void              // bas: ATTRIB_DM2ACIS virtual
  Add_solid_model_cstrns    // eff: add model edges as crv_cstrns
  (DS_dmod *dmod,           // in : tgt dmod to update
   ATTRIB_DSMODEL *dsmodel, // in : src attrib_dsmodel for data
   DS_dmod *future_parent); // in : parent of dmod (just not set yet)

/*
// tbrv
*/
/**
 * @nodoc
 */
  virtual int              // rtn: tag identifier
   Add_spring_set          // eff: add a set of springs to surf
    (int pt_count,         // in : number of springs in spring_set
     const SPAposition *targets,     // in : Spring free end pts [xyz0..,xyzN]
     double gain,          // in : stiffness of the spring
     int tag_flag,         // in : specify tgt dmod
     const SPApar_pos *guesses=NULL);// in : optional guesses for parameters corresponding to targets



  // ATTRIB_DS2ACIS implementations of ATTRIB virtual functions
  // roll_notify(), split_owner(), merge_owner(), trans_owner()
  // use those defined for parent ATTRIB_DM2ACIS

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_DS2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_DS2ACIS

//=============================================================
// class ATTRIB_DC2ACIS Derived from ATTRIB_AGC
//=============================================================
// 26-Apr-99 erik: Removed obsolete is_<class> documentation template.

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_ADM int ATTRIB_DC2ACIS_TYPE;
#define    ATTRIB_DC2ACIS_LEVEL (ATTRIB_DM2ACIS_LEVEL + 1)

/**
 * Connects the deformable curve functions to the ACIS modeler.
 * <br>
 * <b>Role:</b> This is the glue code to connect the deformable modeler functions
 * to the ACIS modeler. Given an ACIS Entity build a deformable model.
 * Save the user inputs on the ACIS model. This object will be stored as an attribute
 * attached to the <tt>ATTRIB_DSGROUP</tt> which is attached to the <tt>ENTITY</tt> being sculpted to
 * automatically handle deletion and garbage collection problems. Derived classes support
 * deformable surfaces and deformable curves.
 * <br><br>
 * The contained <tt>dmod</tt> and its associated rendering data are temporary.
 * They can be completely generated from the geometry and the information stored
 * in an <tt>ATTRIB_DSMODEL</tt> attribute on an <tt>ATTRIB_DSGROUP</tt> attached to an ACIS <tt>FACE</tt> or <tt>EDGE</tt>.
 * These objects will be generated at the start of an editing session and deleted at the end.
 * <br><br>
 * Modifications to the dynamic model during an editing session are accumulated only
 * in the <tt>DS_dmod</tt> model. At the user's discretion those changes can be committed to
 * the <tt>ATTRIB_DSMODEL</tt>. Commits can be rolled forward and back. Edits to the <tt>DS_dmod</tt>
 * model can not be rolled back. <tt>ATTRIB_DM2ACIS</tt> is an attribute associated to the ACIS
 * <tt>ENTITY</tt> being edited that holds pointers to all the different models used during a
 * single edit session. At the end of an edit session the attribute and all the data
 * it points to may be deleted with a call to the API function <tt>api_remove_attrib_ds2acis_from_owner</tt>.
 * <br><br>
 * During sculpting one <tt>ATTRIB_DM2ACIS</tt> may point to hierarchy of <tt>DS_dmod</tt> deformable objects
 * each of which may contain any number of loads and constraints. Every <tt>DS_dmod</tt>, load,
 * and constraint is assigned a unique tag number. Tag number 2 is assigned to the root
 * <tt>DS_dmod</tt>, and all subsequent tag objects are assigned tag values in the order in which
 * they are created. Tag values are guaranteed to remain the same before and after a
 * Save and Restore cycle. Tag values of deleted objects are not reused.
 * <br><br>
 * The active <tt>DS_dmod</tt>: While sculpting a patch hierarchy of <tt>DS_dmod</tt>'s,
 * the <tt>ATTRIB_DM2ACIS</tt> saves an active <tt>DS_dmod</tt> pointer. The function <tt>Make_active</tt>,
 * and any of the methods that operate on or find a constraint or load, make one
 * of the members of the hierarchy active. All I/O methods that act on the values of
 * a <tt>DS_Dmod</tt> or its loads and constraints act on the active member of the hierarchy
 * by default. The tag number of the active member can be found by using the Active method.
 * The <tt>ATTRIB_DM2ACIS</tt> active feature helps simplify the user interface by not
 * requiring that the end-user always specify a <tt>DS_dmod</tt> object by name for every
 * method call. Graphics are supported to distinguish the active <tt>DS_dmod</tt> for
 * the end-user during a sculpting session.
 * <br><br>
 * <tt>d2a_editing</tt>: Some ADM functions cause the split, merge, and lose callbacks
 * on ADM attributes to be called. These same functions are also launched by other
 * ACIS functions, like the Booleans. The behavior of those callbacks may differ
 * depending on who causes the callback to be executed. All ADM commands that edit
 * the <tt>ATTRIB_DM2ACIS</tt> object and which may cause the callbacks to fire, will set the
 * <tt>d2a_editing</tt> value to 1 when starting and clear the value back to 0 when completing.
 */
class DECL_ADM ATTRIB_DC2ACIS: public ATTRIB_DM2ACIS  // identifer = "c2a"
{
  protected:

  public:
 /**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DC2ACIS(...)</tt>), because this reserves the memory
 * on the heap, a requirement to support roll back and history management.
 * <br><br>
 * If <tt>doublept_icon_rad</tt> is -1, then the point icon radius is not changed.
 * <br><br>
 * @param edge_owner
 * new attrib owner.
 * @param pt_icon_rad
 * point icon radius size.
 */
   ATTRIB_DC2ACIS              // Default Constructor
    (ENTITY *edge_owner=NULL,  // new attrib owner(root parent's EDGE)
     float pt_icon_rad= -1.0);  // pt icon radius  size (-1=leave as is)
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_DC2ACIS(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param c2a
 * attribute to copy.
 * @param owner
 * entity to receive attribute.
 */
  ATTRIB_DC2ACIS               // Copy Constructor
   (ATTRIB_DC2ACIS &c2a,       // in : attribute being copied
    ENTITY *owner) ;           // in : owner for new attribute
/**
 * Assignment operator.
 * <br><br>
 * @param c2a
 * in: attribute with values to copy.
 */
  ATTRIB_DC2ACIS &operator=    // assignment operator
   (const ATTRIB_DC2ACIS &c2a);// in : attribute with values to copy
/**
 * Makes and adds a patch to the patch hierarchy.
 * <br><br>
 * @param owner
 * in: owner for new attribute.
 */
  virtual ATTRIB_AGC*          // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)    // in : owner for new attribute
                               { return(ACIS_NEW ATTRIB_DC2ACIS(*this,owner));
                               }
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
  virtual logical pattern_compatible() const;
                                    // returns TRUE
  // ATTRIB_DC2ACIS multi-surface management
/**
 * Connects quad faces into a quilt, returning the tag for the new root sibling or -1 if none.
 * <br><br>
 * @param face
 * in: face to connect to hierarchy.
 * @param tag
 * out: tag_shift = amount face tag numbers have shifted new_tag_id= old_tag_id + tag_shift.
 */
 virtual                  // bas: ATTRIB_DM2AICS pure virtual function
  int                      // rtn: tag for new root sibling or -1 for none
   Add_multi_face          // eff: Connect quad faces into a quilt
    (FACE * face,               // in : face = Face to connect to hierarchy
     int  &tag)               // out: tag_shift = amount face tag numbers have shifted
                           //      new_tag_id = old_tag_id + tag_shift
                           // note: sets active patch
                           { // unsupported function for curves
								SPAUNUSED(face)
								SPAUNUSED(tag)
								sys_error(DS_CODE_NOT_COMPLETE) ;
								return(-1) ;
                           }
/**
 * Disconnects the face from the quilt of faces.
 * <br><br>
 * @param tag
 * in: tag = face id to remove from hierarchy.
 */
  virtual                  // bas: ATTRIB_DM2AICS pure virtual function
  void
   Rm_multi_face                 // eff: Disconnect face from quilt
    (int tag)                 // in : tag = Face id to remove from hierarchy
                           { // unsupported function for curves
								SPAUNUSED(tag)
								sys_error(DS_CODE_NOT_COMPLETE) ;
                           }

  // ATTRIB_DC2ACIS Patch function support
/**
 * Creates and adds a patch to the patch hierarchy.
 * <br><br>
 * <b>Role:</b> Returns the tag for the new patch dmod or -1 for none.
 * This should always be called between <tt>API_BEGIN</tt> and <tt>API_END</tt> and between
 * <tt>Set_editing</tt> and <tt>Clear_editing</tt>.
 * <br><br>
 * @param shape_flag
 * in: shape_flag,0 use seams,1 use rectangle, 2 ellipse.
 * @param min
 * in: pt1,rect = min-corner ellipse = center point.
 * @param max
 * in: pt2,rect = max-corner ellipse = 1st axis end point.
 * @param minmax
 * in: pt3, rect not used ellipse = 2nd axis end point.
 * @param refinement
 * in: parent knot spacing - child knot spacing.
 * @param curve
 * in: number of curves in seam.
 * @param arrcurve
 * in: array of seam curves.
 * @param tag_flag
 * in: specify parent tgt_dmod to get new patch 1 active dmod, 2 root dmod, else member dmod with tag==tag_flag.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  int                      // rtn: tag for new patch dmod or -1 for none
   Add_patch               // eff: make and add a patch to patch hierarchy
   (int shape_flag,                   // in : shape_flag = not-used
    SPApar_pos min,           // in : min-domain (only uses the u component)
    SPApar_pos max,           // in : max-domain (only uses the u component)
    SPApar_pos minmax,               // in : pt3        = not-used
    int refinement,        // in : parent_knot_spacing/child_knot_spacing
    int curve,                   // in : seam_count = not-used
    DS_pfunc ** arrcurve,           // in : seam       = not-used
    int tag_flag) ;        // in : specify parent tgt_dmod to get new patch
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
/**
 * Removes and deletes a patch from the hierarchy.
 * <br><br>
 * <b>Role:</b> This should always be called between <tt>API_BEGIN</tt> and <tt>API_END</tt>.
 * <br><br>
 * @param tgt_dmod
 * in: specifies the tgt_dmod to be removed 1 active dmod 2 root dmod else member dmod with tag==tag_flag.
 */
  virtual                  // bas: ATTRIB_DM2ACIS pure virtual function
  void Rm_patch            // eff: remove and delete patch from hierarchy
   (int tgt_dmod) ;                 // in :  tag_flag=1 = specify the tgt_dmod to be copied
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
                           // note: deletes all offspring as well.
                           // NOTE: always call between API_BEGIN/API_END

  // ATTRIB_DC2ACIS API_commit function sequence
/**
 * Moves data to face and <tt>ATTRIB_DSMODEL</tt>.
 */
  virtual                  // bas: ATTRIB_DM2ACIS virtual function
   void                    // use: called by api_commit_attrib_ds2acis_to_face()
    Commit_to_owner() ;    // eff: move data to face and ATTRIB_DSMODEL
/**
 * Recursive tail of <tt>Commit_to_owner</tt>.
 * <br><br>
 * @param dmod
 * in: current target shape to commit.
 */

  void                     // use: called by ATTRIB_DC2ACIS::Commit_to_owner
   Commit_to_edgeR         // eff: recursive tail of Commit_to_owner()
    (DS_dmod *dmod) ;      // in : current target shape to commit

  // ATTRIB_DC2ACIS API_get function sequence

/**
 * Builds a <tt>DS_dmod</tt> from owner's shape.
 * <br><br>
 * <b>Role:</b> Run by <tt>Refresh_data</tt>.
 * <br><br>
 * @param dsmodel
 * in: current shape model in patch hierarchy.
 * @param parent_dmod
 * in: dmod acting as this new dmod's parent.
 */
 // use: run by attrib_ds2acis constructor
  virtual                  // bas: ATTRIB_DM2DACIS virtual function
  DS_dmod *                // rtn: DS_dmod for input dsmodel
  Owner_2DS_dmod           // eff: build a DS_dmod from owner's shape
  (ATTRIB_DSMODEL *dsmodel,// in : current shape model in patch_hierarchy
   DS_dmod *parent_dmod) ; // in : dmod acting as this new dmod's parent
/**
 * Adds model edges as curve constraints.
 * <br><br>
 * <b>Role:</b> This is called by <tt>Owner_2DS_dmod</tt>.
 * <br><br>
 * @param dmod
 * in: target dmod to  update.
 * @param dsmodel
 * in: source attrib_dsmodel for data.
 * @param future_parent
 * parent of dmod.
 */
  virtual void              // bas: ATTRIB_DM2ACIS virtual function
  Add_solid_model_cstrns    // eff: add model edges as crv_cstrns
  (DS_dmod *dmod,           // in : tgt dmod to update
   ATTRIB_DSMODEL *dsmodel, // in : src attrib_dsmodel for data
   DS_dmod *future_parent); // in : parent of dmod (just not set yet)
/*
// tbrv
*/
/**
 * @nodoc
 */
  virtual int              // rtn: tag identifier
   Add_spring_set          // eff: add a set of springs to surf
    (int pt_count,         // in : number of springs in spring_set
     const SPAposition *targets,     // in : Spring free end pts [xyz0..,xyzN]
     double gain,          // in : stiffness of the spring
     int tag_flag,         // in : specify tgt dmod
     const SPApar_pos *guesses=NULL);// in : optional guesses for parameters corresponding to targets


  // ATTRIB_DC2ACIS implementations of ATTRIB virtual functions
  // roll_notify(), split_owner(), merge_owner(), trans_owner()
  // use those defined for parent ATTRIB_DM2ACIS

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_DC2ACIS, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_DC2ACIS



//=======================================================
// Global Functions - for debugging and convenience only
//=======================================================

/*
// tbrv
*/
/**
 * @nodoc
 */
FACE *                    // rtn: pointer to new FACE object
DS_dmod_to_face           // eff: build a face from a dmod
  (DS_dmod *dmod) ;       // in : input dmod (nested into rtn face)

/*
// tbrv
*/
/**
 * @nodoc
 */
void                          // eff: remove all DS attributes from
DS_remove_boundary_attributes //      the boundaries of an entity
  (ENTITY *entity) ;          // in : entity to clean
/*
// tbrv
*/
/**
 * @nodoc
 */
int                       // rtn: 1=negate norm direction,0=don't
DS_negate_flag_for_face   // eff: check sense and left_handed_uv
  (FACE *face) ;          // in : FACE to query

 /** @} */
#endif // end ATTRIB_DM2ACIS_ATTRB_H

