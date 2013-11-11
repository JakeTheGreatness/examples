/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-dlist.cc - test code for the dlist.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <dlist.h>
#include <cassert>


main ()
{

  {
    dlist l;
    assert (l.is_empty ());
    assert (l.empty ().is_empty ());
  }

  {
    dlist l;
    assert (l.is_empty ());
    assert (l.cons (12).head () == 12);
    std::cout << l << "\n";
  }

  {
    dlist l;
    assert (l.is_empty ());
    assert (l.cons (12).tail ().is_empty ());
  }

  {
    dlist l;
    
    l = l.cons (12);
    assert (l.tail ().is_empty ());
  }

  {
    dlist l;
    
    l = l.cons (12);
    l = l.tail ();
    assert (l.is_empty ());
  }

  {
    dlist l;
    
    l = l.cons (1).cons (2);
    std::cout << l << "\n";
    assert (l.length () == 2);
  }

  {
    dlist l;
    
    l = l.cons (1).cons (2).cons (3).cons (4);
    std::cout << l << "\n";
  }
  {
    dlist a;
    dlist b;
    dlist c;
    
    a = a.cons (1).cons (2);
    b = b.cons (3).cons (4);
    c = c.cons (a);
    std::cout << c << "\n";
    c = c.cons (b);
    std::cout << c << "\n";
  }

  {
    dlist l;
    
    l = l.cons (1).cons (2).cons (3).cons (4).reverse ();
    std::cout << l << "\n";
  }
}

