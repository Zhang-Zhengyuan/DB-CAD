// $Id: blend_thread_ctx.hxx,v 1.9 2002/08/09 17:28:25 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef blend_thread_ctx_hxx
#define blend_thread_ctx_hxx
#include "context_cb.hxx"

#include "debugmsc.hxx"

#include "dcl_blnd.h"

#include "logical.h"

#include "interval.hxx"

#include "param.hxx"

#include "polyadic.hxx"

#include "tim.hxx"

class blend_graph;

class misc_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
#ifdef D3_TIMES
	/* GLOBAL_VAR */ TIMER_BUCKET blend1_misc_bucket;
#endif
	misc_thread_ctx();
	~misc_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class blseq_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* GLOBAL_VAR */ blend_graph *current_blend_graph ; // = NULL;
	blseq_thread_ctx();
	~blseq_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class capping_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
#ifdef D3_TIMES
		/* LOCAL_VAR */ TIMER_BUCKET CAP_INTERSECT_BUCKET;
#endif
	capping_thread_ctx();
	~capping_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class int_stat_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
#ifdef D3_TIMES
	/* GLOBAL_VAR */ TIMER_BUCKET int_edges_surf_bucket;
	/* GLOBAL_VAR */ TIMER_HANDLE int_edges_surf_handle;
		/* LOCAL_VAR */ TIMER_BUCKET extend_and_intersect_bucket;
		/* LOCAL_VAR */ TIMER_HANDLE extend_and_intersect_handle;
#endif
	int_stat_thread_ctx();
	~int_stat_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class bl_vr_ff_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ double max_delta;
	/* LOCAL_VAR */ double last_delta ; // = 0;
	bl_vr_ff_thread_ctx();
	~bl_vr_ff_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class def_crv_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ int __cnt__ ; // = 11;
	def_crv_thread_ctx();
	~def_crv_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class findgeom_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ unsigned i ; // = 1;
	findgeom_thread_ctx();
	~findgeom_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class march_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ logical Still_Marching ; // = TRUE;
	march_thread_ctx();
	~march_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class step_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ SPAinterval range;
	/* LOCAL_VAR */ double max_delta;
	/* LOCAL_VAR */ double last_result;	// need these in case prev_slice is NULL.
	/* LOCAL_VAR */ double last_delta;
		/* LOCAL_VAR */ double fit_tol;
	step_thread_ctx();
	~step_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};



class xspring_thread_ctx : public ACIS_OBJECT {
	int count_accesses;
public:
	/* LOCAL_VAR */ int n_reps;
	/* LOCAL_VAR */ SPApar_pos last_uv;
	xspring_thread_ctx();
	~xspring_thread_ctx();
	void clear();
	void clear_no_dtors();
	void inc_access() {INCREMENT_ACCESS_COUNT}
	int get_access() {return count_accesses;}
};


class blend_thread_ctx : public context_callbacks {
	misc_thread_ctx m_misc;
	blseq_thread_ctx m_blseq;
	capping_thread_ctx m_capping;
	int_stat_thread_ctx m_int_stat;
	bl_vr_ff_thread_ctx m_bl_vr_ff;
	def_crv_thread_ctx m_def_crv;
	findgeom_thread_ctx m_findgeom;
	march_thread_ctx m_march;
	step_thread_ctx m_step;
	xspring_thread_ctx m_xspring;

	int count_accesses;
public:
	misc_thread_ctx& misc() { m_misc.inc_access(); return m_misc; }
	blseq_thread_ctx& blseq() { m_blseq.inc_access(); return m_blseq; }
	capping_thread_ctx& capping() { m_capping.inc_access(); return m_capping; }
	int_stat_thread_ctx& int_stat() { m_int_stat.inc_access(); return m_int_stat; }
	bl_vr_ff_thread_ctx& bl_vr_ff() { m_bl_vr_ff.inc_access(); return m_bl_vr_ff; }
	def_crv_thread_ctx& def_crv() { m_def_crv.inc_access(); return m_def_crv; }
	findgeom_thread_ctx& findgeom() { m_findgeom.inc_access(); return m_findgeom; }
	march_thread_ctx& march() { m_march.inc_access(); return m_march; }
	step_thread_ctx& step() { m_step.inc_access(); return m_step; }
	xspring_thread_ctx& xspring() { m_xspring.inc_access(); return m_xspring; }
	blend_thread_ctx() : count_accesses(0) {}
	~blend_thread_ctx() {}
	void inc_access() {INCREMENT_ACCESS_COUNT}

	void clear_no_dtors() {
		m_misc.clear_no_dtors();
		m_blseq.clear_no_dtors();
		m_capping.clear_no_dtors();
		m_int_stat.clear_no_dtors();
		m_bl_vr_ff.clear_no_dtors();
		m_def_crv.clear_no_dtors();
		m_findgeom.clear_no_dtors();
		m_march.clear_no_dtors();
		m_step.clear_no_dtors();
		m_xspring.clear_no_dtors();
	}
	void clear() {
		m_misc.clear();
		m_blseq.clear();
		m_capping.clear();
		m_int_stat.clear();
		m_bl_vr_ff.clear();
		m_def_crv.clear();
		m_findgeom.clear();
		m_march.clear();
		m_step.clear();
		m_xspring.clear();
	}
	void print_access() {
		fprintf(debug_file_ptr, "misc %d\n", m_misc.get_access());
		fprintf(debug_file_ptr, "blseq %d\n", m_blseq.get_access());
		fprintf(debug_file_ptr, "capping %d\n", m_capping.get_access());
		fprintf(debug_file_ptr, "int_stat %d\n", m_int_stat.get_access());
		fprintf(debug_file_ptr, "bl_vr_ff %d\n", m_bl_vr_ff.get_access());
		fprintf(debug_file_ptr, "def_crv %d\n", m_def_crv.get_access());
		fprintf(debug_file_ptr, "findgeom %d\n", m_findgeom.get_access());
		fprintf(debug_file_ptr, "march %d\n", m_march.get_access());
		fprintf(debug_file_ptr, "step %d\n", m_step.get_access());
		fprintf(debug_file_ptr, "xspring %d\n", m_xspring.get_access());
		fprintf(debug_file_ptr, "Total for blend_thread_ctx %d\n", count_accesses);
	}
};

#ifdef THREAD_SAFE_ACIS

extern safe_object_pointer<blend_thread_ctx> blend_thread_ctx_ptr;

DECL_BLND blend_thread_ctx* blend_context() ;

#else

extern blend_thread_ctx* blend_thread_ctx_ptr;  // Statically allocated in init file

inline DECL_BLND blend_thread_ctx* blend_context() {
	blend_thread_ctx_ptr->inc_access(); 
	return blend_thread_ctx_ptr;
}

#endif /* THREAD_SAFE_ACIS */

#endif /* blend_thread_ctx_hxx */

