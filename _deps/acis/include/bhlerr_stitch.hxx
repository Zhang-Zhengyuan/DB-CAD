/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_ERROR_MESS)
#define BHL_ERROR_MESS

//#include <fstream.h>
#include "dcl_stitch.h"

typedef int MesgCodeType;

/* BHL_MESG_TYPES - the different types of messages
* 		VERBATIM_MSG - Print the SPAparameter string verbatim
*		HEADER_MSG   - The SPAparameter string is to be printed as a heading
*		LISTED_MSG   - The message is a standard message associated with
*				a message code which is passed as SPAparameter
*/
enum BHL_MESG_TYPES { VERBATIM_MSG, HEADER_MSG}; //, LISTED_MSG};

/* Function to print the messages in the log file. The global variable
*   bhl_log_file is set apriori to the log file.
*/
DECL_STITCH void
BhlLogMessage(BHL_MESG_TYPES msg_level,
		const char *msg_str,
		  MesgCodeType msg_code = 0);

// A structure to hold the identifier/message pair
typedef struct DECL_STITCH {
   char const * p_MesgShort;			// a terse message identifier
   char const *	p_MesgDetailed;			// the detailed message string
} MessageStruct;

// Class to hold the messages for a module
class DECL_STITCH BhlMessageModule{
private:
   int 		m_ModuleId;					// unique id for every module
   char const* 	p_ModuleName;			// name of the module
   int 		m_MesgCnt;					// number of messages in the module
   MessageStruct const* p_MesgTable;	// table of messages
public:
	// constructor for the module.
   BhlMessageModule( char const*,			// name of this module
					MessageStruct const*);	// array of messages terminated by NULL

   // Destructor for the module
   ~BhlMessageModule();

   // Returns name of the module
   char const*  	GetModuleName() const;

   // Returns the number of messages
   int                  GetMessageCnt();

   // Returns a unique code number for the message
   MesgCodeType  	MessageCode( int) const;

   // Returns the terse message
   char const *		GetMessageShort( int) const;

   // Returns the detailed message string
   char const *		GetMessageDetailed( int) const;
};

// Function to add a message module to the global module table
//static int AddMesgModule( BhlMessageModule*);

// Get a pointer to the module given offset to the module table
DECL_STITCH BhlMessageModule*  GetModule( int);

#endif
