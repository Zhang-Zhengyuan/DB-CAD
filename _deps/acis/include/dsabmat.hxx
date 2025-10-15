// $Id: dsabmat.hxx,v 1.6 2000/12/26 18:33:14 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsabmat.hxx
// created  : February 5, 1999
// author   : John Sloan
// contains : DS_abs_matrix  - abstract SPAmatrix base class
//            DS_row_matrix  - abstract row SPAmatrix base class
//            DS_clm_matrix  - abstract columnt SPAmatrix base class
// synopsis : Represents a linear algebra SPAmatrix.
//            Interface is tuned to avoid excessive heap allocations - 
//            only inplace arithmetic operations allowed.
//            Anticipates sparse implementation derived classes, so direct
//            access to elements by clients is not allowed.
//
class DS_clm_matrix;


#ifndef DS_ABMAT_H
#define DS_ABMAT_H

#include "dsabvec.hxx"  // class DS_abs_vec  // class DS_abs_vec
                                        // enum DS_abs_vec::EQ_FLAG

class DS_row_matrix;
class DS_abs_matrix   : public ACIS_OBJECT          // identifier abm
{
  public:
// type_id enum
enum TYPE_ID { ds_abm,         // base class DS_abs_matrix
              ds_clm,         // derived class DS_clm_matrix
              ds_rwm,         // derived class DS_row_matrix
              ds_bcm,         // derived class DS_block_clm_matrix
              ds_brm,         // derived class DS_block_row_matrix
              ds_mcm,         // derived class DS_mb_clm_matrix
              ds_mrm,         // derived class DS_mb_row_matrix
              ds_abm_none } ; // a null value for subroutine returns

#ifdef DSDEBUG
   int                                        // rtn: 0==> file open failure       
   Debug_dump_matrix(int itag0=0, int itag1=0, int iappend=0) const;
                                             // eff: dump the SPAmatrix to a file
#endif /* DSDEBUG */
  // data members
  protected:
   const TYPE_ID   abm_type_id ;  // enum showing specialization type
   int            abm_nrows ;    // number of rows in SPAmatrix
   int            abm_nclms ;    // number of columnts in SPAmatrix
// TODO:??? (JHS) data members above should be private, since public or
//                protected access methods are available for both.

  protected:
   void Set_size(                // eff: access method for derived classes
                                 //      to register size changes
                 int nrows,
                 int nclms)   // in : new size
                                 {abm_nrows = nrows;
                                  abm_nclms = nclms;}
   void Set_nrows(                // eff: access method for derived classes
                                 //      to register size changes
                 int nrows)
                                 {abm_nrows = nrows;}
   void Set_nclms(                // eff: access method for derived classes
                                 //      to register size changes
                 int nclms)
                                 {abm_nclms = nclms;}

  public:
   // DS_ABM Type_id, constructor, copy, Make_copy, destructor

   TYPE_ID Type_id() const       // Type_id query method
                                { return abm_type_id ; }
 
  DS_abs_matrix                    // eff: "default" constructor
   (TYPE_ID type_id,             // in : derived class identifier (required)
    int    nrows=0,             // in : initial size
    int    nclms=0)
    :
     abm_type_id(type_id),
     abm_nrows(nrows),
     abm_nclms(nclms)
    {}
                          
  DS_abs_matrix                    // eff: copy constructor
   (const DS_abs_matrix & vec)
   : 
    abm_type_id(vec.Type_id()), 
    abm_nrows(  vec.Nrows()),
    abm_nclms(  vec.Nclms())
   {}
                          
  virtual DS_abs_matrix*          // ret: (upcast) pointer to concrete type
   Make_copy() const=0 ;       // eff: clones derived class
  
  virtual ~DS_abs_matrix() {}     // destructor

// Size control/query functions
  int  Nrows() const            // eff: returns current size
                               {return abm_nrows;}
  int  Nclms() const            // eff: returns current size
                               {return abm_nclms;}

  virtual void 
   Resize                      // eff: grows (or shrinks) array
    (int nrows,             // in : new size
     int nclms,             // in : new size
     double val=0.0)=0;        // in : new elements are initialized to val

  virtual void
   Reserve                      // eff: grows (or shrinks) array
    (int nrows,
     int nclms)=0;           // in : new size
                                //      does not initialize elements
  virtual void
   Wipe()=0;

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though

  virtual const double      // rtn: element value
   operator()               // eff: array element query (rvalue)
   (int row,
    int clm) const=0;      // in : the element index

  const double
   Elem                     // eff: array element query (rvalue)
   (int row,
    int clm) const         // in : the element index
                            {return (*this)(row,clm);}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int row,
    int clm,               // in : the element index
   double value)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int row,
    int clm,               // in : the element index
    double value)=0;        // in : vec[elem] += value

  DS_abs_matrix &              // ret: allow chaining
   Minuseq_elem             // eff: subtracts from the value of one element
   (int row,
    int clm,               // in : the element index
    double value)           // in : vec[elem] -= value
                            {Pluseq_elem(row,clm, -value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int row,
    int clm,               // in : the element index
    double value)=0;        // in : vec[elem] *= value

  DS_abs_matrix &              // ret: allow chaining
   Diveq_elem               // eff: divides one element by the value
   (int row,
    int clm,               // in : the element index
    double value)           // in : vec[elem] /= value
                            {
                             // TODO:!!! (JHS)  Trap for divide-by-0
                             Timeseq_elem(row,clm,1/value); return *this;}

// row manipulations
  virtual int Is_row() const {return 0;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    double value)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    double value)=0;        // in : Row(row) += value

  DS_abs_matrix &              // ret: allow chaining
   Minuseq_row             // eff: subtracts from the value of one element
   (int row,               // in : the element index
    double value)           // in : vec[elem] -= value
                            {Pluseq_row(row, -value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_row             // eff: multiplies one element by the value
   (int row,               // in : the element index
    double value)=0;        // in : vec[elem] *= value

  DS_abs_matrix &              // ret: allow chaining
   Diveq_row               // eff: divides one element by the value
   (int row,               // in : the element index
    double value)           // in : vec[elem] /= value
                            {
                             // TODO:!!! (JHS)  Trap for divide-by-0
                             Timeseq_row(row,1/value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec)=0;        // in : Row(row) += vec*mult

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult)=0;        // in : Row(row) += vec*mult

  // column operations
  virtual int Is_clm() const  {return 0;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    double value)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    double value)=0;        // in : Clm(clm) += value

  DS_abs_matrix &              // ret: allow chaining
   Minuseq_clm             // eff: subtracts from the value of one element
   (int clm,               // in : the element index
    double value)           // in : vec[elem] -= value
                            {Pluseq_clm(clm, -value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_clm             // eff: multiplies one element by the value
   (int clm,               // in : the element index
    double value)=0;        // in : vec[elem] *= value

  DS_abs_matrix &              // ret: allow chaining
   Diveq_clm               // eff: divides one element by the value
   (int clm,               // in : the element index
    double value)           // in : vec[elem] /= value
                            {
                             // TODO:!!! (JHS)  Trap for divide-by-0
                             Timeseq_clm(clm,1/value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult)=0;         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec)=0;        // in : Clm(clm) += vec*mult

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult)=0;        // in : Clm(clm) += vec*mult


// full SPAmatrix changes
  virtual DS_abs_matrix &      // ret: allow chaining
   Zero()=0;                // eff: zeros all elements, can be more optimal
                            //      than Set(0.0), but Set could test for
                            //      zero so maybe not necessary.

  virtual DS_abs_matrix &      // ret: allow chaining
   Negate()=0;              // eff: negates all non-zero elements

// Note that SPAmatrix =,+=,-= operators are use the doubly virtual 
// multi-method "Assign_from" below

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator-=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::MINUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Overwrite                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Minuseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::MINUSEQ); return *this;}

