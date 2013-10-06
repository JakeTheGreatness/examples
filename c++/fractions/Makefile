CCFLAGS = -I. -g

all: force
	g++ $(CCFLAGS) -O0 fract.cc test-fract.cc
	valgrind ./a.out
	g++ $(CCFLAGS) -O3 fract.cc test-fract.cc
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
