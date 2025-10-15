/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 16-Apr-09 Introduce sided evals in surface_law and dsurface_law
#ifndef BASE_LAW_HXX
#define BASE_LAW_HXX

#include "law_base.hxx"
#include "unitvec.hxx"
#include "position.hxx"

/**
 * @file main_law.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */
class base_pointer_map;
class base_pattern_law_data;
class VOID_LIST;

/**
 * Initializes the law library.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_LAW logical initialize_law();

/**
 * Terminates the law library.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_LAW logical terminate_law();

// STI ROLL
class SizeAccumulator;
// STI ROLL

class multiple_data_law;

/**
 * @nodoc
 */
void transform_rotation_constructor(multiple_data_law *law_create,
									law_data **in_law_datas, int in_size,
									logical rotation);

/**
 * Creates a law that is constant.
 * <br>
 * <b>Role:</b> A <tt>constant_law</tt> is a law that represents constant numbers.
 * For example, the law symbols "PI" and "E" are constants based upon classes
 * derived from <tt>constant_law</tt>.
 * <br><br>
 * Applications should call the virtual remove method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW constant_law: public law
{
	double			constant_value;
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * Constructs a <tt>constant_law</tt>.
 * <br><br>
 * <b>Role:</b> This sets the <tt>use_count</tt> to 1 and <tt>dlaw</tt> to <tt>NULL</tt>.
 * <br><br>
 * @param in_constant_value
 * value returned by law.
 */
	constant_law(double in_constant_value);
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int		id();
/**
 * @nodoc
 */
	logical			isa(int t) const; // virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law. All laws
 * derived from or that are a <tt>constant_law</tt> have a <tt>make_one</tt> method. This is used
 * by the parser and simplifier.
 * <br><br>
 * @param in_constant_value
 * value.
 */
	virtual constant_law *make_one(double in_constant_value=0.0) const;
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int             take_size() const; // virtual method in base class

/**
 * @nodoc
 */
	int             type() const; // virtual method in base class
};
/**
 * Provides methods and data that return one term of the input value.
 * <br>
 * <b>Role:</b> This law returns one of the input variables, depending upon what
 * the zero-based argument <tt>which</tt> is set to. All unary, binary, and multiple data
 * laws eventually converge on a constant law or an identity law. An identity law
 * returns one term of the input value.
 * <br><br>
 * Identity laws parse using x, y, z, u, v, t, and a<i>n</i>, where <i>n</i> is a positive
 * integer.  Among these, x, u, t, and a1 parse to identity that returns the first term. For
 * these variables, the argument <tt>which</tt> is set to 0.  In a similar fashion, y, v, and a2 parse
 * to the second term, and <tt>which</tt> is set to 1, while z and a3 parse to the third term, with
 * <tt>which</tt> set to 2. For higher-dimensional identities a<i>n</i>, <tt>which</tt> is set to <i>n</i>-1.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW identity_law: public law
{
	int        which;
	char       name;
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical			isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int             type() const; // virtual method in base class
/**
 * Constructs an <tt>identity_law</tt>.
 * <br><br>
 * <b>Role:</b>: The parameter <tt>in_which</tt> specifies the element of the input array
 * that is to be returned.
 * <br><br>
 * @param in_which
 * element of input array.
 * @param in_name
 * law name.
 */
	identity_law(int  in_which,
		         char in_name='X');
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	int             take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class
};
/**
 * Provides methods and data for laws that have multiple sublaws.
 * <br>
 * <b>Role:</b> A multiple law is a law with one or more arguments. Some special
 * operations, such as maximum, minimum, or vector, are multiple laws.
 * <br><br>
 * Multiple laws with two arguments differ from binary laws in how they parse.
 * A multiple law starts with the operator and is followed by the operands in
 * parenthesis and separated by commas. A binary law expects the left operand,
 * the operator, and then the right operand. The <tt>dot_law</tt> is a multiple law.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 * @see law
 */
class DECL_LAW multiple_law: public law
{
protected:
/**
 * Pointer to the first sublaw that is used as input to this law.
 */
	law        **sub;
/**
 * Integer specifying how many sublaws are used by this law.
 */
	mutable int sub_num;
	virtual ~multiple_law();
public:
    //char			const *class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical			isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int      id();
/**
 * @nodoc
 */
	int             type() const; // virtual method in base class
/**
 * Constructs a <tt>multiple_law</tt>.
 * <br><br>
 * <b>Role:</b> This takes two arguments. The first is a pointer to the first
 * law in a law list. The second is the number of sublaws in the law list.
 * <br><br>
 * @param in_sub
 * array of sublaws.
 * @param in_sub_num
 * size of array.
 */
	multiple_law(law **in_sub=NULL,int in_sub_num=0);
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law. All laws derived from
 * <tt>multiple_law</tt> have a <tt>make_one</tt> method. This is used by the parser and simplifier.
 * <br><br>
 * @param in_sub
 * array of sublaws.
 * @param in_sub_num
 * size of array.
 */
	virtual multiple_law* make_one(law** in_sub, int in_sub_num) const;
/**
 * Returns whether or not the given law is commutative.
 * <br><br>
 * <b>Role:</b> Commutative means that the order of operations is irrelevant.<br>
 *  For example, in the case of the dot product, "DOT(A,B)" is equals to "DOT(B,A)".<br>
 *  The default is <tt>FALSE</tt>, signaling that the law is not commutative.<br>
 * An example of a multiple law that is not commutative is the cross law;
 * "CROSS(A,B)" is not equal to "CROSS(B,A)".
 */
	virtual logical commutative() const;
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	int             return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int             take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int             date() const;   // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class

/**
 * This returns a pointer to the first operand (sublaw) of the multiple law.
 * <br><br>
 * <b>Role:</b> Only applications that create new laws that have parts of old laws
 * should use this method. If the sublaw is to be used elsewhere, the <tt>add</tt> method
 * should called.
 */
	law**       fsub() const   { return sub;}	   // Use with extreme caution!
/**
 * Returns number of operands (sublaws) of the multiple law.
 */
	int             fsub_num() const { return sub_num;}
/**
 * @nodoc
 */
	virtual logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const; // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

/**
 * Provides methods and data for laws that have two sublaws.
 * <br>
 * <b>Role:</b> A binary law is a law with two arguments. Most arithmetic
 * functions are binary laws.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 * binary_laws have a left and right sub law.
 * Some sub classes of binary_law are associative
 * That means (A+B)+C=A+(B+C).
 * Some sub classes of binary_law are commutative
 * That means A+B=B+C.
 * bstring is a function that all sub classes call
 * to create their string.
 * All binary_law sub classes have a make_one(law *,law *) function 
 * @see law
 */
class DECL_LAW binary_law: public law
{
protected:
/**
 * Pointer to the first argument (sublaw) of the binary law.
 */
	law* left_law;
/**
 * Pointer to the second argument (sublaw) of the binary law.
 */
	law* right_law;
	virtual ~binary_law();
public:
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>binary_law</tt>.
 * <br><br>
 * <b>Role:</b> This takes two arguments (sublaws) as the operands.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	binary_law(law* in_left_law = NULL, law* in_right_law = NULL);
/**
 * Returns whether or not the given law is associative.
 * <br><br>
 * <b>Role:</b> Associative means that the association of operations is irrelevant.<br>
 * For example, a plus law is associative: (A+B)+C=A+(B+C).<br>
 * The default is <tt>FALSE</tt>, signaling that the law is not associative. An example of a binary
 * law that is not associative is the minus law.
 */
	virtual int associative() const;
/**
 * Returns whether or not the given law is commutative.
 * <br><br>
 * <b>Role:</b> Commutative means that the order of operations is irrelevant.<br>
 * For example, a plus law is commutative: A+B=B+A.<br>
 * The default is <tt>FALSE</tt>, signaling that the law is not commutative. An example of a binary law that
 * is not commutative is the minus law.
 */
	virtual int commutative() const;
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law. All laws
 * derived from <tt>binary_law</tt> have a <tt>make_one</tt> method. This is used by the parser
 * and simplifier.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	virtual binary_law* make_one(law* in_left_law, law* in_right_law) const;
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
/**
 * @nodoc
 */
	virtual logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class

/**
 * This returns a pointer to the left operand (sublaw) of the binary law.
 * <br><br>
 * <b>Role:</b> Only applications that create new laws that have parts of old laws
 * should use this method. If the sublaw is to be used elsewhere, the <tt>add</tt> method
 * should be called.
 */
	law* fleft() const { return left_law;}	// Use with extreme caution!
/**
 * This returns a pointer to the right operand (sublaw) of the binary law.
 * <br><br>
 * <b>Role:</b> Only applications that create new laws that have parts of old
 * laws should use this method. If the sublaw is to be used elsewhere, the <tt>add</tt>
 * method should be called.
 */
	law* fright() const { return right_law;}  // Use with extreme caution!
/**
 * @nodoc
 */
	virtual logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};
/**
 * Provides methods and data for laws that have one sublaw.
 * <br>
 * <b>Role:</b> A unary law is a law with one argument. Most trigonometry
 * functions are unary laws.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 * @see law
 */
class DECL_LAW unary_law: public law
{
protected:
/**
 * This is a pointer to the sublaw that the unary law is to act upon.
 */
	law* sub_law;
	virtual ~unary_law();
public:
    //const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>unary_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sub_law
 * pointer to sublaw.
 */
	unary_law(law* in_sub_law);
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law. All laws
 * derived from <tt>unary_law</tt> have a <tt>make_one</tt> method. This is used by the parser
 * and simplifier.
 * <br><br>
 * If <i>f1</i> is a law for "X^3" and <i>f2</i> is the law for "COS(any law)", then
 * <tt>f2->make_one(f1)</tt> returns a law which is "COS(X^3)".
 * <br><br>
 * @param in_sub_law
 * pointer to sub law.
 */
	virtual unary_law* make_one(law* in_sub_law) const;
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class

/**
 * This returns the sublaw that is passed into this law.
 * <br><br>
 * <b>Role:</b> Only applications that create new laws that have parts of old
 * laws should use this method. An example of this is the simplifier; the law
 * "abs(x^2)" simplifies to "x^2", where "x^2" is the part of the old law used
 * in the new law. If the sublaw is to be used elsewhere, the <tt>add</tt> method should called.
 */
	law* fsub() const {return sub_law;} // Use with extreme caution!
/**
 * @nodoc
 */
	virtual logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};
/**
 * Provides methods and data for laws that have one law data member.
 * <br>
 * <b>Role:</b> This law has one law data argument, such as a <tt>curve_law</tt>, <tt>wire_law</tt>,
 * or <tt>surface_law</tt>. It is parsed with one law tag. [For example, "CUR(EDGE1)"
 * is followed by an edge, which is the law data.] It permits an application
 * to pass an ACIS entity or other class in the form of a law_data class into laws.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 * @see law_data
 */
class DECL_LAW unary_data_law : public law
{
protected:
/**
 * This is a pointer to the data structure used as input to the unary data law.
 */
	law_data* data;
	virtual ~unary_data_law();
public:
    //const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>unary_data_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw data class passed in as one of
 * its arguments.
 * <br><br>
 * @param in_sub_law
 * pointer to law data.
 */
	unary_data_law(law_data* in_sub_law = NULL);
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law.
 * All laws derived from <tt>unary_law</tt> have a <tt>make_one</tt> method. This is used by the
 * parser and simplifier and should not be called by the application directly.
 * <br><br>
 * @param in_data
 * pointer to law data.
 */
	virtual unary_data_law* make_one(law_data* in_data) const;
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class

/**
 * This returns the sublaw that is passed into this law.
 * <br><br>
 * <b>Role:</b> Only applications that create new laws that have parts of old
 * laws should use this method. An example of this is the simplifier; the law
 * "abs(x^2)" simplifies to "x^2", where "x^2" is the part of the old law used
 * in the new law. If the sublaw is to be used elsewhere, the <tt>add</tt> method should called.
 */
	law_data* fsub() const { return data;}	// Use with extreme caution!
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

// multiple_data_laws have an array of law_data's of size
// "size"

// mdstring is a function that all sub classes call to
// return their string.

// All multiple_data_law sub classes have a make_one(law_data **,int) function
/**
 * Provides methods and data for laws that have multiple law data members.
 * <br>
 * <b>Role:</b> This law has multiple law data arguments, such as <tt>curve_law</tt>,
 * <tt>wire_law</tt>, and <tt>surface_law</tt>. These are parsed with law tags. [For example,
 * "MAP(LAW1, EDGE2)" is followed by a law and an edge, which
 * are law datas that are passed to the <tt>map_law</tt>.] This class permits an application
 * to pass ACIS entities and other classes in the form of <tt>law_data</tt> objects
 * into laws.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW multiple_data_law: public law
{
	friend void transform_rotation_constructor(multiple_data_law *law_create,
											   law_data          **in_law_datas,
											   int               in_size,
											   logical           rotation);
protected:
/**
 * This is the array of law data pointer used by the the multiple data law.
 */
	law_data** datas;
/**
 * This is the number of elements in the data sent to the multiple data law.
 */
	int size;
	virtual ~multiple_data_law();
public:
    //const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>multiple_data_law</tt>.
 * <br><br>
 * <b>Role:</b> This takes two arguments. The first is an array of pointers to
 * the sublaw data members. The second is the number of sublaw data elements in
 * the law list.
 * <br><br>
 * @param in_sub_law
 * array of law data.
 * @param in_size
 * size of array.
 */
	multiple_data_law(law_data** in_sub_law,
		              int        in_size);
/**
 * Default constructor for the multiple law data.
 */
	multiple_data_law();
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law. All laws
 * derived from <tt>unary_law</tt> have a <tt>make_one</tt> method. This is used by the parser
 * and simplifier and should not be called by the application directly.
 * <br><br>
 * @param in_data
 * array of law data.
 * @param in_size
 * size of array.
 */
	virtual multiple_data_law* make_one(law_data** in_data,
		                                int        in_size) const;
/**
 * @nodoc
 */
	int same(const law* law1, const law* law2) const; // virtual method in base class
/**
 * @nodoc
 */
	char* string(
        law_symbol_type type    = DEFAULT,
        int&            count   = *(int*) NULL_REF,
        law_data_node*& ldn     = *(law_data_node**) NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * Returns an array of law data elements associated with sublaws.
 */
	law_data** fsubs() const { return datas;}		// Use with extreme caution!
/**
 * Returns the size of the sublaw array.
 */
	int fsize() const {return size;}
/**
 * @nodoc
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};
/**
 * Constant PI law
 */
class DECL_LAW pi_law: public constant_law
{
public:
    char         const *class_name();    // virtual method in base class
	logical      isa(int t) const;
	static int   id();
	int          type() const;
	pi_law();
	char         *string(law_symbol_type type=DEFAULT,
		                 int &count=*(int *) NULL_REF,
                         law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const   *symbol(law_symbol_type type=DEFAULT) const;
    constant_law *make_one(double junk=0.0) const;
};

/**
 * Constant law with <i>e</i> (Eulers Constant).
 */
class DECL_LAW e_law: public constant_law
{
public:
    char         const *class_name();    // virtual method in base class
	logical      isa(int t) const;
	static int   id();
	int          type() const;
	e_law();
	char         *string(law_symbol_type type=DEFAULT,
		                 int &count=*(int *) NULL_REF,
                         law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const   *symbol(law_symbol_type type=DEFAULT) const;
    constant_law *make_one(double junk=0.0) const;
};
/**
 * True law.
 */
class DECL_LAW true_law: public constant_law
{
public:
    char         const *class_name();    // virtual method in base class
	logical      isa(int t) const;
	static int   id();
	int          type() const;
	true_law();
	char         *string(law_symbol_type type=DEFAULT,
		                 int &count=*(int *) NULL_REF,
                         law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const   *symbol(law_symbol_type type=DEFAULT) const;
    constant_law *make_one(double junk=0.0) const;
	int          date() const;
};

/**
 * False law.
 */
class DECL_LAW false_law: public constant_law
{
public:
    char         const *class_name();    // virtual method in base class
	logical      isa(int t) const;
	static int   id();
	int          type() const;
	false_law();
	char         *string(law_symbol_type type=DEFAULT,
		                 int &count=*(int *) NULL_REF,
                         law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const   *symbol(law_symbol_type type=DEFAULT) const;
    constant_law *make_one(double junk=0.0) const;
	int          date() const;
};

/**
 * Derivative law.
 */
class DECL_LAW derivative_law: public multiple_law
{
	double     glb;
	double     lub;
	double	   _step_size;
protected:
	law             *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	derivative_law(law const *in_sublaw,
		           int in_dim,
				   int in_num_of=1,
				   double step_size = 1.0e-4);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
    int        take_size() const;
	multiple_law  *make_one(law **subs,int size) const;
	logical		  term_domain(int term,SPAinterval &domain) const;
	int singularities(double **where,
					  int    **type,
					  double start=-DBL_MAX,
					  double end=DBL_MAX,
					  double **period=NULL) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

/**
 * Represents the maximum of its component laws.
 */
class DECL_LAW max_law: public multiple_law
{
protected:
	law             *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	max_law(law **in_sublaw,int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	logical    commutative() const;
};
/**
 * Represents the minimum of its component laws.
 */
class DECL_LAW min_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	min_law(law **in_sublaw,int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	logical    commutative() const;
};
/**
 * Combines one dimensional laws into a multi-dimensional law.
 * <br>
 * <b>Role:</b> The range of all of the sublaws must be one-dimensional.
 * The dimension of the range of the resulting law is the number of sublaws.
 * The term "law" is the opposite of the "vector law", in the sense that a law takes
 * a multi-dimensional input and returns a one-dimensional output.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW vector_law: public multiple_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>vector_law</tt> from an array of sublaws.
 * <br><br>
 * <b>Role:</b> Accepts an array of one-dimensional range laws and creates a
 * <tt>vector_law</tt> with an <tt>in_dim</tt> dimensional range.
 * <br><br>
 * @param in_sublaw
 * array of sublaws with one-dimensional ranges.
 * @param in_dim
 * size of array.
 */
	vector_law(law** in_sublaw, int in_dim);
/**
 * Constructs a <tt>vector_law</tt> from a three-dimensional vector.
 * <br><br>
 * <b>Role:</b> Accepts an <tt>SPAvector</tt> and creates a <tt>vector_law</tt> with a three-dimensional
 * range. The <tt>constant</tt> method returns <tt>TRUE</tt> for this law.
 * <br><br>
 * @param v
 * vector defining the law.
 */
	vector_law(SPAvector v);
/**
 * Constructs a <tt>vector_law</tt> from a position.
 * <br><br>
 * <b>Role:</b> Accepts an <tt>SPAposition</tt> and creates a <tt>vector_law</tt> with a
 * three-dimensional range. The <tt>constant</tt> method returns <tt>TRUE</tt> for this law.
 * <br><br>
 * @param p
 * position defining the law.
 */
	vector_law(SPAposition p);
/**
 * Constructs a <tt>vector_law</tt> from a parameter position.
 * <br><br>
 * <b>Role:</b> Accepts an <tt>SPApar_pos</tt> and creates a <tt>vector_law</tt> with a two-dimensional
 * range. The <tt>constant</tt> method returns <tt>TRUE</tt> for this law.
 * <br><br>
 * @param p
 * parameter position defining the law.
 */
	vector_law(SPApar_pos p);
/**
 * Constructs a <tt>vector_law</tt> from a unit vector.
 * <br><br>
 * <b>Role:</b> Accepts a unit vector and creates a <tt>vector_law</tt> with a
 * three-dimensional range. The <tt>constant</tt> method returns <tt>TRUE</tt> for this law.
 * <br><br>
 * @param u
 * unit vector defining the law.
 */
	vector_law(SPAunit_vector u);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	void evaluate_with_guess(
        const double* x, double* answer, const double* guess = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law.
 * <br><br>
 * @param subs
 * array of sublaws.
 * @param size
 * size of array.
 */
	multiple_law* make_one(law** subs, int size) const;
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the term mathematical function that returns a single dimensional element of a multidimensional function.
 * <br>
 * <b>Role:</b> The <tt>term_law</tt> class supports the <tt>term_law</tt> function. It is used
 * for picking off elements of an array.
 * <br>
 * For example, assume that <tt>my_law</tt> is a vector field defined by <tt>"VEC(X, X+1, X+2, X+3)"</tt>.
 * A declaration in Scheme like <tt>(law:eval "term(my_law, 4)" 1)</tt> evaluates the
 * fourth coordinate of <tt>my_law</tt>, <tt>"X+3"</tt>, at the value 1. It returns 4. A declaration
 * like <tt>(law:eval "term(my_law, 3)" 1)</tt> evaluates the third coordinate of <tt>my_law</tt>,
 * "X+2", at the value 1. It returns 3.
 */
class DECL_LAW term_law: public multiple_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>term_law</tt> from an array of sublaws.
 * <br><br>
 * <b>Role:</b> This constructor expects an array of two laws.  The first is the
 * the second of which is the law from which a single term is desired; the second
 * is a <tt>constant_law</tt> specifying the index of the desired term.
 * <br><br>
 * <i><b>Note:</b> the latter
 * should contain a one-based index.  Specifying an index less than one will
 * result in an error.</i>
 * <br><br>
 * @param in_sublaw
 * array of sublaws.
 * @param in_dim
 * size of array (should be 2).
 */
	term_law(law** in_sublaw, int in_dim);
/**
 * Constructs a <tt>term_law</tt> from a law and an index.
 * <br><br>
 * <b>Role:</b> The input law is the law from which a single term is desired, the
 * parameter <tt>which</tt> specifying which one.
 * <br><br>
 * <i><b>Note:</b> the latter should be a one-based index.  Specifying an index less than one will
 * result in an error.</i>
 * <br><br>
 * @param in_law
 * pointer to law.
 * @param which
 * which law (starts at 1).
 */
	term_law(law* in_law, int which); // Which starts at one!
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_law* make_one(law** subs, int size) const; // virtual method in multiple_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Domain law
 */
class DECL_LAW domain_law: public multiple_law
{
protected:
	law             *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	domain_law(law **in_sublaw,int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	void       evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        return_size() const;
	int        take_size() const;
	law_domain *domain();
	logical	   term_domain(int term,SPAinterval &domain) const;
	law        *set_domain(SPAinterval *new_domain, logical set=FALSE);
    virtual law	*sub_simplify(int level=0, char const *& what = *(char const **)NULL_REF ) const;
};
/**
 * Permanent domain law.
 */
class DECL_LAW permanent_domain_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
	law	       *sub_inverse() const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	permanent_domain_law(law **in_sublaw,int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	void       evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        return_size() const;
	int        take_size() const;
	law_domain *domain();
	logical	   term_domain(int term,SPAinterval &domain) const;
	law        *set_domain(SPAinterval *new_domain, logical set=FALSE);
    int        date() const;
	int        singularities(double **where,
						     int    **type,
							 double start=-DBL_MAX,
							 double end=DBL_MAX,
							 double **period=NULL) const;
};

/**
 * Provides methods for the dot product mathematical function.
 */
class DECL_LAW dot_law: public multiple_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>dot_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	dot_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_law* make_one(law** subs, int size) const; // virtual method in multiple_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	logical commutative() const; // virtual method in multiple_law class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the cross product mathematical function.
 */
class DECL_LAW cross_law: public multiple_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>cross_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	cross_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_law *make_one(law **subs,int size) const; // virtual method in multiple_law class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the log mathematical function.
 * The log_law constructor takes an array of size
 * one or two the second argument is the base of the log.
 * The default base is 10. 
 */
class DECL_LAW log_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	log_law(law **in_sublaws,
		    int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs,int size) const;
	virtual logical	in_domain(double *where) const;
};
/**
 * Provides methods for the mod mathematical function.
 * The mod_law constructor takes an array of size
 * one or two the second argument is the base of the log.
 * The default base is 10.
 */
class DECL_LAW mod_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	mod_law(law *number, law *modulus);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs,int size) const;
};

/**
 * The twist_law constructor takes three laws
 * A SPAvector field to twist (i.e the zero twist value or rail_law)
 * A curve_law to twist it around
 * and a twist_law that gives the angle of twist
 * in radians.
 */
class DECL_LAW twist_path_law: public multiple_law
{
	law        *this_law;
protected:
	law        *deriv(int which=0) const;
   logical    m_Sub1IsVectorLaw; //Twist normal is a constant vector, not the path direction
public:
    const char* class_name();    // virtual method in base class
	~twist_path_law();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	twist_path_law(law *in_vec_field,
            law *in_cur_law,
            law *in_twist,
            const logical iCurLawIsVectorLaw = FALSE);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	virtual law	*sub_simplify(int level=0, char const *& what = *(char const **)NULL_REF ) const;
	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
	virtual void hasa(int type, VOID_LIST &out_laws);
};
/**
 * Surface vector law
 */
class DECL_LAW surfvec_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	surfvec_law(law *in_surf_law,
		        law *in_vec_law,
			    law *in_par_pos);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        return_size() const; // 2
	int        take_size() const;
};

/**
 * The step_law always takes an odd number of sub laws
 * starting at zero the odd numbered (i.e. 1, 3, 5,...)
 * must be constant laws that are in acsending order.
 * the constants tell where to stop using one law and start
 * using the next law.  At the change over points the law to
 * the right is used for the evaluate function.
 */
class DECL_LAW step_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	step_law(law **in_sublaw,
		     int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        take_size() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
};

/**
 * The piecewise_law takes a sequence of sub laws of the
 * form c1 s1 c2 s2 ... sd where the first c sub law, from
 * left to right, that returns true tells which s sub law is used.
 * If all c's are false then the sd is used as the default or
 * an error is given.
 */
class DECL_LAW piecewise_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	piecewise_law(law **in_sublaw,int in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
	int        date() const;
};

/**
 * The min_rotation_law caches an array of vectors of size
 * vec_size. These vectors are used to make the evaluate
 * function work faster.
 * The constructor takes an array of four sub_laws the first
 * is a law that is a path, and the second is a law that
 * is a constant starting SPAvector. The next two laws are
 * the lower and upper bounds of the domain.
 */
class DECL_LAW min_rotation_law : public multiple_law
{
	int vec_size;
	SPAvector *vecs;
	logical alt_eval; // Flag for using old evaluator
	logical use_legacy_eval;

protected:
	law *deriv( int which = 0 ) const;

public:
    virtual const char* class_name();
	~min_rotation_law();
	logical isa( int t ) const;
	static int id();
	int type() const;

	min_rotation_law(
		law **in_sublaw, 
		int in_size,
		logical preserve_legacy = FALSE );

	void evaluate( double const *x, double *answer ) const;
	char const *symbol( law_symbol_type type = DEFAULT ) const;
	multiple_law *make_one( law **in_datas, int in_size ) const;
	int return_size() const;
	logical term_domain( int term, SPAinterval &domain ) const;
	virtual void full_size( SizeAccumulator&, logical = TRUE ) const;
	
	// Function called after loading in a sweep_spl_sur that was
	// created with the old evaluator. Will remake the law.

	void set_alt_eval( logical );
};

/**
 * Creates a law to bend from a position around an axis in a given direction a specified amount.
 * @see law
 */
class DECL_LAW bend_law: public multiple_law
{
	law* this_law;
	SPAposition root;
	SPAunit_vector BendingDirection;
	SPAunit_vector BentVector;
	SPAunit_vector Bend_Plane;
	SPAposition neutral_root;
	double radius;

protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
* @nodoc
*/
	SPAunit_vector	bend_direction() const;
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * C++ destructor, deleting a <tt>bend_law</tt>.
 */
	~bend_law();
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>bend_law</tt> from an array of laws.
 * <br><br>
 * <b>Role:</b> This sets <tt>use_count</tt> to 1 and increments the <tt>how_many_laws</tt>.
 * It sets <tt>dlaw</tt>, <tt>slaw</tt>, and <tt>lawdomain</tt> to <tt>NULL</tt>.
 * <br><br>
 * @param in_sublaw
 * array of sublaws describing the bend.
 * @param in_size
 * size of array.
 */
	bend_law(law** in_sublaw, int in_size);
/**
 * Constructs a <tt>bend_law</tt> from a set of bend parameters.
 * <br><br>
 * <b>Role:</b> This sets <tt>use_count</tt> to 1 and increments the <tt>how_many_laws</tt>.
 * It sets <tt>dlaw</tt>, <tt>slaw</tt>, and <tt>lawdomain</tt> to <tt>NULL</tt>.
 * <br><br>
 * @param center_root
 * root position.
 * @param axis
 * bend axis.
 * @param dir
 * direction of the bend.
 * @param radius
 * radius of the bend.
 */
	bend_law(
        SPAposition    center_root,
        SPAunit_vector axis,
        SPAunit_vector dir,
        double         in_radius);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_law* make_one(law** in_datas,
		                   int   in_size) const;// virtual method in multiple_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};
/**
 * The derivative of the bend law for space warps
 */
class DECL_LAW dbend_law: public multiple_law
{
	SPAposition root;
	SPAunit_vector BendingDirection;
	SPAunit_vector BentVector;
	SPAunit_vector Bend_Plane;
	SPAposition neutral_root;
	double radius;
	int xlevel,ylevel,zlevel;

protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
//	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * C++ destructor, deleting a <tt>bend_law</tt>.
 */
	~dbend_law();
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * @nodoc
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * @nodoc
 */
	dbend_law(law** in_sublaw, int in_size);
/**
 * @nodoc
 */
/*	dbend_law(
        SPAposition    center_root,
        SPAunit_vector axis,
        SPAunit_vector dir,
        double         in_radius);
		*/
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_law* make_one(law** in_datas,
		                   int   in_size) const;// virtual method in multiple_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
//	virtual law* sub_simplify(
//        int             level = 0,
//        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
//	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

/**
 * The unbend law for space warps
 */
class DECL_LAW unbend_law: public multiple_law
{
	law        *this_law;
	SPAposition root;
	SPAunit_vector BendingDirection;
	SPAunit_vector BentVector;
	SPAunit_vector Bend_Plane;
	SPAposition neutral_root;
	double radius;
	logical usethislaw;

	mutable law *branched_this;
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	~unbend_law();
	logical    isa(int t) const;
	static int id();
	int        type() const;

	unbend_law(law** in_sublaw, int in_size, law* in_this = NULL);
	unbend_law(
        SPAposition    center_root,
        SPAunit_vector axis,
        SPAunit_vector dir,
        double      radius);

	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;	  // 3
	int        take_size() const;
	virtual law	*sub_simplify(int level=0,char const *& what= *(char const **)NULL_REF) const;
	int        date() const;
	virtual law *enable_branching() const;
	virtual int  branches() const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
	virtual void hasa(int type, VOID_LIST &out_laws);
};

/**
 * Provides methods for the plus, or addition, mathematical function.
 */
class DECL_LAW plus_law: public binary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>plus_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	plus_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, int const* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	int precedence() const; // virtual method in base class
/**
 * @nodoc
 */
	int associative() const; // virtual method in binary_law class
/**
 * @nodoc
 */
	int commutative() const; // virtual method in binary_law class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	binary_law* make_one(law* in_left_law, law* in_right_law) const; // virtual method in binary_law class
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the minus, or subtraction, mathematical function.
 */
class DECL_LAW minus_law: public binary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>minus_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	minus_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, int const* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	int precedence() const; // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	binary_law* make_one(law* in_left_law, law* in_right_law) const; // virtual method in binary_law class
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the times, or multiplication, mathematical function.
 */
class DECL_LAW times_law: public binary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>times_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	times_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, int const* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	int precedence() const; // virtual method in base class
/**
 * @nodoc
 */
	int associative() const; // virtual method in binary_law class
/**
 * @nodoc
 */
	int commutative() const; // virtual method in binary_law class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	binary_law* make_one(law* in_left_law, law* in_right_law) const; // virtual method in binary_law class
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the division mathematical function.
 */
class DECL_LAW division_law: public binary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>division_law</tt>.
 * <br><br>
 * <b>Role:</b> This has two pointers to sublaws passed in as arguments.
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * pointer to 2nd sublaw.
 */
	division_law(law* in_left_law, law* in_right_law);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, int const* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	int precedence() const; // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	binary_law* make_one(law* in_left_law, law* in_right_law) const; // virtual method in binary_law class
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the exponent mathematical function.
 */
class DECL_LAW exponent_law: public binary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical			isa(int t) const;
	static int		id();
	int				type() const;
	exponent_law(law *in_left_law, law *in_right_law);
	void			evaluate(double const *x,double *answer) const;
	void			evaluate_with_side(double const *x,double *answer,int const *side) const;
	int				precedence() const;
	char const 		*symbol(law_symbol_type type=DEFAULT) const;
	binary_law      *make_one(law *in_left_law,law *in_right_law) const;
	law_polynomial  *polynomial(law *in) const;
	logical          in_domain(double *where) const;
	virtual law	    *sub_simplify(int level=0, char const *& what = *(char const **)NULL_REF ) const;
};

// This law has two constructors. The first one returns a law
// that is the composition of the left with the right sub law.
// The second constructor takes and array of sub_laws and returns
// a law that is the conposition of the outer law with a vector_law
// constructed with the array of sub laws of size "size".
/**
 * Constructs the composition of two functions.
 * <br>
 * <b>Role:</b> This class constructs the composition of two functions.
 * The dimension of the range of the second sublaw must equal the dimension of
 * the domain of the first sublaw.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements the <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW composite_law: public binary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>composite_law</tt> from two sublaws.
 * <br><br>
 * <b>Role:</b> The dimension of the range of the second sublaw
 * (inner law, <tt>in_right_law</tt>) must equal the dimension of the domain of the first
 * sublaw (outer law, <tt>in_left_law</tt>).
 * <br><br>
 * @param in_left_law
 * pointer to 1st sublaw.
 * @param in_right_law
 * @param pointer to 2nd sublaw.
 */
	composite_law(law* in_left_law, law* in_right_law);
/**
 * Constructs a composite_law from an array of inner laws and another, outer, law.
 * <br><br>
 * <b>Role:</b> The array of sublaws is taken to make a vector law, where each sublaw is one of
 * the coordinates or terms of the vector law. The vector law is then composed with the
 * outer law. The inner sublaws must have one-dimensional ranges. The outer law must
 * have a domain dimension equal to the number of sublaws.
 * <br><br>
 * @param outer
 * outer law.
 * @param subs
 * array of inner sublaws.
 * @param size
 * size of the array.
 */
	composite_law(law* outer, law** subs, int size);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	void evaluate_with_guess(
        const double* x, double* answer, const double* guess = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	void evaluateVAR(const double* x, double*& y, int& size) const; // virtual method in base class
/**
 * @nodoc
 */
	int precedence() const; // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	binary_law* make_one(law* in_left_law, law* in_right_law) const; // virtual method in binary_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain( SPAinterval* new_domain, logical set = FALSE ); // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* enable_branching() const;   // virtual method in base class
/**
 * @nodoc
 */
	virtual int branches() const; // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_range(SPAinterval* new_range, logical set = FALSE); // virtual method in base class
};

/////////////////////////////////////////////////////////////
//
// Boolean laws
//
/////////////////////////////////////////////////////////////
/**
 * Provides methods for the less than boolean function.
 */
class DECL_LAW less_than_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	less_than_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};
/**
 * Provides methods for the greater than boolean function.
 */

class DECL_LAW greater_than_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	greater_than_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};
/**
 * Provides methods for the less or equal than boolean function.
 */

class DECL_LAW less_than_or_equal_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	less_than_or_equal_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};

/**
 * Provides methods for the greater or equal than boolean function.
 */

class DECL_LAW greater_than_or_equal_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	greater_than_or_equal_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};

/**
 * Provides methods for the equal boolean function.
 */

class DECL_LAW equal_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	equal_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};

/**
 * Provides methods for the not equal than boolean function.
 */

class DECL_LAW not_equal_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	not_equal_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};

/**
 * Provides methods for the not boolean function.
 */

class DECL_LAW not_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	not_law(law *in_sub_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law *make_one(law *in_sub_law) const;
	int        date() const;
};
/**
 * Provides methods for the even value than boolean function.
 */

class DECL_LAW even_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	even_law(law *in_sub_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law *make_one(law *in_sub_law) const;
	int        date() const;
};

/**
 * Provides methods for the odd value boolean function.
 */

class DECL_LAW odd_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	odd_law(law *in_sub_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law *make_one(law *in_sub_law) const;
	int        date() const;
};

/**
 * Provides methods for the prime value than boolean function.
 */

class DECL_LAW prime_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	prime_law(law *in_sub_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law *make_one(law *in_sub_law) const;
	int        date() const;
};
/**
 * Provides methods for the integer value than boolean function.
 */

class DECL_LAW int_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	int_law(law *in_sub_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law *make_one(law *in_sub_law) const;
	int        date() const;
};

/**
 * Provides methods for the AND boolean function.
 */

class DECL_LAW and_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	and_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};
/**
 * Provides methods for the OR boolean function.
 */

class DECL_LAW or_law: public binary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	or_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	int        precedence() const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	binary_law *make_one(law *in_left_law,law *in_right_law) const;
	int        date() const;
};

///////////////////////////////////////////////////////////////
//
// Sub classes of the unitary_law class
//
///////////////////////////////////////////////////////////////
/**
 * This law returns 1 if the sub law is greater than zero else
 * it returns zero.  This law is used in the derivatives of
 * min, max, and abs.  It is also used to create domains for
 * multi dimensional root finders.
 */
class DECL_LAW set_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	set_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

/**
 * Provides methods for the unary minus, or negation, mathematical function.
 */
class DECL_LAW negate_law: public unary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>negate_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	negate_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law *in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	law_polynomial* polynomial(law* in) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};
/**
 * Provides methods for the NATURAL LOG mathematical function.
 */
class DECL_LAW natural_log_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	natural_log_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	    in_domain(double *where) const;
};

/**
 * Provides methods for the EXP mathematical function.
 */

class DECL_LAW exp_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	exp_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	    in_domain(double *where) const;
};

/**
 * Provides methods for the FLOOR mathematical function.
 */

class DECL_LAW floor_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	floor_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	int        date() const;  // 700
};
/**
 * Provides methods for the CEILING mathematical function.
 */

class DECL_LAW ceil_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	ceil_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	int        date() const;  // 700
};
/**
 * Provides methods for the RANDOM mathematical function.
 */
class DECL_LAW rand_law: public unary_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	rand_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	int        date() const;  // 700
};
/**
 * Provides methods for the SINE mathematical function.
 */
