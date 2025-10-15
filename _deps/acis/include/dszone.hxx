/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dszone.hxx
// created  : July 19, 1995
// author   : George Celniker
// synopsis : A zone is a domain sub-region.  In combination
//            with a DS_pfunc a zone marks an area on a surface
//            or curve.  This marked area can be used for a
//            variety of purposes, like isolated deformations,
//            assigned material properties, and pressure loads.
// contains : class DS_zone       abstract base class
//                  DS_rect_zone  derived class
//                  DS_seam_zone  {for the future - a general zone}
//                  DS_elem_zone  {for the future - a general zone}
//
// notes    : 1. A zone can be specified in two ways as an
//               enumeration of elements, or as a boundary.
//               It is an area in a 2D domain_dim.

#ifndef DS_ZONE_HXX
#define DS_ZONE_HXX

// forward class declarations
class DS_pfunc ;
class DS_domain;
class DS_dmod;
class DM_dbl_array;
class DS_dbl_block;
//bmt - lib
#include "dmapinum.hxx" // DS_ZONE enum // DS_ZONE enum
//end bmt - lib
#include "dsstdef.hxx"  // DS_assert()  // DS_assert()
#include "dsblock.hxx"     // DS_dbl_block, DS_int_block     // DS_dbl_block, DS_int_block

// type_id enum, bas value enum, and the max zone_val enum value
//enum DS_ZONE    { ds_zone, ds_rzn } ;
// bmt - lib moved to dmapinum.hxx

/**
* @file dszone.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */

//==========================================================
// class DS_zone
//==========================================================
/**
 * Pointer to this class acts as an handle.
 * <br>
 * <b>Role:</b> Pointers to this SDM interface class <tt>DS_zone</tt> should be considered as
 * handles - they should always be forward-referenced, and never dereferenced.
 * They are created, copied, and deleted by DM API's, and only used as handle (tag)
 * arguments to DM API's. For example, the functions <tt>DM_build_square_zone</tt> and
 * <tt>DM_build_poly_zone</tt> will make a rectangular or polygonal <tt>DS_zone</tt> from input parameters.
 * The resulting <tt>DS_zone</tt> is then used to make an area load with the function <tt>DM_add_area_C0_load</tt>.
 * After completing the desired DM API calls,
 * the user must call <tt>DM_delete_dmod</tt> on the <tt>DS_dmod</tt> pointer.
 */

class DS_zone : public ACIS_OBJECT {               // identifier = zone
  protected:

 /*
 // tbrv
 */
  /**
  * @nodoc
  */
   DS_ZONE zone_type_id ;     // enum showing specialization type

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int zone_total_dof_count ; // [prob-size] total dof count in zone
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
 int zone_local_dof_count ; // [prob-size] local dof count in zone

 /*
 // tbrv
 */
 /**
  * @nodoc
  */

#ifndef DS_USE_SYMEQ
#else
   int zone_elem_count;       // [prob-size] elem count in zone
#endif
  /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int *zone_total_dof_list ; // list of all dofs that affect
                              // any of the zone elems.  A dof that
                              // affects a zone elem and a non-zone
                              // elem will be included.
                              // stored: in ascending order
  /*
  // tbrv
  */
  /**
   * @nodoc
   */
   int *zone_local_dof_list ;  // list of zone dof indices
                              // that only affect the zone elems.
                              // A dof that affects a zone elem
                              // and a non-zone elem will not be
                              // included.
                              // stored: in ascending order
 /*
 // tbrv
 */
 /**
  * @nodoc
  */

#ifndef DS_USE_SYMEQ
#else
   int *zone_elem_list ;      // list of zone elem indices
                              // stored: in ascending order
#endif

  public:

   // Type_id, constructors, copy, Make_copy, destructor
/**
 * @nodoc
 */
   DS_ZONE Type_id()          // Type_id access
                              { return zone_type_id ; }