//      note that the following 3 routines are multi-methods
//      use double dispatch to implement for now
//      See Stroustup "Design + Evolution ..." p.299
//      for discussion/alternatives using Type_id()
  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           DS_abs_vec::EQ_FLAG flag) ;

  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           double mult,
           DS_abs_vec::EQ_FLAG flag) ;


   virtual void                    
    Lmult                             
     (const DS_abs_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const;  

   virtual void                  
    Lmult                             
     (const DS_row_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const;  
                 // uses res.Pluseq_row(mat.Row)

   void                            
    Rmult                              
     (const DS_abs_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const  
                                  {mat.Lmult(*this,res, flag);}

   virtual void                  
    Lmult                          
     (const DS_abs_vec & clm_vec,
            DS_abs_vec & clm_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const=0;  

   virtual void                     
    Rmult                          
     (const DS_abs_vec & row_vec,
            DS_abs_vec & row_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const=0; 

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAmatrix
   (double value)=0;// in : (*this)[i] = value for all i elements of m

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=               // eff: adds to the value of all elements
   (double value)=0;        //      vec[elem] += value for all elem

   DS_abs_matrix &             // ret: allow chaining
   operator-=               // eff: subtracts from the value of all elements
   (double value)           //      vec[elem] -= value for all elem        
                            {return *this += (-value);}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value)=0;        // in : vec[elem] *= value for all elem

   DS_abs_matrix &             // ret: allow chaining
   operator/=               // eff: divides the value of all elements
   (double value)           // in : vec[elem] /= value for all elem
                            {// TODO:!!! (JHS) Trap for divide-by-0
                             return *this *= (1/value);}
} ; // end class DS_abs_matrix

class DS_clm_matrix;
class DS_row_matrix : public DS_abs_matrix             // identifier abm
{
  public:
   // constructor, copy, Make_copy, destructor

  DS_row_matrix                    // eff: "default" constructor
   (TYPE_ID type_id,             // in : derived class identifier (required)
    int    nrows=0,             // in : initial size
    int    nclms=0)
    :
     DS_abs_matrix(type_id,nrows,nclms)
    {}
                          
  DS_row_matrix                    // eff: copy constructor
   (const DS_row_matrix & mat)
   : 
    DS_abs_matrix(mat)
   {}
  
  virtual ~DS_row_matrix() {}     // destructor

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though

  virtual const double      // rtn: element value
   operator()               // eff: array element query (rvalue)
   (int row,
    int clm) const      // in : the element index
                           {return Row(row)[clm];}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int row,
    int clm,               // in : the element index
   double value)         // in : vec[elem] = value
                           {Row(row).Set_elem(clm,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] += value
                           {Row(row).Pluseq_elem(clm,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] *= value
                           {Row(row).Timeseq_elem(clm,value); return *this;}

// row manipulations

  virtual 
   const DS_abs_vec &
    Row(int row) const=0;

  virtual 
   DS_abs_vec &
    Row(int row)=0;

  virtual int Is_row() const {return 1;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    double value)         // in : vec[elem] = value
                            {Row(row) = value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    double value)        // in : Row(row) += value
                            {Row(row) += value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_row             // eff: multiplies one element by the value
   (int row,               // in : the element index
    double value)        // in : vec[elem] *= value
                            {Row(row) *= value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec)         // in : vec[elem] = value
                            {Row(row) = vec;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult)         // in : vec[elem] = value
                            {Row(row).Overwrite(vec,mult);return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec)        // in : Row(row) += vec*mult
                            {Row(row) += vec;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult)           // in : Row(row) += vec*mult
                            {Row(row).Pluseq(vec,mult);return *this;}

// full SPAmatrix changes
/* Use Assign_from instead
  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_matrix & m);// in : (*this)[i] = v[i] for all i elements of v
                            //      note that this is a multi-method
                            //      use double dispatch to implement for now
                            //      See Stroustup "Design + Evolution ..." p.299
                            //      for discussion/alternatives using Type_id()

  virtual DS_row_matrix &
   operator=
   (const DS_row_matrix & rm);

  virtual DS_row_matrix &
   operator=
   (const DS_clm_matrix & cm);

  virtual DS_abs_matrix &        // out: allow chaining
   Pluseq                 // eff: increments by SPAvector
    (const DS_abs_matrix & m,
     double mult=1)=0; // in : (*this)[i] = m[i] for all i elements of m
                              //      requires (*this).Size() == m.Size()
*/

// Note that SPAmatrix =,+=,-= operators are use the doubly virtual 
// multi-method "Assign_from" below

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator-=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::MINUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Overwrite                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Minuseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::MINUSEQ); return *this;}

//      note that the following 3 routines are multi-methods
//      use double dispatch to implement for now
//      See Stroustup "Design + Evolution ..." p.299
//      for discussion/alternatives using Type_id()

  virtual void
   Assign_from
    (const DS_row_matrix & m,
           DS_abs_vec::EQ_FLAG flag) ;

  virtual void
   Assign_from
    (const DS_row_matrix & m,
           double mult,
           DS_abs_vec::EQ_FLAG flag) ;

   virtual void
    Lmult                             
     (const DS_clm_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const; 
                  // uses dotprod(row,clm)

   virtual void 
    Lmult       
     (const DS_abs_vec & clm_vec,
            DS_abs_vec & clm_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const; 

   virtual void                  
    Rmult                        
     (const DS_abs_vec & row_vec,
            DS_abs_vec & row_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const;
              
// redefine 4 hidden routines
  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           DS_abs_vec::EQ_FLAG flag) 
                                  {DS_abs_matrix::Assign_from(m,flag);}

  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           double mult,
           DS_abs_vec::EQ_FLAG flag) 
                                  {DS_abs_matrix::Assign_from(m,mult,flag);}

   virtual void                    
    Lmult                             
     (const DS_abs_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const 
                                  {DS_abs_matrix::Lmult(mat,res,flag);}

   virtual void                  
    Lmult                             
     (const DS_row_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const  
                                  {DS_abs_matrix::Lmult(mat,res,flag);}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAmatrix
   (double value);// in : (*this)[i] = value for all i elements of m

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=               // eff: adds to the value of all elements
   (double value);        //      vec[elem] += value for all elem


  virtual DS_abs_matrix &      // ret: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value);        // in : vec[elem] *= value for all elem

} ; // end class DS_row_matrix

class DS_clm_matrix : public DS_abs_matrix             // identifier abm
{
  public:
   // constructor, copy, Make_copy, destructor

  DS_clm_matrix                    // eff: "default" constructor
   (TYPE_ID type_id,             // in : derived class identifier (required)
    int    nrows=0,             // in : initial size
    int    nclms=0)
    :
     DS_abs_matrix(type_id,nrows,nclms)
    {}
                          
  DS_clm_matrix                    // eff: copy constructor
   (const DS_clm_matrix & vec)
   : 
    DS_abs_matrix(vec)
   {}
   
  virtual ~DS_clm_matrix() {}     // destructor

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though

  virtual const double      // rtn: element value
   operator()               // eff: array element query (rvalue)
   (int row,
    int clm) const      // in : the element index
                           {return Clm(clm)[row];}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int row,
    int clm,               // in : the element index
   double value)         // in : vec[elem] = value
                           {Clm(clm).Set_elem(row,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] += value
                           {Clm(clm).Pluseq_elem(row,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] *= value
                           {Clm(clm).Timeseq_elem(row,value); return *this;}

// clm manipulations

  virtual 
   const DS_abs_vec &
    Clm(int clm) const=0;

  virtual 
   DS_abs_vec &
    Clm(int clm)=0;

  virtual int Is_clm() const {return 1;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    double value)         // in : vec[elem] = value
                            {Clm(clm) = value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    double value)        // in : Clm(clm) += value
                            {Clm(clm) += value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_clm             // eff: multiplies one element by the value
   (int clm,               // in : the element index
    double value)        // in : vec[elem] *= value
                            {Clm(clm) *= value;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec)         // in : vec[elem] = value
                            {Clm(clm) = vec;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult)         // in : vec[elem] = value
                            {Clm(clm).Overwrite(vec,mult);return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec)        // in : Clm(clm) += vec*mult
                            {Clm(clm) += vec;return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult)        // in : Clm(clm) += vec*mult
                            {Clm(clm).Pluseq(vec,mult);return *this;}

// full SPAmatrix changes
/* use Assign_from
  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_matrix & m);// in : (*this)[i] = v[i] for all i elements of v
                            //      note that this is a multi-method
                            //      use double dispatch to implement for now
                            //      See Stroustup "Design + Evolution ..." p.299
                            //      for discussion/alternatives using Type_id()

  virtual DS_clm_matrix &
   operator=
   (const DS_row_matrix & rm);

  virtual DS_clm_matrix &
   operator=
   (const DS_clm_matrix & cm);

  virtual DS_abs_matrix &        // out: allow chaining
   Pluseq                 // eff: increments by SPAvector
    (const DS_abs_matrix & m,
     double mult=1); // in : (*this)[i] = m[i] for all i elements of m
                              //      requires (*this).Size() == m.Size()
*/

// Note that SPAmatrix =,+=,-= operators are use the doubly virtual 
// multi-method "Assign_from" below

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   operator-=                // eff: 
   (const DS_abs_matrix & m)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, DS_abs_vec::MINUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Overwrite                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::OVERWRITE); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::PLUSEQ); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Minuseq                // eff: 
   (const DS_abs_matrix & m,
          double mult)// in : (*this)[i] = m[i] for all i elements of m
                        {Assign_from(m, mult, DS_abs_vec::MINUSEQ); return *this;}

// override multimethods  
   virtual void                  
    Lmult                             
     (const DS_abs_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const;  

   virtual void 
    Lmult       
     (const DS_abs_vec & clm_vec,
            DS_abs_vec & clm_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const; 

   virtual void                  
    Rmult                        
     (const DS_abs_vec & row_vec,
            DS_abs_vec & row_res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const;

// redefine hidden multimethods
  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           DS_abs_vec::EQ_FLAG flag) 
                                  {DS_abs_matrix::Assign_from(m,flag);}

  virtual void
   Assign_from
    (const DS_abs_matrix & m,
           double mult,
           DS_abs_vec::EQ_FLAG flag) 
                                  {DS_abs_matrix::Assign_from(m,mult,flag);}

   virtual void                    
    Lmult                             
     (const DS_row_matrix & mat,
            DS_abs_matrix & res,
            DS_abs_vec::EQ_FLAG flag=DS_abs_vec::OVERWRITE) const  
                                  {DS_abs_matrix::Lmult(mat,res,flag);}

//      new multi-methods
  virtual void
   Assign_from
    (const DS_clm_matrix & m,
           DS_abs_vec::EQ_FLAG flag) ;

  virtual void
   Assign_from
    (const DS_clm_matrix & m,
           double mult,
           DS_abs_vec::EQ_FLAG flag) ;

  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAmatrix
   (double value);// in : (*this)[i] = value for all i elements of m

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=               // eff: adds to the value of all elements
   (double value);        //      vec[elem] += value for all elem

  virtual DS_abs_matrix &      // ret: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value);        // in : vec[elem] *= value for all elem

} ; // end class DS_clm_matrix

#endif // DS_ABMAT_H

