/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  introduce the fractional data type.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(SFRACTH)
#  define SFRACTH

#include <fract.h>
#include <iostream>

typedef enum fract_type_t {value_t, pi_t, e_t, r2_t, r3_t, r6_t,
			   add_t, sub_t, mult_t, div_t, neg_t, power_t,
			   sin_t, cos_t, tan_t} fract_type;

typedef long unsigned int longcard;


class sfract_data
{
 private:
  fract value;
  fract_type_t kind;
  sfract_data *left;
  sfract_data *right;
  friend std::ostream& operator<< (std::ostream& os, const sfract_data *d);

#if 0
  sfract_data simplify (void);
  bool is_positive (void);
  bool is_negative (void);
  sfract_data inc (sfract_data right);
  sfract_data dec (sfract_data right);
  sfract_data negate (void);
  bool is_zero (void);
  sfract_data reciprocal (void);
#endif
 public:
  sfract_data (void);
  ~sfract_data (void);  // destructor
  sfract_data (int);
  sfract_data (int, int);
  sfract_data (longcard w);
  sfract_data (longcard n, longcard d);
  sfract_data (fract_type type, sfract_data *l, sfract_data *r);
  sfract_data (fract_type type);
  sfract_data (const sfract_data &from);  // copy
  sfract_data& operator= (const sfract_data &from);  // assignment
  sfract_data (sfract_data *d);
  sfract_data *dup (sfract_data *d);
};


class sfract
{
 private:
  sfract_data *data;
  friend std::ostream& operator<< (std::ostream& os, const sfract& f);
  
 public:
  sfract (void);
  ~sfract (void);  // destructor
  sfract (const sfract &from);  // copy
  sfract& operator= (const sfract &from);  // assignment
  sfract (int);
  sfract (int, int);
  sfract (longcard w);
  sfract (longcard n, longcard d);
  sfract record (void);
  sfract unrecord (void);

  sfract operator+ (const sfract &right);
  sfract operator+ (int right);
  friend sfract operator+ (int left, const sfract &right);  // friend to int
  sfract operator+ (const fract right);
  friend sfract operator+ (fract left, const sfract &right);  // friend to int
};

#endif