   // knt. Split the following ctor prototype into 2 separate prototypes 
   // to satisfy mkman.
#ifndef DS_USE_SYMEQ
/**
 * @nodoc
 */
   DECL_DM DS_zone                    // eff: default constructor
   (DS_ZONE type_id=ds_zone,  // in : derived class identifier
    int total_dof_count=0,    // in : total dofs for this zone
    int local_dof_count=0,    // in : dofs that affect only this zone
    int elem_count=0,         // in : total elem count for this zone
    int *total_dof_list=NULL, // in : total list of zone dofs [not-nested]
    int *local_dof_list=NULL); // in : dofs that only affect zone elems [not-nested]
#else
/**
 * @nodoc
 */
   DECL_DM DS_zone                    // eff: default constructor
   (DS_ZONE type_id=ds_zone,  // in : derived class identifier
    int total_dof_count=0,    // in : total dofs for this zone
    int local_dof_count=0,    // in : dofs that affect only this zone
    int elem_count=0,         // in : total elem count for this zone
    int *total_dof_list=NULL, // in : total list of zone dofs [not-nested]
    int *local_dof_list=NULL, // in : dofs that only affect zone elems [not-nested]
    int *elem_list=NULL);     // in : list of zone elem indices [not-nested]
#endif
/**
 * @nodoc
 */
   DS_zone                    // eff: copy constructor
    (DS_zone &zone) ;         // in : zone being copied
/**
 * @nodoc
 */
   DS_zone &operator=         // eff: assignment operator
    (DS_zone &zone) ;         // in : right side object of assignment
/**
 * @nodoc
 */
   virtual DS_zone*           // rtn: newly constructed copy
    Make_copy()=0 ;           // eff: Virtual Make_copy
/**
 * @nodoc
 */
   virtual DS_domain* Make_domain(logical outside,double scale)=0;
/**
 * @nodoc
 */
   virtual
  ~DS_zone()                  // eff: Virtual destructor
                              { Size_arrays() ; }
/**
 * @nodoc
 */
    void
    Size_arrays               // eff: Size_arrays
    (int total_dof_count=0,   // in : total dofs for this zone
     int local_dof_count=0,   // in : dofs that affect only this zone
     int elem_count=0);       // in : number of contained elems

   // DS_zone simple data access  Dof_count()
/**
 * @nodoc
 */
 int Total_dof_count()     {return zone_total_dof_count ; }
/**
 * @nodoc
 */
 int Local_dof_count()     {return zone_local_dof_count ; }

/**
 * @nodoc
 */
#ifndef DS_USE_SYMEQ
#else
   int Elem_count()          {return zone_elem_count ; }
#endif
/**
 * @nodoc
 */
 int *Total_dof_list()     {return zone_total_dof_list ; }
/**
 * @nodoc
 */
 int &Total_dof_list       // rtn: ith dof_index
    (int ii)                 // in : which index to return
                             { DS_assert(   ii >= 0
                                         && ii < zone_total_dof_count) ;
                               return(zone_total_dof_list[ii]) ;
                             }

 /**
  * @nodoc
  */
 int *Local_dof_list()     {return zone_local_dof_list ; }
/**
 * @nodoc
 */
  int &Local_dof_list       // rtn: ith dof_index
    (int ii)                 // in : which index to return
                             { DS_assert(   ii >= 0
                                         && ii < zone_local_dof_count) ;
                               return(zone_local_dof_list[ii]) ;
                             }
/**
 * @nodoc
 */
#ifndef DS_USE_SYMEQ
#else
   int *Elem_list()          {return zone_elem_list ; }
/**
 * @nodoc
 */
 int &Elem_list            // rtn: ith elem_index
    (int ii)                 // in : which index to return
                             { DS_assert(   ii >= 0
                                         && ii < zone_elem_count) ;
                               return(zone_elem_list[ii]) ;
                             }
#endif
  // DS_zone pure virtual side-effect function
/**
 * @nodoc
 */
  virtual                   // bas: DS_zone virtual function
  int Domain_dim()=0 ;      // rtn: domain dim of zone
/**
 * @nodoc
 */
  virtual                   // bas: DS_zone virtual function
  void Shift_domain         // eff: Shift basis domain values
   (DS_pfunc *pfunc,        // in : pfunc being partitioned
    double *du)=0 ;         // in : domain shift SPAvector, Sized:Domain_dim
                            //      new_uv = old_uv+du
/**
 * @nodoc
 */
  virtual                   // bas: DS_zone virtual function
  int                       // rtn: 0=done,-1=scale too small
   Scale_domain             // eff: scale the basis domain values
    (DS_pfunc *pfunc,       // in : pfunc being partitioned
     double scale)=0 ;      // in : domain scale amount
                            //      new_uv = gain * old_uv
/**
 * @nodoc
 */
  virtual                   // bas: DS_zone virtual function
   void Domain_min(DM_dbl_array&)const=0 ; // rtn: ptr to zone's bounding SPAbox min
/*
  virtual                   // bas: DS_zone virtual function
  double &Domain_min        // rtn: ith coordinate of max_pt
    (int i)=0 ;             // in : index to query
*/
/**
 * @nodoc
 */
 virtual                   // bas: DS_zone virtual function
   void Domain_max(DM_dbl_array&)const=0 ; // rtn; ptr to zone's bounding SPAbox max
/*
  virtual                   // bas: DS_zone virtual function
  double &Domain_max        // rtn: ith coordinate of max_pt
    (int i)=0 ;             // in ; index to query
*/
#ifndef DS_USE_SYMEQ
#else
  // DS_zone Query functions
/**
 * @nodoc
 */
 int Is_elem_in_zone       // rtn: 0=no, 1=yes
    (int elem_index) ;      // in : elem index to search for
/**
 * @nodoc
 */
  int Is_dof_in_total_zone  // rtn: 0=no, 1=yes
    (int dof_index) ;       // in : dof index to search for
/**
 * @nodoc
 */
  int Is_dof_in_local_zone  // rtn: 0=no, 1=yes
    (int dof_index) ;       // in : dof index to search for
#endif //DS_USE_SYMEQ
} ; // end class DS_zone

//==========================================================
// class DS_rect_zone : public DS_zone
//==========================================================
/**
 * @nodoc
 */
class DS_rect_zone : public DS_zone {  // identifier = rzn
  protected:

   // rect_zone description
   int rzn_domain_dim ;    // [size-arg] Size of min and max pt

   double *rzn_min_pt ;    // lower bound for rectangle zone
   double *rzn_max_pt ;    // upper bound for rectangle zone

  public :

  // DS_rect_zone constructor, copy, Make_copy, destructor, Size_arrays
   DECL_DM DS_rect_zone            // eff: default constructor
    (DS_pfunc *pfunc=NULL, // in : shape being paritioned, [not-nested]
     double *pt1=NULL,     // in : one corner of the rectangle zone [not-nested]
     double *pt2=NULL) ;   // in : other corner of the rectangle zone [not-nested]

   DS_rect_zone            // eff: copy constructor
    (DS_rect_zone &bspln) ;

   DS_rect_zone &operator= // eff: assignment operator
    (DS_rect_zone &bspln) ;// in : right side object of assignment


   DS_zone*
    Make_copy()            // eff: Make_copy, Virtual DS_zone function
                           { return( ACIS_NEW DS_rect_zone(*this)) ; }

   virtual DS_domain* Make_domain(logical outside,double scale);

   void
    Size_arrays            // eff: Size_arrays
    (int domain_dim=0) ;   // in : domain size containing this zone

   virtual
  ~DS_rect_zone()          // eff: destructor
                           { Size_arrays() ; }

  // DS_rect_zone side-effect functions

  void Build_lists          // eff: build and store elem and dof lists
   (DS_pfunc *pfunc) ;      // in : shape being partitioned

  // DS_rect_zone instances of DS_zone virtual side-effect functions

  virtual                   // bas: DS_zone virtual function
  int Domain_dim() ;        // rtn: domain dim of zone

  virtual                   // bas: DS_zone virtual function
  void Shift_domain         // eff: Shift basis domain values
   (DS_pfunc *pfunc,        // in : pfunc being partitioned
    double *du) ;           // in : domain shift SPAvector, Sized:Domain_dim
                            //      new_uv = old_uv + du

  virtual                   // bas: DS_zone virtual function
  int                       // rtn: 0=done,-1=scale too small
   Scale_domain             // eff: scale the basis domain values
   (DS_pfunc *pfunc,        // in : pfunc being partitioned
    double scale) ;         // in : domain scale amount
                            //      new_uv = gain * old_uv


