/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-sin.cc - test code for the fract.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <sfract.h>
#include <cassert>
#include <cstdio>
#include <gc.h>


main ()
{
  sfract v;
  sfract s;
  sfract radians;
  int i;
  sfract two_pi = 2 * pi ();

  two_pi.root ();

  for (i = 0; i<360; i += 15)
    {
      s = sfract (i, 360);
      radians = s * two_pi;
      v = sin (radians);
      std::cout << "sin (" << i << " degrees) can be expressed as " << v;
      v = v();
      std::cout << " and also " << v << "\n";
      sfract_garbage_collect ();
    }
}
