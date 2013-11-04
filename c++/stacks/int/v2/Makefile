CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) stack.cc
	g++ -c $(CCFLAGS) test-stack.cc
	g++ test-stack.o stack.o
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
