/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  dynamic stack implementation.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <stack.h>
#include <cassert>
#include <cstdio>

const bool debugging = false;


/*
 *  stack - constructor, builds an empty stack.
 *          pre-condition:   none.
 *          post-condition:  stack is created and is empty.
 *                           free_list is empty.
 */

stack::stack (void)
  : head_element(0), free_list(0)
{
}


/*
 *  ~stack - deconstructor, releases the memory attached to the stack.
 *           pre-condition:    none.
 *           post-condition:   stack is empty.
 */

stack::~stack (void)
{
  head_element = delete_elements (head_element);
  free_list = delete_elements (free_list);
}


/*
 *  delete_elements - delete all elements of stack.
 *                    pre-condition:  none.
 *                    post-condition: all elements are deleted and zero
 *                                    is returned.
 */

element *stack::delete_elements (element *h)
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
 *  duplicate_elements - return a copy of all elements found in, stack.
 *                       pre-condition:  e points to a stack of elements.
 *                       post-condition: a duplicate stack is returned.
 */

element *stack::duplicate_elements (element *e)
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
 *                  pre-condition :  a stack.
 *                  post-condition:  a copy of the stack and its elements.
 */

stack::stack (const stack &from)
{
  head_element = duplicate_elements (from.head_element);
  free_list = 0;
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a stack.
 *               post-condition:  a copy of the stack and its elements.
 *                                We delete 'this' stacks elements.
 */

stack& stack::operator= (const stack &from)
{
  if (this->head_element == from.head_element)
    return *this;
  
  head_element = delete_elements (head_element);
  head_element = duplicate_elements (from.head_element);
  free_list = 0;
}


/*
 *  empty - returns a new empty stack.
 *          pre-condition:   none.
 *          post-condition:  a new empty stack is returned.
 */

stack stack::empty (void)
{
  stack *l = new stack;
  return *l;
}


/*
 *  is_empty - returns true if stack is empty.
 */

bool stack::is_empty (void)
{
  return head_element == 0;
}


/*
 *  top - returns the value at the top of the stack.
 *         pre-condition :  stack is not empty.
 *         post-condition:  returns the value at the top of the stack.
 *                          The stack is unchanged.
 */

int stack::top (void)
{
  assert (! is_empty());
  return head_element->data;
}


/*
 *  new_element - pre-condition :  none.
 *                post-condition:  return an element either
 *                                 from the free_list or from the heap.
 */

element *stack::new_element (void)
{
  element *e;

  if (free_list == 0)
    e = new element;
  else
    {
      e = free_list;
      free_list = free_list->next;
    }
  return e;
}


/*
 *  delete_element - pre-condition :  e, must not be on the stack.
 *                   post-condition:  places, e, onto the free_list.
 */

void stack::delete_element (element *e)
{
  e->next = free_list;
  free_list = e;
}


/*
 *  push - push i to stack.
 *         pre-condition:   none.
 *         post-condition:  returns the stack which has i at its head
 *                          and the remainer of contents as, stack.
 */

stack stack::push (int i)
{
  element *e = new_element ();

  e->data = i;
  e->next = head_element;
  head_element = e;
  return *this;
}


/*
 *  pop - opposite of cons.  Remove the head value and return it.
 *        pre-condition:   non empty stack.
 *        post-condition:  remove and return value from top of stack.
 */

int stack::pop (void)
{
  element *e = head_element;
  int value = e->data;

  assert (! is_empty());
  head_element = head_element->next;
  if (debugging)
    printf ("wanting to delete 0x%p\n", e);
  else
    delete_element (e);

  return value;
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised stack.
 *                 post-condition:  stack printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const stack& l)
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
