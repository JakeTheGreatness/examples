
all: pacman.exe
	mono pacman.exe

pacman.exe: pacman.cs
	dmcs -debug pacman.cs

clean: force
	$(RM) *.exe

force:
