/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-slist.cc - test code for the slist.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <slist.h>
#include <cassert>


main ()
{

  {
    slist l;
    assert (l.is_empty ());
    assert (l.empty ().is_empty ());
  }

  {
    slist l;
    assert (l.is_empty ());
    assert (l.cons (12).head () == 12);
  }

  {
    slist l;
    assert (l.is_empty ());
    assert (l.cons (12).tail ().is_empty ());
  }

  {
    slist l;
    
    l = l.cons (12);
    assert (l.tail ().is_empty ());
  }

  {
    slist l;
    
    l = l.cons (12);
    l = l.tail ();
    assert (l.is_empty ());
  }

  {
    slist l;
    
    l = l.cons (1).cons (2).cons (3).cons (4);
    std::cout << l << "\n";
  }
  {
    slist a;
    slist b;
    slist c;
    
    a = a.cons (1).cons (2);
    b = b.cons (3).cons (4);
    c = c.cons (a);
    std::cout << c << "\n";
    c = c.cons (b);
    std::cout << c << "\n";
  }

  {
    slist l;
    
    l = l.cons (1).cons (2).cons (3).cons (4).reverse ();
    std::cout << l << "\n";
  }
}

