CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) test-slist.cc
	g++ test-slist.o
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
