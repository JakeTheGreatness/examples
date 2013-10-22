/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  test-fract.cc - test code for the fract.cc module.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <sfract.h>
#include <cassert>
#include <cstdio>


main ()
{
  {
    sfract a = sfract (1, 2);

    std::cout << "testing a = 1/2\n";
    std::cout << "a = " << a << "\n";
  }

  {
    sfract a = sfract (1, 2);
    sfract b = sfract (1, 2);
    sfract c = a + b;

    std::cout << "testing c = a + b\n";
    std::cout << "a = " << a << "\n";
    std::cout << "c = " << c << "\n";
    std::cout << "a = " << a << "\n";
    // std::cout << a << " + " << b << " = " << c << "\n";
  }

  {
    sfract a = sfract (1, 2);
    sfract b = sfract (1, 2);
    sfract c = a + a + b;

    std::cout << "testing c = a + a + b\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";

    a = sfract (9, 10);

    std::cout << "c = " << c << "\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
  }

  {
    sfract a = sfract (1, 2);
    sfract b = a + 1;

    std::cout << "testing b = a + 1\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a = " << a << "\n";
  }

  {
    sfract a = sfract (1, 2);
    sfract b = 1 + a;

    std::cout << "testing b = 1 + a\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a = " << a << "\n";
  }
}
