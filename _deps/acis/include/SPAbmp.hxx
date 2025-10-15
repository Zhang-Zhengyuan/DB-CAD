/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#pragma once
#include <stdio.h>
#ifdef NT
	#include "windows.h"
#else
	typedef const char* LPCTSTR;
#endif
#include "dcl_base.h"
#include "option.hxx"

extern option_header BMP_file_paths;

class DECL_BASE SPAbmp {
	int		m_width;
	int		m_height;
public:
	SPAbmp() : m_width(0), m_height(0) {}
	~SPAbmp() {}
	int	write(char * in_filename, unsigned char * in_image, int in_width, int in_height, bool in_invert = true);
	unsigned char * read(char * in_filename, bool in_invert = true, bool in_bgr_reverse = true); // calling code must free the returned image buffer
	unsigned char * read(FILE * in_fp, bool in_invert = true, bool in_bgr_reverse = true); // calling code must free the returned image buffer
	int get_width() { return m_width; }
	int get_height() { return m_height; }
};

// Code to convert from a DIB to a Hoops image
void InsertRGBImage(int width, int height, unsigned char* pPxl, LPCTSTR , LPCTSTR hoops_name);
DECL_BASE unsigned char * convert_RGB_bytes(int width, int height, unsigned char* pPxl, LPCTSTR hoops_name);

DECL_BASE FILE * acis_fopen_path(const char * in_filename, char * in_openstyle, char * in_paths);
