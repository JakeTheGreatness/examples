.TL
Little Man Computer
.LP
.SH
Extended instruction set
.LP
In the extended instruction set we introduce the instruction
.PRG lca
(load constant to accumulator).  The memory has 256 locations of 12
bit words and they can be viewed as hexadecimal.  Arithmetic is 2s
compliment 12 bit signed.
.TS
tab(!), center, box;
l!l!l
nC!lB!s.
Hex code!mnemonic!Description
=
1xx!add!add contents of address xx to the accumulator.
2xx!sub!subtract contents of address xx from the accumulator.
3xx!sta!store contents of the accumulator into word xx.
4xx!lca!load constant xx into the accumulator.
5xx!lda!load data from address xx into the accumulator.
6xx!bra!jump to location xx.
7xx	BRZ	BRANCH IF ZERO (conditional)	If the accumulator (calculator) contains the value 000, set the program counter to the value xx. Otherwise, do nothing.
Note: since the program is stored in memory, data and program instructions all have the same address/location format.
8xx	BRP	BRANCH IF POSITIVE (conditional)	If the accumulator (calculator) is 0 or positive, set the program counter to the value xx. Otherwise, do nothing.
Note: since the program is stored in memory, data and program instructions all have the same address/location format.
901	INP	INPUT	Go to the INBOX, fetch the value from the user, and put it in the accumulator (calculator)
Note: this will overwrite whatever value was in the accumulator (destructive)
902	OUT	OUTPUT	Copy the value from the accumulator (calculator) to the OUTBOX.
Note: the contents of the accumulator are not changed (non-destructive).
000	HLT/COB	HALT/COFFEE BREAK	Stop working.
DAT	DATA	This is an assembler instruction which simply loads the value into the next available mailbox. DAT can also be used in conjunction with labels to declare variables. For example, DAT 984 will store the value 984 into a mailbox at the address of the DAT instruction.
