/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-stack.cc - test code for the stack.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <stack.h>
#include <cassert>


main ()
{
  {
    stack s;

    assert (s.empty ().is_empty ());
    s = s.empty ().push (1);
    assert (! (s.is_empty ()));
    assert (s.push (2).top () == 2);
    s = s.empty ().push (3);  // note we need the assignment s =
    s.pop ();
    assert (s.is_empty ());

    s = s.empty ();
    s.push (3);
    s.pop ();
    assert (s.is_empty ());
  }

  {
    stack s;

    s.push (1);
    s.push (2);
    s.push (3);
    s.push (4);
    
    std::cout << s << "\n";

    int i = s.pop ();
    std::cout << "top of stack was " << i << "\n";
    std::cout << s << "\n";
  }
}

