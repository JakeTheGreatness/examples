/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  double linked list implementation.
 *  (v3) uses sentinel element.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(DLISTH)
#  define DLISTH

#include <iostream>


class element
{
 public:
  element *left;
  element *right;
  int      data;
};

class dlist
{
 private:
  element *head_element;
  element *tail_element;
  element *duplicate_data_elements (element *e);
  element *duplicate_all_elements (element *e);
  void     delete_all_elements (void);
  dlist    cons (element *e);
  friend std::ostream& operator<< (std::ostream& os, const dlist& l);

 public:
  dlist (void);
  ~dlist (void);
  dlist (const dlist &from);
  dlist& operator= (const dlist &from);
  
  dlist empty (void);
  bool  is_empty (void);
  dlist cons (int i);
  int   head (void);
  dlist tail (void);
  dlist cons (dlist l);
  dlist reverse (void);
  int length (void);

  void add (element **head, element *e);
  void delete_element (element *e);
  element *sub (element *e);
  
  dlist append (int i);
  dlist cut (int low, int high);
  dlist slice (int low, int high);
  dlist ordered_insert (int i);
};

#endif
