CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) slist.cc
	g++ -c $(CCFLAGS) test-slist.cc
	g++ test-slist.o slist.o
	valgrind ./a.out

test-slist.S: test-slist.cc
	g++ -O3 -I. -S -fverbose-asm -g $< -o test-slist.s
	as -alhnd test-slist.s > test-slist.S

clean: force
	$(RM) *.o a.out *~

force:
