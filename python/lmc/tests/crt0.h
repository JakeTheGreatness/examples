#if !defined(CRT0_H)
#  define CRT0_H

#include "stack.h"


/*
 *  set up some useful memory constants.  Notice that the stack
 *  pointer could be placed anywhere, but the PC register must
 *  be located at 0xff in our modified LMC.
 */

#define MAX_MEM   0xff
#define SP_LOC    MAX_MEM     /* we use this location for the stack pointer */
#define SP_TOP    MAX_MEM-1

/*
 *  INIT define a location for our top of stack.
 *  Note that the assembler will be at location
 *  zero after this byte has been defined as the memory
 *  address will wrap around.
 */

#define _INIT	   \
  .origin SP_LOC ; \
SP:  dat SP_TOP ;  \


#define ASSIGN(X,Y) \
  lda Y ;	    \
  sta X ;

#define IS_EQUAL(X,Y) \
  lda X ;	      \
  sub Y ;

/*
 *  invert True/False in the accumulator
 */

#define NOT(L)	\
  brz L ## 1 ;	\
  ldc 1 ;	\
  bra L ## 2 ;	\
L ## 1: ;	\
  ldc 0 ;	\
L ## 2:


#if 0
_INIT
#endif

#endif
