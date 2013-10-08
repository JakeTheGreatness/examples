#if !defined(FRACTH)
#  define FRACTH

#include <iostream>

/*
 *  introduce the fractional data type.
 */

typedef long unsigned int longcard;


class fract
{
 private:
  bool positive;
  bool dirty;
  longcard whole;
  longcard num;
  longcard denom;
  fract clean (void);
  bool is_dirty (void);
  fract addND (fract right);
  fract subND (fract right);
  std::ostream& printND (std::ostream& os);
  friend std::ostream& operator<< (std::ostream& os, const fract& l);
  
 public:
  fract (void);
  ~fract (void);
  fract (const fract &from);
  fract& operator= (const fract &from);
  fract (int);
  fract (int, int);
  fract (longcard w);
  fract (longcard n, longcard d);
  fract simplify (void);
  bool is_positive (void);
  bool is_negative (void);
  fract inc (fract right);
  fract dec (fract right);
  fract negate (void);
  bool is_zero (void);

  fract operator+ (const fract &right);
  fract operator+ (int right);
  friend fract operator+ (int left, const fract &right);  // friend to int

  fract operator* (const fract &right);
  fract operator* (int right);
  friend fract operator* (int left, const fract &right);  // friend to int
};

#endif
