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

class DECL_KERN bend_to_curve_law : public multiple_law
{
protected:
	law        *deriv(int which=0) const;
	 
public:
    char       const *class_name();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	
	bend_to_curve_law(const SPAposition&, 
		              const SPAposition&,
                      const SPAunit_vector&,
			          curve_law* final_curve, 
			          law* final_rail=NULL); 

	bend_to_curve_law(law **subs, 
                      int size, 
                      const SPAposition& start, 
                      const SPAposition& end, 
                      const SPAunit_vector& initial_rail);
	
	virtual			~bend_to_curve_law();
	void			evaluate(double const *in_point, double *out_point) const;
	char const*		symbol(law_symbol_type type=DEFAULT) const;
	virtual int		take_dim() const {return 3;};
	virtual int		return_dim() const {return 3;};
    virtual int		take_size() const {return 3;};
	virtual int		return_size() const {return 3;};
    virtual int     singularities(double** where, int**  type, double start, double end, double**) const; 
	virtual logical term_domain(int term, SPAinterval &domain) const;
    virtual law*    set_domain(SPAinterval *new_domain, logical set);
    int				date() const {return 1100;};
	multiple_law*   make_one(law **subs, int size) const;

    // data
private:
    SPAposition m_start;			// start of  rail
    SPAposition m_end;				// end of rail
    SPAunit_vector m_initial_rail;	
	double line_length;
	SPAunit_vector line_direction;  // local frame of the curve
	SPAunit_vector perp1;
	SPAunit_vector perp2;
	double my_min;
	double my_max;
	
};
class DECL_KERN dbend_to_curve_law : public multiple_law
{
protected:
	law        *deriv(int which=0) const;
	 
public:
    char       const *class_name();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	
	dbend_to_curve_law(const SPAposition&, 
		              const SPAposition&,
                      const SPAunit_vector&,
			          curve_law* final_curve, 
			          law* final_rail=NULL); 

	dbend_to_curve_law(law **subs, 
                       int size, 
                       const SPAposition& start, 
                       const SPAposition& end, 
                       const SPAunit_vector& initial_rail);
	
	virtual			~dbend_to_curve_law();
	void			evaluate(double const *in_point, double *out_point) const;
	char const*		symbol(law_symbol_type type=DEFAULT) const;
	virtual int		take_dim() const {return 3;};
	virtual int		return_dim() const {return 3;};
    virtual int		take_size() const {return 3;};
	virtual int		return_size() const {return 3;};
    virtual int     singularities(double** where, int**  type, double start, double end, double**) const; 
	virtual logical term_domain(int term, SPAinterval &domain) const;
    virtual law*    set_domain(SPAinterval *new_domain, logical set);
    int				date() const {return 1100;};
	multiple_law*   make_one(law **subs, int size) const;

    // data
private:
	int xlevel,ylevel,zlevel;
	SPAvector A1,A2,A3;
	double c;
	double mymin;
	law *final_curve;
	law *dfinal_curve;

	SPAvector S;
    SPAposition m_start;			// start of  rail
    SPAposition m_end;				// end of rail
    SPAunit_vector m_initial_rail;	
	double line_length;
	SPAunit_vector line_direction;  // local frame of the curve
	SPAunit_vector perp1;
	SPAunit_vector perp2;
	double my_min;
	double my_max;
	
};
