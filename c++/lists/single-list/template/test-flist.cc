/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-slist.cc - test code for the slist.h template module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <slist.h>
#include <cassert>
#include <fract.h>


main ()
{

  {
    slist <fract> l;

    assert (l.is_empty ());
    assert (l.empty ().is_empty ());
  }

  {
    slist <fract> l;
    fract f12 = fract (1, 2);
    fract f13 = fract (1, 3);
    fract f14 = fract (1, 4);
    fract f15 = fract (1, 5);

    l = l.cons (f12).cons (f13).cons (f14).cons (f15);
    printf("length of l = %d\n", l.length ());
    assert (l.length () == 4);
    std::cout << l << "\n";
  }
}

