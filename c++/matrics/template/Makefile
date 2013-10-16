CCFLAGS = -I. -g -O0

all: test-matrix


test-matrix: test-matrix.o fract.o
	@echo "fract list tests"
	g++ test-matrix.o fract.o

run: test-matrix
	valgrind ./a.out

%.o: %.cc
	g++ -c $(CCFLAGS) -I../../fractions -I. $<

fract.o: ../../fractions/fract.cc ../../fractions/fract.h
	g++ -c $(CCFLAGS) -I../../fractions ../../fractions/fract.cc

clean: force
	$(RM) *.o a.out *~

force:
