CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = phonebook

all: $(TARGET)

$(TARGET): main.o phonebook.o
	$(CC) $(CFLAGS) main.o phonebook.o -o $(TARGET)

main.o: main.c phonebook.h
	$(CC) $(CFLAGS) -c main.c

phonebook.o: phonebook.c phonebook.h
	$(CC) $(CFLAGS) -c phonebook.c

clean:
	rm -f *.o $(TARGET)