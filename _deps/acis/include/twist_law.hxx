/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "acis.hxx"

#include "law.hxx"
#include "sg_law.hxx"

// class to directly implement the twist law
class DECL_KERN twist_law : public multiple_law
{
protected:
	law        *deriv(int which=0) const;
	 
public:
    char       const *class_name();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	
	twist_law(double  in_height,       // zone of influence
		      double  in_theta1,       // start rotation angle
			  double  in_theta2,       // end rotation angle
			  int     in_continuity,   // 0,1,2 continuity
			  logical in_invert);

	twist_law(law **subs,int size,double in_height);

	
	virtual			~twist_law();
	void			evaluate(double const *in_point, double *out_point) const;
	char const*		symbol(law_symbol_type type=DEFAULT) const;
	virtual int		take_dim() const {return 3;};
	virtual int		return_dim() const {return 3;};
    virtual int		take_size() const {return 3;};
	virtual int		return_size() const {return 3;};
    virtual int     singularities(double** where, int**  type, double start, double end, double**) const; 
//	virtual logical term_domain(int term, SPAinterval &domain) const;
    virtual law*    set_domain(SPAinterval *new_domain, logical set);
    int				date() const {return 1100;};
	multiple_law*   make_one(law **subs, int size) const;

    // data
private:
	double height;
    double theta1;			
    double theta2;
	double *twist_theta;
	int continuity;
	logical invert;	
};


// class to directly implement the twist law's derivative
class DECL_KERN dtwist_law : public multiple_law
{
protected:
	law        *deriv(int which=0) const;
	 
public:
    char       const *class_name();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	
/*	dtwist_law(int     in_continuity,   // 0,1,2 continuity
				logical invert,
				double	*twist_theta);
*/
	dtwist_law(law **subs,int size);

//	bend_to_curve_law(law **subs, int size, SPAposition start, SPAposition end, SPAunit_vector initial_rail);
	
	virtual			~dtwist_law();
	void			evaluate(double const *in_point, double *out_point) const;
	char const*		symbol(law_symbol_type type=DEFAULT) const;
	virtual int		take_dim() const {return 3;};
	virtual int		return_dim() const {return 3;};
    virtual int		take_size() const {return 3;};
	virtual int		return_size() const {return 3;};
//    virtual int     singularities(double** where, int**  type, double start, double end, double**) const; 
//	virtual logical term_domain(int term, SPAinterval &domain) const;
//    virtual law*    set_domain(SPAinterval *new_domain, logical set);
    int				date() const {return 1100;};
	multiple_law*   make_one(law **subs, int size) const;

    // data
private:
	double height;
    double theta1;			
    double theta2;
	double *twist_theta;
	int continuity;
	logical invert;
	int xlevel, ylevel, zlevel;
	
};


