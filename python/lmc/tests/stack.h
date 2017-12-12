#if !defined(STACK_H)
#  define STACK_H

#include "indirect.h"

/*
 *  macros to implement INC and DEC
 */

# define INC(X)  \
  lca  1;	 \
  add  X;	 \
  sta  X


# define DEC(X)  \
  lca  0xfff;	 \
  add  X;	 \
  sta  X


/*
 *  macro to mimic a push instruction
 *  push accumulator to the stack.
 */

# define PUSH  \
  DEC(SP) ;    \
  SIND(SP)


/*
 *  macro to pop the top of stack to the
 *  accumulator
 */

#define POP    \
  LIND(SP) ;   \
  INC(SP)


/*
 *  call function X
 */

#define CALL(X) \
  LPC ;		\
  PUSH ;	\
  bra X
  

/*
 *  RET return from function
 */

#define RET  \
  lca 5 ;    \
  ADS ;      \
  POP ;	     \
  SPC


/*
 *  ADS(X)  add contents of X to the top of stack
 */

#define ADS(X) \
  LIND(SP) ;   \
  add X ;      \
  SIND(SP)


/*
 *  ADC(X)  add constant X to the top of stack
 */

#define ADC(X) \
  lca  X+0x100 ; \
  add  .+2 ;	 \
  lda SP ;	 \
  dat 0 ;	 \
  SIND(SP)
 
#endif
