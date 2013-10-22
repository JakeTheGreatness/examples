/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-fract.cc - test code for the fract.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <fract.h>
#include <cassert>
#include <cstdio>


main ()
{
  {
    fract a = fract (1, 2);
    std::cout << "a = " << a << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = fract (1, 2);
    fract c = a + b;

    std::cout << a << " + " << b << " = " << c << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = fract (1, 2);
    fract c = a + a + b;

    std::cout << a << " + " << a << " + " << b << " = " << c << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = a + 1;

    std::cout << a << " + " << 1 << " = " << b << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = 1 + a;

    std::cout << 1 << " + " << a << " = " << b << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = 3 * a;

    std::cout << 3 << " * " << a << " = " << b << "\n";
  }

  {
    fract a = fract (1, 2);
    fract b = a * 3;

    std::cout << a << " * " << 3 << " = " << b << "\n";
  }

  {
    fract a = fract (2, 3);
    fract b = fract (1, 4);
    fract c = a + b;

    std::cout << a << " + " << b << " = " << c << "\n";
  }

  {
    fract a = fract (2, 3);
    fract b = a * 2;

    std::cout << a << " * " << 2 << " = " << b << "\n";
  }

  {
    fract a = fract (2, 3);
    fract b = fract (1, 4);
    fract c = a - b;

    std::cout << a << " - " << b << " = " << c << "\n";
  }

  {
    fract a = fract (1, 4);
    fract b = fract (2, 3);
    fract c = a - b;

    std::cout << a << " - " << b << " = " << c << "\n";
  }

  {
    fract a = fract (1, 2);

    std::cout << "testing == and / work\n";
    assert (a/2 == fract (1, 4));
    std::cout << "yes\n";
  }
}
