/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//      Define image utilities
#ifndef rh_types_h
#define rh_types_h
/*
 * Check for ANSI compilation
 */
#ifdef __STDC__
#ifndef ANSI
#define ANSI
#endif /* ANSI */
#endif /* __STDC___ */
/**
 * @file rh_types.h
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 * \brief Declared at <rh_types.h>
 * @{
 */
/**
 *.
 */
#define         LwVoid          void

#ifndef LW_SHADER_TYPES
 /**
  *.
  */
#define   LW_CONVERSION_DITHER_RGB     (LwConversionMethod)0
 /**
  *.
  */
#define   LW_CONVERSION_FS_RGB         (LwConversionMethod)1
 /**
  *.
  */
#define   LW_CONVERSION_FS_DITHER_RGB  (LwConversionMethod)2
 /**
  *.
  */
#define   LW_CONVERSION_FS_MONOCHROME  (LwConversionMethod)3
 /**
  *.
  */
#define   LW_CONVERSION_GREYSCALE      (LwConversionMethod)4
 /**
  *.
  */
#define   LW_CONVERSION_NEAREST_RGB    (LwConversionMethod)5
 /**
  *.
  */
#define   LW_CONVERSION_O_DITHER_RGB   (LwConversionMethod)6
/**
 *.
 */
#define   LW_CONVERSION_TC_DITHER      (LwConversionMethod)7
/**
 * @nodoc
 */
typedef         char*           LwString ;
/**
 * 32-bit signed integer.
 * <br>
 * <b>Role</b>:A 32-bit, signed integer which represents values in the range -2^31 to (2^31) - 1.
 */
typedef         int             LwInt32 ;
/**
 * @nodoc
 */
typedef         int             LwBoolean ;
/**
 * Floating point value.
 * <br>
 * <b>Role</b>:A data type which represents a single precision floating point value.
 */
typedef         float           LwFloat ;
/**
 * @nodoc
 */
typedef         LwFloat         LwVector[3] ;
/**
 * @nodoc
 */
typedef         LwVector        LwPoint ;
/**
 * @nodoc
 */
typedef         LwVector        LwColour ;
/**
 * @nodoc
 */
typedef		char*		LwGenericPtr ;

#endif  /* LW_SHADER_TYPES */
/**
 * @nodoc
 */
typedef		char		LwChar ;
/**
 * @nodoc
 */
typedef		short		LwInt16 ;
/**
 * 8-bit unsigned integer.
 * <br>
 * <b>Role</b>:An 8-bit, unsigned integer which represents values in the range 0 to 255.
 */
typedef		unsigned char	LwNat8 ;
/**
 * @nodoc
 */
typedef		unsigned short	LwNat16 ;
/**
 * @nodoc
 */
typedef		unsigned int	LwNat32 ;
/**
 * @nodoc
 */
typedef		double		LwDouble ;
/**
 * @nodoc
 */
typedef		LwFloat		Lw2dPoint[2] ;
/**
 * @nodoc
 */
typedef		LwFloat		LwHVector[4] ;
/**
 * @nodoc
 */
typedef		LwHVector	LwHPoint ;
/**
 * @nodoc
 */
typedef		LwDouble	LwMatrix[4][4] ;
/**
 * @nodoc
 */
typedef		LwFloat		LwBound[6] ;
/**
 * @nodoc
 */
typedef		LwInt32		LwHandle ;
/**
 * Image conversion method type.
 * <br>
 * <b>Role</b>:A 32-bit, signed integer which represents an image conversion method type.
 */
typedef		LwInt32		LwConversionMethod ;
/**
 * Image display method.
 * <br>
 * <b>Role</b>:A 32-bit, signed integer which represents an image display method.
 */
typedef		LwInt32		LwDisplayMethod ;


/** @} */

#endif
