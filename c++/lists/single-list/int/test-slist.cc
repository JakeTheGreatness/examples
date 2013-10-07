/*
 *  test-slist.cc - test code for the slist.cc module.
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
    std::cout << l;
  }
}

