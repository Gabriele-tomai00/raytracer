CC = gcc-15
CFLAGS = -O3 -Wall -pedantic -fopenmp

all: main

main: main.o scene.o ppm.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c scene.h ppm.h
	$(CC) $(CFLAGS) -c $<

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -c $<

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f main *.o

.PHONY: all clean