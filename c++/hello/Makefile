CCFLAGS = -I. -g -O0

all: force
	g++ $(CCFLAGS) hello.cc
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

force:
