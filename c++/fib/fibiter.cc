#include <cstdio>

static const int terms = 45;


/*
 *  fibonacci - generate nth term in the classical sequence.
 *              precondition :  n > 0
 *              postcondition:  returns the nth term
 */

static int fibonacci (int n)
{
  int n1 = 1;   // fibonacci (n-1)
  int n2 = 0;   // fibonacci (n-2)
  int n3;       // fibonacci (n-3)

  for (int i=2; i<n; i++)
    {
      n3 = n2;  // remember old n2
      n2 = n1;  // new n2 is old n1
      n1 = n2+n3;  // calculate new n1
    }
  return n1+n2; // return result fibonacci(n-1) + fibonacci(n-2)
}


/*
 *  main - first user function executed.
 *         precondition :  none.
 *         postcondition:  returns 0 (silently).
 */

int main (int argc, char *argv[])
{
  printf ("Fibonacci value for the first %d are: ", terms);
  printf ("... %d\n", fibonacci (terms));
}