class DECL_LAW sin_law : public unary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>sin_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	sin_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(double const *x,double *answer,int const *side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law* in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the COSINE mathematical function.
 */

class DECL_LAW cos_law : public unary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>cos_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	cos_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(double const *x,double *answer,int const *side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law* in_sublaw) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

/**
 * Provides methods for the TANGENT mathematical function.
 * Where ever the cos(x) is zero this law is undefined
 * i.e. pi/2+n*pi for all integers n.
 */
class DECL_LAW tan_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	tan_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};
/**
 * Provides methods for the COTANGENT mathematical function.
 * Where ever the sin(x) is zero this law is undefined
 * i.e.	n*pi for all integers n. 
 */
class DECL_LAW cot_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	cot_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

/**
 * Provides methods for the SECANT mathematical function.
 * Where ever the cos(x) is zero this law is undefined
 * i.e. pi/2+n*pi for all integers n.
 */
class DECL_LAW sec_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	sec_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

// Where ever the sin(x) is zero this law is undefined
// i.e.	n*pi for all integers n.
/*
// tbrv
*/
/**
 * Provides methods for the COSECANT mathematical function.
 */
class DECL_LAW csc_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	csc_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

// Defined only for x such that -1<=x<=1
/*
// tbrv
*/
/**
 * Provides methods for the ARCSINE mathematical function.
 */
