#include <cstdio>

static const int low  = 1;
static const int high = 1000000;


/*
 *  sum - generate the sum of terms lower..upper.
 *        precondition :  lower <= upper.
 *        postcondition:  returns the sum of lower..upper.
 */

static int sum (int lower, int upper)
{
  if (lower == upper)
    return lower;
  else
    return lower + sum (lower + 1, upper);
}


/*
 *  main - first user function executed.
 *         precondition :  none.
 *         postcondition:  returns 0 (silently).
 */

int main (int argc, char *argv[])
{
  printf ("Sum of numbers from %d..%d is: ", low, high);
  printf ("%d\n", sum (low, high));
}
