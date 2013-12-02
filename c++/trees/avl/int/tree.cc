/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  tree implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <tree.h>
#include <cassert>
#include <cstdio>

const bool debugging = false;


static int max (int a, int b)
{
  if (a > b)
    return a;
  else
    return b;
}


/*
 *  tree - constructor, builds an empty list.
 *          pre-condition:    none.
 *          post-condition:   tree is created and is empty.
 */

tree::tree (void)
{
  root_element = 0;
}


/*
 *  ~tree - deconstructor, releases the memory attached to the list.
 *          pre-condition:   none.
 *          post-condition:  tree is empty.
 */

tree::~tree (void)
{
  delete_element (root_element);
  root_element = 0;
}


/*
 *  delete_element - pre-condition :  none.
 *                   post-condition:  left and right branches are deleted.
 */

void tree::delete_element (element *e)
{
  if (e != 0)
    {
      delete_element (e->left);
      delete_element (e->right);
      delete e;
    }
}


/*
 *  copy operator - redefine the copy operator.
 *                  pre-condition :  a tree.
 *                  post-condition:  a copy of the tree and its elements.
 */

tree::tree (const tree &from)
{
  root_element = duplicate_elements (from.root_element);
}


/*
 *  duplicate_elements - pre-condition :  none.
 *                       post-condition:  returns a new tree of elements
 *                                        which is a copy of, e.
 */

