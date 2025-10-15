/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( ENTITY_PHL_MACRO )
#define ENTITY_PHL_MACRO

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// Declarations for previous and next pointers.
// and declarations/definitions for previous and next functions.
#define PHL_CHAIN_DEC( T) \
  T* previous_ptr; \
  T* next_ptr; \
public: \
  T* previous() const { return( previous_ptr); }; \
  T* next() const { return( next_ptr); }; \
protected: \
  void set_previous( T*); \
  void set_next( T*);

// Add this PHL entity to chain in front of ent
// and return pointer to this.
#define PHL_HOOK_DEC( T) \
  T* hook( T*);

#define PHL_HOOK_DEF( T) \
  T* T::hook( T* ent) { \
    if ( ent != 0) { \
      check_valid( this); \
      check_valid( ent); \
      if ( ent->previous_ptr) { \
        check_valid( ent->previous_ptr); \
        ent->previous_ptr->next_ptr = this; \
      } \
      this->previous_ptr = ent->previous_ptr; \
      ent->previous_ptr = this; \
      this->next_ptr = ent; \
    } \
  return( this); \
} \
  void T::set_previous( T* ent) { \
    if ( ent != previous_ptr) { \
      check_valid( this); \
      previous_ptr = ent; \
    } \
} \
  void T::set_next( T* ent) { \
    if ( ent != next_ptr) { \
      check_valid( this); \
      next_ptr = ent; \
    } \
}

// Remove this PHL entity from it's chain
// and return the pointer to the next PHL entity in chain.
#define PHL_UNHOOK_DEC( T) \
  T* unhook();

#define PHL_UNHOOK_DEF( T) \
  T* T::unhook() { \
    T* nx = this->next_ptr; \
    if ( this->previous_ptr || this->next_ptr) { \
      check_valid( this); \
      if ( this->previous_ptr) { \
        check_valid( this->previous_ptr); \
        this->previous_ptr->next_ptr = this->next_ptr; \
      } \
      if ( this->next_ptr) { \
        check_valid( this->next_ptr); \
        this->next_ptr->previous_ptr = this->previous_ptr; \
      } \
      this->next_ptr = 0; \
      this->previous_ptr = 0; \
    } \
  return( nx); \
}

#endif
