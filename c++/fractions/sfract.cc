/*
 *  introduce the symbolic fractional data type.
 */

#include <sfract.h>

#include <cassert>
#include <cstdio>
#include "gc.h"


static const bool debugging = false;

void init_gc (void);


sfract_data::sfract_data (void)
  : value(0), kind(value_t), left(0), right(0), sfract_data_entity (0)
{
}


sfract_data::~sfract_data (void)
{
}


sfract_data::sfract_data (int whole)
  : kind(value_t), left(0), right(0), sfract_data_entity (0)
{
  value = fract (whole);
}


sfract_data::sfract_data (int num, int denom)
  : kind(value_t), left(0), right(0), sfract_data_entity (0)
{
  value = fract (num, denom);
}


sfract_data::sfract_data (longcard whole)
  : kind(value_t), left(0), right(0), sfract_data_entity (0)
{
  value = fract (whole);
}


sfract_data::sfract_data (longcard num, longcard denom)
  : kind(value_t), left(0), right(0), sfract_data_entity (0)
{
  value = fract (num, denom);
}


sfract_data::sfract_data (fract_type type, sfract_data *l, sfract_data *r)
{
  kind = type;
  left = l;
  right = r;
  sfract_data_entity = 0;
}


sfract_data::sfract_data (fract_type type, sfract_data *value)
{
  kind = type;
  left = value;
  right = 0;
  sfract_data_entity = 0;
}


sfract_data::sfract_data (fract_type type)
  : kind(type), left(0), right(0), sfract_data_entity (0)
{
}


/*
 *   copy
 */

sfract_data::sfract_data (const sfract_data &from)
{
  value = from.value;
  kind = from.kind;
  left = from.left->dup ();
  right = from.right->dup ();
}


sfract_data& sfract_data::operator= (const sfract_data &from)
{
  value = from.value;
  kind = from.kind;
  left = from.left->dup ();
  right = from.right->dup ();
}


/*
 *  dup - duplicate entire tree.
 *        pre-condition : none.
 *        post-condition: complete tree duplicated and returned.
 */

sfract_data *sfract_data::dup (void)
{
  sfract_data *copy = new sfract_data;
  copy->value = value;
  copy->kind = kind;

  /*
   *  now we need to copy left and right trees
   */

  if (left == 0)
    copy->left = 0;
  else
    copy->left = left->dup ();

  if (right == 0)
    copy->right = 0;
  else
    copy->right = right->dup ();
  return copy;
}


bool sfract_data::is_rational (void)
{
  return (kind == pi_t) || (kind == e_t) || (kind == r2_t) || (kind == r3_t) || (kind == r6_t);
}


bool sfract_data::is_value (void)
{
  return kind == value_t;
}


bool sfract_data::are_equal (sfract l, sfract_data *r)
{
  sfract_data *d = l.data;

  return are_equal (d, r);
}


bool sfract_data::are_equal (sfract_data *l, sfract r)
{
  sfract_data *d = r.data;

  return are_equal (l, d);
}

/*
 *  are_equal - return true if the left hand tree is the same as the right hand tree.
 *              It does understand communicative:   ie    a + b  is the same as    b + a
 *                                                  and   a * b  is the same as    b * a
 */

bool sfract_data::are_equal (sfract_data *l, sfract_data *r)
{
  /*
   *  same tree, therefore the expressions are same.
   */
  if (l == r)
    return true;
  /*
   *  both constants check their values.
   */
  else if ((l->kind == value_t) && (r->kind == value_t))
    return l->value == r->value;
  /*
   *  both rationals check to see if they are the same rational
   */
  else if (l->is_rational () && r->is_rational ())
    return l->kind == r->kind;
  /*
   *  is one a rational?  In which case they are not the same
   */
  else if (l->is_rational () || r->is_rational ())
    return false;
  /*
   *  maybe they have the same sub expressions ?
   */
  else if (l->kind == r->kind)
    {
      switch (l->kind)
	{
	  /*
	   *   check for communicative properties.
	   */
	case add_t:
	case mult_t:
	  return ((are_equal (l->left, r->left) && are_equal (l->right, r->right)) ||
		  (are_equal (l->left, r->right) && are_equal (l->right, r->left)));

	case sub_t:
	case div_t:
	case power_t:
	  return are_equal (l->left, r->left) && are_equal (l->right, r->right);
	  /*
	   *   check for unary operators.
	   */
	case sin_t:
	case cos_t:
	case tan_t:
	case neg_t:
	  return are_equal (l->left, r->left);
         
	default:
	  assert (false);
	}
    }
  return false;
}


/*
 *  assign_value - 
 */

void sfract_data::assign_value (sfract_data *expr)
{
  assert (expr->is_value () || expr->is_rational ());

  value = expr->value;
  kind = expr->kind;
}


/*
 *  assign_value - 
 */

void sfract_data::assign_value (fract expr)
{
  value = expr;
  kind = value_t;
}


/*
 *  assign_value - 
 */

void sfract_data::assign_value (sfract expr)
{
  assert (expr.data->is_value () || expr.data->is_rational ());
  value = expr.data->value;
  kind = value_t;
}


void sfract_data::assign_binary (sfract expr)
{
  kind = expr.data->kind;
  left = expr.data->left->dup ();
  right = expr.data->right->dup ();
}


void sfract_data::assign_unary (sfract expr)
{
  kind = expr.data->kind;
  left = expr.data->left->dup ();
}


void sfract_data::assign (sfract expr)
{
  switch (expr.data->kind)
    {
    case value_t:
      assign_value (expr);
      break;
    case pi_t:
    case e_t:
    case r2_t:
    case r3_t:
    case r6_t:
      assign_value (expr);
      break;
    case add_t:
    case sub_t:
    case mult_t:
    case div_t:
      assign_binary (expr);
      break;
    case neg_t:
    case power_t:
    case sin_t:
    case cos_t:
    case tan_t:
      assign_unary (expr);
      break;
    default:
      assert (false);
    }
}


/*
 *  map_angle - pre-condition:  none.
 *              post-condition: if angle == value then assign this with result
 *                                 and return true;
 *                              otherwise
 *                                 return false.
 */

bool sfract_data::map_angle (sfract_data *angle, sfract value, sfract result)
{
  if (are_equal (angle, value))
    {
      assign (result);
      return true;
    }
  return false;
}


/*
 *  do_add - attempts to reduce the expression by applying the following rules:
 *           (0 + x)    ->   x
 *           (x + 0)    ->   x
 *           (x + x)    ->   2 * x
 *
 *           it will also attempt to reduce left and right subtrees
 *           it also will fold constant values, ie:
 *
 *           (1 + 2) -> 3
 *
 *           true is returned if _any_ change has been made to the tree.
 */

bool sfract_data::do_add (void)
{
  bool modified = false;

  /*
   *  always want to run the rules on left and right subtrees.
   *  we might be able to reduce them as well.
   */

  if (left->do_rules ())
    modified = true;

  if (right->do_rules ())
    modified = true;

  /*
   *  we check for    (0 + right) -> right
   */

  if (are_equal (left, zero ()))
    {
      assign_value (right);
      return true;
    }

  /*
   *  we check for    (left + 0) -> left
   */

  if (are_equal (zero (), right))
    {
      assign_value (left);
      return true;
    }

  if (left->is_value () && right->is_value ())
    {
      assign_value (left->value + right->value);
      return true;
    }

  if (are_equal (left, right))
    assign (2 * sfract (left));
  return modified;
}


bool sfract_data::do_sub (void)
{
  return false;
}


bool sfract_data::do_mult (void)
{
  return false;
}


bool sfract_data::do_div (void)
{
  return false;
}


bool sfract_data::do_neg (void)
{
  bool modified = false;

  /*
   *  always want to run the rules on the left subtree.
   *  we might be able to reduce them as well.
   */

  if (left->do_rules ())
    modified = true;

  if (left->is_value ())
    {
      assign_value (- (left->value));
      return true;
    }
  return modified;
}


bool sfract_data::do_power (void)
{
  return false;
}


bool sfract_data::do_sin (void)
{
  bool modified = left->do_rules ();

  if (map_angle (left, zero (), zero ()))
    return true;
  if (map_angle (left, pi () / two (), one ()))
    return true;
  return modified;
}


bool sfract_data::do_cos (void)
{
  bool modified = left->do_rules ();

  if (map_angle (left, zero (), one ()))
    return true;
  if (map_angle (left, pi () / two (), zero ()))
    return true;
  return modified;
}


bool sfract_data::do_tan (void)
{
  return false;
}


bool sfract_data::do_rules (void)
{
  switch (kind)
    {

    case value_t:  return false;    // constant therefore no reduction possible
    case pi_t   :  return false;    // constant therefore no reduction possible
    case e_t    :  return false;    // constant therefore no reduction possible
    case r2_t   :  return false;    // constant therefore no reduction possible
    case r3_t   :  return false;    // constant therefore no reduction possible
    case r6_t   :  return false;    // constant therefore no reduction possible
    case add_t  :  return do_add ();
    case sub_t  :  return do_sub ();
    case mult_t :  return do_mult ();
    case div_t  :  return do_div ();
    case neg_t  :  return do_neg ();
    case power_t:  return do_power ();
    case sin_t  :  return do_sin ();
    case cos_t  :  return do_cos ();
    case tan_t  :  return do_tan ();

    default:
      assert (false);
    }
}


/*
 *  simplify - 
 */

void sfract_data::simplify (void)
{
  while (do_rules ())
    ;
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
      os << "(-" << d->left << ")";
      break;
    case power_t:
      os << "pow (" << d->left << ", " << d->right << ")";
      break;
    case sin_t:
      os << "sin (" << d->left << ")";
      break;
    case cos_t:
      os << "cos (" << d->left << ")";
      break;
    case tan_t:
      os << "tan (" << d->left << ")";
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
  data = from.data->dup ();
}


/*
 *  operator= - pre-condition : none.
 *              post-condition: copied the data field.
 */

sfract& sfract::operator= (const sfract &from)
{
  if (data != from.data)
    data = from.data->dup ();
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to a whole number.
 */

sfract::sfract (int value)
{
  init_gc ();
  data = new sfract_data (value);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to num/denom.
 */

sfract::sfract (int num, int demon)
{
  init_gc ();
  data = new sfract_data (num, demon);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to a whole number.
 */

sfract::sfract (longcard whole)
{
  init_gc ();
  data = new sfract_data (whole);
}


/*
 *  sfract - constructor.
 *           pre-constructor : none.
 *           post-constructor: symbolic fraction set to num/denom.
 */

sfract::sfract (longcard num, longcard denom)
{
  init_gc ();
  data = new sfract_data (num, denom);
}


sfract::sfract (sfract_data *expr)
{
  data = expr;
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

  copy.data = new sfract_data (add_t, data->dup (), right.data->dup ());
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

  copy.data = new sfract_data (add_t, data->dup (), r);
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

  copy.data = new sfract_data (add_t, l, right.data->dup ());
  return copy;
}


/*
 *  the / operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this / right)
 */

sfract sfract::operator/ (const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b / c
   *              ^ will alter b
   */
  sfract copy = *this;

  copy.data = new sfract_data (div_t, data->dup (), right.data->dup ());
  return copy;
}


/*
 *  the / operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this / right)
 */

sfract sfract::operator/ (int right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b / c
   *              ^ will alter b
   */
  sfract copy = *this;
  sfract_data *r = new sfract_data (right);

  copy.data = new sfract_data (div_t, data->dup (), r);
  return copy;
}


/*
 *  the / operator - pre-condition:   an initialised sfract.
 *                   post-condition:  left / right
 */

sfract operator/ (int left, const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   */
  sfract copy;
  sfract_data *l = new sfract_data (left);

  copy.data = new sfract_data (div_t, l, right.data->dup ());
  return copy;
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this / right)
 */

sfract sfract::operator* (const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b * c
   *              ^ will alter b
   */
  sfract copy = *this;

  copy.data = new sfract_data (mult_t, data->dup (), right.data->dup ());
  return copy;
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this / right)
 */

sfract sfract::operator* (int right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b * c
   *              ^ will alter b
   */
  sfract copy = *this;
  sfract_data *r = new sfract_data (right);

  copy.data = new sfract_data (mult_t, data->dup (), r);
  return copy;
}


/*
 *  the * operator - pre-condition:   an initialised sfract.
 *                   post-condition:  left / right
 */

sfract operator* (int left, const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   */
  sfract copy;
  sfract_data *l = new sfract_data (left);

  copy.data = new sfract_data (mult_t, l, right.data->dup ());
  return copy;
}


/*
 *  the - operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this - right)
 */

sfract sfract::operator- (const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b - c
   *              ^ will alter b
   */
  sfract copy = *this;

  copy.data = new sfract_data (sub_t, data->dup (), right.data->dup ());
  return copy;
}


/*
 *  the - operator - pre-condition:   an initialised fract.
 *                   post-condition:  (this + right)
 */

sfract sfract::operator- (int right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   *
   *  otherwise   b - c
   *              ^ will alter b
   */
  sfract copy = *this;
  sfract_data *r = new sfract_data (right);

  copy.data = new sfract_data (sub_t, data->dup (), r);
  return copy;
}


/*
 *  the - operator - pre-condition:   an initialised sfract.
 *                   post-condition:  left + right
 */

sfract operator- (int left, const sfract &right)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   */
  sfract copy;
  sfract_data *l = new sfract_data (left);

  copy.data = new sfract_data (sub_t, l, right.data->dup ());
  return copy;
}


/* unary - */
sfract sfract::operator- (void)
{
  /*
   *  notice we must not destroy the original, so we make a copy,
   *  then we add right to the copy and return the copy, leaving
   *  'this' alone.
   */
  sfract copy;

  copy.data = new sfract_data (neg_t, this->data->dup ());
  return copy;
}


/*
 *  pi - pre-condition :  none.
 *       post-condition:  return a symbolic sfract representing
 *                        pi.
 */

sfract sfract::pi (void)
{
  init_gc ();
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (pi_t);
  return temp;
}


/*
 *  e - pre-condition :  none.
 *      post-condition:  return a symbolic sfract representing e.
 */

sfract sfract::e (void)
{
  init_gc ();
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (e_t);
  return temp;
}


/*
 *  r2 - pre-condition :  none.
 *       post-condition:  return a symbolic sfract representing root 2.
 */

sfract sfract::r2 (void)
{
  init_gc ();
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (r2_t);
  return temp;
}


/*
 *  r3 - pre-condition :  none.
 *       post-condition:  return a symbolic sfract representing root 3.
 */

sfract sfract::r3 (void)
{
  init_gc ();
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (r3_t);
  return temp;
}


/*
 *  r6 - pre-condition :  none.
 *       post-condition:  return a symbolic sfract representing root 6.
 */

sfract sfract::r6 (void)
{
  init_gc ();
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (r6_t);
  return temp;
}


/*
 *  sin - pre-condition :  none.
 *        post-condition:  return a symbolic sfract tree representing sin (this).
 */

sfract sfract::sin (void)
{
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (sin_t, this->data->dup ());
  return temp;
}


/*
 *  cos - pre-condition :  none.
 *        post-condition:  return a symbolic sfract tree representing cos (this).
 */

sfract sfract::cos (void)
{
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (cos_t, this->data->dup ());
  return temp;
}


/*
 *  tan - pre-condition :  none.
 *        post-condition:  return a symbolic sfract tree representing tan (this).
 */

sfract sfract::tan (void)
{
  sfract temp;  // we use a temp so we do not trash the calling sfract value

  temp.data = new sfract_data (tan_t, this->data->dup ());
  return temp;
}


/*
 *  operator () - pre-condition :  none.
 *                post-condition:  returns a copy of the current sfract
 *                                 which has been simplified.
 */

sfract sfract::operator() (void)
{
  sfract temp;

  temp.data = data->dup ();
  temp.data->simplify ();
  return temp;
}


/*
 *  wrapper functions are below here which make for easier access to the
 *  sfract functionality.
 */


/*
 *  zero - a wrapper for the constant 0.
 */

sfract zero (void)
{
  sfract f = sfract (0);

  return f;
}


/*
 *  one - a wrapper for the constant 1.
 */

sfract one (void)
{
  sfract f = sfract (1);

  return f;
}


/*
 *  two - a wrapper for the constant 2.
 */

sfract two (void)
{
  sfract f = sfract (2);

  return f;
}


/*
 *  half - a wrapper for the constant 1/2.
 */

sfract half (void)
{
  sfract f = sfract (1, 2);

  return f;
}


/*
 *  third - a wrapper for the constant 1/3.
 */

sfract third (void)
{
  sfract f = sfract (1, 3);

  return f;
}


/*
 *  quarter - a wrapper for the constant 1/4.
 */

sfract quarter (void)
{
  sfract f = sfract (1, 4);

  return f;
}


/*
 *  pi - a wrapper for the constant sfract pi.
 */

sfract pi (void)
{
  sfract f;

  return f.pi ();
}


/*
 *  r2 - a wrapper for the constant sfract root 2.
 */

sfract r2 (void)
{
  sfract f;

  return f.r2 ();
}


/*
 *  r3 - a wrapper for the constant sfract root 3.
 */

sfract r3 (void)
{
  sfract f;

  return f.r3 ();
}


/*
 *  r6 - a wrapper for the constant sfract root 6.
 */

sfract r6 (void)
{
  sfract f;

  return f.r6 ();
}


/*
 *  sin - a wrapper for the function sin.
 */

sfract sin (sfract radians)
{
  return radians.sin ();
}


/*
 *  cos - a wrapper for the function cos.
 */

sfract cos (sfract radians)
{
  return radians.cos ();
}


/*
 *  tan - a wrapper for the function sin.
 */

sfract tan (sfract radians)
{
  return radians.tan ();
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


static gc *sfract_data_gc = 0;
static sfract_data *head_root = 0;


void init_gc (void)
{
  if (sfract_data_gc == 0)
    sfract_data_gc = init_garbage (sizeof (sfract_data), __FILE__);
}

void sfract_data::walk_used (void)
{
  if (sfract_data_entity == 0)
    sfract_data_entity = sfract_data_gc->get_entity (this);

  if (! sfract_data_entity->is_used ())
    {
      if (left != 0)
	left->walk_used ();
      if (right != 0)
	right->walk_used ();
      // mark ourselves last
      sfract_data_entity->used ();
    }
}


void sfract::walk_used (void)
{
  if (data != 0)
    data->walk_used ();
}


/*
 *  root - register the sfract_data as always in use.
 *         pre-constructor : none.
 *         post-constructor: sfract_data placed onto root_list.
 */

void sfract_data::root (void)
{
  if (sfract_data_entity == 0)
    sfract_data_entity = sfract_data_gc->get_entity (this);
  if (! sfract_data_gc->is_rooted (sfract_data_entity))
    {
      next_root = head_root;
      head_root = this;
      sfract_data_gc->root (sfract_data_entity);  // and with the garbage collector
    }
}


/*
 *  root - register the fraction to the garbage collector.
 *         pre-constructor : none.
 *         post-constructor: fraction registered.
 */

void sfract::root (void)
{
  if (data != 0)
    data->root ();
}


/*
 *  unroot - deregister the fraction to the garbage collector.
 *           pre-constructor : none.
 *           post-constructor: fraction unregistered.
 */

void sfract::unroot (void)
{
  /* finish unroot */
}


void sfract_data::rooted_used (void)
{
  walk_used ();
  if (next_root != 0)
    next_root->rooted_used ();
}


/*
 *  sfract_rooted_used - 
 */

void sfract_rooted_used (void)
{
  if (head_root == 0)
    head_root->rooted_used ();
}


/*
 *  sfract_garbage_collect - pre-condition :  garbage collector has been initialised.
 *                                            important sfract have been rooted.
 *                           post-condition:  any unused sfract objects are returned to
 *                                            the free list.
 */

void sfract_garbage_collect (void)
{
  sfract_data_gc->stats ();
  sfract_data_gc->mark_allocated ();
  sfract_rooted_used ();
  sfract_data_gc->collect ();
  sfract_data_gc->stats ();
}
