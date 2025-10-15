/* ORIGINAL: acis2.1/faceter/meshing/af_ladut.hxx */
// $Id: af_ladut.hxx,v 1.20 2002/06/18 15:21:40 ktatters Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _AF_LADUT
#define _AF_LADUT

#ifndef mac		// STIPORT shd CodeWarrior does not have malloc.h (in stdlib.h)
#include <malloc.h>
#endif
#include "dcl_fct.h"
#include "af_par.hxx"

// STI jmb:  portable way of pasting tokens in preprocessor
#include "ccat.hxx"
// STI jmb: end

// Dynamic array class macros

// STI jmb:  Use name2 for portability
//#define DECLARE_DA(T) DA##T
#define DECLARE_DA(T) name2(DA,T)
// STI jmb: end

// STI jmb:  What type does malloc/free use on various platforms
//           Introduce a cast on platforms that need it
#ifdef sun4_cxx4
#define malloc_cast (char*)
#else
#define malloc_cast
#endif
// STI jmb: end -- also changed lines containing malloc_cast

#define TEMPLATE_DA(T) class DECLARE_DA(T) {			\
  private:												\
    int		_s ;										\
	int		_Bound;										\
	int		_last;										\
	T		*pa ;										\
  public:												\
	DA##T() {											\
		_s = 0;											\
		_last = -1;										\
		pa = ( T *) ACIS_MALLOC( 5*sizeof( T ) ) ;			\
		_Bound = 5;										\
	}													\
	DA##T(unsigned s) : _s(s), _Bound(s) {				\
		pa = ( T *) ACIS_MALLOC(s*sizeof( T ));				\
		_last = -1;										\
	}													\
	virtual		~DA##T() { ACIS_FREE(malloc_cast pa); }		\
	T			&operator[] ( int i ) const				\
	{													\
		if( i<0 || i>=_s )								\
		{												\
			return pa[0];								\
		}												\
		else											\
		{												\
			return pa[i];								\
		}												\
	}													\
	void ReCreate(int s) {								\
		ACIS_FREE(malloc_cast pa);							\
		pa = ( T *) ACIS_MALLOC(s*sizeof( T ));				\
		_s = s;											\
		_last = -1;										\
/* STI jmb: Need to reset _Bound */             		\
		_Bound = s;										\
/* STI jmb: end */										\
	}													\
	void add( T elem ) {								\
		if( _last > _Bound-2 ) {						\
			size_t old_Bound = _Bound;					\
			_Bound += 10;								\
			pa = ( T *) ACIS_SAFE_REALLOC( malloc_cast pa, old_Bound*sizeof(T), _Bound*sizeof(T) ); \
		}												\
		pa[++_last] = elem;								\
		_s++;											\
	}													\
	void SetLast( unsigned l ) { _last = l; }			\
	unsigned getLen() const { return _s; }				\
	unsigned lastIndex() const { return _last; }		\
}	

TEMPLATE_DA(int);
TEMPLATE_DA(double);
TEMPLATE_DA(PAR_POS);								

class DOUBLEARR : public DECLARE_DA(double) {
public:
	DOUBLEARR();
	DOUBLEARR(int size);
	DOUBLEARR(double *p, unsigned size);
	DOUBLEARR(DOUBLEARR const &other);
	DOUBLEARR &operator=(DOUBLEARR const &other);
	double maxElem() const;
	double average() const;
	void reverse(int num_ele);
};



class PARAMARR : public DOUBLEARR {
	unsigned _sense; // True for increasing and False for decreasing
	double _tol;
	int _next;
public:
	PARAMARR()
		{
			_sense = _next = 0;
			_tol = 0;
		}

	PARAMARR(int size, double tol, unsigned sense) : DOUBLEARR(size)
		{ 
			_tol = tol;
			_sense = sense; 
			_next=0;

		}
	PARAMARR(double *p , int size, double tol, unsigned sense) 
		:DOUBLEARR(p, size) 
		{ 
			_tol = tol;
			_sense = sense;
			_next = size;
		}
	PARAMARR(PARAMARR const &other): DOUBLEARR(other) 
	{
		_tol = other.getTol();
		_sense = other.getSense();
		_next = other.lastUsedIndex()+1;
	}
	double getTol() const {return _tol; }
	void setTol(double tol) { _tol = tol; }
	unsigned getSense() const { return _sense;}
	void setSense(unsigned sense) { _sense = sense;  }
	
	PARAMARR &operator=(PARAMARR const &other);
	int lastUsedIndex() const { return _next-1; }
	void mapTo(double begin, double end);
	unsigned addElement( double elem );
	int indexBefore(double elem) const;
	~PARAMARR(){};
};


class PCLINFUN : public ACIS_OBJECT {
public:
	PARAMARR par;
	DOUBLEARR coord;
	PCLINFUN( PARAMARR const &_p, DOUBLEARR const &_coord )
	{
		par = _p;
		coord = _coord;
	}
	double coordAtPar( double pvalue ) const;
	int	pIndexBefore( double p ) const { return par.indexBefore(p); }
	void mapParamToInterval( double begin, double end )
	{
		par.mapTo( begin, end );
	}
	double totalArea() const;
	double dpForIncArea( double incArea, double stParam )const;
	~PCLINFUN() {};
};






#endif
