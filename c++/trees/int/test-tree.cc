/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-tree.cc - test code for the tree.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <tree.h>
#include <cassert>


main ()
{

  {
    tree t;
    assert (t.is_empty ());
    assert (t.empty ().is_empty ());
  }

  {
    tree t;
    
    t.insert (1);
    std::cout << t << "\n";
  }

  {
    tree t;
    
    t = t.insert (1).insert (2).insert (3).insert (4);
    std::cout << t << "\n";
  }

  {
    tree t;
    
    t = t.insert (4).insert (3).insert (2).insert (1);
    std::cout << t << "\n";
  }

  {
    tree t;
    
    t = t.insert (1).insert (4).insert (3).insert (2);
    std::cout << t << "\n";

    // t.diagram (std::cout);
  }

  {
    tree t;
    
    t = t.insert (1);
    std::cout << t << "\n";

    // t.diagram (std::cout);

    t = t.insert (4);
    std::cout << t << "\n";

    t = t.insert (3);
    std::cout << t << "\n";

    t = t.insert (2);
    std::cout << t << "\n";
  }

  {
    tree t;
    
    t = t.insert (2).insert (3).insert (1).insert (4);
    std::cout << t << "\n";
  }
}

