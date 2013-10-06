#if !defined(FRACTH)
#  define FRACTH

/*
 *  introduce the fractional data type.
 */

typedef long int longint;


class fract
{
 private:
  bool unknown;
  bool positive;
  bool dirty;
  longint whole;
  longint num;
  longint demon;
  fract clean (void);
  bool is_dirty (void);
  
 public:
  fract (void);
  ~fract (void);
  fract (const fract &from);
  fract& operator= (const fract &from);
  fract fract (int);
  fract fract (longint);
  fract fract (int, int);
  fract simplify (void);
  bool is_positive (void);
  bool is_negative (void);
  bool is_unknown (void);
  fract operator* (const fract &right);
};

#endif