  virtual                   // bas: DS_zone virtual function
   void Domain_min(DM_dbl_array&)const;     // rtn: bbox min
/*
  virtual                  // bas: DS_zone virtual function
  double &Domain_min(int i)// rtn: ith coordinate of min_pt
                           { DS_assert(   i >= 0
                                       && i <  rzn_domain_dim) ;
                             return(rzn_min_pt[i]) ;
                           }
*/
// provisional - keep for compatability with adm 7.0 DS_zone_2Save
   double *Domain_min()     // rtn: ptr to zone's bounding SPAbox min
                            { return rzn_min_pt ; }

  virtual                   // bas: DS_zone virtual function
   void Domain_max(DM_dbl_array&)const;     // rtn; ptr to zone's bounding SPAbox max

/*
  virtual                  // bas: DS_zone virtual function
  double &Domain_max(int i)// rtn: ith coordinate of max_pt
                           { DS_assert(   i >= 0
                                       && i <  rzn_domain_dim) ;
                             return(rzn_max_pt[i]) ;
                           }
*/
// provisional - keep for compatability with adm 7.0 DS_zone_2Save
   double *Domain_max()     // rtn; ptr to zone's bounding SPAbox max
                            { return rzn_max_pt ; }

  // DS_rect_zone instances of DS_zone non-side-effect virtual functions


} ; // end class DS_rect_zone


// For now we skip W-centric stuff req'd for DS_area_cstrn
// bmt - todo!!! should be noop + trapped error when passed to DM_add_area_cstrn - may need to adjust dmapi
/**
 * The <tt>DS_poly_zone</tt> is used for building a <tt>DS_area_load</tt>.
 * <br>
 * <b>Role:</b> A <tt>DS_poly_zone</tt> delineates a piecewise linear subarea of a deformable surface.
 * The DM interface function <tt>DM_build_poly_zone</tt> is used to create a <tt>DS_poly_zone</tt> and the
 * DM interface function <tt>DM_delete_zone</tt> is used to delete a <tt>DS_poly_zone</tt>.
 * @see DS_dmod
 */

  class DS_poly_zone : public DS_zone {

  private:
    DS_poly_zone&
     operator=                    // assignment operator
     (const DS_poly_zone&);

    DS_poly_zone                // copy constructor; users must call Make_copy
      (const DS_poly_zone &) ;  // in : object to copy deeply

  protected:
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
    int my_domain_dim;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
    DS_dbl_block my_uval;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
    DS_dbl_block my_vval;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
    DS_dmod* my_dmod;

  public:
    // CTOR's
/**
 * @nodoc
 */
    DS_poly_zone(DS_dmod *dmod=NULL) {my_dmod=dmod;my_domain_dim=2;}
/**
 * @nodoc
 */
    DECL_DM DS_poly_zone(DS_dbl_block&,DS_dbl_block&,DS_dmod*);
    // DS_dmod access
/**
 * @nodoc
 */
    void Set_dmod(DS_dmod*);
/**
 * @nodoc
 */
    DS_dmod* Get_dmod()const;
    // polygon access
/**
 * @nodoc
 */
    void Set_vals(const DS_dbl_block&, const DS_dbl_block&);
/**
 * @nodoc
 */
    DECL_DM void Get_vals(DS_dbl_block&,DS_dbl_block&) const;
    // override pure virtual's
/**
 * @nodoc
 */
    virtual
      ~DS_poly_zone();
/**
 * @nodoc
 */
    virtual int
      Domain_dim();
/**
 * @nodoc
 */
    virtual void
      Domain_max(DM_dbl_array&)const;
/**
 * @nodoc
 */
    virtual void
      Domain_min(DM_dbl_array&)const;
/**
 * @nodoc
 */
    virtual DS_zone *
      Make_copy();
/**
 * @nodoc
 */
    virtual DS_domain *
      Make_domain(logical outside,double);
/**
 * @nodoc
 */
    virtual int
      Scale_domain(DS_pfunc*,double);
/**
 * @nodoc
 */
    virtual void
      Shift_domain(DS_pfunc*,double*);

};

/** @} */
#endif // DS_ZONE_HXX


