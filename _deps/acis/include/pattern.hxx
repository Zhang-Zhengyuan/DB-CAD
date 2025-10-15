/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PATTERN_HXX
#define PATTERN_HXX

/**
* @file pattern.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPATTERNS
 *
 * @{
 */
#include "dcl_kern.h"
#include "logical.h"

#include "vlists.hxx"
#include "position.hxx"
#include "transf.hxx"

#include "api.hxx"
#include "lists.hxx"

class law;
class SPAtransf;
class ENTITY;
class pattern_datum;
class pointer_map;
class law_data;
class APATTERN;
class ENTITY_LIST;
class VOID_LIST;
class restore_data;
class pattern;
class ATTRIB;
class ATTRIB_PAT_HOLDER;
class FACE;

// This enum is to specify the type of pattern, when known
/**
 * @nodoc
 */
enum PAT_TYPE
{
    PAT_UNKNOWN                     =   1,   // The nature of the pattern is unknown
    PAT_UNKNOWN_ON_EDGE             =  -1,   // The nature of the pattern is unknown, except that
                                             // it has been applied to a feature connected to one
                                             // or more edges partly external to the feature
    PAT_MISC                        =   2,   // The pattern has been checked, but doesn't belong
    PAT_MISC_ON_EDGE                =  -2,   // to any special category
    PAT_CIRCULAR_UNIFORM            =   3,   // The pattern is circular and uniform (and the
    PAT_CIRCULAR_UNIFORM_ON_EDGE    =  -3,   // elements rotate with the pattern)
    PAT_LINEAR_1D_UNIFORM           =   4,   // The pattern is linear in 1D, and uniform
    PAT_LINEAR_1D_UNIFORM_ON_EDGE   =  -4,
    PAT_LINEAR_2D_UNIFORM           =   5,   // The pattern is linear in 2D, and uniform
    PAT_LINEAR_2D_UNIFORM_ON_EDGE   =  -5,
    PAT_LINEAR_3D_UNIFORM           =   6,   // The pattern is linear in 3D, and uniform
    PAT_LINEAR_3D_UNIFORM_ON_EDGE   =  -6,
    PAT_CYLINDRICAL_UNIFORM         =   7,   // The pattern is cylindrical and uniform (and the
    PAT_CYLINDRICAL_UNIFORM_ON_EDGE =  -7,   // elements rotate with the pattern)
    PAT_RADIAL_UNIFORM              =   8,   // The pattern is cylindrical and uniform (and the
    PAT_RADIAL_UNIFORM_ON_EDGE      =  -8,   // elements rotate with the pattern)
    PAT_POLAR_GRID_UNIFORM          =   9,   // The pattern is cylindrical and uniform (and the
    PAT_POLAR_GRID_UNIFORM_ON_EDGE  =  -9,   // elements rotate with the pattern)
//    PAT_DUMMY                       =  10,   // To be renamed
//    PAT_DUMMY_ON_EDGE               = -10,
    PAT_HEX_2D_UNIFORM              =  11,   // The pattern is hex in 2D, and uniform
    PAT_HEX_2D_UNIFORM_ON_EDGE      = -11,
    PAT_HEX_3D_UNIFORM              =  12,   // The pattern is hex in 3D, and uniform
    PAT_HEX_3D_UNIFORM_ON_EDGE      = -12
};

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_compose</tt> instead.
 * <br><br>
 * <b>Role:</b> Creates a pattern that is the composition of <tt>pat1</tt> and <tt>pat2</tt> and returns its
 * pointer. (MAC NT UNIX platforms only).
 * <br><br>
 * @param pat1
 * first pattern.
 * @param pat2
 * second pattern.
 */
DECL_KERN pattern* operator*(const pattern& pat1, const pattern& pat2);// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_compose</tt> instead.
 * <br><br>
 * <b>Role:</b> Creates a pattern that is the concatenation of <tt>pat1</tt> and <tt>pat2</tt> and returns
 * its pointer. (MAC NT UNIX platforms only).
 * <br><br>
 * @param pat1
 * first pattern.
 * @param pat2
 * second pattern.
 */
DECL_KERN pattern* operator+(const pattern& pat1, const pattern& pat2);// internal use only
/**
 * @nodoc
 */
DECL_KERN law_data* restore_law_data();// internal use only
/**
 * @nodoc
 */
DECL_KERN pattern* restore_pattern();// internal use only

/**
 * Provides all information necessary to generate a regular or irregular pattern of entities from a single, "seed" entity.
 * <br>
 * @see SPAinterval, SPAtransf, law
 */
class DECL_KERN pattern : public ACIS_OBJECT
{
    friend class pattern_holder;
    friend class APATTERN;
    friend DECL_KERN pattern* operator*(
        const pattern& pat1,
        const pattern& pat2
        );
    friend DECL_KERN pattern* restore_pattern();
    friend DECL_KERN law_data* restore_law_data();
    friend DECL_KERN pattern* operator+(
        const pattern& pat1,
        const pattern& pat2
        );
private:
	// Saved data
	mutable int		use_count;
	law*			trans_vec;
	law*			x_vec;
	law*			y_vec;
	law*			scale;
	law*            z_vec;
	law*			keep;
	pattern_datum** dl;
	int             list_size;
	SPAtransf*      root_transf;
    PAT_TYPE        type;

	// Cached data
	double*         coords;
	SPAinterval*    domains;
	int*            dim_sizes;
	int             dim;
	int             elements;
	int*            map;
    APATTERN*       apat;

	void            update_cache_data();
	void            update_map_data();
	logical         get_coords(int which_one) const;
    void            create_dl();
    // Returns TRUE if any of the pattern's laws are non-NULL
    logical         has_laws() const;
    // Returns TRUE if dl exists and every member is populated
    logical         has_full_list() const;
    // Create a pattern_datum, using data from the indexed element
    pattern_datum*  create_datum(int index, logical is_present);
    // Compose routines
    logical         make_z_vec(logical reflect = FALSE);
    logical         strip_all_law_domains();
    logical         update_all_law_domains(SPAinterval* new_domains);
    void            compose_with_law(law* in_law);
    void            compose_transform(pattern& that_pat, int root_index = 0);
    void            compose_keep(law* in_keep);
    void            compose_dl(const pattern& that_pat);
    void            concatenate_transform(pattern& that_pat,
                        law* keep_this, law* keep_that, const SPAtransf& rel_transf);
    void            concatenate_keep(pattern& that_pat, law* keep_this, law* keep_that,
                        law** offset_law, logical keep_that_root);
    void            concatenate_dl(const pattern& that_pat, const SPAtransf& cat_trans);
	void            set_scale_shear(law* scale_law,
                        const SPAposition& root = SPAposition(0, 0, 0), logical merge = TRUE);
    logical         scale_element(int index, const SPAtransf& in_scale,
                        const SPAposition& root, logical merge = TRUE);

protected:

	~pattern();

public:

