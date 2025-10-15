/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SABFILE_H
#define SABFILE_H

//---------------------------------------------------------------------
// Define the SabFile class for doing ACIS save and restore to
// stream files using the new binary format which supports unknown
// ENTITY data.

//---------------------------------------------------------------------

#include <stdio.h>
#include "dcl_kern.h"
#include "binfile.hxx"
#include "tagdata.hxx"

/**
* @file sabfile.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */
//---------------------------------------------------------------------
/**
 * Performs save and restore to stream files.
 * <br>
 * <b>Role</b>: This class performs ACIS save and restore to stream files using
 * the new binary format that supports unknown ENTITY data.
 */
class DECL_KERN SabFile : public BinaryFile 
{
protected:

    FILE* m_pFILE;

protected:

    // These are the virtual methods which actually do the reading
    // and writing of the data.  They must be implemented for all
    // derived classes.

    virtual size_t read(void* buf, size_t length, logical swap);
    virtual void write(const void* data, size_t len, logical swap);

public:

    // The methods for positioning the file pointer must also be
    // implemented for each derived class.
/**
 * Returns the current file position within the SabFile.
 */
    virtual FilePosition set_mark();
/**
 * Moves the file pointer to the specified position in the SabFile.
 * @param fpos
 * file position
 */
    virtual FilePosition goto_mark(FilePosition fpos);

	// get information about the file
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual logical get_file_data( FileData* );


/**
 * Returns the string "sabfile".
 */
	virtual const char *file_type_name() const;

/**
 * C++ constructor, creating a SabFile using the specified parameters.
 * @param fp
 * file pointer
 */
    SabFile(FILE* fp);
/**
 * C++ destructor, deleting a SabFile.
 */
    virtual ~SabFile();

};

//---------------------------------------------------------------------
/** @} */
#endif
