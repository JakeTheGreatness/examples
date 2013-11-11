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
  : data(0), status(freed), a_next(0), r_next(0)
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
  data = 0;
  status = in_error;
  a_next = 0;
  r_next = 0;
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
  data = 0;
  status = in_error;
  a_next = 0;
  r_next = 0;
}



void entity::do_assert (void)
{
  assert ((status & in_error) != 0);
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


gc::gc (int no_of_bytes, char *description)
  : rooted(0), allocated(0), bytes(no_of_bytes), free_list(0)
{
  desc = strdup (description);
  next = list_of_gc;
}


gc::~gc ()
{
  // not done yet
}


void gc::collect (void)
{
  // not done yet
}


void *gc::allocate (entity *&e)
{
  e = 0;
  return malloc (bytes);
}


void gc::root (entity *e)
{
  // to do
}


void gc::unroot (entity *e)
{
  // to do
}
