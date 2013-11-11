CCFLAGS = -I. -g


all: sfract

sfract: force
	g++ $(CCFLAGS) -O0 sfract.cc fract.cc test-sfract.cc gc.cc
	valgrind ./a.out

fract: force
	g++ $(CCFLAGS) -O0 fract.cc test-fract.cc
	valgrind ./a.out

opt: force
	g++ $(CCFLAGS) -O3 fract.cc test-fract.cc
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
