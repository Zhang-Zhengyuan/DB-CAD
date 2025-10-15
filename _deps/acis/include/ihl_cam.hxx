/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Defines a camera for IHL.                                          */
/**********************************************************************/
#if ! defined ( IHL_CAM_INCLUDE )
/**********************************************************************/
#include "position.hxx"
#include "en_ihl.hxx"
/**
 * @file ihl_cam.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <ihl_cam.hxx>
 *  @{
 */
/**********************************************************************/
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(IHL_CAMERA, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
extern DECL_IHL int IHL_CAMERA_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define IHL_CAMERA_LEVEL (ENTITY_LEVEL + 2)

/**********************************************************************/
/**
 * Defines a camera viewpoint against which hidden lines are calculated.
 * <br>
 * <b>Role:</b> Each camera is defined by its position (eye position) in global model space,
 * its aim (target position) in global model space, and by a perspective flag. The perspective
 * flag is <tt>TRUE</tt> for a simple perspective projection, where two positions define the view direction
 * and the distance between eye and target. The flag is <tt>FALSE</tt> for a parallel projection, where the
 * eye and target positions define the view direction, and the distance is not relevant.
 * <br><br>
 * The orientation of the camera (i.e., rotation about its own view axis) has no influence on the
 * result of the hidden line calculation.
 * <br><br>
 * <tt>IHL_CAMERA</tt> constructors make a new bulletin entry in the current bulletin board to record the
 * creation of the camera.
 */
class DECL_IHL IHL_CAMERA : public ENTITY_IHL
{
protected :
    SPAposition cam_eyepos,              // eye SPAposition
             cam_target;              // target SPAposition
    logical  cam_persp;               // TRUE if perspective projection

public :
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
 * call this constructor only with the overloaded <tt>new</tt> operator, because this reserves the memory
 * on the heap, a requirement to support roll back and history management.
 */
    IHL_CAMERA() {}
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param cam
 * camera.
 */
    IHL_CAMERA(
        const IHL_CAMERA& cam);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param eyepos
 * eye position.
 * @param target
 * target position.
 * @param fPersp
 * TRUE for perspective projection.
 */
    IHL_CAMERA(
        const SPAposition& eyepos,       // eye SPAposition
        const SPAposition& target,       // target SPAposition
        logical         fPersp );     // TRUE if perspective projection

    // Inquiry functions

    // Returns eye SPAposition
/**
 * Returns the eye position.
 */
    const SPAposition& eyepos() const { return(cam_eyepos); }

    // Returns target SPAposition
/**
 * Returns the target position.
 */
    const SPAposition& target() const { return(cam_target); }

    // Returns TRUE if perspective
/**
 * Returns the perspective flag.
 */
    logical perspective() const { return(cam_persp); }

    // Compares two cameras
/**
 * Returns <tt>TRUE</tt> if the eye position, target position, and perspective projection flag of the supplied camera are all the same as those of this camera.
 * <br><br>
 * @param to_chk
 * camera to check.
 */
    logical operator == (
        const IHL_CAMERA& to_chk ) const;
/**
 * Returns <tt>TRUE</tt> if either the eye position, target position, or perspective projection flag of the supplied camera differ from those of this camera.
 * <br><br>
 * @param to_chk
 * camera to check.
 */
    logical operator != (
        const IHL_CAMERA& to_chk ) const;

    // Modification functions
/**
 * Sets the eye position.
 * <br><br>
 * @param eyepos
 * new eye position.
 */
    void set_eyepos(
        const SPAposition& eyepos );
/**
 * Sets the target position.
 * <br><br>
 * @param target
 * new target position.
 */
    void set_target(
        const SPAposition& target );
/**
 * Sets the perspective flag.
 * <br><br>
 * @param fPersp
 * new perspective flag,TRUE for perspective projection.
 */
    void set_perspective(
        logical fPersp );

    // Include the standard member functions for all entities.
    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS( IHL_CAMERA, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};

/**********************************************************************/
#define IHL_CAM_INCLUDE
/* @} */
#endif
/**********************************************************************/
