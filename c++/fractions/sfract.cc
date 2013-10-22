/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  introduce the fractional data type.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <sfract.h>

#include <cassert>
#include <cstdio>

static const bool debugging = false;





sfract_data::sfract_data (void)
  : value(0), kind(value_t), left(0), right(0)
{
}


sfract_data::~sfract_data (void)
{
}


sfract_data::sfract_data (int whole)
  : kind(value_t), left(0), right(0)
{
  value = fract (whole);
}


sfract_data::sfract_data (int num, int denom)
  : kind(value_t), left(0), right(0)
{
  value = fract (num, denom);
}


sfract_data::sfract_data (longcard whole)
  : kind(value_t), left(0), right(0)
{
  value = fract (whole);
}


sfract_data::sfract_data (longcard num, longcard denom)
  : kind(value_t), left(0), right(0)
{
  value = fract (num, denom);
}


sfract_data::sfract_data (fract_type type, sfract_data *l, sfract_data *r)
{
  kind = type;
  left = l;
  right = r;
}


sfract_data::sfract_data (fract_type type)
  : kind(type), left(0), right(0)
{
}


/*
 *   copy
 */

sfract_data::sfract_data (const sfract_data &from)
{
  value = from.value;
  kind = from.kind;
  left = dup (from.left);
  right = dup (from.right);
}


sfract_data& sfract_data::operator= (const sfract_data &from)
{
  value = from.value;
  kind = from.kind;
  left = dup (from.left);
  right = dup (from.right);
}


/*
 *  dup - duplicate entire tree.
 *        pre-condition : none.
 *        post-condition: complete tree duplicated and returned.
 */

sfract_data *sfract_data::dup (sfract_data *d)
{
  if (d == 0)
    return 0;
  else
    {
      sfract_data *copy = new sfract_data ();
      copy->value = d->value;
      copy->kind = d->kind;

      /*
       *  now we need to copy left and right trees
       */

      copy->left = dup (d->left);
      copy->right = dup (d->right);
      return copy;
    }
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised sfract.
 *                 post-condition:  sfract is printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const sfract_data *d)
{
  switch (d->kind)
    {
    case value_t:
      os << d->value;
      break;
    case pi_t:
      os << "pi";
      break;
    case e_t:
      os << "e";
      break;
    case r2_t:
      os << "r2";
      break;
    case r3_t:
      os << "r3";
      break;
    case r6_t:
      os << "r6";
      break;
    case add_t:
      os << "(" << d->left << " + " << d->right << ")";
      break;
    case sub_t:
      os << "(" << d->left << " - " << d->right << ")";
      break;
    case mult_t:
      os << "(" << d->left << " * " << d->right << ")";
      break;
    case div_t:
      os << "(" << d->left << " / " << d->right << ")";
      break;
    case neg_t:
      os << "(-" << d->right << ")";
      break;
    case power_t:
      os << "pow (" << d->left << ", " << d->right << ")";
      break;
    case sin_t:
      os << "sin (" << d->right << ")";
      break;
    case cos_t:
      os << "cos (" << d->right << ")";
      break;
    case tan_t:
      os << "tan (" << d->right << ")";
      break;
    default:
      assert (false);
    }
  return os;
}




/*
**********************************************************************
                            s f r a c t
**********************************************************************
 */


/*
 *  sfract - constructor.
 *           pre-condition : none.
 *           post-condition: symbolic fraction set to a constant type.
 */

sfract::sfract (void)
  : data (0)
{
}


sfract::~sfract (void)
{
  data = 0;
}


/*
 *  copy - pre-condition : none.
 *         post-condition: copied the data field.
 */

sfract::sfract (const sfract &from)
{
  data = from.data;
}


/*
 *  operator= - pre-condition : none.
 *              post-condition: copied the data field.
 */

sfract& sfract::operator= (const sfract &from)
{
  if (data != from.data)
    data = from.data;
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to a whole number.
 */

sfract::sfract (int value)
{
  data = new sfract_data (value);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to num/denom.
 */

sfract::sfract (int num, int demon)
{
  data = new sfract_data (num, demon);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to a whole number.
 */

sfract::sfract (longcard whole)
{
  data = new sfract_data (whole);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to num/denom.
 */

sfract::sfract (longcard num, longcard denom)
{
  data = new sfract_data (num, denom);
}


/*
 *  record - register the fraction to the garbage collector.
 *           pre-constructor : none.
 *           post-constructor: fraction registered.
 */

sfract sfract::record (void)
{
  /* finish register */
  return *this;
}


/*
 *  unrecord - deregister the fraction to the garbage collector.
 *             pre-constructor : none.
 *             post-constructor: fraction unregistered.
 */

sfract sfract::unrecord (void)
{
  /* finish unregister */
  return *this;
}


/*
 *  the + operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this + right)
 */

sfract sfract::operator+ (const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b + c
   *              ^ will alter b
   */
  sfract copy = *this;

  copy.data = new sfract_data (add_t, data, right.data);
  return copy;
}


/*
 *  the + operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this + right)
 */

sfract sfract::operator+ (int right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b + c
   *              ^ will alter b
   */
  sfract copy = *this;
  sfract_data *r = new sfract_data (right);

  copy.data = new sfract_data (add_t, data, r);
  return copy;
}


/*
 *  the + operator - pre-condition:   an initialised sfract.
 *                   post-condition:  left + right
 */

sfract operator+ (int left, const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   */
  sfract copy;
  sfract_data *l = new sfract_data (left);

  copy.data = new sfract_data (add_t, l, right.data);
  return copy;
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised sfract.
 *                 post-condition:  sfract is printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const sfract &f)
{
  os << f.data;
  return os;
}
