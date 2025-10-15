/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//  Description : Rendering Entities
#if ( ! defined( RENDERER_ENTITY_HEADER ) )
#define RENDERER_ENTITY_HEADER
#include "dcl_rb.h"
#include "logical.h"
#include "entity.hxx"
#include "usecount.hxx"
/**
 * @file rh_enty.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */
extern DECL_RB int RH_ENTITY_TYPE;
#define RH_ENTITY_LEVEL 1

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE ( RH_ENTITY, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_MATERIAL, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_LIGHT, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_BACKGROUND, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_FOREGROUND, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_TEXTURE_SPACE, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( RH_ENVIRONMENT_MAP, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


/*
// tbrv
*/
/**
 * @nodoc
 */
typedef int (*save_fn)( RH_ENTITY * e );

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef RH_ENTITY * (*restore_fn)( RH_ENTITY * e );

/*
// tbrv
*/
/**
 * @nodoc
 */
void rh_set_external_savres ( save_fn, restore_fn );

//
// Flags for RH_ENTITY save/restore
//

#ifndef ADV_RENDER_HUSK
#define ADV_RENDER_HUSK
#endif
/**
 *.
 */
#define RH_NO_DATA       0
/**
 *.
 */
#define RH_EXTERNAL_DATA 1
/**
 *.
 */
#define RH_STANDARD_DATA 2
/**
 *.
 */
#define RH_ADVANCED_DATA 4
/**
 *.
 */
#define DATA_ID RH_ADVANCED_DATA

/**
 * Provides common methods and data for other rendering classes.
 * <br>
 * <b>Role:</b> Rendering entities provide the basis for manipulating the
 * appearance of ACIS geometric entities, image backgrounds and lighting conditions.
 * Child classes include <tt>RH_BACKGROUND</tt>, <tt>RH_FOREGROUND</tt>, <tt>RH_ENVIRONMENT_MAP</tt>,
 * <tt>RH_LIGHT</tt>, <tt>RH_MATERIAL</tt>, and <tt>RH_TEXTURE_SPACE</tt>.
 * <br><br>
 * The primary constructors for <tt>RH_LIGHT</tt>, <tt>RH_FOREGROUND</tt>, and <tt>RH_BACKGROUND</tt>
 * require a parameter that specifies the type of shader of that class.
 * The parameterless constructors for these types create a <tt>NULL</tt> entity that has
 * no effect in terms of rendering, but are supplied to support attribute save
 * and restore.
 */
class DECL_RB RH_ENTITY : public ENTITY
{

   //
   // Save Restore function pointers, used to hold addresses of
   // application provided save and restore functions
   //

   protected:

      void *li_handle;   // Internal Handle
      int   ext_id;      // External id

      void set_handle( void *handle ) ;
      void set_id( int new_id ) { ext_id = new_id ; }

      //
      // Internal save and restore functions. Render Entity Data may be
      // stored either internally, that is will the ACIS Geometry or
      // externally in a application database/file. Which option is used
      // depends on whether or not the application has provided pointers
      // to save restore functions ( see below ).
      //

      logical saveres_external() const; // query save/restore  method

      //
      // Each Render entity has its own internal save/restore methods
      //

      virtual void save_internal()  const;
      virtual void restore_internal( int data_id );

      //
      //  External Save/Restore
      //
      //  External save and restore is performed by two application
      //  supplied functions. These are set using the friend function
      //  rh_set_external_savres . If the two functions are not provided
      //  ( one or both are NULL ), then the render husk will in the case
      //   of the standard rendering husk save and restore from the current
      //  ACIS sat file. In the case of the advanced husk,
      //  no action is performed
      //
      //  Only an external 'id' number is actually saved with the ACIS file
      //  The full set of parameters necessary to restore the RH_ENTITY
      //  must be saved in an external file/data base.
      //  Typically the external id would be used by a database manager
      //  to access the parameters of a RH_MATERIAL say. These parameters
      //  would then be used to recreate that material, using the appropriate
      //  api functions.
      //

      void save_external() const;
      void restore_external();

   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 */
      RH_ENTITY()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
      }

      //
      // Get the internal handle
      //

/**
 * Returns a pointer to the internal data structure for this entity.
 */
      void* handle() const { return ( li_handle ) ; }

      //
      // Get the external id
      //
/**
 * Returns the ID for the <tt>RH_ENTITY</tt> list.
 */
      int id() const { return ( ext_id ) ; }

      //
      // Check that a RH_ENTITY has a valid internal handle
      //
/**
 * Returns <tt>TRUE</tt> if the handle pointing to the entity's internal data is valid - else, it returns <tt>FALSE</tt> if the handle is <tt>NULL</tt>.
 */
      logical check_handle() ;


      //
      // Give details of render entity, usually shader parameters
      //
/**
 * Supports the debug mechanism by providing details of the background.
 * <br><br>
 * @param fp
 * file pointer.
 */
      virtual void debug_details( FILE * fp ) const;

      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_ENTITY, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


// ********************************************************************
// Function:
//    rh_set_external_savres
//
// Action:
//    Sets an external ID number to be saved and restored with the file.
//
// Prototype:
//    void rh_set_external_savres (
//        safe_fn,
//        restore_fn
//        );
//
// Includes:
//    #include "kernel/acis.hxx"
//    #include "rnd_husk/include/rh_enty.hxx"
//
// Description:
//    External save and restore is performed by two application
//    supplied methods, save_external and restore_external, that are set using the
//    rh_set_external_savres function.
//
//    Only an external ID number is actually saved with the ACIS file. The full set of
//    parameters necessary to restore the RH_ENTITY must be saved in an external
//    file/data base. Typically, the external ID is used by a database manager to
//    access the parameters of a RH_MATERIAL. These parameters are then used to
//    recreate that material, using the appropriate API functions.
//
// Errors:
//    None
//
// Limitations:
//    Not applicable
//
// Library:
//    rnd_husk
//
// Filename:
//    rbase/rnd_husk/include/rh_enty.hxx
//
// Effect:
//    System routine
//
// Access:
//    document
//
// Mechanisms:
//    Rendering Control
//
// ********************************************************************

	/**
	 * @nodoc
	 */
	friend void rh_set_external_savres( save_fn, restore_fn );
} ;


extern DECL_RB int RH_MATERIAL_TYPE;
#define RH_MATERIAL_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines a material consisting of color, displacement, reflectance, and transparency.
 * <br>
 * <b>Role:</b> A material defines the appearance of the surface of an ACIS
 * topological entity in terms of four components: color, reflectance,
 * transparency, and displacement.
 * <br><br>
 * A color defines the color for any point on the surface of an entity to which
 * applies and can be a simple single color or a complex pattern, such as a
 * procedurally-defined marble effect.
 * <br><br>
 * The reflectance governs how the surface behaves visually in the presence of
 * light. The reflectance defines the surface finish of an entity and models
 * effects, such as matte, metal, or mirrored surfaces. Reflectance is not
 * supported in the flat or <tt>gouraud</tt> rendering modes.
 * <br><br>
 * Transparency defines how transparent or opaque a surface is and the consequent
 * visibility of entities that lie behind that surface. Transparency is not
 * supported in the flat or <tt>gouraud</tt> rendering modes. The transparency component
 * of a material can be switched on or off. If switched on, the transparency
 * component of a material takes effect for those rendering modes where it
 * is applicable.
 * <br><br>
 * A displacement simulates small surface perturbations by modifying the surface
 * normal vector. They are an efficient method of simulating surface features
 * such as regular indentations, which would be difficult or inefficient to model
 * geometrically. Displacement is not supported in the flat or <tt>gouraud</tt> rendering
 * modes. The displacement component of a material can be switched on or off.
 * If switched on, the displacement component of a material takes effect for
 * those rendering modes where it is applicable.
 * <br><br>
 */
class DECL_RB RH_MATERIAL : public RH_ENTITY
{
   protected:

      void save_internal() const;
      void restore_internal( int data_id );

   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 */
      RH_MATERIAL()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
		  use_count_data = 0;  // STI jmb:
     }
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the
 * overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param old
 * old material.
 */
      RH_MATERIAL( RH_MATERIAL *old ) ;

      //
      // Give details of render entity, usually shader parameters
      //
/**
 * Supports the debug mechanism by providing details of the material.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
      //
      // Standard entity declarations
      //

      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_MATERIAL, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	  /**
	   * @nodoc
	   */
	  USE_COUNTED_DECL // STI jmb:
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * Usually use counted entities are marked not deletable because their lifetime is controlled by the entities that point to them.
 */
	  virtual logical deletable() const; // STI jmb

      //
      // create a material with a pre-defined  li_handle
      //
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs a material, initializing it using the internal data structure
 * pointed to by handle.
 * <br><br>
 * @param handle
 * data structure.
 */
      RH_MATERIAL( void * handle );

} ;



