program: display.o program.o lib3d.a
	gcc -l3d display.o program.o -o program

display.o: display.c display.h
	gcc -c display.c -o display.o

program.o: program.c
	gcc -c program.c -o program.o
