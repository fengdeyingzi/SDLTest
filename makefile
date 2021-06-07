CC=gcc.exe
SDLDIR=D:\app\mingw64\bin\
CFLAGS=-I $(SDLDIR)\include\SDL2 -L $(SDLDIR)\lib -lmingw32 -lSDL2main -lSDL2 -O2 -mwindows

main.exe: main.c
	$(CC) main.c $(CFLAGS) -o main.exe
