#include <fract.h>
#include <cassert>


/*
 *  fract - constructor.
 *          pre-condition : none.
 *          post-condition: an unknown fract is created.
 */

fract::fract (void)
  : unknown(true), positive(true), dirty(false), whole(0), num(0), demon(0)
{
}


fract::~fract (void)
{
  unknown = true;
}


/*
 *  fract - the copy operator.
 */

fract fract::fract (const fract &from)
{
  this = from;
}


/*
 *  fract - the assignment operator.
 */

fract fract::fract& operator= (const fract &from)
{
  this = from;
}


/*
 *  fract - assign a whole number to the fractional datatype.
 */

fract fract::fract (int value)
{
  return fract ((longint) value);
}


/*
 *  fract -
 */

fract fract::fract (longint value)
{
  unknown = false;
  positive = true;
  dirty = false;
  whole = value;
  num = 0;
  demon = 0;
}


/*
 *  fract -
 */

fract fract::fract (longint n, longint d)
{
  unknown = false;
  positive = true;
  dirty = false;
  whole = 0;
  num = n;
  demon = d;
}


fract fract::fract (int n, int d)
{
  return fract ((longint) n, (longint) d);
}


bool fract::is_positive (void)
{
  return positive;
}


bool fract::is_negative (void)
{
  return !positive;
}


bool fract::is_unknown (void)
{
  return unknown;
}


/*
 *  addND - add the numerator/demonimator pairs of, this, and, right,
 *          ignoring whole values.
 *          Although it will propagate the carry (if it exists) into whole.
 *
 *          pre-condition:   two initialised fracts
 *          post-condition:  this += right ; return this
 */

fract fract::addND (fract right)
{
  if (is_zero ())
    {
      num = right.num;
      demon = right.demon;
    }
  else if (right.is_zero ())
    ;    /* nothing to do */
  else
    {
      longint g, lg, rg;

      g = gcd (demon, right.demon);
      lg = right.demon / g;
      rg = demon / g;
      
      num = (num * lg) + (right.num * rg);
      demon = demon * lg;
      return simplify(l);   /* handles carry */
    }
  return this;
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
    return this;
  else if (isZero ())
    {
      this = right;
      return this;
    }
  if (positive == right.positive)
    {
      dirty();
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
    return negate ().dec (r).negate ();

  /*
   *  all cases should be handled above
   */
  assert (false);
}


/*
 *  the * operator - pre-condition:   an initialised fract.
 *                   post-condition:  this.mult(right)
 */

fract fract::operator* (const fract &right)
{
  if (positive && right.positive)
    {
      fract f = inc(inc (fract (whole * right.whole),
			 inc (fract (whole * right.num, right.demon),
			      fract (right.whole * num, demon))),
		    fract (num * right.num, demon * right.demon));
      return f;
    }
  else if (positive)
    {
      assert(right.is_negative ());
      fract f = right.negate();
      f = inc(inc(negate(cardinal(whole * f.whole)),
                  inc(negate(initFract(0, l^.whole * r^.num, r^.demon)),
                       negate(initFract(0, r^.whole * l^.num, l^.demon)))),
               negate(initFract(0, l^.num*r^.num, l^.demon*r^.demon)))
   ELSIF r^.positive
   THEN
      Assert(NOT l^.positive) ;
      l := negate(l) ;
      f := inc(inc(negate(cardinal(l^.whole*r^.whole)),
                   inc(negate(initFract(0, l^.whole * r^.num, r^.demon)),
                       negate(initFract(0, r^.whole * l^.num, l^.demon)))),
               negate(initFract(0, l^.num*r^.num, l^.demon*r^.demon)))
   ELSE
      Assert((NOT l^.positive) AND (NOT r^.positive)) ;
      l := negate(l) ;
      r := negate(r) ;
      f := inc(inc(cardinal(l^.whole*r^.whole),
                   inc(initFract(0, l^.whole * r^.num, r^.demon),
                       initFract(0, r^.whole * l^.num, l^.demon))),
               initFract(0, l^.num*r^.num, l^.demon*r^.demon))
   END ;
   RETURN simplify(f)

}


/*
 *  simplify - pre-condition :  an initialised fract
 *             post-condition:  the same value is returned by the
 *                              whole, num, demon are converted into their
 *                              simplist form.
 */

fract fract::simplify (void)
{
  longint d;


  if (! dirty)
    return this;

  if ((num != 0) && (demon != 0))
    {
      if (num > demon)
	{
	  d = num/demon;
	  if (whole>=0)
	    whole += d;
	  else
	    whole -= d;
	  num = num % demon;
	}
      if ((num != 0) AND (demon != 0))
	{
	  d = gcd(num, demon);
	  if (d>1)
	    {
	      num = num / d;
	      demon = demon DIV d;
	    }
	}
      if (num == demon)
	{
	  assert (num == 1);
	  num = 0;
	  demon = 0;
	  whole++;
	}
      if (num == 0)
	demon = 0;
    }
  return clean ();
}


/*
 *  gcd - pre-condition :  x and y are both >0
 *        post-condition:  return the greatest demoninator of, x, and, y.
 */
   
longint gcd (longint x, longint y)
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

