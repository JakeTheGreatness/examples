/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  single linked list implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <slist.h>
#include <cassert>
#include <cstdio>

const bool debugging = false;


#undef USE_RECURSION


/*
 *  slist - constructor, builds an empty list.
 *          pre-condition:    none.
 *          post-condition:   list is created and is empty.
 */

slist::slist (void)
  : head_element(0)
{
}


/*
 *  ~slist - deconstructor, releases the memory attached to the list.
 *           pre-condition:    none.
 *           post-condition:   list is empty.
 */

slist::~slist (void)
{
  head_element = delete_elements ();
}


/*
 *  delete_elements - delete all elements of list.
 *                    pre-condition:  none.
 *                    post-condition: zero is returned and
 *                                    all elements are deleted.
 */

element *slist::delete_elements (void)
{
  element *h = head_element;

  while (h != 0) {
    element *t = h;
    h = h->next;
    if (debugging)
      printf ("wanting to delete 0x%p\n", t);
    else
      delete t;
  }
  return 0;
}


/*
 *  duplicate_elements - return a copy of all elements found in, slist.
 *                       pre-condition:  e points to a list of elements.
 *                       post-condition: a duplicate list is returned.
 */

element *slist::duplicate_elements (element *e)
{
  element *h = 0;
  element *l = 0;
  element *n;

  while (e != 0)
    {
      n = new element;
      n->data = e->data;
      n->next = 0;
      if (h == 0)
	h = n;
      else
	l->next = n;
      l = n;
      e = e->next;
    }
  return h;
}


/*
 *  copy operator - redefine the copy operator.
 *                  pre-condition :  a list.
 *                  post-condition:  a copy of the list and its elements.
 */

slist::slist (const slist &from)
{
  head_element = duplicate_elements (from.head_element);
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a list.
 *               post-condition:  a copy of the list and its elements.
 *                                We delete 'this' lists elements.
 */

slist& slist::operator= (const slist &from)
{
  head_element = delete_elements ();
  head_element = duplicate_elements (from.head_element);
}


/*
 *  empty - returns a new empty list.
 *          pre-condition:   none.
 *          post-condition:  a new empty list is returned.
 */

slist slist::empty (void)
{
  slist *l = new slist;
  return *l;
}


/*
 *  is_empty - returns true if list is empty.
 */

bool slist::is_empty (void)
{
  return head_element == 0;
}


/*
 *  cons - concatenate i to slist.
 *         pre-condition:   none.
 *         post-condition:  returns the list which has i at its head
 *                          and the remainer of contents as, slist.
 */

slist slist::cons (int i)
{
  element *e = new element;

  e->data = i;
  e->next = head_element;
  head_element = e;
  return *this;
}


/*
 *  head - returns the data at the front of the list.
 *         pre-condition :  slist is not empty.
 *         post-condition:  data at the front of the list is returned.
 *                          slist is unchanged.
 */

int slist::head (void)
{
  assert (! is_empty());
  return head_element->data;
}


/*
 *  tail - opposite of cons.  Remove the head value and return
 *         the remainder of the list.
 *         pre-condition:   non empty list.
 *         post-condition:  return the list without the first element.
 */

slist slist::tail (void)
{
  element *e = head_element;

  assert (! is_empty());
  head_element = head_element->next;
  if (debugging)
    printf ("wanting to delete 0x%p\n", e);
  else
    delete e;
  return *this;
}


#if defined(USE_RECURSION)

/*
 *  cons - append elements, e, to the end of our slist.
 *         pre-condition :  e is a non empty list.
 *         post-condition:  e is appended to slist.
 */

slist slist::cons (element *e)
{
  if (is_empty ())
    head_element = e;
  else
    {
      element *h = head_element;

      while (h->next != 0)
	h = h->next;
      h->next = e;
    }
  return *this;
}


/*
 *  cons - concatenate list, l, to the end of the current list.
 *         pre-condition :  none.
 *         post-condition:  returns the current list with a copy of
 *                          contents of list, l, appended.
 */

slist slist::cons (slist l)
{
  if (l.is_empty ())
    return *this;
  else
    {
      int h = l.head ();  // use h to force evaluation order
      return cons (h).cons (l.tail ());
    }
}

#else

/*
 *  cons - append elements, e, to the end of our slist.
 *         pre-condition :  e is a non empty list.
 *         post-condition:  e is appended to slist.
 */

slist slist::cons (element *e)
{
  if (is_empty ())
    head_element = e;
  else
    {
      element *h = head_element;

      while (h->next != 0)
	h = h->next;
      h->next = e;
    }
  return *this;
}


/*
 *  cons - concatenate list, l, to the end of the current list.
 *         pre-condition :  none.
 *         post-condition:  returns the current list with a copy of
 *                          contents of list, l, appended.
 */

slist slist::cons (slist l)
{
  if (l.is_empty ())
    return *this;
  else
    return cons (duplicate_elements (l.head_element));
}
#endif


/*
 *  reverse - reverses the contents of list.
 */

slist slist::reverse (void)
{
  if (is_empty ())
    return *this;
  else
    return tail ().reverse().cons (empty().cons (head ()));
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised list.
 *                 post-condition:  list printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const slist& l)
{
  element *e = l.head_element;
  bool seen = false;

  os << "[";
  while (e != 0)
    {
      if (seen)
	os << ", ";
      os << e->data;
      e = e->next;
      seen = true;
    }
  os << "]";
  return os;
}
