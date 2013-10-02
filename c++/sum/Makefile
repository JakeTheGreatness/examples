CCFLAGS = -I. -g -O3

all: force
	g++ $(CCFLAGS) sum.cc
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
