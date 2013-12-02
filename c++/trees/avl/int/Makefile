CCFLAGS = -I. -g -O0

all: force
	g++ -c $(CCFLAGS) tree.cc
	g++ -c $(CCFLAGS) test-tree.cc
	g++ test-tree.o tree.o
	valgrind ./a.out

leak: all
	valgrind --leak-check=full ./a.out 

clean: force
	$(RM) *.o a.out *~

force:
