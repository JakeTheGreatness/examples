CCFLAGS = -I. -g -O0

all: test-slist test-fract


test-fract: test-flist.o fract.o
	@echo "fract list tests"
	g++ test-flist.o fract.o
	valgrind ./a.out

%.o: %.cc
	g++ -c $(CCFLAGS) -I../../../fractions $<

fract.o: ../../../fractions/fract.cc ../../../fractions/fract.h
	g++ -c $(CCFLAGS) -I../../../fractions ../../../fractions/fract.cc

test-slist: force
	@echo "integer list tests"
	g++ -c $(CCFLAGS) test-slist.cc
	g++ test-slist.o
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
