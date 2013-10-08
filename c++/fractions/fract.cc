#include <fract.h>
#include <cassert>
#include <cstdio>


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
 *  fract - constructor.
 *          pre-condition : none.
 *          post-condition: an unknown fract is created.
 */

fract::fract (void)
  : positive(true), dirty(false), whole(0), num(0), denom(0)
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
  this->dirty = from.dirty;
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
  this->dirty = from.dirty;
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
  dirty = false;
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
  dirty = (n != 0);
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
 *  clean - mark fract as clean.
 */

fract fract::clean (void)
{
  dirty = false;
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

  simplify();
  right.simplify();

  if (right.is_zero ())
    return *this;
  else if (is_zero ())
    {
      *this = right;
      return *this;
    }
  if (positive == right.positive)
    {
      dirty = true;
      whole += right.whole ;
      return addND (right);
    }
  /*
   *  call upon dec to perform the next computations - but we ensure that
   *  its arguments are positive.
   */
  else if (positive && (right.is_negative ()))
    return dec (right.negate ());
  else if (is_negative () && (right.is_positive ()))
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
  /* --fixme-- finish this code */
}

/*
 *  subND - sub the numerator/denomimator pairs of, this, and, right
 *          ignoring whole values.  It returns true if it needs to borrow
 *          from this.whole.
 *
 *          pre-condition:   two initialised fracts.
 *          post-condition:  this -= right ; return carry required.
 */

fract fract::subND (fract right)
{
  longcard g, lg, rg;

  if (num == 0)
    {
      num = right.denom-num;
      denom = right.denom;
      dirty = true;
      return right.num != 0;
    }
  else if (right.num == 0)
    {
      /* nothing to do */
      return false;
    }
  else
    {
      dirty = true;
      g = gcd (denom, right.denom) ;
      lg = right.denom / g;
      rg = denom / g;
      
      if ((num * lg) >= (right.num * rg))
	{
	  /* no need to borrow */
	  num = (num * lg) - (right.num * rg);
	  denom = denom * lg ;
	  return false;
	}
      else
	{
	  /* need to borrow */
	  denom = denom * lg;
	  num = (right.num * rg) - (num * lg);
	  return true;
	}
    }
}

#if 0
(*
   dec - returns, l, after, r, has been subtracted.
*)

PROCEDURE dec (l, r: Fract) : Fract ;
VAR
   n   : BOOLEAN ;
   t, s: Fract ;
BEGIN
   l := simplify(l) ;
   r := simplify(r) ;
   IF isZero(r)
   THEN
      RETURN l
   ELSIF isZero(l)
   THEN
      IF isZero(r)
      THEN
         l^.positive := TRUE
      ELSE
         l^.positive := NOT r^.positive
      END ;
      l^.whole := r^.whole ;
      l^.num := r^.num ;
      l^.denom := r^.denom ;
      RETURN dirty(l)
   END ;
   IF r=l
   THEN
      r := dup(l)
   END ;
   IF l^.positive AND r^.positive
   THEN
      l := dirty(l) ;
      IF r^.whole<=l^.whole
      THEN
         (* positive whole result *)
         l^.whole := l^.whole-r^.whole ;
         IF isZero(l)
         THEN
            l^.num := r^.num ;
            l^.denom := r^.denom ;
            IF (l^.whole=0) AND (l^.num=0)
            THEN
               l^.positive := TRUE
            ELSE
               l^.positive := FALSE
            END
         ELSIF subND(l, r)
         THEN
            IF l^.whole>=1
            THEN
               DEC(l^.whole)
            ELSE
               l^.positive := FALSE
            END
         END
      ELSE
         (* negative whole result, therefore flip the operands and subtract *)
         l^.whole := r^.whole-l^.whole ;
         l^.positive := FALSE ;
         s := initFract(0, l^.num, l^.denom) ;
         t := initFract(0, r^.num, r^.denom) ;
         IF subND(t, s)
         THEN
            DEC(l^.whole)
         END ;
         l^.num := t^.num ;
         l^.denom := t^.denom
      END
   ELSIF (NOT l^.positive) AND (NOT r^.positive)
   THEN
      l := inc(l, negate(r))
   ELSIF l^.positive AND (NOT r^.positive)
   THEN
      l := inc(l, negate(r))
   ELSIF (NOT l^.positive) AND r^.positive
   THEN
      RETURN negate(inc(negate(l), r))
   ELSE
      HALT
   END ;
   RETURN simplify(l)
END dec ;
#endif


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
  assert (false);
}


/*
 *  the + operator - pre-condition:   an initialised fract.
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
 *  simplify - pre-condition :  an initialised fract
 *             post-condition:  the same value is returned by the
 *                              whole, num, denom are converted into their
 *                              simplist form.
 */

fract fract::simplify (void)
{
  longcard d;

  if (! dirty)
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
  *this = clean ();
  return *this;
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
