/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for routines for miscellaneous geometric enquiries.
#ifndef GEOMETRY_ROUTINES 
#define GEOMETRY_ROUTINES
#include "dcl_kern.h"
#include "logical.h"
#include "param.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
class COEDGE;
class EDGE;
class SPAtransf;
class SPAparameter;
class FACE;
class COEDGE;
class EDGE;
class SPAtransf;
class pcurve;
/**
 * \defgroup ACISEVALUATORS Evaluators
 * \ingroup KERNAPI
 */
/**
 * @file geometry.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISEVALUATORS
 * @{
 */
/**
 * Returns either the start or end position of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start position if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's end position.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition coedge_start_pos(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the middle position of the of the EDGE underlying a COEDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition coedge_mid_pos(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns either the start or end position of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's end position if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's start position.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition coedge_end_pos(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the position on the EDGE underlying a COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * position is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine
 * @param
 * Parameter at which to evaluate.
 * @param transf
 * Transform to apply to the edge position.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAposition coedge_param_pos(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = FALSE
		);
/**
 * Returns the position at the start of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition edge_start_pos(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the middle position of an EDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition edge_mid_pos(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the position at the end of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition edge_end_pos(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the position on an EDGE corresponding to a specified parameter value.
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf
 * Transform to apply to the edge position.
 */
