CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = calculator

all: $(TARGET)

$(TARGET): main.o calc.o
	$(CC) $(CFLAGS) main.o calc.o -o $(TARGET)

main.o: main.c calc.h
	$(CC) $(CFLAGS) -c main.c

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c

clean:
	rm -f *.o $(TARGET)