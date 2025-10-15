/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_LEGACY_SEG_CB_HEADER
#define DM_LEGACY_SEG_CB_HEADER

#include "tlkit_cb.hxx"        // class toolkit_callback        // class toolkit_callback
#include "dcl_ds.h"

class DS_pfunc;

class DECL_DM DM_legacy_seg_callback : public toolkit_callback
{
   friend class DM_legacy_seg_callback_list;

// Should not call these
   DM_legacy_seg_callback&
   operator=                               // Assignment Operator
   (const DM_legacy_seg_callback&);

   DM_legacy_seg_callback(const DM_legacy_seg_callback&) ; // Copy constructor

 
protected:

public:

   virtual ~DM_legacy_seg_callback(){};

    DM_legacy_seg_callback(){};

	virtual int get_priority() const {return 0;}

    virtual double *Legacy_CW_segmenter
      (void *src_data, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count)=0; 

    virtual double *Legacy_pfunc_segmenter
      (DS_pfunc *crv_pfunc, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count)=0; 

    virtual int Legacy_pfunc_1d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double max_dist, double *image_pt,
       double &iline_u, double *pfunc_uv)=0;

    virtual int Legacy_pfunc_2d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double *image_pt,
       double &iline_u, double *pfunc_uv)=0;

    virtual void Legacy_delete_double_array
       (double *ptr)=0;

    virtual void Legacy_delete_int_array
       (int *ptr)=0;
};

class DM_legacy_seg_callback_list : public toolkit_callback_list
{
 public:
	void add(DM_legacy_seg_callback *cb) { clear(); toolkit_callback_list::add(cb); }

    double *Legacy_CW_segmenter
      (void *src_data, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count) ; 

    double *Legacy_pfunc_segmenter
      (DS_pfunc *crv_pfunc, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count) ; 

    int Legacy_pfunc_1d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double max_dist, double *image_pt,
       double &iline_u, double *pfunc_uv);

    int Legacy_pfunc_2d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double *image_pt,
       double &iline_u, double *pfunc_uv);

    void Legacy_delete_double_array
       (double *ptr);

    void Legacy_delete_int_array
       (int *ptr);
};

void DECL_DM DS_add_DM_legacy_seg_cb(DM_legacy_seg_callback *& cb);
//void DS_remove_DM_legacy_seg_cb(DM_legacy_seg_callback *cb);
void DECL_DM DS_clear_DM_legacy_seg_cb_list();

        
#endif // DM_LEGACY_SEG_CB_HEADER