DECL_KERN SPAposition edge_param_pos(
			EDGE const *edge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns either the start or end parameter of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start parameter if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's end parameter.
 * @param coedge
 * COEDGE to examine.
 */
DECL_KERN double coedge_start_param( COEDGE const *coedge );
/**
 * Returns either the start or end parameter of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's end parameter if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's start parameter.
 * @param coedge
 * COEDGE to examine.
 */
DECL_KERN double coedge_end_param( COEDGE const *coedge );
/**
 * Returns the direction at either the start or end of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start direction if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the negative of the edge's end direction.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * direction is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAunit_vector coedge_start_dir(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the direction at the middle position of the EDGE underlying a COEDGE, or its negation.
 * <br><br>
 * <b>Role:</b> Returns the direction at the edge's middle position if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the negative of that direction.  The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * direction is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAunit_vector coedge_mid_dir(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the direction at either the start or end of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's end direction if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the negative of the edge's start direction.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * direction is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAunit_vector coedge_end_dir(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the direction on the EDGE underlying a COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> Returns the edge direction if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the negative of this direction.  The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * direction is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf
 * Transform to apply to the edge direction.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAunit_vector coedge_param_dir(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the direction at the start of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 */
DECL_KERN SPAunit_vector edge_start_dir(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns an EDGE's direction corresponding to the middle position of the EDGE's parameter range.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 */
DECL_KERN SPAunit_vector edge_mid_dir(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction at the end of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge direction.
 */
DECL_KERN SPAunit_vector edge_end_dir(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction on an EDGE corresponding to a specified parameter value.
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf
 * Transform to apply to the edge direction.
 */
DECL_KERN SPAunit_vector edge_param_dir(
			EDGE const *edge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);

/**
 * Returns either the start or end curvature of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start curvature if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's end curvature.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * curvature is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAvector coedge_start_curv(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the curvature at the middle position of the EDGE underlying a COEDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * curvature is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAvector coedge_mid_curv(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns either the start or end curvature of the EDGE underlying a COEDGE,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's end curvature if the coedge's sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's start curvature.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * curvature is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAvector coedge_end_curv(
			COEDGE const *coedge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the curvature on the EDGE underlying a COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  When <tt>nominal</tt> is set to FALSE and the coedge is a TCOEDGE, the
 * curvature is taken from the TCOEDGE's 3D curve instead of from the EDGE curve.
 * @param coedge
 * COEDGE to examine.
 * @param param
 * Parameter to evaluate.
 * @param transf
 * Transform to apply to the edge curvature.
 * @param nominal
 * Use a TCOEDGE's 3D curve, if set to FALSE.
 */
DECL_KERN SPAvector coedge_param_curv(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF,
			logical nominal = TRUE
		);
/**
 * Returns the curvature corresponding to the start parameter of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 */
DECL_KERN SPAvector edge_start_curv(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the curve curvature at the middle range parameter of an EDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 */
DECL_KERN SPAvector edge_mid_curv(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the curve curvature at the end parameter of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param transf
 * Transform to apply to the edge curvature.
 */
DECL_KERN SPAvector edge_end_curv(
			EDGE const *edge,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the curve curvature at the give parameter of an EDGE.
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf
 * Transform to apply to the edge curvature.
 */
DECL_KERN SPAvector edge_param_curv(
			EDGE const *edge,
			double param,
			SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);

/**
 * Returns the minimum radius of curvature of an EDGE.
 * @param edge
 * EDGE to examine.
 */
DECL_KERN double edge_min_radius_curv(
			EDGE const *edge
		);
/**
 * Returns the minimum radius of curvature of the EDGE underlying a COEDGE.
 * @param coedge
 * COEDGE to examine.
 */
DECL_KERN double coedge_min_radius_curv(
			COEDGE const *coedge
		);
/**
 * Returns the direction of the normal of a FACE at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's start position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's end position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * The face whose normal is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_start_norm(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at the middle position of the EDGE
 * underlyling a COEDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * The face whose normal is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_mid_norm(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's end position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's start position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * The face whose normal is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_end_norm(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at a position on the EDGE underlying a
 * COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * @param coedge
 * COEDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * The face whose normal is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_param_norm(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at the start of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_start_norm(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at the middle position of an EDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  An input face must be supplied,
 * since an edge may be associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_mid_norm(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at the end of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_end_norm(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction of the normal of a FACE at a position on an EDGE,
 * given a parameter value.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * Transform to apply to the face normal.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_param_norm(
			EDGE const *edge,
			double param,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's start position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's end position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * The face whose surface's outward direction is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_start_outdir(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at the middle position of the
 * EDGE underlying a COEDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * The face whose surface's outward direction is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * COEDGE geometry. 
 */
DECL_KERN SPAunit_vector coedge_mid_outdir(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's end position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's start position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * The face whose surface's outward direction is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_end_outdir(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at a position on the EDGE underlying a
 * COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param coedge
 * COEDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * The face whose surface's outward direction is desired.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN SPAunit_vector coedge_param_outdir(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF
		);

/**
 * Returns the direction outwards from a surface at the start of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_start_outdir(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at the middle position of an EDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  An input face must be supplied, since
 * an edge may be associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_mid_outdir(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at the end of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param edge
 * EDGE to examine.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_end_outdir(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the direction outwards from a surface at a position on an EDGE,
 * given a parameter value.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * The direction returned by this routine is usually the normal to the surface, but if the
 * point is a singularity of the surface (like the apex of a cone or one apex of a
 * degenerate torus), it just returns some direction guaranteed to point outwards
 * from the surface (and not tangential, except in the case of a vortex).
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * Transform to apply to the outward direction.
 * @param face
 * FACE to be evaluated.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param pcur
 * Parametric curve associated with edge and face.
 */
DECL_KERN SPAunit_vector edge_param_outdir(
			EDGE const *edge,
			double param,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			pcurve const & pcur = *(pcurve *)NULL_REF
		);
/**
 * Returns the cross-curvature at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's start position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's end position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN double coedge_start_cross(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at the middle position of the
 * EDGE underlying a COEDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN double coedge_mid_cross(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at either the start or end of the
 * EDGE underlying a COEDGE, depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Uses the edge's end position if the coedge sense is <tt>FORWARD</tt>;
 * otherwise, uses the edge's start position.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param coedge
 * COEDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN double coedge_end_cross(
			COEDGE const *coedge,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at a position on the
 * EDGE underlying a COEDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.  If <tt>face</tt> is given, it is
 * assumed that the COEDGE lies on it, and this face is used for evaluation of the normal;
 * otherwise, the FACE owning the coedge's LOOP is used.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the optional input face
 * into the same coordinate system as that of the the untransformed COEDGE geometry.
 * It should only be non-NULL if the optional face is given, and is used primarily
 * by Boolean operations when testing a graph COEDGE against BODY faces.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param coedge
 * COEDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 */
DECL_KERN double coedge_param_cross(
			COEDGE const *coedge,
			double param,
			SPAtransf const & transf1 = *(SPAtransf *)NULL_REF,
			FACE const *face = NULL,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
            logical use_sided_eval = FALSE
		);

/**
 * Returns the cross-curvature at the start of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param rhs_curv
 * For cases of discontinuity, use the value to the right of the test point if TRUE.
 * @param pcur
 * Parametric curve associated with edge and face.
 */ 
DECL_KERN double edge_start_cross(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			logical rhs_curv = FALSE,
			pcurve const & pcur = *(pcurve *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at the middle position of an EDGE.
 * <br><br>
 * <b>Role:</b> The middle position is defined to be the position corresponding to
 * the middle parameter of the edge's range.  An input face must be supplied, since
 * an edge may be associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param rhs_curv
 * For cases of discontinuity, use the value to the right of the test point if TRUE.
 * @param pcur
 * Parametric curve associated with edge and face.
 */ 
DECL_KERN double edge_mid_cross(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			logical rhs_curv = FALSE,
			pcurve const & pcur = *(pcurve *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at the end of an EDGE.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param edge
 * EDGE to examine.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param rhs_curv
 * For cases of discontinuity, use the value to the right of the test point if TRUE.
 * @param pcur
 * Parametric curve associated with edge and face.
 */ 
DECL_KERN double edge_end_cross(
			EDGE const *edge,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			logical rhs_curv = FALSE,
			pcurve const & pcur = *(pcurve *)NULL_REF,
            logical use_sided_eval = FALSE
		);
/**
 * Returns the cross-curvature at at a position on an EDGE, given a parameter value.
 * <br><br>
 * <b>Role:</b> An input face must be supplied, since an edge may be
 * associated with multiple faces.  If <tt>transf2</tt> is
 * given, it represents the transformation needed to translate the input face
 * into the same coordinate system as that of the edge.  If the face's surface
 * is parametric, and there exists a parametric curve (pcurve) for the input 
 * edge with respect to the surface, then the parametric curve should be passed
 * as the argument <tt>pcur</tt> to speed up finding the normal.
 * <br><br>
 * Cross-curvature refers to the curvature of the curve that is the
 * intersection of the surface with a plane through the test point
 * and normal to the coedge or edge direction, or to a given direction.
 * By convention, where the intersection has a discontinuity of
 * curvature, it is the value to the left of the test point, looking
 * along the given direction with the surface normal upwards, which is
 * returned. For edges, this convention can be reversed. In all cases,
 * the curvature is positive if the surface is convex (that is, curves
 * away from the outward normal) and negative if it is concave.
 * @param edge
 * EDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf1
 * A transform to be applied to the position and direction used for the evaluation.
 * @param face
 * The face used in the cross-curvature evaluation.
 * @param transf2
 * A transform to be applied to the <tt>face</tt> argument.
 * @param rhs_curv
 * For cases of discontinuity, use the value to the right of the test point if TRUE.
 * @param pcur
 * Parametric curve associated with edge and face.
 */ 
DECL_KERN double edge_param_cross(
			EDGE const *edge,
			double param,
			SPAtransf const & transf1,
			FACE const *face,
			SPAtransf const & transf2 = *(SPAtransf *)NULL_REF,
			logical rhs_curv = FALSE,
			pcurve const & pcur = *(pcurve *)NULL_REF,      
            logical use_sided_eval = FALSE
		);
/**
 * @nodoc
 * Returns whether or not a given position is within an edge's extremes.
 * <br><br>
 * <b>Role:</b> Returns TRUE if the input parameter is between the start and end parameters
 * or if the input position is within the tolerance of the start or end positions.
 * @param pos
 * Position to be tested.
 * @param par
 * Parameter to be tested.
 * @param edge
 * EDGE to be tested against.
 */
DECL_KERN logical pt_on_edge(
			SPAposition const & pos,
			SPAparameter const & par,
			EDGE const *edge
		);
/**
 * Returns the surface u and v parameters for a given parameter on the EDGE
 * underlying a COEDGE.
 * <br><br>
 * <b>Role:</b> The parameter <tt>param</tt> is taken with respect to the coedge
 * range.
 * @param coedge 
 * COEDGE to examine.
 * @param param
 * Parameter at which to evaluate.
 * @param transf
 * A transform to be applied to the position used for the evaluation.
 */
DECL_KERN SPApar_pos coedge_uv_param(
		     COEDGE const *coedge,
		     double param,
		     SPAtransf const & transf = *(SPAtransf *)NULL_REF
		);
/** @} */
#endif
