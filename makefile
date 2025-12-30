CC = gcc-15
TARGET = main
CFLAGS = -O3 -Wall -pedantic -fopenmp

all: $(TARGET)

$(TARGET): main.o scene.o ppm.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

main.o: main.c scene.h
	$(CC) $(CFLAGS) -c $<

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -c $<

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean