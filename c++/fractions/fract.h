/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  introduce the fractional data type.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(FRACTH)
#  define FRACTH

#include <iostream>


typedef long unsigned int longcard;

class fract
{
 private:
  bool positive;
  bool top_heavy;  // if true it _might_ be top heavy, false it is _not_ top heavy
  longcard whole;
  longcard num;
  longcard denom;
  fract not_top_heavy (void);
  bool is_top_heavy (void);   // if true it _might_ be top heavy, false it is _not_ top heavy
  fract addND (fract right);
  bool subND (fract &left, fract right);
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
  fract reciprocal (void);

  fract operator+ (const fract &right);
  fract operator+ (int right);
  friend fract operator+ (int left, const fract &right);  // friend to int

  fract operator* (const fract &right);
  fract operator* (int right);
  friend fract operator* (int left, const fract &right);  // friend to int

  fract operator- (const fract &right);
  fract operator- (int right);
  friend fract operator- (int left, const fract &right);  // friend to int
  fract operator- (void);

  fract operator/ (const fract &right);
  fract operator/ (int right);
  friend fract operator/ (int left, const fract &right);  // friend to int

  fract& operator+= (const fract &right);
  fract& operator+= (int right);

  fract& operator-= (const fract &right);
  fract& operator-= (int right);

  fract& operator*= (const fract &right);
  fract& operator*= (int right);

  fract sin (fract radians);
  fract cos (fract radians);

  fract power (int);
};

#endif
