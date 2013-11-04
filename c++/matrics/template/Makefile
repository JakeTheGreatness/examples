CCFLAGS = -I. -g -O0

all: test-matrix run

test-matrix: test-matrix.o fract.o sfract.o
	@echo "fract list tests"
	g++ test-matrix.o fract.o sfract.o

run: test-matrix
	valgrind ./a.out

%.o: %.cc
	g++ -c $(CCFLAGS) -I../../fractions -I. $<

fract.o: ../../fractions/fract.cc ../../fractions/fract.h
	g++ -c $(CCFLAGS) -I../../fractions ../../fractions/fract.cc

sfract.o: ../../fractions/sfract.cc ../../fractions/sfract.h \
          ../../fractions/fract.cc ../../fractions/fract.h
	g++ -c $(CCFLAGS) -I../../fractions ../../fractions/sfract.cc

clean: force
	$(RM) *.o a.out *~

force:
