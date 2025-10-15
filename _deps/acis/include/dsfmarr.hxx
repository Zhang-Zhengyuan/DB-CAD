// $Id: dsfmarr.hxx,v 1.1 2001/03/21 23:13:29 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsrdlog.hxx
// created  : February 23, 1999
// author   : John Sloan
// contains : DS_face_model_array container class

#ifndef DS_FMARR_H
#define DS_FMARR_H
#include "mmgr.hxx"
class FACE;
class ATTRIB_DSMODEL;

class DS_face_model_pair : public ACIS_OBJECT
// TODO:??? (JHS) should be struct, but genblk macros not equipped
//                to handle this.  Could put class/struct extra macro
//                into genblk - may not be worth it.
{
 public:

  FACE* face;
  ATTRIB_DSMODEL* model;

  DS_face_model_pair(FACE* f=0, ATTRIB_DSMODEL* m=0)  // default constructor for array construction
   :
    face(f),
    model(m)
   {}

  DS_face_model_pair(const DS_face_model_pair & p)
   :
    face(p.face),
    model(p.model)
   {}

  // shallow copy so default operator= ok

  // pointers are aliases, so default destructor ok
};

// define DS_reduce_log as block of DS_reduce_events
#define DS_GENBLK_CLASS     DS_face_model_pair
#define DS_GENBLK_CONTAINER DS_face_model_array

#include "dsgenblk.hxx"

#undef  DS_GENBLK_CLASS
#undef  DS_GENBLK_CONTAINER

// add push function
void Push_onto(DS_face_model_array & arr,
                          const DS_face_model_pair & p);

inline void Erase(DS_face_model_array & arr)
 {arr.Need(0);}

#endif // DS_FMARR_H
