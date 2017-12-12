#include "crt0.h"

	lca  hello
	sta  ptr    /* ptr = "hello world\n" */
while:	LIND(ptr)
	brz end     /* while (*ptr != 0) do */
	out         /*   write (*ptr); */
	INC(ptr)    /*   p++;  */
	bra while   /* end */
end:	hlt

	/*
	 *  null terminated string
	 */
hello:	.string "hello world\n"
	dat 0
ptr:	dat 0

