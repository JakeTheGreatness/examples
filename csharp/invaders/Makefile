

all: invaders.exe

invaders.exe:  invaders.cs
	dmcs -pkg:gtk-sharp-2.0 -r:Mono.Cairo $<

run: invaders.exe
	mono invaders.exe

clean: force
	$(RM) *.exe *.mdb *~

force:
