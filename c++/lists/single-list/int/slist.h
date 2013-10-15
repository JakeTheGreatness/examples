/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  single linked list implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(SLISTH)
#  define SLISTH

#include <iostream>


class element
{
 public:
  element *next;
  int      data;
};

class slist
{
 private:
  element *head_element;
  element *duplicate_elements (element *e);
  element *delete_elements (void);
  slist    cons (element *e);
  friend std::ostream& operator<< (std::ostream& os, const slist& l);

 public:
  slist (void);
  ~slist (void);
  slist (const slist &from);
  slist& operator= (const slist &from);
  
  slist empty (void);
  bool  is_empty (void);
  slist cons (int i);
  int   head (void);
  slist tail (void);
  slist cons (slist l);
  slist reverse (void);
};

#endif
