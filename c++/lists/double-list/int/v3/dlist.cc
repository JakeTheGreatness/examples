/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  double linked list implementation.
 *  (v3) uses sentinel element.
 *       uses head_element and tail_element.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <dlist.h>
#include <cassert>
#include <cstdio>

const bool debugging = false;



/*
 *  dlist - constructor, builds an empty list.
 *          pre-condition:    none.
 *          post-condition:   list is created and is empty.
 */

dlist::dlist (void)
{
  head_element = new element;
  head_element->data = 0;  // this should be unreferenced
  head_element->right = head_element;
  head_element->left = head_element;
  tail_element = head_element;
}


/*
 *  ~dlist - deconstructor, releases the memory attached to the list.
 *           pre-condition:    none.
 *           post-condition:   list is empty.
 */

dlist::~dlist (void)
{
  delete_all_elements ();
}


/*
 *  sub - remove, e, from the list.
 *        Pre-condition : e is on the list.
 *        Post-condition: e is removed and returned.
 */

element *dlist::sub (element *e)
{
  assert (! (head_element == e));
  assert (! ((e->right == head_element) && (e == head_element)));
      
  /*
   *  now unhook, e, from our double linked list
   */
  e->left->right = e->right;
  e->right->left = e->left;
  return e;
}


/*
 *  delete_element - pre-condition : e is an element on the list.
 *                   post-condition: e is removed from the list and deleted.
 */

void dlist::delete_element (element *e)
{
  delete sub (e);
  tail_element = head_element->left;
}


/*
 *  delete_all_elements - delete all elements of list.
 *                        pre-condition:  none.
 *                        post-condition: all elements are deleted.
 */

void dlist::delete_all_elements (void)
{
  while (! is_empty ())
    delete_element (head_element->right);
  delete head_element;  // now delete the sentinel
  head_element = 0;
  tail_element = 0;
}


/*
 *  add - add, e, to the end of list, defined by head.
 *        pre-condition :  head points to a list.
 *                         e is not on a list.
 *        post-condition:  e is appended to the end of head.
 */

void dlist::add (element **head, element *e)
{
  e->right = *head;  // add e to the end of list
  e->left = (*head)->left;  // copy the current end to e
  (*head)->left->right = e;  // add e to the end of the last
  (*head)->left = e;  // alter the left of head to point to e
}


/*
 *  duplicate_all_elements - return a copy of _all_ elements defined by, e,
 *                           including the sentinel element.
 *                           pre-condition:  e points to a list of elements.
 *                           post-condition: a duplicate list is returned.
 */

element *dlist::duplicate_all_elements (element *e)
{
  element *h = new element;
  element *f = e->right;
  element *n;

  h->right = h;
  h->left = h;
  h->data = 0;
  while (f != e)
    {
      n = new element;
      n->data = f->data;
      n->right = 0;
      n->left = 0;
      add (&h, n);
      f = f->right;
    }
  return h;
}


/*
 *  duplicate_data_elements - return a copy of all data elements defined by, e.
 *                            e should be the head of a list.  All elements other
 *                            than the sentinel element are duplicated.
 *                            pre-condition:  e points to a non empty list of elements.
 *                            post-condition: a duplicate list is returned.
 */

element *dlist::duplicate_data_elements (element *e)
{
  assert (e->right != e);

  e = duplicate_all_elements (e);
  /*
   *  now delete the sentinel element
   */
  element *h = e->right;
  e->left->right = h;
  h->left = e->right;
  delete e;  // and throw it away
  return h;
}


/*
 *  copy operator - redefine the copy operator.
 *                  pre-condition :  a list.
 *                  post-condition:  a copy of the list and its elements.
 */

dlist::dlist (const dlist &from)
{
  head_element = duplicate_all_elements (from.head_element);
  tail_element = head_element->left;
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a list.
 *               post-condition:  a copy of the list and its elements.
 *                                We delete 'this' lists elements.
 */

dlist& dlist::operator= (const dlist &from)
{
  if (this->head_element == from.head_element)
    return *this;
  
  delete_all_elements ();
  head_element = duplicate_all_elements (from.head_element);
  tail_element = head_element->left;
}


/*
 *  empty - returns a new empty list.
 *          pre-condition:   none.
 *          post-condition:  a new empty list is returned.
 */

dlist dlist::empty (void)
{
  dlist *l = new dlist;
  return *l;
}


/*
 *  is_empty - returns true if list is empty.
 */

bool dlist::is_empty (void)
{
  return head_element == tail_element;
}


/*
 *  cons - concatenate i to dlist.
 *         pre-condition:   none.
 *         post-condition:  returns the list which has i at its head
 *                          and the remainer of contents as, dlist.
 */

dlist dlist::cons (int i)
{
  element *e = new element;

  e->data = i;
  head_element = head_element->right;  // move over the sentinel
  add (&head_element, e);  // adds e just after the sentinel
  head_element = e->left;  // head points to the sentinel again
  tail_element = head_element->left;
  return *this;
}


/*
 *  head - returns the data at the front of the list.
 *         pre-condition :  dlist is not empty.
 *         post-condition:  data at the front of the list is returned.
 *                          dlist is unchanged.
 */

int dlist::head (void)
{
  assert (! is_empty());
  return head_element->right->data;  // skip over sentinel
}


/*
 *  tail - opposite of cons.  Remove the head value and return
 *         the remainder of the list.
 *         pre-condition:   non empty list.
 *         post-condition:  return the list without the first element.
 */

dlist dlist::tail (void)
{
  element *e;

  assert (! is_empty());
  e = sub (head_element->right);
  if (debugging)
    printf ("wanting to delete 0x%p\n", e);
  else
    delete e;
  return *this;
}


/*
 *  cons - append elements, e, to the end of our dlist.
 *         pre-condition :  e is a non empty list containing no sentinel.
 *         post-condition:  e is appended to dlist.
 */

dlist dlist::cons (element *e)
{
  element *tail = tail_element;
  tail_element->right = e; // 2
  head_element->left = e->left;  // 1
  e->left->right = head_element; // 4
  e->left = tail;  // 3
  tail_element = head_element->left;
  return *this;
}


/*
 *  cons - concatenate list, l, to the end of the current list.
 *         pre-condition :  none.
 *         post-condition:  returns the current list with a copy of
 *                          contents of list, l, appended.
 */

dlist dlist::cons (dlist l)
{
  if (l.is_empty ())
    return *this;
  else
    return cons (duplicate_data_elements (l.head_element));
}


/*
 *  reverse - reverses the contents of list.
 */

dlist dlist::reverse (void)
{
  if (is_empty ())
    return *this;
  else
    return tail ().reverse().cons (empty().cons (head ()));
}


/*
 *  length - return the length of the list.
 *           pre-condition : none.
 *           post-condition: returns an integer indicating
 *                           the number of elements in list.
 *
 */

int dlist::length (void)
{
  int n = 0;
  element *e = head_element->right;

  while (e != head_element)
    {
      e = e->right;
      n++;
    }
  return n;
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised list.
 *                 post-condition:  list printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const dlist& l)
{
  element *e = l.head_element->right;
  bool seen = false;

  os << "[";
  
  while (e != l.head_element)
    {
      if (seen)
	os << ", ";
      os << e->data;
      e = e->right;
      seen = true;
    }
  os << "]";
  return os;
}