class DECL_LAW arcsin_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arcsin_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	logical    in_domain(double *where) const;
};

// Defined only for x such that -1<=x<=1
/*
// tbrv
*/
/**
 * Provides methods for the ARCCOSINE mathematical function.
 */
class DECL_LAW arccos_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccos_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	logical    in_domain(double *where) const;
};
/*
// tbrv
*/
/**
 * Provides methods for the ARCTANGENT mathematical function.
 */
class DECL_LAW arctan_law: public multiple_law
{
	law        *this_law;
protected:
	law        *deriv(int which=0) const;
public:
	~arctan_law();
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arctan_law(law **in_sublaw,int in_dim);
	arctan_law(law *in_law);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **in_sublaw,int size) const;
	int        date() const;  // 600 if in_dim>1 else 202
	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
	virtual void hasa(int type, VOID_LIST &out_laws);
};
/*
// tbrv
*/
/**
 * Provides methods for the ARCCOTANGENT mathematical function.
 */
class DECL_LAW arccot_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccot_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

/*
// tbrv
*/
/**
 * Provides methods for the ARCSECANT mathematical function.
 */
class DECL_LAW arcsec_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arcsec_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	logical    in_domain(double *where) const;
};

/*
// tbrv
*/
/**
 * Provides methods for the ARCCOSECANT mathematical function.
 */
class DECL_LAW arccsc_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccsc_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

/*
// tbrv
*/
/**
 * Provides methods for the SINH (Hyperbolic Sine) mathematical function.
 */
class DECL_LAW sinh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	sinh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

/**
 * Provides methods for the COSH (Hyperbolic Cosine) mathematical function.
 */
class DECL_LAW cosh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	cosh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

/**
 * Provides methods for the TANH (Hyperbolic Tangent) mathematical function.
 */
class DECL_LAW tanh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	tanh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

// Not defined for x=0.
/**
 * Provides methods for the COTH (Hyperbolic Cotangent) mathematical function.
 */
class DECL_LAW coth_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	coth_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};
/*
// tbrv
*/
/**
 * Provides methods for the SECH mathematical function.
 */
class DECL_LAW sech_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	sech_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

// Not defined for x=0;
/*
// tbrv
*/
/**
 * Provides methods for the COSECH  (Hyperbolic Cosecant) mathematical function.
 */
class DECL_LAW csch_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	csch_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

/*
// tbrv
*/
/**
 * Provides methods for the ARCSINH (Hyperbolic Arcsine) mathematical function.
 */
class DECL_LAW arcsinh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arcsinh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

// Defined only for x>=1.

/*
// tbrv
*/
/**
 * Provides methods for the ARCCOSH (Hyperbolic Arccosine) mathematical function.
 */
class DECL_LAW arccosh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccosh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
};

// Defined only for x such that -1<x<1.
/*
// tbrv
*/
/**
 * Provides methods for the ARCTANH (Hyperbolic Arc Tangent) mathematical function.
 */
class DECL_LAW arctanh_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arctanh_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	logical    in_domain(double *where) const;
};

// Defined only for x such that x<-1 or x>1.
/*
// tbrv
*/
/**
 * Provides methods for the ARCCOTH mathematical function.
 */
class DECL_LAW arccoth_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccoth_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

// Defined only for x such that 0<x<=1.
/*
// tbrv
*/
/**
 * Provides methods for the ARCSECH mathematical function.
 */
class DECL_LAW arcsech_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arcsech_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	logical    in_domain(double *where) const;
};

// Not defined for x=0.
/*
// tbrv
*/
/**
 * Provides methods for the ARCCOSECH mathematical function.
 */
class DECL_LAW arccsch_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	arccsch_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	logical    in_domain(double *where) const;
};

// Note that the derivative of abs(f(x)) is may not
// exist where f(x) is zero.
/*
// tbrv
*/
/**
 * Provides methods for the ABS mathematical function.
 */
class DECL_LAW abs_law: public unary_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	abs_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
};

// Only defined for non negative values
/**
 * Provides methods and data for the square root mathematical function.
 */
class DECL_LAW sqrt_law : public unary_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>sqrt_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	sqrt_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(double const *x,double *answer,int const *side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law *in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	logical	in_domain(double* where) const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};

// Not defined for a value of zero lenght
// The this_law is used to take its derivative and
// to evaluate the law.

/**
 * Provides methods for the normalize mathematical function.
 * <br>
 * <b>Role:</b> This law normalizes the length of <tt>my_law</tt> to be of unit length.
 * This is accomplished by dividing each dimension element by the square root
 * of the sum of the squares of all of the return elements. This is applicable
 * to a law that returns any dimension.
 * @see law
 */
class DECL_LAW norm_law : public unary_law
{
	law        *this_law;
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * Destructor for this law class.
 */
	~norm_law();
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>norm_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	norm_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law* in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

// Not defined if the second derivative of the path
// sub law does not exits
// The this_law is used to take its derivative and
// to evaluate the law.
/**
 * Describes a vector field for a given curve that points in the direction of curvature.
 * <br>
 * <b>Role:</b> This returns a vector in the direction of curvature at a position
 * on its sublaw. This is the ideal vector field for use on a helix.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 * <br><br>
 * <b>Limitations:</b> This vector field will fail to be defined if the second
 * derivative of the curve is zero. This vector field may flip directions at
 * points of inflection on the curve, and may exhibit an undesirable amount of twist.
 * @see law
 */
class DECL_LAW frenet_law : public unary_law
{
	law* this_law;
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * Destroys a <tt>frenet_law</tt>.
 * <br><br>
 * <b>Role:</b> Applications should call the virtual remove method instead of the
 * tilde (~) destructor to get rid of a law.
 */
	~frenet_law();
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>frenet_law</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>in_sublaw</tt> must be a law with a one-dimensional domain and a
 * three-dimensional range.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	frenet_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law* in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
    // STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

/**
 * Composes a law mathematical function that returns the normal to a surface at a given position.
 * <br>
 * <b>Role:</b> When evaluated, a surfnorm_law returns the normal to a surface at a given
 * <i>uv</i> position.<br>
 * ACIS defines its own parameter range for a surface, which is used by this law.
 * This law does not normalize the returned vector, because many applications
 * only require the direction of the vector and not its normalized value.
 * @see law
 */
class DECL_LAW surfnorm_law : public unary_law
{
	law* this_law;
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * C++ destructor.
 */
	~surfnorm_law();
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>surfnorm_law</tt>.
 * <br><br>
 * <b>Role:</b> This has a pointer to a sublaw passed in as its argument.
 * <br><br>
 * @param in_sublaw
 * pointer to sublaw.
 */
	surfnorm_law(law* in_sublaw);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(const double* x, double* answer, const int* side) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_law* make_one(law* in_sublaw) const; // virtual method in unary_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;// virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void hasa(int type, VOID_LIST& out_laws);// virtual method in base class
};

/*
// tbrv
*/
/**
 * Size law
 */
class DECL_LAW size_law: public unary_law
{
	law        *this_law;
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	~size_law();
	logical    isa(int t) const;
	static int id();
	int        type() const;
	size_law(law *in_sublaw);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_law  *make_one(law *in_sublaw) const;
	virtual law	*sub_simplify(int level=0, char const *& what = *(char const **)NULL_REF ) const;
	int        return_size() const;  // 1
	int        date() const;
	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
	virtual void hasa(int type, VOID_LIST &out_laws);
};

////////////////////////////////////////////////////
//
// unary_data_law sub class
//
////////////////////////////////////////////////////

class path_law_data;
class base_curve_law_data;
class base_wire_law_data;
class base_surface_law_data;
class base_pcurve_law_data;

/**
 * This law returns the curvature of the path sub law
 * It is not defined for x's where the path sub law's
 * derivative is undefined.
 */
class DECL_LAW curvature_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	curvature_law(path_law_data *in_law_datas);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        singularities(double **where,
							 int    **type,
							 double start=-DBL_MAX,
							 double end=DBL_MAX,
							 double **period=NULL) const;
};

// Curve_law's returns the parametric SPAposition of a curve.
// It returns three values and takes 1.

class SPAtransf;

/**
 * Returns the position on a curve when evaluated.
 * <br>
 * <b>Role:</b> The parameterization of the <tt>curve_law</tt> is equal to the
 * parameterization of the underlying ACIS <tt>curve</tt>.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW curve_law : public unary_data_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * Constructs a <tt>curve_law</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>in_law_data</tt> argument is a <tt>curve_law_data</tt> structure that holds
 * an ACIS curve data structure and a starting and ending parameter.
 * <br><br>
 * @param in_law_data
 * curve law data that contains a curve.
 */
	curve_law(base_curve_law_data* in_law_data);
//changed to make_curve_law
//curve_law(EDGE *in_edge,SPAtransf &in_trans= *(SPAtransf *)NULL_REF);
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_data_law* make_one(law_data* in_data) const; // virtual method in unary_data_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class

	// if the curve is planar, returns true and sets plane_point and plane_normal.
/**
 * If the curve is planar, returns <tt>TRUE</tt> and sets <tt>plane_point</tt> and <tt>plane_normal</tt>.
 * <br><br>
 * @param plane_point
 * plane point.
 * @param plane_normal
 * plane normal.
 */
	logical is_planar(SPAposition& plane_point, SPAunit_vector& plane_normal);
	// if the curve is linear, returns true and sets line_point and line_direction.
/**
 * If the curve is linear, returns <tt>TRUE</tt> and sets <tt>line_point</tt> and <tt>line_direction</tt>.
 * <br><br>
 * @param line_point
 * line point.
 * @param line_direction
 * line direction.
 */
	logical is_linear(SPAposition& line_point, SPAunit_vector& line_direction);
	// The CALLER owns the returned points, and should ACIS_DELETE them.
/**
 * Returns a list of points on the curve.
 * <br><br>
 * <b>Role:</b> The caller owns the returned points, and should <tt>ACIS_DELETE</tt> them.
 * <br><br>
 * @param num_pts
 * number of points on the curve.
 */
	SPAposition* get_points_on_curve(int num_pts);
};

// When evaluated, a pcurve_law returns an SPApar_pos <i>uv</i> value, given an
// SPAparameter value as input.
// It returns two values and takes 1.
/**
 * Creates a law to support parameter curve calculations.
 * <br>
 * <b>Role:</b> When evaluated, <tt>pcurve_law</tt> returns an <tt>SPApar_pos</tt> <i>uv</i> value given,
 * a parameter value as input. It returns two values and takes one.
 */
class DECL_LAW pcurve_law : public unary_data_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * C++ initialize constructor requests memory for an instance of <tt>pcurve_law</tt> and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> This sets <tt>use_count</tt> to 1 and increments <tt>how_many_laws</tt>.
 * It sets <tt>dlaw</tt>, <tt>slaw</tt>, and <tt>lawdomain</tt> to <tt>NULL</tt>.
 * <br><br>
 * @param in_law_data
 * pointer to base_pcurve_law_data.
 */
	pcurve_law(base_pcurve_law_data* in_law_data);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_data_law* make_one(law_data* in_data) const; // virtual method in unary_data_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * Returns a <tt>law_domain</tt> class, which contains information about the domain of <tt>this</tt> law.
 */
	law_domain* domain();
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class
/**
 * @nodoc
 */
	int date() const;   // virtual method in base class
};

/*
// tbrv
*/
/**
 * Wire_law's return the parametric postion of a wire
 * where the parameterization has been scaled to the
 * length of each of the sub coedges and starts at zero
 * and ends at the length of the wire.  Internal to a
 * coedge the parameterization may not be uniform.
 * This law takes in 1 value and returns 3.
 */
class DECL_LAW wire_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	wire_law(base_wire_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 3
	law_domain *domain();
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        singularities(double **where,
							 int    **type,
							 double start=-DBL_MAX,
							 double end=DBL_MAX,
							 double **period=NULL) const;
};

/**
 * Returns the position on a surface.
 * <br>
 * <b>Role:</b> The parameterization of the surface_law is equal to the
 * parameterization of the underlying ACIS <tt>surface</tt>.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW surface_law : public unary_data_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>surface_law</tt>.
 * <br><br>
 * <b>Role:</b> The argument <tt>in_law_data</tt> is a <tt>surface_law_data</tt> structure that holds an
 * ACIS surface data structure, and a starting and ending parameter.
 * <br><br>
 * @param in_law_data
 * surface law data that contains a surface.
 */
	surface_law(base_surface_law_data* in_law_data);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(double const *x,double *answer,int const *side) const;

/**
 * @nodoc
 */
	void eval_normal(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	unary_data_law* make_one(law_data* in_data) const; // virtual method in unary_data_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain( SPAinterval* new_domain, logical set = FALSE ); // virtual method in base class
};

/**
 * The gaussian_curvature_law's return the Gaussian curvature
 * at the u,v coordinates of the surface.  The Gaussian curvature
 * is the result of multiplying the two principal curvatures
 * at the point on the surface.
 * This law takes in 2 values and returns 1.
 */
class DECL_LAW gaussian_curvature_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	gaussian_curvature_law(base_surface_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 1
	int        take_size() const;   // 2
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 600
};

/**
 * The mean_curvature_law's return the mean curvature
 * at the u,v coordinates of the surface.  The mean curvature
 * is the average of the two principal curvatures
 * at the point on the surface.
 * This law takes in 2 values and returns 1.
 */
class DECL_LAW mean_curvature_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	mean_curvature_law(base_surface_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 1
	int        take_size() const;   // 2
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 600
};

/**
 * The max_curvature_law's return the greater curvature
 * value at the u,v coordinates of the surface.
 * This law takes in 2 values and returns 1.
 */
class DECL_LAW max_curvature_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	max_curvature_law(base_surface_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 1
	int        take_size() const;   // 2
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 600
};

/**
 * The min_curvature_law's return the lessor curvature
 * value at the u,v coordinates of the surface.
 * This law takes in 2 values and returns 1.
 */
class DECL_LAW min_curvature_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	min_curvature_law(base_surface_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 1
	int        take_size() const;   // 2
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 600
};

/**
 * The bs3_surface_law's return the SPAposition of the
 * spline approximating surface at the u,v parameters.
 * This law takes in 2 values and returns 3.
 */
class DECL_LAW bs3_surface_law: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	bs3_surface_law(base_surface_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const; // 3
	int        take_size() const;   // 2
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 600
};

////////////////////////////////////////////////////
//
// multiple_data_law sub class
//
/////////////////////////////////////// * * * * * */
/**
 * The map_law takes in a sub_law as its first law_data
 * and a path as its second law_data and its maps the
 * SPAinterval [0,1] to the domain of the given path.
 * Curve_laws and wire_laws that have been mapped with
 * a map_law evaluate from zero to one.
 */
class DECL_LAW map_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	map_law(law_data **in_datas,
		    int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const;
	//         take_size() const;is 1;
};

