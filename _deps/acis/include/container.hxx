/* $Id: container.hxx,v 1.4 2002/08/09 17:15:16 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( ENTITY_DATA_CONTAINER_CLASS )
#define ENTITY_DATA_CONTAINER_CLASS

#include "dcl_kern.h"
#include "logical.h"
#include "mmgr.hxx"

class BULLETIN;
class HISTORY_STREAM;
class ENTITY;
class SizeAccumulator;
class entity_handle;
class entity_proxy;

typedef logical REVBIT;
typedef int tag_id_type;

class DECL_KERN entity_data : public ACIS_OBJECT {

    friend class entity_data_container;

private:

    // Points to a bulletin if this entity has been changed
	// in this operation, or NULL if not. Also used (via
	// pointer punning) for chaining of entities in free lists.
    BULLETIN *rollback_ptr;

    HISTORY_STREAM *history_ptr;
	tag_id_type tag_no;

    // use count data
    int use_count;

    // number of bulletins referring to entity
    int bull_count;

	// backpointer to corresponding entity proxy
	entity_proxy* proxy_ptr;

private:

    // Data writing routines
    void set_history(const ENTITY *ent, HISTORY_STREAM *hs);
    logical assign_tag(const ENTITY *ent, tag_id_type id);
    int add_bulletin_count() { bull_count++; return bull_count; }
    int remove_bulletin_count( const ENTITY *ent, logical clear_history );

	void set_proxy( entity_proxy * proxy );

    // use count control
    void add();
    void remove();

protected:
    
	// constructor and destructor are protected so only entity_data_containers 
    // can make and delete these
    entity_data();
    ~entity_data();

public:

    // Data reading routines
    BULLETIN *&rollback() { return rollback_ptr; }
    HISTORY_STREAM *history() const { return history_ptr; }
    tag_id_type tag(const ENTITY *ent, logical assign_new = TRUE);
    int bulletin_count() { return bull_count; }
	entity_proxy* proxy() const {return proxy_ptr;}

// Jeff - Debug helper for non-repeatable addresses - internal use only
#ifdef INTERNAL_DEBUG_CHECKS
/**
 * @nodoc
 */
	int index;
#endif
};

class DECL_KERN entity_data_container {

    friend class ENTITY;
    friend class BULLETIN;  // for restoring history

private:

    entity_data *entity_data_ptr;

    // The constructor and destructor are private so only ENTITY can
    // make and delete these.
    entity_data_container();   
    entity_data_container( const entity_data_container& edc );
    ~entity_data_container();

    // share data for rollback
    void share_data( entity_data_container& edc );

    // Data writing routines
    void set_history(const ENTITY *ent, HISTORY_STREAM *hs) { entity_data_ptr->set_history(ent, hs); }
    logical assign_tag(const ENTITY *ent, tag_id_type id) const { return entity_data_ptr->assign_tag(ent, id); }
    int add_bulletin_count() { return entity_data_ptr->add_bulletin_count(); }
    int remove_bulletin_count( const ENTITY *ent, logical clear_history ) 
    { 
        return entity_data_ptr->remove_bulletin_count( ent, clear_history ); 
    }

	void set_proxy(entity_proxy* proxy) { entity_data_ptr->set_proxy(proxy); }

protected:

	void full_size(SizeAccumulator&, logical = TRUE) const;
    void init_ptr();

public:

    // Data reading routines
    BULLETIN *&rollback() const { return entity_data_ptr->rollback(); }
    HISTORY_STREAM *history() const { return entity_data_ptr->history(); }
    tag_id_type tag(const ENTITY *ent, logical assign_new = TRUE) const; 
    int bulletin_count() const { return entity_data_ptr->bulletin_count(); }
    int use_count() const { return entity_data_ptr->use_count; }
	entity_proxy* proxy() const {return entity_data_ptr->proxy();}

    // Assignment operator
    entity_data_container& operator=(const entity_data_container& e);
};


#include "vers.hxx"

class SPAbox;
class SPApar_box;
class ps_polygon;
class use_counted_ps_polygon;

/**
 * @nodoc
 */
class use_counted_box : public ACIS_OBJECT {

	friend class entity_box_container;

	SPAbox* box_ptr;
	int use_count;

private:

	use_counted_box();

protected:

	use_counted_box( SPAbox* in_box);
	~use_counted_box();
	
	void add();
	void remove();
};

/**
 * @nodoc
 */
class use_counted_ver_box : public ACIS_OBJECT {

	friend class entity_ver_box_container;

	SPAbox* ver_box_ptr;
	AcisVersion av;
	int use_count;

private:

	use_counted_ver_box();

protected:

	use_counted_ver_box( SPAbox* in_ver_box);
	~use_counted_ver_box();
	
	void add();
	void remove();
};

/**
 * @nodoc
 */
class use_counted_par_box : public ACIS_OBJECT {

	friend class entity_par_box_container;

	SPApar_box* par_box_ptr;
	int use_count;

private:

	use_counted_par_box();

protected:

	use_counted_par_box( SPApar_box* in_par_box);
	~use_counted_par_box();
	
	void add();
	void remove();
};

/**
 * @nodoc
 */
class DECL_KERN entity_box_container {

	use_counted_box* uc_box_ptr;

public:

	entity_box_container();
	~entity_box_container();

    entity_box_container& operator=(const entity_box_container&);

	logical set_box( ENTITY*, SPAbox*);
	SPAbox* get_box() const;
};

/**
 * @nodoc
 */
class DECL_KERN entity_ver_box_container {

	use_counted_ver_box* uc_ver_box_ptr;

public:

	entity_ver_box_container();
	~entity_ver_box_container();

    entity_ver_box_container& operator=(const entity_ver_box_container&);

	logical set_ver_box( ENTITY*, SPAbox*);
	SPAbox* get_ver_box() const;
};

/**
 * @nodoc
 */
class DECL_KERN entity_par_box_container {

	use_counted_par_box* uc_par_box_ptr;

public:

	entity_par_box_container();
	~entity_par_box_container();

    entity_par_box_container& operator=(const entity_par_box_container&);

	logical set_par_box( ENTITY*, SPApar_box*);
	SPApar_box* get_par_box() const;
};

/**
 * @nodoc
 */
class DECL_KERN entity_ps_polygon_container {

	use_counted_ps_polygon* uc_ps_polygon_ptr;

public:

	entity_ps_polygon_container();
	~entity_ps_polygon_container();

    entity_ps_polygon_container& operator=(const entity_ps_polygon_container&);

	logical set_ps_polygon( ENTITY*, ps_polygon*);
	ps_polygon* get_ps_polygon() const;
};


#endif
