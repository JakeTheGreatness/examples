/*
 *  implement a single linked list.
 */

#include <slist.h>
#include <cassert>
#include <cstdio>


const bool debugging = false;


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
  head_element = delete_elements (head_element);
}


/*
 *  delete_elements - delete all elements specified by, h.
 *                    pre-condition:  h points to a list of elements.
 *                    post-condition: zero is returned and all elements are
 *                                    deleted.
 */

element *slist::delete_elements (element *h)
{
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
 *  duplicate_elements - return a copy of all elements found in, e.
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
  head_element = delete_elements (head_element);
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
