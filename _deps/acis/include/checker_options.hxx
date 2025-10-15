/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header defining data structures for checker options class
/*******************************************************************/

#ifndef CHK_OPTS_HXX
#define CHK_OPTS_HXX

#include "dcl_intr.h"
#include "mmgr.hxx"

#include "checker_options_enums.hxx"

class checker_properties;
class checker_modes;
class checker_selections;

class checker_options_reader;

class DECL_INTR checker_options : public ACIS_OBJECT 
{
public:
    checker_options();
    checker_options(const checker_options& rhs);
    ~checker_options();

    checker_options& operator=(const checker_options& rhs);

    //////////////////////////////////////////////////////
    // Properties
	void set_props_to_defaults(void);
    void set_props_using_global_options(void);
    void set_prop( CHECK_GLOBAL_OPTS_ENUM opt, int val );
    int  get_prop( CHECK_GLOBAL_OPTS_ENUM opt ) const;
    const char* get_prop_string( const CHECK_GLOBAL_OPTS_ENUM opt ) const;
    const checker_properties* get_properties( void ) const;

    /////////////////////////////////////////////////////
    // Modes
    void set_mode( CHECK_MODES_ENUM, logical state );
    logical get_mode( CHECK_MODES_ENUM ) const;
    const char* get_mode_string( const CHECK_MODES_ENUM mode) const;
    const checker_modes* get_modes( void ) const;

    /////////////////////////////////////////////////////
    // Selections
    void add_sel(CHECK_SELECTION_ENUM);
    void add_sel_intersection( CHECK_SELECTION_ENUM, CHECK_SELECTION_ENUM );
    void add_sel_cond( CHECK_SELECTION_ENUM, CHECK_RUNTIME_LEVEL_ENUM );
    void remove_sel(CHECK_SELECTION_ENUM);
    const checker_selections* get_selections( void ) const;

private:
    // Data
    class checker_options_impl;
    checker_options_impl *m_pImpl;

    friend class checker_options_reader;
};

#endif