// This law is the same as curve_law with an extra
// law_data field that holds how many derivatives to take.
// The first field is the curve_law the second is the
// number of derivatives.
/**
 * Returns a position or one of the derivatives on a curve.
 * <br>
 * <b>Role:</b> The parameterization of the <tt>curve_law</tt> is equal to the
 * parameterization of the underlying ACIS <tt>curve</tt>.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW dcurve_law : public multiple_data_law
{
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>dcurve_law</tt>.
 * <br><br>
 * <b>Role:</b> The size of the array of <tt>law_data</tt> pointers is always 2. The first element
 * in the array is the <tt>curve_law_data</tt> structure, which holds the ACIS curve with its
 * bounding parameters. The second refers to a <tt>law_law_data</tt> object containing a
 * law that evaluates to non-negative integer, specifying the order of the
 * derivative.
 * <br><br>
 * @param in_law_datas
 * pointer to law data input.
 * @param in_size
 * size of array (always 2).
 */
	dcurve_law(law_data** in_law_datas, int in_size);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_data_law* make_one(law_data** in_datas,
		                        int        in_size) const; // virtual method in multiple_data_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * @nodoc
 */
	law* set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
/**
 * @nodoc
 */
	int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const; // virtual method in base class
};


/**
 * This law is the same as pcurve_law with an extra
 * law_data field that holds how many derivatives to take.
 * The first field is the pcurve_law the second is the
 * number of derivatives.
 */
class DECL_LAW dpcurve_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	dpcurve_law(law_data **in_law_datas,
		        int      in_size);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const;
	//         take_size() const;is 1;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 401
};

/**
 * This law is the same as wire_law with an extra
 * law_data field that holds how many derivatives to take.
 * The first field is the wire_law the second is the
 * number of derivatives.
 */
class DECL_LAW dwire_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	dwire_law(law_data **in_law_datas,
		       int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const;
	//         take_size() const;is 1.
	//logical	   term_domain(int term,SPAinterval &domain) const;
};

/**
 * This law is the same as surface_law with an extra
 * law_data field that holds how many derivatives to take.
 * The first field is the surface_law the second is the
 * number of derivatives.
 */
class DECL_LAW dsurface_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	dsurface_law(law_data **in_law_datas,
		         int      in_size);
	void evaluate(double const *x,double *answer) const;
	void evaluate_with_side(double const *x,double *answer,int const* side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const; // 3
	int        take_size() const;   // 2
	//logical	   term_domain(int term,SPAinterval &domain) const;
	law*       set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
};

class SPAtransf;
// This law takes in three values and returns three values
// The first argument to the transfore_law is a law.
// The second argument is a transform_law_data.
// This law is used to transform laws that return positions.
/**
 * Applies an ACIS transform to a law that returns a three dimensional position.
 * <br>
 * <b>Role:</b> The sublaw must be a law with a three-dimensional range.
 * The output of the sublaw is transformed by the ACIS transformation passed to it.
 * The transform may rotate, scale, and translate the output of the sublaw.
 * <br><br>
 * Applications should call the virtual <tt>remove</tt> method instead of the tilde (~)
 * destructor to get rid of a law. This method decrements <tt>use_count</tt>, and
 * calls the law destructor if <tt>use_count</tt> falls to zero.  The destructors of laws
 * containing sublaws should call <tt>remove</tt> on the latter for correct memory management.
 */
class DECL_LAW transform_law: public multiple_data_law
{
	friend void transform_rotation_constructor(multiple_data_law *law_create,
											   law_data          **in_law_datas,
											   int               in_size,
											   logical           rotation);
protected:
/**
 * @nodoc
 */
	law* deriv(int which = 0) const; // virtual method in base class
/**
 * @nodoc
 */
	law* sub_inverse() const; // virtual method in base class
public:
/**
 * @nodoc
 */
    const char* class_name();    // virtual method in base class
/**
 * @nodoc
 */
	logical isa(int t) const; // virtual method in base class
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.  The <tt>isa</tt>, <tt>id</tt>, and <tt>type</tt>
 * methods are used to identify a law's class type.
 */
	static int id();
/**
 * @nodoc
 */
	int type() const; // virtual method in base class
/**
 * Constructs a <tt>transform_law</tt>.
 * <br><br>
 * <b>Role:</b> Accepts an array of size 2, where the first term of the array
 * is a <tt>law_law_data</tt> class instance that contains the law to be transformed.
 * The second term is a <tt>transform_law_data</tt> class instance that contains the ACIS
 * transformation.
 * <br><br>
 * @param in_law_datas
 * array of law data.
 * @param in_size
 * size of array (2).
 */
	transform_law(law_data** in_law_datas, int in_size);
/**
 * @nodoc
 */
	void evaluate(const double* x, double* answer) const;  // virtual method in base class
/**
 * @nodoc
 */
	void evaluate_with_side(
        const double* x, double* answer, const int* side = NULL) const; // virtual method in base clas
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type = DEFAULT) const; // virtual method in base class
/**
 * @nodoc
 */
	multiple_data_law* make_one(law_data** in_datas,
		                        int in_size) const; // virtual method in multiple_data_law class
/**
 * @nodoc
 */
	int return_size() const; // virtual method in base class
/**
 * @nodoc
 */
	int take_size() const; // virtual method in base class
/**
 * @nodoc
 */
	logical term_domain(int term, SPAinterval& domain) const; // virtual method in base class
/**
 * Returns the transform from the law transform.
 * <br><br>
 * @param f_simple_trans
 * returns TRUE if the transform is the identity.
 */
	SPAtransf get_trans(logical& f_simple_trans);
/**
 * @nodoc
 */
	virtual law* sub_simplify(
        int             level = 0,
        const char*&    what = *(const char**)NULL_REF) const; // virtual method in base class
};


/*
// tbrv
*/
/**
 * This law takes in three values and returns three values
 * The first argument to the rotate_law is a law.
 * The second argument is a transform_law_data.
 * This law is used to transform laws that return SPAvector.
 */
class DECL_LAW rotate_law: public multiple_data_law
{
	friend void transform_rotation_constructor(multiple_data_law *law_create,
											   law_data          **in_law_datas,
											   int               in_size,
											   logical           rotation);
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	rotate_law(law_data **in_law_datas,
		       int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const; // 3
	int        take_size() const;
	logical    term_domain(int term,SPAinterval &domain) const;
};

/**
 * @nodoc
 */
void transform_rotation_constructor(multiple_data_law *law_create,
									law_data          **in_law_datas,
								    int               in_size,
									logical           rotation);


/**
 * Surface point perp
 */
class DECL_LAW surfperp_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	surfperp_law(law_data **in_law_datas,
		       int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	void	   evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        return_size() const; // 2
	int        take_size() const;	 // take_size of second arg
	int        date() const;
	logical    term_domain(int term,SPAinterval &domain) const;
	law*       set_domain(SPAinterval* new_domain, logical set = FALSE); // virtual method in base class
};

/**
 * Curve point perp
 */
class DECL_LAW curveperp_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
	curveperp_law(law_data **in_law_datas,
		          int      in_size);
    char const        *class_name();    // virtual method in base class
	logical           isa(int t) const;
	static int        id();
	int               type() const;
	void              evaluate(double const *x,double *answer) const;
	void              evaluate_with_side(double const *x,double *answer,int const *side) const;
	void			  evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char const        *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int               take_size() const;	 // take_size of second arg
	int               date() const;
	virtual int       branches() const;
	law               *set_range(SPAinterval *new_range, logical set=FALSE);
};

/**
 * Curve closest point
 */
class DECL_LAW curveclosest_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
	curveclosest_law(law_data **in_law_datas,
		          int      in_size);
    char const        *class_name();    // virtual method in base class
	logical           isa(int t) const;
	static int        id();
	int               type() const;
	void              evaluate(double const *x,double *answer) const;
	void			  evaluate_with_side(double const *x,double *answer,int const *side) const;
	void			  evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char const        *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int               take_size() const;	 // take_size of second arg
	int               date() const;
	virtual int       branches() const;
	law               *set_range(SPAinterval *new_range, logical set=FALSE);
};

/**
 * curve range
 */
class DECL_LAW length_param_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	length_param_law(law_data **in_law_datas,
		       int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        take_size() const;	 // max of law_data 1 and 2
	int        date() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
};
/*
// tbrv
*/
/**
 * curve length
 */
class DECL_LAW length_law: public multiple_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	length_law(law_data **in_law_datas,
		       int      in_size);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_data_law *make_one(law_data **in_datas,
		                        int      in_size) const;
	int        take_size() const;	 // max of law_data 1 and 2
	int        date() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
};

// ********************************************************
//
// The following are laws for internal use only
//
// ********************************************************

// This law returns the square of the distance between
// to points on the path sub law divided by the square of
// the distance between the SPAparameter values.  This law
// is not defined for values (x,y) where x=y.
// This function takes in 2 values and returns 1.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW intersect_lawi: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	intersect_lawi(base_curve_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        take_size() const; // 2
};

// This law returns the distance between to points on
// the path sub law.  This function takes in 2 values and
// returns 1.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW distance_lawi: public unary_data_law
{
protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	distance_lawi(base_curve_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        take_size() const; // 2
};

// This law takes in four sublaws a SPAposition P and a SPAvector V that
// define a ray and two constants R and K such that if c=1/R then
// a conic surface is defined by z=c*p^2/(1+sqrt(1-(1_K)c^2*p^2))
// where p^2=x^2+y^2.
// The law returns four t values are such that
// P+tV is an intersection of the line and the conic trough.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW conic_line_law: public multiple_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	conic_line_law(law **in_sublaw,int in_dim);
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	void       evaluateVAR(double const *x,double *&y,int &size) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        return_size() const; // Variable
	int        date() const;	   // 401
};