extern DECL_RB int RH_LIGHT_TYPE;
#define RH_LIGHT_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines a light source.
 * <br>
 * <b>Role:</b> <tt>RH_LIGHT</tt>s define light sources within the Renderer. Supported
 * light source types are <tt>ambient</tt>, <tt>distant</tt>, <tt>eye</tt>, 
 * <tt>point</tt>, and <tt>spot</tt>.
 * <br><br>
 * An enumerated type, <tt>Fall_Off_Type</tt>, is a parameter to some light types which
 * selects how the intensity of the light varies with the distance from the
 * light source, and has possible values of <tt>FALL_OFF_CONSTANT</tt>, <tt>FALL_OFF_INVERSE</tt>,
 * or <tt>FALL_OFF_INVERSE_SQUARE</tt>.
 * <br><br>
 * Shadowing is supported for distant, point, and spot in all rendering modes
 * except flat and simple. If an image is to be rendered with shadows, a shadow
 * map must be computed before rendering, using <tt>api_rh_create_light_shadow</tt> for
 * each light for which shadows are required. A shadow map is view-independent
 * and can be reused for any number of images provided there is no change in
 * the light source geometry or the entities it illuminates.
 */
class DECL_RB RH_LIGHT : public RH_ENTITY
{
// STI DJP Begin...
   private:
      logical is_active;	// True if this this light is in the active light list
// STI DJP End.
   protected:
      void save_internal() const;
      void restore_internal( int data_id );
   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 */
      RH_LIGHT()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
		  is_active = FALSE;
      }
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new RH_LIGHT(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 * <br><br>
 * Overloads the C++ <tt>new</tt> operator to allocate space on the portion of the heap
 * controlled by ACIS. This is used in conjunction with the other constructors.
 * The C++ <tt>sizeof</tt> function can be used to obtain the <tt>size_t</tt> of the object.
 * <br><br>
 * @param old
 * old light.
 */
      RH_LIGHT( RH_LIGHT *old ) ;

      //
      // Primary constructor for a light. The 'name' should be
      // one of the published light types, eg. "eye" or " spot"
      //
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs a light of the type specified by the character string. This string
 * should be the name of an implemented shader, such as <tt>ambient</tt> or <tt>spot</tt>
 * depending on the renderer.
 * <br><br>
 * @param name
 * type of light.
 */
      RH_LIGHT( const char * name ) ;

      //
      // Give details of render entity, usually shader parameters
      //
/**
 * Supports the debug mechanism by providing details of the light.
 * <br><br>
 * @param fp
 * @param debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
      //
      //
      // Standard entity declarations
      //

      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_LIGHT, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	  // Interface for changing active status
/**
 * Marks the light as being active (<tt>TRUE</tt>) or inactive (<tt>FALSE</tt>).
 * <br><br>
 * <b>Role:</b> Multiple lights can be active at any given time.
 * <br><br>
 * @param flag
 * active or not.
 */
      void active(logical flag) { is_active = flag; }

	  //
	  // Rollback notification
	  //
	/**
	 * Field notification of a rollback operation. The first argument specifies the type of bulletin
	 * being rolled back (create, delete or change). In the case of a change notification, the second
	 * argument contains the information to be restored into this object.
	 * <br><br>
	 * @param bulletin_type
	 * type of bulletin being rolled back.
	 * @param ent
	 * info to be restored into this argument.
	 */
	virtual void roll_notify( BULLETIN_TYPE bulletin_type, ENTITY * ent);

// STI pes Begin...
	  // Interface for checking active status

/**
 * Interface for checking the active state.
 */
      logical active_state() { return is_active; }
// STI pes End.
} ;


