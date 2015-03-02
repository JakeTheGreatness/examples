CCFLAGS = -I. -g


all: sfract test-sin # sfract-gc test-sin-gc

sfract: force
	@echo "test sfract values"
	g++ $(CCFLAGS) -O0 sfract.cc fract.cc test-sfract.cc gc.cc
	@echo "Running without Garbage collection enabled"
	valgrind ./a.out

sfract-gc: force
	@echo "test sfract values"
	g++ $(CCFLAGS) -DUSE_GC -O0 sfract.cc fract.cc test-sfract.cc gc.cc
	@echo "Running with Garbage collection enabled"
	valgrind ./a.out

test-sin: force
	@echo "test sin values"
	g++ $(CCFLAGS) -O0 sfract.cc fract.cc test-sin.cc gc.cc
	@echo "Running without Garbage collection enabled"
	valgrind ./a.out

test-sin-gc: force
	@echo "test sin values"
	g++ $(CCFLAGS) -DUSE_GC -O0 sfract.cc fract.cc test-sin.cc gc.cc
	@echo "Running with Garbage collection enabled"
	valgrind ./a.out

fract: force
	g++ $(CCFLAGS) -O0 fract.cc test-fract.cc
	valgrind ./a.out

opt: force
	g++ $(CCFLAGS) -O3 fract.cc test-fract.cc
	valgrind ./a.out

clean: force
	$(RM) *.o a.out *~

decrypt: force
	@if [ -f gc.cc.sol.gpg ] ; then \
	   rm gc.cc.sol ; \
	   gpg -d gc.cc.sol.gpg > gc.cc.sol ; \
        else \
           echo "no gc.cc.sol.gpg file found" ; \
        fi

encrypt: force
	@if [ -f gc.cc.sol ] ; then \
           rm gc.cc.sol.gpg ; \
	   if gpg -c gc.cc.sol ; then \
              rm gc.cc.sol ; \
           else \
              echo "warning encrypt failed" ; \
           fi ; \
        else \
           echo "no gc.cc.sol file found" ; \
        fi

force:
