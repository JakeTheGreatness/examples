/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  introduce the fractional data type.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#include <fract.h>
#include <cassert>
#include <cstdio>

static const bool debugging = false;


/*
 *  gcd - Euclid's Greatest Common Denominator algorithm.
 *        pre-condition :  x and y are both >0
 *        post-condition:  return the greatest denominator of, x, and, y.
 */
   
longcard gcd (longcard x, longcard y)
{
  while (x != y)
    {
      if (x > y)
	x -= y;
      else
	y -= x;
    }
  return x;
}


/*
 *  factorial - return n!
 */

longcard factorial (longcard n)
{
  longcard value = 1;

  while (n > 0)
    {
      value *= n;
      n--;
    }
  return value;
}


/*
 *  fract - constructor.
 *          pre-condition : none.
 *          post-condition: an unknown fract is created.
 */

fract::fract (void)
  : positive(true), top_heavy(false), whole(0), num(0), denom(0)
{
}


fract::~fract (void)
{
}


/*
 *  fract - the copy operator.
 */

fract::fract (const fract &from)
{
  this->positive = from.positive;
  this->top_heavy = from.top_heavy;
  this->whole = from.whole;
  this->num = from.num;
  this->denom = from.denom;
}


/*
 *  fract - the assignment operator.
 */

fract& fract::operator= (const fract &from)
{
  this->positive = from.positive;
  this->top_heavy = from.top_heavy;
  this->whole = from.whole;
  this->num = from.num;
  this->denom = from.denom;
}


/*
 *  fract - construct a fract representing whole number, value.
 */

fract::fract (longcard value)
{
  positive = true;
  top_heavy = false;
  whole = value;
  num = 0;
  denom = 0;
}


/*
 *  fract - construct a fract representing n/d.
 */

fract::fract (longcard n, longcard d)
{
  positive = true;
  top_heavy = (n != 0);
  whole = 0;
  num = n;
  denom = d;
  this->simplify();
}


/*
 *  fract - assign a whole number to the fractional datatype.
 */

fract::fract (int value)
{
  *this = fract ((longcard) value);
}


/*
 *  fract - construct a fract representing n/d.
 */

fract::fract (int n, int d)
{
  *this = fract ((longcard) n, (longcard) d);
}


/*
 *  fract - return true if positive.
 */

bool fract::is_positive (void)
{
  return positive;
}


/*
 *  fract - return true if negative.
 */

bool fract::is_negative (void)
{
  return !positive;
}


/*
 *  not_top_heavy - mark fract as not top_heavy.
 */

fract fract::not_top_heavy (void)
{
  top_heavy = false;
  return *this;
}


/*
 *  is_zero - pre-condition:  an initialised fraction.
 *            post-condition: returns true if value == 0.
 */

bool fract::is_zero (void)
{
  return (whole == 0) && (num == 0);
}


/*
 *  negate - flip the sign of fraction.
 *           The magnitude remains the same, so:    1+1/2
 *                                      becomes:  -(1+1/2)
 */

fract fract::negate (void)
{
  positive = !positive;
  return *this;
}


/*
 *  addND - add the numerator/denomimator pairs of, this, and, right,
 *          ignoring whole values.
 *          Although it will propagate the carry (if it exists) into whole.
 *
 *          pre-condition:   two initialised fracts
 *          post-condition:  this += right ; return this
 */

fract fract::addND (fract right)
{
  if (num == 0)                // cannot use is_zero since whole might be >0
    {
      num = right.num;
      denom = right.denom;
    }
  else if (right.num == 0)     // cannot use is_zero since whole might be >0
    ;    /* nothing to do */
  else
    {
      longcard g, lg, rg;

      g = gcd (denom, right.denom);
      lg = right.denom / g;
      rg = denom / g;
      
      num = (num * lg) + (right.num * rg);
      denom = denom * lg;
      simplify ();   /* handles carry */
    }
  return *this;
}


/*
 *  inc - increments fract by right.
 *        pre-condition:  an initialised fract.
 *        post-condition: this += right; return this;
 */

fract fract::inc (fract right)
{
  bool n;

  simplify ();        // ensure that fract is not top heavy
  right.simplify ();  // likewise for right operand

  if (right.is_zero ())
    return *this;
  else if (is_zero ())
    {
      return right;
    }
  /*
   *  check for both positive or both negative
   *
   *    1 + 1  =>  2     (sign remains the same)
   *   -1 + -1 => -2     (in both cases)
   */
  if (positive == right.positive)
    {
      top_heavy = true;
      whole += right.whole ;
      return addND (right);
    }
  /*
   *  ok from now on one of the operands is negative and the other positive
   *  so we call upon dec to perform the next computations - but we ensure that
   *  its arguments are positive (as dec may call inc if its arguments are negative).
   */
  else if (positive && (right.is_negative ()))
    return dec (right.negate ());
  else if (is_negative() && (right.is_positive ()))
    return negate ().dec (right).negate ();

  /*
   *  all cases should be handled above
   */
  assert (false);
}


/*
 *  dec - decrements fract by right.
 *        pre-condition:  an initialised fract.
 *        post-condition: this -= right; return this;
 */

fract fract::dec (fract right)
{
  simplify ();        // ensure that fract is not top heavy
  right.simplify ();  // likewise for right operand

  if (right.is_zero ())
    // nothing to do, finish now
    return *this;
  if (is_zero ())
    {
      assert (! right.is_zero ());
      positive = !positive;
      whole = right.whole;
      num = right.num;
      denom = right.denom;
      return top_heavy;
    }
  // easy cases done, now check the four combinations of sign
  if (positive && right.positive)
    {
      top_heavy = true;
      if (right.whole <= whole)
	{
	  // positive whole result, now work out fractional part
	  whole -= right.whole;
	  if (is_zero ())
	    {
	      num = right.num;
	      denom = right.denom;
	      if (whole == 0 && num == 0)
		positive = true;
	      else
		positive = false;
	    }
	  else if (subND (*this, right))
	    {
	      /*
	       *  we have performed the fractional computation from
	       *  this = this - right and we need to borrow from whole
	       */
	      if (whole >= 1)
		whole--;
	      else
		positive = false;
	    }
	}
      else
	{
	  // negative whole result, therefore flip operands and subtract, ie  (right - left)
	  whole = right.whole - whole;
	  positive = false;

	  // we call subND to calculate the fractional component
	  fract a = fract (num, denom);
	  fract b = fract (right.num, right.denom);
	  if (subND (b, a))
	    whole--;
	  num = b.num;
	  denom = b.denom;
	}
    }
  /*
   *  fortunately it gets easier from now as we can re-use the above code after sign flipping
   */
  else if ((! positive) && (! right.positive))
    /*
     *  both operands are negative, use increment, after negating the second operand and ignore the sign
     *  for example:  -3 - -2  =>  -3 + 2  =>  -1
     */
    
    *this = inc (right.negate ());
  else if (positive && (! right.positive))
    /*
     *  subtracting a negative right operand
     *  for example:  +3 - -2  =>  +3 + 2  =>  5
     */

    *this = inc (right.negate ());    // notice the same as above, we could combine, but safer to enumerate all cases
  else if (((! positive) && (right.positive)))
    /*
     *  negative left - positive right operand
     *  for example:  -3 - +2  =>   -1
     */
    *this = negate ().inc (right).negate ();
  else
    assert (false);   // all cases should be checked above
  return simplify ();
}


/*
 *  subND - sub the numerator/denomimator pairs of, left, and, right
 *          ignoring whole values.  It returns true if it needs to borrow
 *          from left.whole.
 *
 *          pre-condition:   two initialised fracts.
 *          post-condition:  left -= right ; return carry required.
 */

bool fract::subND (fract &left, fract right)
{
  longcard g, lg, rg;

  if (left.num == 0)
    {
      left.num = right.denom-left.num;
      left.denom = right.denom;
      left.top_heavy = true;
      return right.num != 0;
    }
  else if (right.num == 0)
    {
      /* nothing to do */
      return false;
    }
  else
    {
      left.top_heavy = true;
      g = gcd (left.denom, right.denom) ;
      lg = right.denom / g;
      rg = left.denom / g;
      
      if ((left.num * lg) >= (right.num * rg))
	{
	  /* no need to borrow */
	  left.num = (num * lg) - (right.num * rg);
	  left.denom = left.denom * lg ;
	  return false;
	}
      else
	{
	  /* need to borrow */
	  left.denom = left.denom * lg;
	  left.num = (right.num * rg) - (left.num * lg);
	  return true;
	}
    }
}


/*
 *  reciprocal - perform the reciprocal function.
 *               pre-condition :  an initialised fraction.
 *               post-condition:  returns 1/this
 */

fract fract::reciprocal (void)
{
  if (num == 0 && whole == 0)
    return fract (0);
  else if (num == 0)
    /* only a whole number */
    return fract ((longcard)1, whole);
  else
    {
      fract r = fract (denom, whole * denom + num);
      r.top_heavy = true;

      if (! positive)
	r = r.negate ();
      
      return r.simplify ();
    }
}


/*
 *  the + operator - pre-condition:   an initialised fract.
 *                   post-condition:  this += right
 *
 */

fract fract::operator+ (const fract &right)
{
  fract t = *this;  // we make a copy so we do not destroy *this

  return t.inc (right).simplify ();
}


/*
 *  the + operator - pre-condition:   an initialised fract.
 *                   post-condition:  left + right
 */

fract fract::operator+ (int right)
{
  fract r = fract (right);
  fract l = *this;
  return l + r;
}


/*
 *  the + operator - pre-condition:   an initialised fract.
 *                   post-condition:  left + right
 */

fract operator+ (int left, const fract &right)
{
  fract l = fract (left);
  return l + right;
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  this.mult(right)
 *
 *                   (as with the + operator we must not alter *this)
 */

fract fract::operator* (const fract &right)
{
  if (positive && right.positive)
    {
      /*
       *         b            e                A*e     D*b     b*e
       *     A   -    x    D  -   =  (A*D)  +  ---  +  ---  +  ---
       *         c            f                 f       c      c*f
       *
       *
       *                               W        X       Y       Z
       */

      fract W = fract (whole * right.whole);
      fract X = fract (whole * right.num, right.denom);
      fract Y = fract (right.whole * num, denom);
      fract Z = fract (num * right.num, denom * right.denom);
      fract r = W+X+Y+Z;

      return r;
    }
  if (positive && (! right.positive))
    {
      fract r = right;
      
      r = (*this) * r.negate();
      return r.negate ();
    }
  // to do, finish the other two cases of multiply
  if ((! positive) && right.positive)
    {
      fract r;

      negate ();
      r = (*this) * right;
      r = r.negate ();
      return r;
    }
  if ((! positive) && (! right.positive))
    {
      fract r = right;

      r = r.negate ();
      negate ();
      r = (*this) * r;
      return r;
    }

  assert (false);
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  left * right
 */

fract fract::operator* (int right)
{
  fract r = fract (right);
  fract l = *this;
  return l * r;
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  left * right
 */

fract operator* (int left, const fract &right)
{
  fract l = fract (left);
  return l * right;
}


/*
 *  the - operator - pre-condition:   an initialised fract.
 *                   post-condition:  this -= right
 *
 */

fract fract::operator- (const fract &right)
{
  fract t = *this;  // we make a copy so we do not destroy *this

  return t.dec (right).simplify ();
}


/*
 *  the - operator - pre-condition:   an initialised fract.
 *                   post-condition:  left - right
 */

fract fract::operator- (int right)
{
  fract r = fract (right);
  fract l = *this;
  return l - r;
}


/*
 *  the - operator - pre-condition:   an initialised fract.
 *                   post-condition:  left - right
 */

fract operator- (int left, const fract &right)
{
  fract l = fract (left);
  return l - right;
}


/*
 *  the / operator - pre-condition:   an initialised fract.
 *                   post-condition:  this / right
 *
 */

fract fract::operator/ (const fract &right)
{
  fract l = *this;
  fract r = right;
  
  r = r.reciprocal ();
  return l * r;
}


/*
 *  the / operator - pre-condition:   an initialised fract.
 *                   post-condition:  left / right
 */

fract fract::operator/ (int right)
{
  fract r = fract (right);
  fract l = *this;

  r = r.reciprocal ();
  return l * r;
}


/*
 *  the / operator - pre-condition:   an initialised fract.
 *                   post-condition:  left - right
 */

fract operator/ (int left, const fract &right)
{
  fract l = fract (left);
  fract r = right;

  r = r.reciprocal ();
  return l * r;
}


/*
 *  sin - compute sin (r) using Taylors expansion.
 *        r is 'this'.
 *
 *                 3      5      7      9
 *                r      r      r      r
 *        = r  -  --  +  --  -  --  +  --   etc
 *                3!     5!     7!     9!
 *
 */

fract fract::sin (void)
{
  fract r = *this;
  fract sum = r;
  bool pos = false;

  for (int i = 3; i < 9; i += 2)
    {
      if (debugging)
	{
	  fract t = (r.power (i) / fract (factorial (i)));
    
	  std::cout << t << "\n";
	}
      if (pos)
	sum += (r.power (i) / fract (factorial (i)));
      else
	sum -= (r.power (i) / fract (factorial (i)));
      pos = !pos;
    }
  sum = sum.simplify ();
  if (debugging)
    std::cout << " sin (" << *this << ") = " << sum << "\n";
  return sum;
}


/*
 *  cos - compute cos (r) using Taylors expansion.
 *        r is 'this'.
 *
 *                 2      4      6      8
 *                r      r      r      r
 *        = 1  -  --  +  --  -  --  +  --   etc
 *                2!     4!     6!     8!
 *
 */

fract fract::cos (void)
{
  fract r = *this ;
  fract sum = fract (1);
  bool pos = false;

  for (int i = 2; i < 8; i += 2)
    {
      if (pos)
	sum += (r.power (i) / fract (factorial (i)));
      else
	sum -= (r.power (i) / fract (factorial (i)));
      pos = !pos;
    }
  return sum;
}


/*
 *                     n
 *  power - return this
 *          pre-condition :  none.
 *          post-condition:  this^n.
 */

fract fract::power (int n)
{
  fract r = fract (1);

  while (n > 0)
    {
      r *= (*this);
      n--;
    }
  return r;
}


/*
 *  operator+=  implement the += operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator+= (const fract &right)
{
  fract r = (*this) + right;

  *this = r;
}


/*
 *  operator+=  implement the += operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator+= (int right)
{
  fract r = (*this) + fract (right);

  *this = r;
}


/*
 *  operator-=  implement the -= operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator-= (const fract &right)
{
  fract r = (*this) - right;

  *this = r;
}


/*
 *  operator-=  implement the -= operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator-= (int right)
{
  fract r = (*this) - fract (right);

  *this = r;
}


/*
 *  operator*=  implement the *= operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator*= (const fract &right)
{
  fract r = (*this) * right;

  *this = r;
}


/*
 *  operator*=  implement the *= operator.
 *              pre-condition :  none.
 *              post-condition:  return this += right
 */

fract& fract::operator*= (int right)
{
  fract r = (*this) * fract (right);

  *this = r;
}


/*
 *  operator- - unary - operator overload.
 *              pre-condition :  none.
 *              post-condition:  -this.
 */

fract fract::operator- (void)
{
  return negate ();
}


/*
 *  f - convert fract to float.
 */

float fract::f (void)
{
  return ((float)whole)+((float)(num))/((float)(denom));
}


/*
 *  f - convert fract to int.
 */

int fract::i (void)
{
  return ((int)whole)+((int)(num))/((int)(denom));
}


/*
 *  d - convert fract to double.
 */

double fract::d (void)
{
  return ((double)whole)+((double)(num))/((double)(denom));
}


/*
 *  ld - convert fract to long double.
 */

long double fract::ld (void)
{
  return ((long double)whole)+((long double)(num))/((long double)(denom));
}


/*
 *  simplify - pre-condition :  an initialised fract
 *             post-condition:  the same value is returned by the
 *                              whole, num, denom are converted into their
 *                              simplist form.
 */

fract fract::simplify (void)
{
  longcard d;

  if (! top_heavy)
    return *this;

  if ((num != 0) && (denom != 0))
    {
      if (num > denom)
	{
	  d = num/denom;
	  if (whole>=0)
	    whole += d;
	  else
	    whole -= d;
	  num = num % denom;
	}
      if ((num != 0) && (denom != 0))
	{
	  d = gcd(num, denom);
	  if (d>1)
	    {
	      num = num / d;
	      denom = denom / d;
	    }
	}
      if (num == denom)
	{
	  assert (num == 1);
	  num = 0;
	  denom = 0;
	  whole++;
	}
      if (num == 0)
	denom = 0;
    }
  *this = not_top_heavy ();
  return *this;
}


/*
 *  operator==  implement comparison.
 *              pre-condition :  none.
 *              post-condition:  returns left == right.
 */

bool operator== (const fract &left, const fract &right)
{
  return ((left.positive == right.positive) &&
	  (left.whole == right.whole) &&
	  (left.num == right.num) &&
	  (left.denom == right.denom));
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  an initialised list.
 *                 post-condition:  fraction printed and stream returned.
 */

std::ostream& operator<< (std::ostream& os, const fract &f)
{
  fract g = f;

  if (g.is_zero ())
    {
      os << "0";
      return os;
    }
  if (! f.positive)
    {
      os << "-";
      if (f.whole != 0 && f.num != 0)
	{
	  os << "(";
	  if (f.whole != 0)
	    {
	      os << f.whole;
	      os << "+";
	    }
	  os << f.num;
	  os << "/";
	  os << f.denom;
	  os << ")";
	}
      else if (f.num == 0)
	os << f.whole;
      else
	{
	  os << f.num;
	  os << "/";
	  os << f.denom;
	}
    }
  else if (f.whole == 0)
    {
      os << f.num;
      os << "/";
      os << f.denom;
    }
  else
    {
      os << f.whole;
      if (f.num != 0)
	{
	  os << "+";
	  os << f.num;
	  os << "/";
	  os << f.denom;
	}
    }
  return os;
}
