/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  tree implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(TREEH)
#  define TREEH

#include <iostream>


class element
{
 public:
  element *left;
  element *right;
  int      data;
};

class tree
{
 private:
  element *root_element;
  friend std::ostream& operator<< (std::ostream& os, const tree& l);
  // helper methods
  int  no_of_items (element *e);
  int  height (element *e);
  void delete_element (element *e);
  element *duplicate_elements (element *e);
  void insert (element **e, int i);
  tree cons (int i, element *l, element *r);
  int width (int n);
  int width (element *e);

 public:
  tree (void);
  ~tree (void);
  tree (const tree &from);
  tree& operator= (const tree &from);

  void inorder (std::ostream& os, element *e);
  void preorder (std::ostream& os, element *e);
  void postorder (std::ostream& os, element *e);

  void diagram (std::ostream& os, element *e);
  void write_spaces (std::ostream& os, int n);
  void diagram (std::ostream& os);
  tree empty (void);
  bool is_empty (void);
  tree cons (int i, tree l, tree r);
  int  root (void);
  tree cons (tree l);
  int  height (void);
  int  no_of_items (void);
  tree insert (int i);
  tree right (void);
  tree left (void);
  tree insert_non_empty (int i);
  tree remove (int i);
  int minv (void);
  int maxv (void);
};

#endif
