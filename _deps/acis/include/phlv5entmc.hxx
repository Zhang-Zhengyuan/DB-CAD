// $Id: phlv5entmc.hxx,v 1.1.1.1 2002/06/11 22:12:17 rocon Exp $
// COPYRIGHT DASSAULT SYSTEMES 1989
// =============================================================================
// IncludeFile: entity_phlv5_macro.hxx	Macros for Hidden Line Removal Entities
// -----------------------------------------------------------------------------
//

#if !defined( ENTITY_PHLV5_MACRO )
#define ENTITY_PHLV5_MACRO

#include "dcl_phlv5.h"

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// Declarations for previous and next pointers.
// and declarations/definitions for previous and next functions.
#define PHLV5_CHAIN_DEC( T) \
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
#define PHLV5_HOOK_DEC( T) \
  T* hook( T*);

#define PHLV5_HOOK_DEF( T) \
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
#define PHLV5_UNHOOK_DEC( T) \
  T* unhook();

#define PHLV5_UNHOOK_DEF( T) \
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




