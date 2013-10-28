CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) dlist.cc
	g++ -c $(CCFLAGS) test-dlist.cc
	g++ test-dlist.o dlist.o
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