extern DECL_RB int RH_BACKGROUND_TYPE;
#define RH_BACKGROUND_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines a background.
 * <br>
 * <b>Role:</b> This class defines the color of a pixels at any point in the
 * image which is not covered by an entity surface. A background can comprise
 * a single uniform color or pattern, or can be composed of a
 * previously-generated image or an image scanned from a photograph. Only one
 * background can be active at any one time.
 * <br><br>
 * The primary constructors for <tt>RH_BACKGROUND</tt>s accept a parameter to identify
 * the type of background. The parameterless constructor creates a <tt>NULL</tt> entity
 * that has no effect in terms of rendering, but it is supplied to support
 * attribute save and restore.
 */
class DECL_RB RH_BACKGROUND : public RH_ENTITY
{
// STI DJP Begin...
   private:
      logical is_active;	// True if this is the current active background
// STI DJP End.
   protected:
      void save_internal() const;
      void restore_internal( int data_id );
   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 */
      RH_BACKGROUND()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
		  is_active = FALSE;
      }

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * @param old
 * old background.
 */
      RH_BACKGROUND( RH_BACKGROUND *old ) ;

      //
      // Primary constructor for a background. The 'name' should be
      // one of the published background types, eg. "plain" or "graduated"
      //
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs a background of the type specified by the character string.
 * This string should be the name of an implemented shader, such as <tt>"plain"</tt> or
 * <tt>"graduated"</tt> depending on the renderer.
 * <br><br>
 * @param name
 * type of background.
 */
      RH_BACKGROUND( const char * name );

      //
      // Give details of render entity, usually shader parameters
      //
/**
 * Supports the debug mechanism by providing details of the background.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
      //
      //
      // Standard entity declarations
      //
      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_BACKGROUND, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	  // Interface for changing active status
/**
 * Marks the background as being active (<tt>TRUE</tt>) or inactive (<tt>FALSE</tt>).
 * <br><br>
 * <b>Role:</b> Only the single active background participates in rendering operations.
 * <br><br>
 * @param flag
 * active.
 */
      void active(logical flag) { is_active = flag; }

	  //
	  // Rollback notification
	  //
	/**
	 * Field notification of a rollback operation.
	 * <br><br>
	 * <b>Role:</b> The first argument specifies the type of bulletin
	 * being rolled back (create, delete or change). In the case of a change notification, the second
	 * argument contains the information to be restored into this object.
	 * <br><br>
	 * @param bulletin_type
	 * type of bulletin being rolled back.
	 * @param ent
	 * info to be restored into this argument.
	 */
	virtual void roll_notify( BULLETIN_TYPE bulletin_type, ENTITY * ent);
} ;


extern DECL_RB int RH_FOREGROUND_TYPE;
#define RH_FOREGROUND_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines a foreground.
 * <br>
 * <b>Role:</b> A foreground is the counterpart to a background. Foreground shaders
 * provide an extra level of image processing during the shading process. It can
 * be thought of as a filter and may be used to support atmospheric effects, such
 * as fog or depth cueing. Only one foreground can be active at any given time.
 * <br><br>
 * The primary constructors for <tt>RH_FOREGROUND</tt>s accept a parameter to identify
 * the type of foreground. The parameterless constructor creates a <tt>NULL</tt> entity
 * that has no effect in terms of rendering, but it is supplied to support
 * attribute save and restore.
 */
class DECL_RB RH_FOREGROUND : public RH_ENTITY
{
// STI DJP Begin...
   private:
      logical is_active;	// True if this is the current active foreground
// STI DJP End.
   protected:
      void save_internal() const;
      void restore_internal( int data_id );
   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications
 * should call this constructor only with the overloaded <tt>new</tt> operator, because
 * this reserves the memory on the heap, a requirement to support roll back and
 * history management.
 */
      RH_FOREGROUND()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
		  is_active = FALSE;
      }

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * @param old
 * type of old foreground.
 */
      RH_FOREGROUND( RH_FOREGROUND *old ) ;

      //
      // Primary constructor for a foreground. The 'name' should be
      // one of the published foreground types, eg. "fog" or "depth cue"
      //
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs a foreground of the type specified by the character string.
 * This string is the name of an implemented shader, such as <tt>"fog"</tt> depending on
 * the renderer.
 * <br><br>
 * @param name
 * type for foreground.
 */
      RH_FOREGROUND( const char * name );

      //
      // Give details of render entity, usually shader parameters
      //
/**
 * Supports the debug mechanism by providing details of the foreground.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
      //
      //
      // Standard entity declarations
      //
      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_FOREGROUND, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	  // Interface for changing active status
/**
 * Marks the foreground as being active (<tt>TRUE</tt>) or inactive (<tt>FALSE</tt>).
 * <br><br>
 * <b>Role:</b> Only one foreground can be active during a rendering operation.
 * <br><br>
 * @param flag
 * active or not.
 */
      void active(logical flag) { is_active = flag; }

	  //
	  // Rollback notification
	  //
	/**
	 * Field notification of a rollback operation.
	 * <br><br>
	 * <b>Role:</b> The first argument specifies the type of bulletin
	 * being rolled back (create, delete or change). In the case of a change notification, the second
	 * argument contains the information to be restored into this object.
	 * <br><br>
	 * @param bulletin_type
	 * type of bulletin being rolled back.
	 * @param ent
	 * info to be restored into this argument.
	 */
	virtual void roll_notify( BULLETIN_TYPE bulletin_type, ENTITY * ent);
} ;



extern DECL_RB int RH_TEXTURE_SPACE_TYPE;
#define RH_TEXTURE_SPACE_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines a texture space.
 * <br>
 * <b>Role:</b> <tt>RH_TEXTURE_SPACE</tt> entities assist in the production of a shading
 * effect known as a wrapped texture. A wrapped texture produces the effect of
 * a sheet of paper shrink wrapped onto the surface of a solid object. A texture
 * space uses one of several texture space shaders to map between the coordinate
 * system of the sheet and the coordinate system of the surface of the solid
 * object. Texture space arguments are treated in a similar fashion to those of
 * material components.
 */
class DECL_RB RH_TEXTURE_SPACE : public RH_ENTITY
{
      void save_internal() const;
      void restore_internal( int data_id );
   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications
 * should call this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 */
      RH_TEXTURE_SPACE()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
		  use_count_data = 0;  // STI jmb
      }

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * @param old
 * old texture space.
 */
      RH_TEXTURE_SPACE( RH_TEXTURE_SPACE *old );

      //
      // Primary constructor for a texture space. The name should
      // be one of the published texture space types eg. "z plane"
      // or "spherical"
      //

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs a texture space of the type specified by the character string.
 * This string should be the name of an implemented shader, such as <tt>"x"</tt> or
 * <tt>"cylindrical"</tt> depending on the renderer.
 * <br><br>
 * @param name
 * type of texture space.
 */
      RH_TEXTURE_SPACE( const char * name );

      //
      // Give details of render entity, usually shader parameters
      //

/**
 * Supports the debug mechanism by providing details of the texture space.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
      //
      //
      // Standard entity declarations
      //
      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_TEXTURE_SPACE, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	  /**
	   * @nodoc
	   */
	  USE_COUNTED_DECL // STI jmb:
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * Usually use counted entities are marked not deletable because their lifetime is controlled by the entities that point to them.
 */
	  virtual logical deletable() const; // STI jmb
} ;


class RH_ENV_DESC ;
extern DECL_RB int RH_ENVIRONMENT_MAP_TYPE;
#define RH_ENVIRONMENT_MAP_LEVEL  ( RH_ENTITY_LEVEL + 1 )

/**
 * Defines an environment map.
 * <br>
 * <b>Role:</b> Environment maps simulate interobject reflections, both between
 * bodies in a scene and between a body and the external environment.
 * <tt>RH_ENVIRONMENT_MAP</tt> objects are used with one of the component shaders of the
 * other rendering entities specified by an <tt>RH_MATERIAL</tt>.
 * <br><br>
 * Environment mapping is supported for all shading modes, except flat and simple.
 * Interobject reflections can be simulated by computing an environment map for
 * the currently defined entities using the function, <tt>api_rh_render_cube_environment</tt>.
 * More commonly, a set of scanned images form the basis for a environment mapping
 * using <tt>api_rh_create_cube_environment</tt>. The application should not construct
 * environment maps directly; instead, it should use the appropriate API.
 */
class DECL_RB RH_ENVIRONMENT_MAP : public RH_ENTITY
{
      RH_ENV_DESC          *data;
      void save_internal() const;
      void restore_internal( int data_id );
   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 */
      RH_ENVIRONMENT_MAP()
      {
          ext_id = -1 ;
          li_handle = (void *)NULL ;
          data = NULL;
      }

      //
      //  create an environment map from an environment map descriptor
      //  If logical immediate is set, then the internal LightWorks
      //  data will be generated immediately
      //

      // STI: pes mac doesn't like extraneous RH_ENVIRONMENT_MAP::
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * If immediate is set, then the internal data is generated immediately.
 * <br><br>
 * @param desc
 * descriptor.
 * @param immediate
 * generate dataset.
 */
      RH_ENVIRONMENT_MAP
      (
         RH_ENV_DESC * desc,
         logical immediate  = TRUE
      );

      //
      //  Build an internal LightWorks environment map from the
      //  data parameters that describe the RH_ENVIRONMENT_MAP
      //

      // STI: pes mac doesn't like extraneous RH_ENVIRONMENT_MAP::
/**
 * Builds an internal environment map from the data parameters that describe the <tt>RH_ENVIRONMENT_MAP</tt>.
 */
      void evaluate();

      //
      // Give details of render entity, usually shader parameters
      //

/**
 * Supports the debug mechanism by providing details of the environment map.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug_details( FILE * fp ) const;

      //
      // lookup function for Acis debugging
      //

      /**
       * @nodoc
       */
      LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

      //
      // Standard entity declarations
      //
      /**
       * @nodoc
       */
      ENTITY_FUNCTIONS( RH_ENVIRONMENT_MAP, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ;

/** @} */
#endif
