/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 09-Apr-03 decl export DM_stub_seg_callback for use in dslonet_stub
//
#ifndef DM_STUB_SEG_CB_HEADER 
#define DM_STUB_SEG_CB_HEADER

#include "dm_seg_cb.hxx"        // class DM_legacy_seg_callback        // class DM_legacy_seg_callback
#include "dcl_dm2stub.h"

class DS_pfunc;

class DECL_DM2STUB DM_stub_seg_callback : public DM_legacy_seg_callback
{
   friend class DM_legacy_seg_callback_list;

// Should not call these
   DM_stub_seg_callback&
   operator=                               // Assignment Operator
   (const DM_stub_seg_callback&);

   DM_stub_seg_callback(const DM_stub_seg_callback&) ; // Copy constructor

 
protected:

public:

    virtual ~DM_stub_seg_callback();
 
    DM_stub_seg_callback();

    virtual double *Legacy_CW_segmenter
      (void *src_data, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count); 

    virtual double *Legacy_pfunc_segmenter
      (DS_pfunc *crv_pfunc, DS_pfunc *surf_pfunc, int knot_count_u, double *knot_u, 
       int *cont_u, int knot_count_v, double *knot_v, int *cont_v, int *&xsect_cont, int &seg_count); 

    virtual int Legacy_pfunc_1d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double max_dist, double *image_pt,
       double &iline_u, double *pfunc_uv);

    virtual int Legacy_pfunc_2d_xsect_iline
      (DS_pfunc *pfunc, double *p0, double *p1, double *image_pt,
       double &iline_u, double *pfunc_uv);

    virtual void Legacy_delete_double_array
       (double *ptr);

    virtual void Legacy_delete_int_array
       (int *ptr);
};

#endif // DM_STUB_SEG_CB_HEADER