// This law takes in four sublaws a SPAposition P and a SPAvector V that
// define a ray and two constants R and K such that if c=1/R then
// a conic trough along the x axis defined by
// z=c*y^2/(1+sqrt(1-(1_K)c^2*y^2)).
// The law returns four t values are such that
// P+tV is an intersection of the line and the conic trough.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW conic_trough_line_law: public multiple_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	conic_trough_line_law(law **in_sublaw,int in_dim);
	//void       evaluate(double const *x,double *answer) const;
	void       evaluateVAR(double const *x,double *&y,int &size) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law  *make_one(law **subs,int size) const;
	int        return_size() const; // Variable
	int        date() const;	   // 401
};

// ****************************************************************
// swa 10Dec98 -- multiple curve law stuff - originally used in
//                local ops to solve the shelling lofting bug

// This class represents the join of more than one law into a
// single "longer" law which is a composite of its pieces.  The
// domain of the multiple curve law is 0 to the sum of the
// lengths of the sub-laws.  This class was written primarily to
// join several curves, that meet end to end, into one longer
// curve, however any laws from R1 to R3 can be pieced together
// as long as each sub-law has a finite domain.
//
// The multiple_curve_law constructor takes a list of the sub-laws
// used to form the composite.
//
// As an example, consider the multiple curve law g made of law f1
// with domain (a1, b1) and law f2 with domain (a2, b2).  Then g
// has domain (0, b1-a1 + b2-a2).  Evaluating g(x) where 0 < x < b1-a1
// is f1(x+a1) and where b1-a1 < x < b1-a1 + b2-a2, g(x) is
// f2(x - (b1-a1)+a2).  This example generalizes to cases with more
// than two sub-laws making up the multiple_curve_law.
//
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW multiple_curve_law: public multiple_law
{
    double *segment_start;
    double *segment_end;
    double *param_at_end;
    double total_length;
    mutable int last_segment;
	logical closed;
protected:
	law        *deriv(int which=0) const;
	law	       *sub_inverse() const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	multiple_curve_law(law **in_sublaw, int in_dim);
    ~multiple_curve_law();
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side=NULL) const;
	char       *string(law_symbol_type type=DEFAULT,
		               int &count=*(int *) NULL_REF,
                       law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs, int size) const;
	logical	   term_domain(int term,SPAinterval &domain) const;
    int        return_size() const;
    int        take_size() const;
    int        date() const; // 500
	law        *set_domain(SPAinterval *new_domain, logical set=FALSE);
	int        which_segment(double in_param,double &out_param, int side=0) const;
	int        singularities(double **where,
							 int    **type,
							 double start=-DBL_MAX,
							 double end=DBL_MAX,
							 double **period=NULL) const;
	int isclosed(){ return closed;}

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

// The multiple_curveperp_law represents the inverse of a
// multiple_curve_law.  It's constructor takes the same
// sublaws as were used to make the multiple_curve_law.  The
// evaluator of the multiple_curveperp_law takes a point in
// R3 and returns the SPAparameter of the point on the
// underlying multiple_curve_law that is closest to the given
// point.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW multiple_curveperp_law: public multiple_law
{
    double *segment_start;
    double *segment_end;
	multiple_curve_law *mcl;

protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	multiple_curveperp_law(law **in_sublaw, int in_dim);
    ~multiple_curveperp_law();
	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_guess(double const *x,double *answer,double const *guess=NULL) const;
	char       *string(law_symbol_type type=DEFAULT,
		               int &count=*(int *) NULL_REF,
                       law_data_node *& ldn=*(law_data_node **) NULL_REF) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs, int size) const;
    int        return_size() const;
    int        take_size() const;
    int        date() const; // 500
    law        *set_range(SPAinterval *new_range, logical set=FALSE);

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

// swa 16Oct98 -- cyclide law stuff -- move this
// ****************************************************************

// The constructor takes an array of three sub_laws which should
// be the constant shape parameters a, c, and m.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW cyclide_law: public multiple_law
{
	double a, b, c, m;

protected:
	law        *deriv(int which=0) const;
    law        *sub_inverse() const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	cyclide_law(law **in_sublaw,
		         int   in_size);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
    int        take_size() const;
    int        date() const; // 500

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW dcyclide_law: public multiple_law
{
	double a, b, c, m, a2, b2, c2;
    int xlevel, ylevel;

protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	dcyclide_law(law **in_sublaw,
		         int   in_size);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
    int        take_size() const;
    int        date() const; // 500

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_LAW cyclide_perp_law: public multiple_law
{
	double a, b, c, m;
    logical weak_form;
    law *this_cyclide;

protected:
	law        *deriv(int which=0) const;
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;

    // if 4 or more arguments, use weak form of point perp
	cyclide_perp_law(law **in_sublaw,
		             int   in_size);
    ~cyclide_perp_law();
	void       evaluate(double const *x,double *answer) const;
    double get_m( double const *x ) const;
    logical point_on_cyclide( double const *x ) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
    int        take_size() const;
    int        date() const; // 500

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};



/**
 * @nodoc
 */
class DECL_LAW simple_helix_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;

public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	simple_helix_law(law **in_sublaw, int in_size);

	simple_helix_law(double current_radians,
					 double delta_radians,
					 double thread_distance,
					 law* radians_identity,
					 double radius,
					 logical handedness,
					 law* variable_radius_law,
					 logical law_specified_radius,
					 double current_height);

	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
    int        take_size() const;
    int        date() const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

private:
	double m_current_radians;
	double m_delta_radians;
	double m_radius;
	double m_handedness;
	double m_law_specified_radius;
	double m_current_height;
	double m_thread_distance;
	double m_deriv_level;
};

class DECL_LAW simple_rail_law: public multiple_law
{
protected:
	law        *deriv(int which=0) const;

public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	simple_rail_law(law **in_sublaw, int in_size);

	void       evaluate(double const *x,double *answer) const;
	void       evaluate_with_side(double const *x,double *answer,int const *side) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **in_datas,
		                   int in_size) const;
	int        return_size() const;
    int        take_size() const;
    int        date() const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

private:
	double m_deriv_level;
};

/**
 * @nodoc
 */
class DECL_LAW scaletrans_law: public multiple_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	scaletrans_law(law *in_left_law, law *in_right_law,
        law *in_xvec = NULL, law *in_yvec = NULL, law *in_zvec = NULL);
    void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs,int size) const;
	int        return_size() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;        // 700
};

/**
 * @nodoc
 */
class DECL_LAW vectrans_law: public multiple_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	vectrans_law(law *in_left_law, law *in_right_law,
        law* in_xvec = NULL, law* in_yvec = NULL, law* in_zvec = NULL, law* in_index = NULL);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs,int size) const;
	int        return_size() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;        // 700
};

/**
 * @nodoc
 */
class DECL_LAW postrans_law: public multiple_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	postrans_law(law *in_left_law, law *in_right_law);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	multiple_law *make_one(law **subs,int size) const;
	int        return_size() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;        // 700
};

/**
 * @nodoc
 */
class DECL_LAW patterntrans_law: public unary_data_law
{
public:
    const char* class_name();    // virtual method in base class
	logical    isa(int t) const;
	static int id();
	int        type() const;
	patterntrans_law(base_pattern_law_data *in_law_data);
	void       evaluate(double const *x,double *answer) const;
	char const *symbol(law_symbol_type type=DEFAULT) const;
	unary_data_law *make_one(law_data *in_data) const;
	int        return_size() const;
	int        take_size() const;
	int        scale_dim() const;
	logical	   term_domain(int term,SPAinterval &domain) const;
	int        date() const;	   // 700
};


/**
 * @nodoc
 */
law *substitution(law *in_law,law **sub_laws);

/**
 * @nodoc
 */
law *substitution_identity(law *in_law,law **sub_laws);

/**
 * @nodoc
 */
law *substitution_unary(law *in_law,law **sub_laws);

/**
 * @nodoc
 */
law *substitution_binary(law *in_law,law **sub_laws);

/**
 * @nodoc
 */
law *substitution_multiple(law *in_law,law **sub_laws);

/**
 * @nodoc
 */
law *substitution_other(law *in_law,law **sub_laws);


// moved from law_util.hxx (declaration)
//		simplify.cxx (code)
// bstander 1/27/99
//

/**
 * @nodoc
 */
DECL_LAW law *numerator(law *eq);

/**
 * @nodoc
 */
DECL_LAW law *denominator(law *eq);

/**
 * @nodoc
 */
logical simplify_zero(law const *inlaw);


/**
 * @nodoc
 */
DECL_LAW double law_multiple( law *in_law1,
					 law *in_law2);

/**
 * @nodoc
 */
logical simplify_linear(law    *inlaw,
						       double *a,
						       double *b);

// Sorts the list of singularities,
// removes duplicates, and removes items before start or after end.
// NOTE: The size of the arrays may change.

/**
 * @nodoc
 */
DECL_LAW int sort_singularities(double** where,
					            int**  type,
								int num,
					            double start=-DBL_MAX,
					            double end=-DBL_MAX
								);
/** @} */
#endif
