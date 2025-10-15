/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PATTERN_DATUM_HXX
#define PATTERN_DATUM_HXX

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

/**
 * @nodoc
 */
class DECL_KERN pattern_datum : public ACIS_OBJECT
{
private:
    SPAtransf       trans;
	logical      present;

    void transform(
        const SPAtransf& in_transf
        );

    void compose(
        const pattern_datum& in_datum
        );

    void reflect(
        const SPAposition&   root,
        const SPAvector&     normal
        );

	void save();

    void get_transf(
        SPAtransf& out_transf
        ) const;

    // Comparison
    logical operator==(
        const pattern_datum& datum
        );

    logical operator!=(
        const pattern_datum& datum
        );

public:
	pattern_datum();

    // Initializes the datum, returning TRUE if successful
	logical create(
        const SPAtransf& in_transf,  // sets the transform
                                        // properties of the datum
		logical             in_present  // sets the "keep" value
                                        // of the datum
        );

    SPAmatrix affine() const {return trans.affine();}
    SPAvector translation() const {return trans.translation();}
    double scale() const {return trans.scaling();}
    logical rotate() const {return trans.rotate();}
    logical reflect() const {return trans.reflect();}
    logical shear() const {return trans.shear();}
    logical identity() const {return trans.identity();}

    friend class pattern;
    friend class APATTERN;
};

#endif // PATTERN_DATUM_HXX


