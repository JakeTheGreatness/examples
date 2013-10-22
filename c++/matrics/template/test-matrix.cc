/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-slist.cc - test code for the slist.h template module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <cassert>
#include <fract.h>
#include <matrix.h>


main ()
{

  {
    matrix <fract> m;
  }

  {
    fract f12 = fract (1, 2);
    fract f13 = fract (1, 3);
    fract f14 = fract (1, 4);
    fract f15 = fract (1, 5);
    fract f16 = fract (1, 6);
    fract f17 = fract (1, 7);
    fract f18 = fract (1, 8);
    fract f19 = fract (1, 9);
    fract f110 = fract (1, 10);

    matrix <fract> m = matrix <fract> (f12, f13, f14,
				       f15, f16, f17,
				       f18, f19, f110);

    std::cout << m << "\n";
  }

  {
    fract f12 = fract (1, 2);
    fract f13 = fract (1, 3);
    fract f14 = fract (1, 4);
    fract f15 = fract (1, 5);
    fract f16 = fract (1, 6);
    fract f17 = fract (1, 7);
    fract f18 = fract (1, 8);
    fract f19 = fract (1, 9);
    fract f110 = fract (1, 10);

    matrix <fract> m = matrix <fract> (f12, f13, f14,
				       f15, f16, f17,
				       f18, f19, f110);

    matrix <fract> r;
    fract pi = fract (22, 7);
    // fract pi = fract (314159, 100000);
    fract ninety = pi/2;

    std::cout << (pi / 6).sin ().ld () << "\n";

    r = r.rotate (ninety);
    std::cout << " rotation matrix for 90 degrees is\n" << r << "\n";
  }
}
