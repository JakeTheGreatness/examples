

all: paint.exe

paint.exe:  paint.cs
	dmcs -pkg:gtk-sharp-2.0 -r:Mono.Cairo paint.cs

run: paint.exe
	mono $<

clean: force
	$(RM) *.exe

force:

