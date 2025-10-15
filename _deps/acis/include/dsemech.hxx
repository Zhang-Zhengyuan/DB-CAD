// $Id: dsemech.hxx,v 1.13 2001/07/23 20:05:33 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsemech.hxx
// created  : September 13, 1999
// author   : John Sloan
// contains : classes DS_enforcement_mechanism
//                    DS_link_cstrn_mech
//                    DS_link_load_mech
//
// synopsis : Used to make link constraints
//            look like a constraint or a load

#ifndef DS_EMECH_H
#define DS_EMECH_H

//class DS_eqns;
#include "dseqns.hxx" // DS_eqns // DS_eqns
class DS_link_cstrn;
class DS_dmod;
class DS_area_load;

enum DM_xtan_trans {DM_XTAN_TRANS_NONE=0,
                    DM_XTAN_TRANS_LOCAL};

class DS_enforcement_mechanism : public ACIS_OBJECT
{
 public:

  enum TYPE_ID { ds_lcm=1,        // DS_link_cstrn_mech
                 ds_llm,          // DS_link_load_mech
                 ds_ccm,          // DS_curve_cstrn_mech
                 ds_clm,          // DS_curve_load_mech
                 ds_axm,          // DS_area_mech
                 ds_blm } ;       // Bad DS_link_mech

  virtual TYPE_ID Type_id() const=0;

  virtual 
   DS_enforcement_mechanism * Make_copy() const=0;

  virtual ~DS_enforcement_mechanism() {}

  virtual DM_xtan_trans Default_xtan_trans() const=0;

  virtual void Add_to_LHS(DS_eqns *eqns,
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const=0;
  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const=0;

  virtual int Builds_into() const=0;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const=0;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const=0;
  virtual void Set_LHS_changed(DS_dmod* dmod) const=0;
  virtual void Set_RHS_changed(DS_dmod* dmod) const=0;

  virtual int Set_gain(double/* gain*/)         {return 1;}
  virtual int Get_gain(double & gain) const {gain = 0.0; return 1;}
  virtual int Is_link_mech()          const {return 0;}
  virtual int Is_area_mech()          const {return 0;}
};

class DS_area_mech : public DS_enforcement_mechanism
{
 private:
  DS_area_load* mech_owner;
  double my_gain;
 
 public:
  DS_area_mech(DS_area_load* owner=NULL) 
   : mech_owner(owner) {}
  void Set_owner(DS_area_load* al) {mech_owner=al;}
  virtual int Is_area_mech() const
   {return 1;}
  virtual TYPE_ID Type_id() const
   { return ds_axm; }
  virtual 
   DS_enforcement_mechanism *Make_copy() const
    {return ACIS_NEW DS_area_mech;}
  virtual
   DM_xtan_trans Default_xtan_trans() const
    {return DM_XTAN_TRANS_NONE;}

  virtual void Add_to_LHS(DS_eqns *eqns,
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {
    if (row>=clm) // KLUDGE - symeq A in sym storage
     {eqns->Add_to_A(row,clm,val);}
    }

  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {eqns->Add_to_b(row,clm,val);}

  virtual int Builds_into() const;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_LHS_changed(DS_dmod* dmod) const;
  virtual void Set_RHS_changed(DS_dmod* dmod) const;
  virtual int Set_gain(double gain) {my_gain=gain; return 0;}
  virtual int Get_gain(double & gain) const {gain=my_gain; return 0;}
};
 
class DS_link_mech : public DS_enforcement_mechanism
{
 protected:
  DS_link_cstrn* mech_owner;
  int Is_linked()             const;
  int Is_fixed(DS_dmod* dmod) const;

 public:
  DS_link_mech(DS_link_cstrn* owner=NULL)
   : mech_owner(owner) {}

  void New_owner(DS_link_cstrn* owner)
   {mech_owner = owner;}

  virtual int Is_link_mech() const
   {return 1;}
};

class DS_link_cstrn_mech : public DS_link_mech
{
 public:
  virtual TYPE_ID Type_id() const
    {
     DS_assert(ds_lcm==1);  // protect against enum changes for SAT purposes
     return ds_lcm;
    }

  virtual
   DS_enforcement_mechanism * Make_copy() const
    {return ACIS_NEW DS_link_cstrn_mech;}

  virtual
   DM_xtan_trans Default_xtan_trans() const
    {return DM_XTAN_TRANS_NONE;}

  virtual void Add_to_LHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const
   {
#ifndef DS_USE_SYMEQ
    eqns->Add_to_C_min(row+C_offset,clm,val);
#else
// KLUDGE - should be _min, not _line
    eqns->Add_to_C_line(row+C_offset,clm,val);
#endif
   }

  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const
#ifndef DS_USE_SYMEQ
   {eqns->Add_to_d_min(row+C_offset,clm,val);}
#else
   {eqns->Add_to_d_line(row+C_offset,clm,val);}
#endif

  virtual int Builds_into() const;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_LHS_changed(DS_dmod* dmod) const;
  virtual void Set_RHS_changed(DS_dmod* dmod) const;

};

class DS_link_load_mech : public DS_link_mech
{
 private:
  double llm_gain;

 public:
  DS_link_load_mech(double gain=1000000) {llm_gain = gain;}

  virtual TYPE_ID Type_id() const
    {
     DS_assert(ds_llm==2);  // protect against enum changes for SAT purposes
     return ds_llm;
    }

  virtual
   DS_enforcement_mechanism * Make_copy() const
    {return ACIS_NEW DS_link_load_mech(*this);}

  virtual
   DM_xtan_trans Default_xtan_trans() const
    {return DM_XTAN_TRANS_LOCAL;}

  virtual void Add_to_LHS(DS_eqns *eqns,
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {
    if (row>=clm) // KLUDGE - symeq A in sym storage
     {eqns->Add_to_A(row,clm,llm_gain*val);}
    }

  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {eqns->Add_to_b(row,clm,llm_gain*val);}

  // non-virtual interface
  virtual int Get_gain(double & gain) const {gain = llm_gain; return 0;}
  virtual int Set_gain(double   gain)       {llm_gain = gain; return 0;}

  virtual int Builds_into() const;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_LHS_changed(DS_dmod* dmod) const;
  virtual void Set_RHS_changed(DS_dmod* dmod) const;

};

class DS_curve_cstrn_mech : public DS_enforcement_mechanism
{
 public:
  virtual TYPE_ID Type_id() const
    {
     DS_assert(ds_ccm==3);  // protect against enum changes for SAT purposes
     return ds_ccm;
    }

  virtual
   DS_enforcement_mechanism * Make_copy() const
    {return ACIS_NEW DS_curve_cstrn_mech;}

  virtual
   DM_xtan_trans Default_xtan_trans() const
    {return DM_XTAN_TRANS_NONE;}

  virtual void Add_to_LHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const
   {
#ifndef DS_USE_SYMEQ
    eqns->Add_to_C_min(row+C_offset,clm,val);
#else
// KLUDGE - should be _min, not _line
    eqns->Add_to_C_line(row+C_offset,clm,val);
#endif
   }

  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int C_offset) const
#ifndef DS_USE_SYMEQ
   {eqns->Add_to_d_min(row+C_offset,clm,val);}
#else
   {eqns->Add_to_d_line(row+C_offset,clm,val);}
#endif

  virtual int Builds_into() const;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_LHS_changed(DS_dmod* dmod) const;
  virtual void Set_RHS_changed(DS_dmod* dmod) const;

};

class DS_curve_load_mech : public DS_enforcement_mechanism
{
 private:
  double clm_gain;

 public:
  DS_curve_load_mech(double gain=1.e+7) {clm_gain = gain;}

  virtual TYPE_ID Type_id() const
    {
     DS_assert(ds_clm==4);  // protect against enum changes for SAT purposes
     return ds_clm;
    }

  virtual
   DS_enforcement_mechanism * Make_copy() const
    {return ACIS_NEW DS_curve_load_mech(*this);}

  virtual
   DM_xtan_trans Default_xtan_trans() const
    {return DM_XTAN_TRANS_NONE;}

  virtual void Add_to_LHS(DS_eqns *eqns,
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {
    if (row>=clm) // KLUDGE - symeq A in sym storage
     {eqns->Add_to_A(row,clm,clm_gain*val);}
    }

  virtual void Add_to_RHS(DS_eqns *eqns, 
                          int row, 
                          int clm, 
                          double val,
                          int /*C_offset*/) const
   {eqns->Add_to_b(row,clm,clm_gain*val);}

  // non-virtual interface
  virtual int Get_gain(double & gain) const {gain = clm_gain; return 0;}
  virtual int Set_gain(double   gain)       {clm_gain = gain; return 0;}

  virtual int Builds_into() const;

  virtual void Set_LHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_RHS_rebuild(DS_dmod* dmod) const;
  virtual void Set_LHS_changed(DS_dmod* dmod) const;
  virtual void Set_RHS_changed(DS_dmod* dmod) const;

};

#endif // DS_EMECH_H
