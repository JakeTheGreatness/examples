/*
 *  implement the garbage collector
 */

#include "gc.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>


/*
 *  here we record all the garbage collectors created.
 */

static gc *list_of_gc = 0;


entity::entity (void)
  : data(0), status(freed), a_next(0), r_next(0), f_next(0)
{
}

entity::~entity (void)
{
  /* we ignore deletion of an entity */
}


/*
 *  copy   - the user of the garbage collector is responsible
 *           for maintaining entities.  They cannot be copied
 *           and must be allocated explicitly whenever required
 *           by the user - and not by the C++ substructure.
 *
 *           pre-condition :  none.
 *           post-condition:  all fields are set to null and
 *                            the state set to in_error.
 */

entity::entity (const entity &from)  // copy
{
  // to do
}


/*
 *  operator=  - the user of the garbage collector is responsible
 *               for maintaining entities.  They cannot be copied
 *               and must be allocated explicitly whenever required
 *               by the user - and not by the C++ substructure.
 *
 *               pre-condition :  none.
 *               post-condition:  all fields are set to null and
 *                                the state set to in_error.
 */

entity& entity::operator= (const entity &from)  // assignment
{
  // to do
}



void entity::do_assert (void)
{
  assert ((status & in_error) == 0);
}


void entity::unmark (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (marked))) % (static_cast <unsigned int> (max_state))));
}


void entity::mark (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (marked));
}


bool entity::is_marked (void)
{
  do_assert ();
  return (status & marked) != 0;
}


void entity::free (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (freed));
}


void entity::unfree (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (freed))) % (static_cast <unsigned int> (max_state))));
}


bool entity::is_free (void)
{
  do_assert ();
  return (status & freed) != 0;
}


void entity::used (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (in_use));
}


void entity::unused (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (in_use))) % (static_cast <unsigned int> (max_state))));
}


bool entity::is_used (void)
{
  do_assert ();
  return (status & in_use) != 0;
}


gc::gc (int no_of_bytes, const char *description)
  : rooted(0), allocated(0), bytes(no_of_bytes), free_list(0)
{
  desc = strdup (description);
  next = list_of_gc;
}


gc::~gc ()
{
  // to do
}


void *gc::allocate (entity *&e)
{
  // to do
}


void gc::unroot (entity *e)
{
  // to do
}


gc *gc::find_gc (unsigned int no_bytes)
{
  // to do
}


/*
 *  garbage_collect - wrapper to invoke the garbage collection methods.
 */

void garbage_collect (void)
{
  // to do
}


/*
 *  allocate - pre-condition:  init_garbage has been called to maintain a, bytes, heap.
 *             post-condition: entity, e, is filled in and the allocated memory is returned.
 */

void *allocate (unsigned int bytes, entity *&e)
{
  // to do
}


/*
 *  init_garbage - pre-condition :  none.
 *                 post-condition:  a garbage collector is created to serve calls
 *                                  for bytes amount of memory.
 */

gc *init_garbage (unsigned int bytes, const char *description)
{
  // to do
}


entity *gc::get_entity (void *data)
{
  // to do
}


bool gc::is_rooted (entity *e)
{
  // to do
}


void gc::root (entity *e)
{
  // to do
}


int gc::no_of_allocated (void)
{
  // to do
}


int gc::no_of_freed (void)
{
  // to do
}


int gc::no_of_rooted (void)
{
  // to do
}


void gc::stats (void)
{
  // to do
}


/*
 *  mark_allocated - pre-condition :  none.
 *                   post-condition:  only marks allocated entities.
 *                                    all other entities are left alone.
 */

void gc::mark_allocated (void)
{
  // to do
}


/*
 *  collect - pre-condition :  assumes that all allocated entities have been marked
 *                             and then all rooted entities walked and set as used.
 *            post-condition:  any allocated entity still set marked is no longer needed
 *                             and is thus returned to the free list.
 */

void gc::collect (void)
{
  // to do
}


void *operator new (std::size_t bytes)
{
#if defined(USE_GC)
  entity *e;
  
  return allocate (bytes, e);
#else
  return malloc (bytes);
#endif
}
