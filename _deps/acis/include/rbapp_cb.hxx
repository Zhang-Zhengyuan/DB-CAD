/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RBASE_APP_HXX )
#define RBASE_APP_HXX

#include "dcl_rb.h"
#include "tlkit_cb.hxx"

/**
* @file rbapp_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */
/**
 * Implements routines to handle various rendering application callbacks for image output and interrupts.
 * <br>
 * <b>Role:</b> Callbacks handled include starting and stopping image output,
 * output of a scanline of image data to the application, and interrupt handling.
 */
class DECL_RB rbase_app_callback : public toolkit_callback
{
	friend class rbase_app_callback_list;

protected:
	virtual ~rbase_app_callback();

public:
/**
 * Terminates image output.
 * <br><br>
 * @param frmno
 * frame number.
 */
	virtual void image_end(int frmno);
/**
 * Begins image output.
 * <br><br>
 * @param frmno
 * frame number of image.
 * @param ploc1
 * upper-left pixel loc.
 * @param ploc2
 * upper-left pixel loc.
 * @param iwidth
 * image width in pixels.
 * @param iheight
 * image height in pixels.
 * @param factor
 * scaling factor.
 * @param cscreen
 * clear screen before displaying.
 */
	virtual void image_start(int frmno, int ploc1, int ploc2, int iwidth, int iheight, float factor, int cscreen);
/**
 * Displays a scanline of image data.
 * <br><br>
 * @param yraster
 * y raster line value.
 * @param pdata
 * array of pixel data.
 */
	virtual void image_scanline(int yraster, void *pdata);
/**
 * Handles user interrupts.
 */
	virtual int check_interrupt();
};

/**
 * @nodoc
 */
class DECL_RB rbase_app_callback_list : public toolkit_callback_list
{
public:
	void add(rbase_app_callback *cb) { toolkit_callback_list::add(cb); }
	void append(rbase_app_callback *cb) { toolkit_callback_list::append(cb); }

	void image_end(int);

	void image_start(int, int, int, int, int, float, int);

	void image_scanline(int, void *);

	int check_interrupt();
};
/**
 * Creates a register of the callback.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cb
 * callback to register.
 **/
DECL_RB void add_rbase_app_cb(rbase_app_callback *cb);

/**
 * Removes the callback.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cb
 * given callback.
 **/
DECL_RB void remove_rbase_app_cb(rbase_app_callback *cb);
/** @} */
#endif
