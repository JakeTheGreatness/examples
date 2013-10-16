/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  Ordered list, a template module for simple ordered list manipulation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(SLISTH)
#  define SLISTH

#include <slist.h>
#include <cassert>
#include <cstdio>
#include <iostream>

const bool debugging = false;


/*
 *  single linked list implementation.
 */


template <class T> class element
{
 public:
  element<T> *next;
  T           data;
};

template <typename T> class slist;  // forward declaration, to say slist is a template class

/*
 *  forward declaration of the operator<< method
 */
template <typename T> std::ostream& operator<< (std::ostream& os, const slist <T>& l);



template <class T> class slist
{
 private:
  element<T> *head_element;
  element<T> *e_dup (element<T> *e);  
  element<T> *e_delete (element<T> *h);
  element<T> *e_tail (element<T> *l);
  int e_length (element<T> *l);

 public:
  slist (void);
  ~slist (void);
  slist (const slist &from);
  slist& operator= (const slist &from);
  friend std::ostream& operator<< <> (std::ostream& os, const slist<T>& l);
  
  slist empty (void);
  bool  is_empty (void);
  slist cons (T i);
  T     head (void);
  slist tail (void);
  slist dup (void);
  int   length (void);
};


/*
 *  slist - constructor, builds an empty list.
 *          pre-condition:    none.
 *          post-condition:   list is created and is empty.
 */

template <class T> slist<T>::slist ()
  : head_element(0)
{
}


/*
 *  ~slist - deconstructor, releases the memory attached to the list.
 *           pre-condition:    none.
 *           post-condition:   list is empty.
 */

template <class T> slist<T>::~slist (void)
{
  head_element = e_delete (head_element);
}


/*
 *  e_delete - delete all elements specified by, h.
 */

template <class T> element<T> *slist<T>::e_delete (element<T> *h)
{
  while (h != 0) {
    element<T> *t = h;
    h = h->next;
    if (debugging)
      printf ("wanting to delete 0x%p\n", t);
    else
      delete t;
  }
  return 0;
}


/*
 *  e_dup - return a copy of all elements found in, e.
 */

template <class T> element<T> *slist<T>::e_dup (element<T> *e)
{
  element<T> *h = 0;
  element<T> *l = 0;
  element<T> *n;

  while (e != 0)
    {
      n = new element<T>;
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

template <class T> slist<T>::slist (const slist<T> &from)
{
  head_element = e_dup (from.head_element);
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a list.
 *               post-condition:  a copy of the list and its elements.
 *                                We delete 'this' lists elements.
 */

template <class T> slist<T>& slist<T>::operator= (const slist<T> &from)
{
  if (this->head_element == from.head_element)
    return *this;

  head_element = e_delete (head_element);
  head_element = e_dup (from.head_element);
}


/*
 *  empty - returns a new empty list.
 *          pre-condition:   none.
 *          post-condition:  a new empty list is returned.
 */

template <class T> slist<T> slist<T>::empty (void)
{
  slist<T> *l = new slist<T>;
  return *l;
}


/*
 *  is_empty - returns true if list is empty.
 *             pre-condition:   none.
 *             post-condition:  true is returned if list is empty.
 */

template <class T> bool slist<T>::is_empty (void)
{
  return head_element == 0;
}


/*
 *  cons - concatenate i to slist.
 *         pre-condition:   none.
 *         post-condition:  returns the list which has i at its head
 *                          and the remainer of contents as, slist.
 */

template <class T> slist<T> slist<T>::cons (T i)
{
  element<T> *e = new element<T>;

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

template <class T> T slist<T>::head (void)
{
  assert (! is_empty ());
  return head_element->data;
}


/*
 *  tail - opposite of cons.  Remove the head value and return
 *         the remainder of the list.
 *         pre-condition:   non empty list.
 *         post-condition:  return the list without the first element.
 *                          It deletes the first element.
 */

template <class T> slist<T> slist<T>::tail (void)
{
  element<T> *e = head_element;

  assert (! is_empty ());
  head_element = head_element->next;
  if (debugging)
    printf ("wanting to delete 0x%p\n", e);
  else
    delete e;
  return *this;
}


/*
 *  dup - duplicate the list and return a new copy.
 *        pre-condition:   none.
 *        post-condition:  duplicate slist is returned.
 */

template <class T> slist<T> slist<T>::dup (void)
{
  slist<T> l;

  l.head_element = e_dup (head_element);
  return l;
}


/*
 *  e_tail - given a list, l, return the list without the head element.
 *           pre-condition:   non empty list.
 *           post-condition:  return the list without the first element.
 *                            The original list is unaltered.
 */

template <class T> element<T> *slist<T>::e_tail (element<T> *l)
{
  return l->next;
}


/*
 *  e_length - return the length of element list, l.
 */

template <class T> int slist<T>::e_length (element<T> *h)
{
  if (h == 0)
    return 0;
  else
    return (1 + e_length (e_tail (h)));
}


/*
 *  length - return the length of list, l.
 */

template <class T> int slist<T>::length (void)
{
  return e_length (head_element);
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised list.
 *                 post-condition:  list printed and stream returned.
 */

template<class T>
std::ostream& operator<< (std::ostream& os, const slist<T>& l)

{
  element<T> *e = l.head_element;
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

#endif
