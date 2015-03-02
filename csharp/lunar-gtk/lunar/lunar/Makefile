
all: Main.exe

Main.exe:  Main.cs
	dmcs -pkg:gtk-sharp-2.0 -r:Mono.Cairo -resource:eagle.png $<

run: Main.exe
	mono Main.exe

clean: force
	$(RM) *.exe

force:

