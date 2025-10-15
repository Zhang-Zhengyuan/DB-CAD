/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SIZE_ACCUMULATOR )
#define SIZE_ACCUMULATOR

#include <stddef.h>
#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"

#include "vlists.hxx"
 
class DECL_BASE SizeAccumulator : public ACIS_OBJECT
{
	protected:
		int	m_totalSize;
		int m_lastSize;
        VOID_LIST hit_list;

	public:
		SizeAccumulator() : m_totalSize(0), m_lastSize(0) {}
		void clearSize() { m_totalSize = 0; }
		void clearLast() { m_lastSize = 0; }
		void operator += (int in) { m_lastSize += in; m_totalSize += in;}; 
		int getSize() { return m_totalSize; };
		int getLast() { int s = m_lastSize; m_lastSize = 0; return s; };
		logical can_process(void*, int = 0);
};

#endif
