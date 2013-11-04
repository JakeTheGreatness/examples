/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  dynamic stack implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(STACKH)
#  define STACKH

#include <iostream>


class element
{
 public:
  element *next;
  int      data;
};

class stack
{
 private:
  element *head_element;
  element *duplicate_elements (element *e);
  element *delete_elements (void);
  friend std::ostream& operator<< (std::ostream& os, const stack& l);

 public:
  stack (void);
  ~stack (void);
  stack (const stack &from);
  stack& operator= (const stack &from);
  
  stack empty (void);
  bool  is_empty (void);
  stack push (int i);
  int   top (void);
  int   pop (void);
};

#endif