element *tree::duplicate_elements (element *e)
{
  if (e == 0)
    return 0;
  else
    {
      element *n = new element;
      n->data = e->data;
      n->left = duplicate_elements (e->left);
      n->right = duplicate_elements (e->right);
      return n;
    }
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a list.
 *               post-condition:  a copy of the list and its elements.
 *                                We delete 'this' lists elements.
 */

tree& tree::operator= (const tree &from)
{
  if (this->root_element == from.root_element)
    return *this;
  
  delete_element (root_element);
  root_element = duplicate_elements (from.root_element);
}


/*
 *  empty - returns a new empty tree.
 *          pre-condition:   none.
 *          post-condition:  a new empty tree is returned.
 */

tree tree::empty (void)
{
  tree *t = new tree;
  return *t;
}


/*
 *  is_empty - returns true if tree is empty.
 */

bool tree::is_empty (void)
{
  return root_element == 0;
}


/*
 *  cons - concatenate i to tree.
 *         pre-condition:   none.
 *         post-condition:  returns a tree which has the
 *                          contents of, l, and, r, as its branches.
 */

tree tree::cons (int i, element *l, element *r)
{
  tree *t = new tree;

  t->root_element = new element;

  t->root_element->data = i;
  t->root_element->left = l;
  t->root_element->right = r;
  return *t;
}


/*
 *  cons - concatenate i to tree.
 *         pre-condition:   none.
 *         post-condition:  returns a tree which has the
 *                          contents of, l, and, r, as its branches.
 */

tree tree::cons (int i, tree l, tree r)
{
  return cons (i,
	       duplicate_elements (l.root_element),
	       duplicate_elements (r.root_element));
}


/*
 *  root - returns the data at the root of the tree.
 *         pre-condition :  tree is not empty.
 *         post-condition:  data at the front of the list is returned.
 *                          tree is unchanged.
 */

int tree::root (void)
{
  assert (! is_empty());
  return root_element->data;
}


/*
 *  height - pre-condition : none:
 *           post-condition: returns the max height between
 *                           the left and right branches.
 */

int tree::height (element *e)
{
  if (e == 0)
    return 0;
  else
    return 1 + max (height (e->left), height (e->right));
}


/*
 *  height - return the height of the tree.
 *           pre-condition : none.
 *           post-condition: returns the max height between the
 *                           left and right branches.
 */

int tree::height (void)
{
  return height (root_element);
}


/*
 *  no_of_items - pre-condition : none:
 *                post-condition: returns the number of items in
 *                                the left and right branches.
 */

int tree::no_of_items (element *e)
{
  if (e == 0)
    return 0;
  else
    return no_of_items (e->left) + no_of_items (e->right);
}


/*
 *  no_of_items - return the number of items in the tree.
 *                pre-condition : none.
 *                post-condition: returns an integer indicating
 *                                the number of items in the tree.
 *
 */

int tree::no_of_items (void)
{
  return no_of_items (root_element);
}


tree tree::left (void)
{
  if (root_element == 0)
    return empty ();
  if (root_element->left == 0)
    return empty ();
  return cons (root_element->left->data,
	       duplicate_elements (root_element->left->left),
	       duplicate_elements (root_element->left->right));
}

tree tree::right (void)
{
  if (root_element == 0)
    return empty ();
  if (root_element->right == 0)
    return empty ();
  return cons (root_element->right->data,
	       duplicate_elements (root_element->right->left),
	       duplicate_elements (root_element->right->right));
}

tree tree::balanced (void)
{
  tree r = right ();
  tree l = left ();
  int  d = l.height () - r.height ();
  tree t = *this;

  if ((d == 0) || (d == 1) || (d == -1))
    return t;
  std::cout << "this tree is unbalanced " << t;

  assert (false);
  return t;
}


tree tree::balance (void)
{
  tree r = right ();
  tree l = left ();
  int  d = l.height () - r.height ();
  tree t = *this;

  if ((d == 0) || (d == 1) || (d == -1))
    return t;

  /*
   *  now rotate
   */

  if (d < 0)
    return t.rotate_left ();
  else if (d > 0)
    return t.rotate_right ();

  return cons (t.root (),
	       r.balance (),
	       l.balance ());
}


tree tree::insert (int i)
{
  if (is_empty ())
    return cons (i, empty (), empty ());
  else
    return insert_non_empty (i).balance ();
}

tree tree::insert_non_empty (int i)
{
  if (i == root ())
    return cons (i, left (), right ());
  else if (i < root ())
    return cons (root (), left ().insert (i), right ());
  else
    return cons (root (), left (), right ().insert (i));
}


/*
 *  rotate_right - pre-condition : left branch is not empty.
 *                 post-condition: a new tree is return which
 *                                 is the result of rotating the
 *                                 current tree.
 */

tree tree::rotate_right (void)
{
  if (is_empty ())
    return *this;
  else
    return cons (left ().root (),
		 left ().left (),
		 cons (root (),
		       left ().right (),
		       right ()));
}


/*
 *  rotate_left - pre-condition : right branch is not empty.
 *                post-condition: a new tree is return which
 *                                is the result of rotating the
 *                                current tree.
 */

tree tree::rotate_left (void)
{
  if (is_empty ())
    return *this;
  else
    return cons (right ().root (),
		 cons (root (),
		       left (),
		       right ().left ()),
		 right ().right ());
}


/*
 *  minv - return the smallest value in the tree.
 */

int tree::minv (void)
{
  if (left ().is_empty ())
    return root ();
  else
    return left ().minv ();
}


/*
 *  maxv - return the largest value in the tree.
 */

int tree::maxv (void)
{
  if (right ().is_empty ())
    return root ();
  else
    return right ().maxv ();
}


tree tree::remove (int i)
{
  return do_remove (i).balance ();
}

tree tree::do_remove (int i)
{
  if (is_empty ())
    return empty ();
  if ((root () == i) && right ().is_empty ())
    return left ();
  if ((root () == i) && left ().is_empty ())
    return right ();
  if (root () == i)
    return cons (right ().minv (), left (), right().remove (right ().minv ()));
  
  if (i < root ())
    return cons (root (), left ().remove (i), right ());
  else
    return cons (root (), left (), right ().remove (i));
}


int tree::width (int n)
{
  int d = 1;
  int p = 10;
  
  while (n > p)
    {
      p *= 10;
      d++;
    }
  return d;
}


/*
 *  width
 */

int tree::width (element *e)
{
  int n;

  if (e == 0)
    return 0;

  n = width (e->data);
  return width (e->left) + width (e->right) + n;
}


void tree::write_spaces (std::ostream& os, int n)
{
  while (n > 0)
    {
      n--;
      os << " ";
    }
}


void tree::diagram (std::ostream& os, element *e)
{
  if (e != 0)
    {
      int l = width (e->left) + 2;
      int r = width (e->right) + 2;
      
      write_spaces (os, l/2+1); os << e->data << "\n";
      write_spaces (os, l/2); os << "/" ; write_spaces (os, l/2); write_spaces (os, r/2); os << "\\\n" ;
#if 0
      diagram (os, e->left);
      diagram (os, e->right);
#endif
    }
}


void tree::diagram (std::ostream& os)
{
  diagram (os, root_element);
}


/*
 *  inorder - pre-condition :  none.
 *            post-condition:  prints out left, root, right.
 */

void tree::inorder (std::ostream& os, element *e)
{
  if (e != 0)
    {
      os << "[";
      if (e->left != 0)
	inorder (os, e->left);

      os << e->data;

      if (e->right != 0)
	inorder (os, e->right);
      os << "]";
    }
}


/*
 *  preorder - pre-condition :  none.
 *             post-condition:  prints out root, left, right.
 */

void tree::preorder (std::ostream& os, element *e)
{
  if (e != 0)
    {
      os << e->data;
      os << ", ";

      if (e->left != 0)
	preorder (os, e->left);

      if (e->right != 0)
	preorder (os, e->right);
    }
}


/*
 *  postorder - pre-condition :  none.
 *              post-condition:  prints out left, right, root.
 */

void tree::postorder (std::ostream& os, element *e)
{
  if (e != 0)
    {
      if (e->left != 0)
	postorder (os, e->left);

      if (e->right != 0)
	postorder (os, e->right);

      os << e->data;
      os << ", ";
    }
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised list.
 *                 post-condition:  list printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const tree& l)
{
  tree t = l;

  t.inorder (os, t.root_element);
  os << "\n";
  return os;
}
