// $Id: internal_law.hxx,v 1.6 2001/03/18 18:42:31 goetting Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// The laws contained in this file are for internal use only.
// That is to say that they cannot be saved or restored from files
// because they do not have string member functions.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef INTERNAL_LAW_HXX
#define INTERNAL_LAW_HXX

#include "dcl_law.h"
// ywoo 08Jan01: added the header file.
#include "law_base.hxx"
// ywoo: end

/////////////////////////////////////////////////////////////
//
//  This makes a one dimensional law f such that
//  f(s)=Integrate[sub_law[s,p],{p,a_law[s],b_law[s]}]
//
//  splits is an array of laws in s that tell where
//  sub_law' is undefined.
//
/////////////////////////////////////////////////////////////

class DECL_LAW integrate_law: public law
{
	law           *sub_law;
	law           *a_law;
	law           *b_law;
	int           split_size;
	law           **splits;
	double        tol;
	int           min_level;
protected:
	~integrate_law();
public:
	logical       isa(int t) const;
	static int    id();
	int           type() const;
	char		  const *class_name();
	integrate_law(law    *in_sub,
		          law    *in_a_law,
	              law    *in_b_law,
				  int    split_size=0,
				  law    **splits=NULL,
				  double in_tol=1E-12,
				  int    in_min_level=2);
	void          evaluate(double const *x,double *answer) const;
	law           *derivative(int which=0) const;
	char		  *string(law_symbol_type type,int &count,law_data_node *&ldn) const;
};

//////////////////////////////////////////////////////////
//
//  This a one dimensional law
//  This is to say that m_law must be constant.
//  return fmod(m_law+fmod(sub_law,m_law),m_law)
//
//////////////////////////////////////////////////////////

class DECL_LAW shift_law: public law
{
	law        *sub_law;
	law        *m_law;
protected:
	~shift_law();
public:
	logical    isa(int t) const;
	static int id();
	int        type() const;
	char	   const *class_name();
	shift_law  (law *in_sub_law,
		       law *in_m_law);
	void       evaluate(double const *x,double *answer) const;
	law        *derivative(int which=0) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};


#endif
