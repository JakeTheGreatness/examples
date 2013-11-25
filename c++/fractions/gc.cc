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


/*
 *  pre-condition :  none.
 *  post-condition:  entity initialised and set to freed state.
 */

entity::entity (void)
  : data(0), status(freed), a_next(0), r_next(0), f_next(0)
{
}


/*
 *  pre-condition :  none.
 *  post-condition:  none.
 */

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


/*
 *  do_assert:  pre-condition :  entity is not in an error state.
 *              post-condition:  none.
 */

void entity::do_assert (void)
{
  assert ((status & in_error) == 0);
}


/*
 *  unmark - pre-condition :  none.
 *           post-confition:  mark flag is removed.
 */

void entity::unmark (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (marked))) % (static_cast <unsigned int> (max_state))));
}


/*
 *  mark - pre-condition :  none.
 *         post-confition:  mark flag is set.
 */

void entity::mark (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (marked));
}


/*
 *  is_marked - pre-condition :  none.
 *              post-confition:  return true if entity is marked.
 */

bool entity::is_marked (void)
{
  do_assert ();
  return (status & marked) != 0;
}


/*
 *  free - pre-condition :  none.
 *         post-confition:  free flag is set.
 */

void entity::free (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (freed));
}


/*
 *  unfree - pre-condition :  none.
 *           post-confition:  free flag is removed.
 */

void entity::unfree (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (freed))) % (static_cast <unsigned int> (max_state))));
}


/*
 *  is_free - pre-condition :  none.
 *            post-confition:  return true if entity is free.
 */

bool entity::is_free (void)
{
  do_assert ();
  return (status & freed) != 0;
}


/*
 *  used - pre-condition :  none.
 *         post-confition:  used flag is set.
 */

void entity::used (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) | static_cast <unsigned int> (in_use));
}


/*
 *  unused - pre-condition :  none.
 *           post-confition:  used flag is removed.
 */

void entity::unused (void)
{
  do_assert ();
  status = static_cast <state> (static_cast <unsigned int> (status) &
				((~(static_cast <unsigned int> (in_use))) % (static_cast <unsigned int> (max_state))));
}


/*
 *  is_used - pre-condition :  none.
 *            post-confition:  return true if entity is used.
 */

bool entity::is_used (void)
{
  do_assert ();
  return (status & in_use) != 0;
}


/*
 *  pre-condition :  none.
 *  post-condition:  construct garbage collector class.
 *                   It chains this garbage collector onto the list
 *                   of created garbage collectors.
 */

gc::gc (int no_of_bytes, const char *description)
  : rooted(0), allocated(0), bytes(no_of_bytes), free_list(0)
{
  desc = strdup (description);
  next = list_of_gc;
}


/*
 *  pre-condition :  none.
 *  post-condition:  gc is taken off the list_of_gc's
 *                   all entities and their contents are freed.
 */

gc::~gc ()
{
  // to do
}


/*
 *  allocate - pre-condition :  none.
 *             post-condition:  will return an entity from the
 *                              freelist (if one is present).
 *                              Otherwise it creates a new entity
 *                              and mallocs memory and assigns it
 *                              to the entities data field.
 *                              The entity is set 'in_use'.
 */

void *gc::allocate (entity *&e)
{
  // to do
}


/*
 *  unroot - pre-condition :  e was created via allocate.
 *           post-condition:  e is removed from the rooted list.
 */

void gc::unroot (entity *e)
{
  // to do
}


/*
 *  find_gc - pre-condition :  none.
 *            post-condition:  returns a previously created garbage
 *                             collector object which manages heaps of
 *                             no_bytes in size.  It returns 0 if no
 *                             garbage collector is found.
 */

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


/*
 *  get_entity - pre-condition :  data must have been created by
 *                                a call to new below.
 *               post-condition:  returns the entity associated with
 *                                data.
 */

entity *gc::get_entity (void *data)
{
  // to do
}


/*
 *  is_rooted - pre-condition :  e has been created by a call to allocate.
 *              post-condition:  returns true if entity, e, has been rooted.
 */

bool gc::is_rooted (entity *e)
{
  // to do
}


/*
 *  root - pre-condition :  e has been create by a call to allocate.
 *         post-condition:  e is added to the rooted list.
 *                          e is removed from the allocated list.
 */

void gc::root (entity *e)
{
  // to do
}


/*
 *  no_of_allocated - pre-condition :  none.
 *                    post-condition:  returns the number of entities
 *                                     on the allocated list.
 */

int gc::no_of_allocated (void)
{
  // to do
}


/*
 *  no_of_freed - pre-condition :  none.
 *                post-condition:  returns the number of entities
 *                                 on the free_list.
 */

int gc::no_of_freed (void)
{
  // to do
}


/*
 *  no_of_rooted - pre-condition :  none.
 *                 post-condition:  returns the number of entities
 *                                  on the rooted list.
 */

int gc::no_of_rooted (void)
{
  // to do
}


/*
 *  stats - pre-condition :  none.
 *          post-condition:  prints some interesting statistics.
 */

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


/*
 *  new - pre-condition :  none  (a garbage collector has been created).
 *        post-condition:  a pointer to at least, bytes, spare memory is returned.
 */

void *operator new (std::size_t bytes)
{
#if defined(USE_GC)
  entity *e;
  
  return allocate (bytes, e);
#else
  return malloc (bytes);
#endif
}