	// Basic constructors for a pattern
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_create_from_laws</tt> instead.
 * <br><br>
 * <b>Role:</b> Constructs a pattern from a set of laws and an optional root transformation.
 * <br><br>
 * @param in_trans_vec
 * translation law.
 * @param in_x_vec
 * x-axis orientation law.
 * @param in_y_vec
 * y-axis orientation law.
 * @param in_scale
 * scaling law.
 * @param in_z_vec
 * z-axis orientation law.
 * @param in_keep
 * keep filter law.
 * @param in_root_transf
 * root transformation.
 * @param in_type
 * internal use only.
 */
	pattern(
        law*                in_trans_vec,           // translation law
		law*                in_x_vec        = NULL, // x-axis orientation law
		law*                in_y_vec        = NULL, // y-axis orientation law
		law*                in_scale        = NULL, // scaling law
		law*                in_z_vec        = NULL, // z-axis orientation law
		law*                in_keep         = NULL, // keep-filter law
		const SPAtransf&    in_root_transf          // root transformation
                                            = *(const class SPAtransf*)NULL_REF,
        PAT_TYPE            in_type         = PAT_UNKNOWN
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_create_from_list</tt> instead.
 * <br><br>
 * <b>Role:</b> Constructs a pattern from an array of positions. The pattern positions are
 * relative to pattern coordinates.
 * <br><br>
 * @param in_positions
 * array of pattern positions.
 * @param in_list_size
 * number of pattern transforms.
 * @param in_type
 * internal use only.
 */
	pattern(
        const SPAposition* in_positions     // array of pattern positions
            = NULL,                         // relative to pattern coordinate
                                            // system
		int			    in_list_size        // number of pattern positions
            = 0,                            // in in_positions array
        PAT_TYPE        in_type         
		    = PAT_UNKNOWN
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_create_from_list</tt> instead.
 * <br><br>
 * <b>Role:</b> Constructs a pattern from an array of transformations. The pattern transforms are
 * relative to pattern coordinates.
 * <br><br>
 * @param in_transfs
 * array of pattern transforms.
 * @param in_list_size
 * number of pattern transforms.
 * @param in_type
 * internal use only.
 */
	pattern(
        const SPAtransf*    in_transfs,     // array of pattern transforms
                                            // relative to pattern coordinate
                                            // system
		int			        in_list_size,   // number of pattern transforms
                                            // in in_transfs array
        PAT_TYPE            in_type         
		    = PAT_UNKNOWN
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_copy</tt> instead.
 * <br><br>
 * <b>Role:</b> Constructs a pattern from another pattern.
 * <br><br>
 * @param in_pattern
 * pattern to copy.
 */
	pattern(const pattern& in_pattern);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Creates a copy of an item that does not share any data with the original.
 * Allocates new storage for all member data and any pointers. Returns a pointer to
 * the copied item.
 * <br><br>
 * In a deep copy, all the information about the copied item is self-contained in a new memory
 * block. By comparison, a shallow copy stores only the first instance of the item in memory, and
 * increments the reference count for each copy.
 * <br><br>
 * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that have already been
 * deep copied. For example, a deep copy of a complex model results in self-contained data, but
 * identical sub-parts within the model are allowed to share a single set of data.
 * <br><br>
 * @param pm
 * list of items already deep copied.
 */
    pattern* deep_copy(pointer_map* pm = NULL) const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_compose</tt> instead.
 * <br><br>
 * <b>Role:</b> Composes the pattern with the pattern referenced by <tt>in_pat</tt>.
 * <br><br>
 * @param in_pat
 * pattern to be composed with this pattern.
 */
    void compose(const pattern& in_pat);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_compose</tt> instead.
 * <br><br>
 * <b>Role:</b> Replaces this pattern with its composition with <tt>in_pat</tt>.
 * <br><br>
 * @param in_pat
 * pattern to compose with this pattern.
 */
    const pattern& operator*=(const pattern& in_pat);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_concatenate</tt> instead.
 * <br><br>
 * <b>Role:</b> Concatenates the pattern with the pattern referenced by <tt>cat_pat</tt>. If <tt>cat_trans</tt>
 * is given, it is applied to <tt>cat_pat</tt> prior to concatenation.
 * <br><br>
 * @param cat_pat
 * pattern to be concatenated with this pattern.
 * @param cat_trans
 * transformation to be applied to cat_pat.
 */
    void concatenate(const pattern& cat_pat, const SPAtransf& cat_trans = *(SPAtransf*)NULL_REF);

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_concatenate</tt> instead.
 * <br><br>
 * <b>Role:</b> Replaces this pattern with its concatenation with <tt>in_pat</tt>.
 * <br><br>
 * @param in_pat
 * pattern to concatenate with this pattern.
 */
    const pattern& operator+=(const pattern& in_pat);

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_reflect</tt> instead.
 * <br><br>
 * <b>Role:</b> Replaces this pattern by its reflection, using <tt>root</tt> and <tt>normal</tt> to define the reflecting plane.
 * <br><br>
 * @param root
 * a point on the reflecting plane.
 * @param normal
 * a vector normal to the reflecting plane.
 */
    void reflect(const SPAposition& root, const SPAvector& normal);

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_mirror</tt> instead.
 * <br><br>
 * <b>Role:</b> Replaces this pattern by its concatenation with its reflection, using <tt>root</tt> and <tt>normal</tt> to define the reflecting plane.
 * <br><br>
 * @param root
 * a point on the reflecting plane.
 * @param normal
 * vector normal to the reflecting plane.
 */
    void mirror(const SPAposition& root, const SPAvector& normal);

    // Reference-counting methods
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> This is not usually called by an application directly.  In order to preserve
 * a copy of this pattern, an application calls this method. It is also called by the pattern
 * constructors for the pattern being constructed, as well as for all of its sublaws. It
 * increments the <tt>use_count</tt>.
 */
	void add() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_destroy</tt> instead.
 * <br><br>
 * <b>Role:</b> Applications should call remove instead of the tilde (~) destructor to
 * get rid of a pattern. Decrements the <tt>use_count</tt>. This is called by the pattern destructors for the pattern
 * being destructed, as well as for all of its sublaws. The remove method calls the destructor if
 * <tt>use_count</tt> falls to zero. Used for memory management.
 */
	void remove();// internal use only
/**
 * @nodoc
 */
    void map_index(int& index) const;// internal use only
/**
 * @nodoc
 */
    void unmap_index(int& index) const;// internal use only

    // Comparison
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if pat is identical to this pattern.
 * <br><br>
 * @param pat
 * pattern to compare with this pattern.
 */
    logical operator==(const pattern& pat);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if pat is not identical to this pattern.
 * <br><br>
 * @param pat
 * pattern to compare.
 */
    logical operator!=(const pattern& pat);// internal use only

    // If the indices being used have already been mapped, turn off
    // mapping within get_rel_transf() by setting use_map=FALSE, thus
    // preventing a second mapping from being applied.  Returns FALSE
    // if an invalid index is encountered.
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_query_transformation</tt> instead.
 * <br><br>
 * <b>Role:</b> Returns the relative transformation used to generate the pattern element
 * indexed by "from" from the element indexed by "to". The argument <tt>use_map</tt> is a flag to
 * map the indices to skip over any suppressed elements. The function returns <tt>FALSE</tt> if
 * either from or to are invalid indices.
 * <br><br>
 * @param from
 * index of the existing element.
 * @param to
 * index of new element.
 * @param out_trans
 * associated relative transform (output).
 * @param use_map
 * index flag.
 */
	logical get_transf(
        int             from,
		int             to,
		SPAtransf&      out_trans,
        logical         use_map
            = TRUE
        ) const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the transformation used to generate the pattern element indexed
 * by index. The argument <tt>use_map</tt> is a flag to map index to skip over any suppressed
 * elements. The function returns <tt>FALSE</tt> if index is invalid.
 * <br><br>
 * @param index
 * index.
 * @param out_trans
 * transform.
 * @param use_map
 * use map.
 */
	logical get_transf(
        int             index,
		SPAtransf&      out_trans,
        logical         use_map
            = TRUE
        ) const;// internal use only

/**
 * @nodoc
 */
    logical get_transfs(SPAtransf*& transforms, int& num) const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Saves the pattern to a SAT file.
 */
	void save() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Sets the list of <tt>pattern_datum</tt> pointers to <tt>in_dl</tt>, and updates the list size
 * stored with the pattern.
 * <br><br>
 * @param in_dl
 * pointer list.
 * @param in_list_size
 * list size.
 */
	void set_list(pattern_datum** in_dl, int in_list_size);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the translation law of the pattern.
 */
	law* get_trans();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the x-axis law of the pattern.
 */
	law* get_x();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the y-axis law of the pattern.
 */
	law* get_y();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the scale law of the pattern.
 */
	law* get_scale();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the z-axis law of the pattern.
 */
	law* get_z();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the pattern keep law.
 */
	law* get_keep();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern has a non-<tt>NULL</tt> <tt>z_vec</tt> member.
 */
    logical has_z_vec() const;// internal use only
/**
 * @nodoc
 */
    logical has_shear() const;// internal use only
/**
 * @nodoc
 */
    logical has_duplicates() const;// internal use only
/**
 * @nodoc
 */
    logical remove_duplicates();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_to_entity</tt> instead.
 * <br><br>
 * <b>Role:</b> Returns the pattern as an entity.
 */
    APATTERN* get_APATTERN();// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the list of <tt>pattern_datum</tt> pointers.
 */
	const pattern_datum* const* get_list() const;// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the size of the list of <tt>pattern_datum</tt> pointers.
 */
	int get_list_size() const;// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_query_coordinate_dimension</tt> instead.
 * <br><br>
 * <b>Role:</b> Returns the dimension of the pattern laws' domain (input).
 */
	int take_dim() const;// internal use only

    // Returns the size of the pattern domain in the dimension
    // specified by index.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the size of the pattern domain in the dimension specified by the zero-based <tt>index</tt>.
 * <br><br>
 * @param index
 * zero-based index specifying which dimension is being queried.
 */
	int domain_size(int index) const;// internal use only

    // Returns the number of elements in the pattern.
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_query_size</tt> instead.
 * <br><br>
 * <b>Role:</b> Returns the number of elements in the pattern.
 */
	int num_elements() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the internal index of the first element included in the pattern
 * (the first element not suppressed by a keep law or list entry).
 */
    int first_included_element() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Adds the element indexed by index to the pattern. If this element was not formerly
 * suppressed (via a keep law or list entry), this method has no effect. Unlike the
 * <tt>restore_element</tt> methods, this method adds the element whether or not it already has an entry
 * in the pattern list.
 * <br><br>
 * @param index
 * index.
 */
    logical add_element(int index);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the element referred to by the internal value index is not suppressed by the pattern keep law or a list entry.
 * <br><br>
 * @param index
 * index.
 */
    logical is_included_element(int index) const;// internal use only
/**
 * @nodoc
 */
	logical get_check_pairs(
		int*&   these,
		int*&   those,
		int&    num_pairs
		);// internal use only
/**
 * @nodoc
 */
	logical classify_linear(
		const SPAtransf*    transfs,
		int                 num_transfs,
        SPAvector&          out_vec  = *(SPAvector*)NULL_REF
		);// internal use only
/**
 * @nodoc
 */
	logical classify_circular(
		const SPAposition&  in_test_pos, 
		const SPAvector&    in_test_vec,
		const SPAtransf*    transfs,
		int                 num_transfs,
        SPAposition&        out_pos  = *(SPAposition*)NULL_REF,
        SPAvector&          out_vec  = *(SPAvector*)NULL_REF
		);// internal use only
/**
 * @nodoc
 */
    PAT_TYPE classify(
        SPAposition&        out_pos  = *(SPAposition*)NULL_REF,
        SPAvector&          out_vec  = *(SPAvector*)NULL_REF,
        logical             force_redo = FALSE,
        const SPAposition&  test_pos = *(SPAposition*)NULL_REF,
        const SPAvector&    test_vec = *(SPAvector*)NULL_REF
        );// internal use only
/**
 * @nodoc
 */
    void mark_for_reclassification();// internal use only
/**
 * @nodoc
 */
    void mark_on_edge();// internal use only

    // Returns TRUE if the pattern exhibits circular symmetry
    // with respect to the specified circle
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits circular symmetry with respect to the specified circle.
 * <br><br>
 * @param root
 * circle's center.
 * @param axis
 * direction of circle's axis.
 */
	logical is_circular(
        const SPAposition& root,      // circle's center
        const SPAvector& axis         // direction of circle's axis
        ) const;// internal use only

    // Returns TRUE if the pattern exhibits cylindrical symmetry
    // with respect to the specified cylinder
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits cylindrical symmetry with respect to the specified cylinder.
 * <br><br>
 * @param root
 * position on axis.
 * @param axis
 * direction of axis.
 */
	logical is_cylindrical(
        const SPAposition& root,      // SPAposition on cylinder's axis
        const SPAvector& axis         // direction of cylinder's axis
        ) const;// internal use only

    // Returns TRUE if the pattern scales uniformly about the
    // specified axis

/**
 * @nodoc
 */
	logical is_scale_axial(
        const SPAvector& axis         // direction of test axis
        ) const;// internal use only

    // Returns TRUE if the pattern exhibits cylindrical symmetry
    // with respect to the specified cylinder
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits cylindrical symmetry with respect to the specified cylinder.
 * <br><br>
 * @param in_face
 * cylindrical face.
 */
	logical is_cylindrical(FACE* in_face) const;// internal use only

    // Returns TRUE if the pattern exhibits planar symmetry
    // with respect to the specified plane

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits planar symmetry with respect to the
 * specified plane.
 * <br><br>
 * @param root
 * position on plane.
 * @param normal
 * normal to plane.
 */
	logical is_planar(
        const SPAposition& root,      // SPAposition on plane
        const SPAvector& normal       // direction normal to plane
        ) const;// internal use only

    // Returns TRUE if the pattern exhibits planar symmetry
    // with respect to the specified plane

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits planar symmetry with respect to the specified plane.
 * <br><br>
 * @param in_face
 * planar face.
 */
	logical is_planar(FACE* in_face) const;// internal use only

    // Returns TRUE if the pattern exhibits spherical symmetry
    // with respect to the specified sphere
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits spherical symmetry with respect to the specified sphere.
 * <br><br>
 * @param root
 * center of sphere.
 */
	logical is_spherical(const SPAposition& root) const;// internal use only

    // Returns TRUE if the pattern exhibits spherical symmetry
    // with respect to the specified sphere
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the pattern exhibits spherical symmetry with respect to the specified sphere.
 * <br><br>
 * @param in_face
 * spherical face.
 */
	logical is_spherical(FACE* in_face) const;// internal use only

    // Returns TRUE if the pattern generates any rotation.  If
    // check_root is FALSE, the root transformation is not included
    // in the check.
/**
 * @nodoc
 */
	logical is_rotational(logical check_root) const;// internal use only
/**
 * @nodoc
 */
    logical is_uniform() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_transform</tt> instead.
 * <br><br>
 * <b>Role:</b> Transforms this pattern using <tt>in_transf</tt>.
 * <br><br>
 * @param in_transf
 * transform used.
 */
    void transform(const SPAtransf& in_transf);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_query_root_transformation</tt> instead.
 * <br><br>
 * <b>Role;</b> Returns a pointer to the root transformation of the pattern.
 */
 	const SPAtransf* get_root_transf() const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_root_transformation</tt> instead.
 * <br><br>
 * <b>Role:</b> Sets the root transform to that given by <tt>in_transf</tt>.
 * <br><br>
 * @param in_transf
 * transform to be used as the root transform.
 */
    void set_root_transf(const SPAtransf& in_transf);// internal use only

    // Returns TRUE if there was one to be removed
/**
 * @nodoc
 */
    logical remove_root_transf();// internal use only

/**
 * @nodoc
 */
    void root_to_first_element();// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_transform</tt> instead.
 * <br><br>
 * <b>Role:</b> Translates the pattern by merging the displacement law <tt>disp_law</tt> with
 * the pattern.
 * <br><br>
 * @param disp_law
 * displacement law used.
 */
	void translate(law* disp_law);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_transform</tt> instead.
 * <br><br>
 * <b>Role:</b> Rotates the orientation laws of the pattern by applying the rotation laws
 * <tt>rx_law</tt>, <tt>ry_law</tt>, and <tt>rz_law</tt> when <tt>merge</tt> is <tt>TRUE</tt>; otherwise, these three laws replace the
 * existing orientation laws.
 * <br><br>
 * @param rx_law
 * x rotation.
 * @param ry_law
 * y rotation.
 * @param rz_law
 * z rotation.
 * @param merge
 * merge flag.
 */
	void rotate(law* rx_law, law* ry_law, law* rz_law, logical merge = TRUE);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_scale</tt> instead.
 * <br><br>
 * <b>Role:</b> Sets the scale law, if absent, or modifies the existing one. In the latter case,
 * the <tt>merge</tt> flag determines whether or not <tt>scale_law</tt> is factored into the existing one or
 * completely replaces it. The position root specifies the point about which the scaling is
 * applied.
 * <br><br>
 * @param scale_law
 * new scale law.
 * @param root
 * point about which scaling is applied.
 * @param merge
 * flag for merge/replace.
 */
	void set_scale(
        law*                    scale_law,
        const SPAposition&   root
            = SPAposition(0, 0, 0),
        logical                 merge
            = TRUE
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Orients <tt>in_vec</tt> according to the axes defined for the first element of this pattern and returns the result as <tt>out_vec</tt>.
 * <br><br>
 * @param in_vec
 * vector to be oriented.
 * @param out_vec
 * resulting vector (output).
 */
    void orient_vec(const SPAvector& in_vec, SPAvector& out_vec) const;// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_filter</tt> instead.
 * <br><br>
 * <b>Role:</b> Sets the keep law, if absent, or modifies the existing one. In the latter case,
 * the merge flag determines whether or not <tt>keep_law</tt> is factored into the existing one or
 * completely replaces it.
 * <br><br>
 * @param keep_law
 * new keep law.
 * @param merge
 * flag for merge/replace.
 */
	void set_keep(law* keep_law, logical merge = TRUE);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_transform</tt> instead.
 * <br><br>
 * <b>Role:</b> Transforms the element referenced by index according to the transform move.
 * <br><br>
 * @param index
 * index of the affected element.
 * @param move
 * applied transform.
 */
    logical move_element(int index, const SPAtransf& move);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_transform</tt> instead.
 * <br><br>
 * <b>Role:</b> Transforms the element referenced by the coordinates <tt>in_coords</tt> according
 * to the transform move.
 * <br><br>
 * @param in_coords
 * coordinates of the affected element.
 * @param move
 * applied transform.
 */
    logical move_element(const double* in_coords, const SPAtransf& move);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_scale</tt> instead.
 * <br><br>
 * <b>Role:</b> Uniformly scales the element referenced by index by a factor equal to
 * <tt>in_scale</tt>, about the position root. The existing scaling is either factored in or replaced,
 * depending upon the setting of the flag merge. A value of <tt>FALSE</tt> is returned if <tt>in_scale</tt>
 * is unity.
 * <br><br>
 * @param index
 * index.
 * @param in_scale
 * scale factor.
 * @param root
 * point about which scaling is applied.
 * @param merge
 * flag for merge/replace.
 */
    logical scale_element(
        int                     index,
        double                  in_scale,
        const SPAposition&      root,
        logical                 merge
            = TRUE
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_scale</tt> instead.
 * <br><br>
 * <b>Role:</b> Uniformly scales the element referenced by <tt>in_coords</tt> by a factor equal to
 * <tt>in_scale</tt>, about the position root. The existing scaling is either factored in or replaced,
 * depending upon the setting of the flag merge. A value of <tt>FALSE</tt> is returned if <tt>in_scale</tt> is
 * unity.
 * <br><br>
 * @param in_coords
 * coordinates.
 * @param in_scale
 * scale factor.
 * @param root
 * point about which scaling is applied.
 * @param merge
 * flag for merge/replace.
 */
    logical scale_element(
        const double*           in_coords,
        double                  in_scale,
        const SPAposition&      root,
        logical                 merge
            = TRUE
        );// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_scale</tt> instead.
 * <br><br>
 * <b>Role:</b> Scales the element referenced by index by the factors taken from <tt>in_scale</tt>,
 * about the position root. The existing scaling is either factored in or replaced, depending
 * upon the setting of the flag merge. A value of <tt>FALSE</tt> is returned if all <tt>in_scale</tt> components
 * are unity.
 * <br><br>
 * @param index
 * index of the affected element.
 * @param in_scale
 * vector of scale factors.
 * @param root
 * root position for scaling.
 * @param merge
 * flag for merge/replace.
 */
    logical scale_element(
        int                 index,
        const SPAvector&    in_scale,
        const SPAposition&  root,
        logical             merge
            = TRUE
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_scale</tt> instead.
 * <br><br>
 * <b>Role:</b> Scales the element referenced by the coordinates <tt>in_coords</tt> by the factors
 * taken from <tt>in_scale</tt>, about the position root. The existing scaling is either factored
 * in or replaced, depending upon the setting of the flag merge. A value of <tt>FALSE</tt> is returned
 * if all <tt>in_scale</tt> components are unity.
 * <br><br>
 * @param in_coords
 * coordinates of the affected element.
 * @param in_scale
 * vector of scale factors.
 * @param root
 * root position for scaling.
 * @param merge
 * flag for merge/replace.
 */
    logical scale_element(
        const double*       in_coords,
        const SPAvector&    in_scale,
        const SPAposition&  root,
        logical             merge
            = TRUE
        );// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Restores the pattern element referenced by index to the location defined by
 * the pattern laws, by removing the element from the pattern list. Returns <tt>FALSE</tt>
 * if the element is not present in the list.
 * <br><br>
 * @param index
 * index of the affected element.
 */
    logical undo_move_element(int index);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Restores the pattern element referenced by the coordinates <tt>in_coords</tt> to the
 * location defined by the pattern laws, by removing the element from the pattern list.
 * Returns <tt>FALSE</tt> if the element is not present in the list.
 * <br><br>
 * @param in_coords
 * coordinates of the affected element.
 */
    logical undo_move_element(const double* in_coords);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_remove</tt> instead.
 * <br><br>
 * <b>Role:</b> Suppresses the element referenced by index, returning <tt>FALSE</tt> if the element
 * has already been removed. The element may be restored using the <tt>restore_element</tt> method.
 * <br><br>
 * @param index
 * index of the affected element.
 */
    logical remove_element(int index);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_remove</tt> instead.
 * <br><br>
 * <b>Role:</b> Suppresses the element referenced by the coordinates <tt>in_coords</tt>, returning
 * <tt>FALSE</tt> if the element has already been removed. The element may be restored using the
 * <tt>restore_element</tt> method.
 * <br><br>
 * @param in_coords
 * coordinates of the affected element.
 */
    logical remove_element(const double* in_coords);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_restore</tt> instead.
 * <br><br>
 * <b>Role:</b> Restores to the pattern the element referenced by index, returning <tt>FALSE</tt>
 * if the element is already present in the pattern. Unlike the <tt>add_element</tt> method, this method
 * does nothing unless there is already an entry for the element in the pattern list.
 * <br><br>
 * @param index
 * index of the affected element.
 */
    logical restore_element(int index);// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> use <tt>api_pattern_modify_element_restore</tt> instead.
 * <br><br>
 * <b>Role:</b> Restores to the pattern the element referenced by the coordinates <tt>in_coords</tt>,
 * returning <tt>FALSE</tt> if the element is already present in the pattern. Unlike the <tt>add_element</tt>
 * method, this method does nothing unless there is already an entry for the element in the
 * pattern list.
 * <br><br>
 * @param in_coords
 * coordinates of the affected element.
 */
    logical restore_element(const double* in_coords);// internal use only

	// Map from (idx,idy,idz) to a single index idn:
	// idn = (Nz (Ny idx + idy) + idz))
	// returning the associated law and the number of elements

/**
 * @nodoc
 */
    logical restore_elements(const pattern* pat);// internal use only
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Creates a scalar index law of the same size as the pattern.
 * <br><br>
 * @param index_law
 * law.
 */
	int make_element_index_law(law*& index_law) const;// internal use only

	// Map from (ix,iy,iz) to a single index n:
	// n = (Nz (Ny ix + iy) + iz))
    // returning the value of n

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the index of the pattern element, given its coordinates.
 * <br><br>
 * @param in_coords
 * coordinates.
 */
	int get_index(const double* in_coords) const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns the pattern coordinates of an element, given its index.
 * <br><br>
 * @param index
 * index.
 * @param out_coords
 * coordinates.
 */
    void get_coords(int index, double*& out_coords) const;// internal use only

    // Behaves as does get_index, except that suppressed
    // pattern elements are ignored.

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Behaves as does <tt>get_index</tt>, except that suppressed pattern elements are ignored.
 * <br><br>
 * @param in_coords
 * coordinates.
 */
	int get_visible_index(const double* in_coords) const;// internal use only

    // Behaves as does get_coords, except that suppressed
    // pattern elements are ignored.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Behaves as does <tt>get_coords</tt>, except that suppressed pattern elements are ignored.
 * <br><br>
 * @param index
 * index.
 * @param out_coords
 * coordinates.
 */
    void get_visible_coords(int index, double*& out_coords) const;// internal use only

/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Establishes the domain of a given term in the pattern laws.
 * <br><br>
 * @param which
 * term to bound.
 * @param answer
 * bounds for term.
 */
    logical term_domain(int which, SPAinterval& answer) const;// internal use only
};

/** @} */
#endif

