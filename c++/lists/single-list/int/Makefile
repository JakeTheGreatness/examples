CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) slist.cc
	g++ -c $(CCFLAGS) test-slist.cc
	g++ test-slist.o slist.o
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
