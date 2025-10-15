/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
 * @file off_wire.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup OFSTAPI
 *  @{
 */
#ifndef OFFSET_WIRE_HXX
#define OFFSET_WIRE_HXX
/**
* Defines the method to use to close an offset gap.
* <br>
* @param arc
* close with an arc.
* @param corner
* close with a corner - extend linearly until intersection.
* @param natural
* natural close - extend naturally until intersection.
**/
enum sg_gap_type {
		arc,
        corner,
	    natural    // Added by paul new option
};
/*! @} */
#endif
