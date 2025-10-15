// COPYRIGHT DASSAULT SYSTEMES 1989
#if !defined( PHLV5_OPTIONS_CLASS )
#define PHLV5_OPTIONS_CLASS
#include "acis.hxx"
#include "dcl_phlv5.h"
/**
 * @file phlv5_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup PHLV5API
 *  \brief Declared at <phlv5_opts.hxx>
 *  @{
 */
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
enum phlv5_hidden_line_style
{
	PHLV5_HIDDEN,
	PHLV5_DASHED,
	PHLV5_COLORED,
    PHLV5_THICK_AND_THINNING
};

/**
 * Specifies options for hidden line removal.
 * <br>
 * <b>Role:</b> Options for hidden line removal are supplied to APIs via this class.
 * Use the "set" and "get" methods to build the object with the desired option
 * settings, then pass this as the last argument to <tt>api_phlv5_compute</tt>.
 * <br><br>
 * The options that may be set are:
 * <br><br>
 * <table>
 * <tr>
 * <td valign="top" width="20%"><i>Resolution</i></td>
 * <td>The smallest meaningful quantity in PHLV5. Reduce this value if you are viewing parts
 * modeled in very small coordinates. The default is 0.001.</td>
 * </tr>
 * <tr>
 * <td valign="top"><i>Sag Resolution</i></td>
 * <td>The resolution used to determine if two lines are occluding each other or not. The default is 0.02.</td>
 * </tr>
 * <tr>
 * <td valign="top"><i>Self-Calibration</i></td>
 * <td>If this is TRUE, PHLV5 computes its own resolution values based on the model size. This is highly recommended.
 * The default is TRUE.</td>
 * </tr>
 * <tr>
 * <td valign="top"><i>smooth_cosine</i></td>
 * <td>Sets the cosine of the angle at which two faces can meet and still be considered tangentially smooth.  A value of
 * 1.0 will produce no smooth edges and essentially turns the option off (default). A value of 0.95 will trap reasonably smooth edges.</td>
 * </tr>
 * <tr>
 * <td valign="top"><i>Surface Approximation</i></td>
 * <td>If this is TRUE, PHLV5 uses the approximations of the surface instead of the actual procedural surface.
 * The default is TRUE.</td>
 * </tr>
 * </table><br>
 * The hidden line style can also be set in <tt>phlv5_options</tt>. However, it is used
 * in the Scheme interface; it is ignored by <tt>api_phlv5_compute</tt>.
 */
class DECL_PHLV5 phlv5_options : public ACIS_OBJECT
{
private:
	phlv5_hidden_line_style m_hidden_line_style;
	double m_sag_resolution;
	double m_resolution;
	logical m_self_calibrate;
	logical m_use_approx_surface;
    double m_smooth;

public:
/**
 * C++ initialize constructor.
 */
	phlv5_options();
/**
 * C++ destructor.
 */
	~phlv5_options();
/**
 * Sets the hidden line style.
 * <br><br>
 * <b>Role:</b> The hidden line style is used in the Scheme interface; it is
 * ignored by <tt>api_phlv5_compute</tt>.
 * <br><br>
 * @param _in_style
 * style to use.
 */
	void set_hidden_line_style(phlv5_hidden_line_style _in_style) { m_hidden_line_style = _in_style; };
/**
 * Gets the current hidden line style.
 * <br><br>
 * <b>Role:</b> The hidden line style is used in the Scheme interface; it is
 * ignored by <tt>api_phlv5_compute</tt>.
 */
	phlv5_hidden_line_style get_hidden_line_style() { return m_hidden_line_style; };

/**
 * Sets the sag resolution.
 * <br><br>
 * @param in_resolution
 * sag resolution.
 */
	void set_sag_resolution(double in_resolution) {m_sag_resolution = in_resolution; };
/**
 * Gets the sag resolution.
 */
	double get_sag_resolution() {return m_sag_resolution; };
/**
 * Sets the resolution.
 * <br><br>
 * @param in_resolution
 * resolution.
 */
	void set_resolution(double in_resolution) {m_resolution = in_resolution;};
/**
 * Gets the resolution.
 */
	double get_resolution() {return m_resolution; };

/**
 * Sets the self-calibration option.
 * <br><br>
 * @param in_self_calibrate
 * self-cal flag.
 */
	void set_self_calibrate(logical in_self_calibrate) {m_self_calibrate = in_self_calibrate;};
/**
 * Gets the self-calibration option.
 */
	logical get_self_calibrate() {return m_self_calibrate; };
/**
 * Sets the surface approximation flag.
 * <br><br>
 * @param in_use_approx_surface
 * surface approx flag.
 */
	void set_self_approx_surface(logical in_use_approx_surface) {m_use_approx_surface = in_use_approx_surface;};
/**
 * Gets the surface approximation option.
 */
	logical get_use_approx_surface() {return m_use_approx_surface; };
/**
 * Sets the cosine of the angle at which two faces can meet and still be considered tangentially smooth.
 * <br><br>
 * @param smooth_angle
 * cosine of the angle between the two edges.
 */
    void set_smooth_cosine(double smooth_angle) {m_smooth = smooth_angle;};
/**
 * Gets the cosine of the angle between the two edges.
 */
    double get_smooth_cosine() {return m_smooth; };
};
/*! @} */
#endif
