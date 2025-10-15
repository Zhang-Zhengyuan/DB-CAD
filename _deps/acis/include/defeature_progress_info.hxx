/*******************************************************************/
/*    Copyright (c) 2006-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DEFEATURE_PROGRESS_INFO_HXX
#define DEFEATURE_PROGRESS_INFO_HXX
#include "spa_progress_info.hxx"
enum SPA_defeature_progress_op_id {RECOGNIZE,REMOVE,BOTH};
class DEFEATURE_progress_info : public SPA_progress_info
{
	private:
		logical m_update_done;
		logical m_canceled;
		int m_percentage;
		SPA_defeature_progress_op_id m_op_id;
	public:
		DEFEATURE_progress_info();
		~DEFEATURE_progress_info();
		virtual int percentage() const;
		virtual SPA_defeature_progress_op_id  get_op() const;
		virtual void set_op(SPA_defeature_progress_op_id op_id);
		virtual void set_percentage(int per);
		virtual SPA_progress_info_type_id get_type() const;
		logical update();
};
#endif

