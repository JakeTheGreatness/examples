#if !defined(INDIRECT_H)
#  define INDIRECT_H

/*
 *  macro to mimic an indirect instruction
 *  load indirect into accumulator
 *
 *  acc = *X
 */

# define LIND(X) \
  lca  0x500 ;	 \
  add  X;	 \
  sta  .+1;	 \
  dat  0

/*
 *  macro to mimic an indirect instruction
 *  store indirect from accumulator
 *
 *  *X = acc
 */

#define SIND(X) \
  lca  0x300;	\
  add  X;	\
  sta  .+1;	\
  dat  0

/*
 *  macro to store the program counter into the
 *  accumulator.
 */

#define LPC  \
  lca .

/*
 *  macro to store the accumulator into the program
 *  counter
 */

#define SPC  \
  sta  .+4 ; \
  lca  0x600 ;  /* 0x600 is a branch */ \
  add  .+2 ; \
  sta  .+1 ; \
  dat 0

#endif
