

all: gamewindow.exe

gamewindow.exe:  breakout.cs
	dmcs -pkg:gtk-sharp-2.0 -r:Mono.Cairo gamewindow.cs breakout.cs

run: gamewindow.exe
	mono gamewindow.exe

clean: force
	$(RM) *.exe *.mdb *~

force:
