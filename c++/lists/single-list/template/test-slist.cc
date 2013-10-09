/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-slist.cc - test code for the slist.h template module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <slist.h>
#include <cassert>


main ()
{

  {
    slist <int> l;

    assert (l.is_empty ());
    assert (l.empty ().is_empty ());
  }

  {
    slist <int> l;

    assert (l.is_empty ());
    assert (l.cons (12).head () == 12);
  }

  {
    slist <int> l;

    assert (l.is_empty ());
    assert (l.cons (12).tail ().is_empty ());
  }

  {
    slist <int> l;
    
    l = l.cons (12);
    assert (l.tail ().is_empty ());
  }

  {
    slist <int> l;
    
    l = l.cons (12);
    l = l.tail ();
    assert (l.is_empty ());
  }

  {
    slist <int> l;

    l = l.cons (1).cons (2).cons (3).cons (4);
    printf("length of l = %d\n", l.length ());
    assert (l.length () == 4);
  }
}

