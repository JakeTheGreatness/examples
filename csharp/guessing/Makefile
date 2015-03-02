
all: guessing.exe
	mono guessing.exe

guessing.exe: guessing.cs
	dmcs guessing.cs

clean: force
	$(RM) *.exe

force: