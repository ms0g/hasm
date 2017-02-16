OBJS = hasm.o parser.o symbol_table.o
TARGET = hasm
CC = gcc
MODE = -std=c99
DEBUG = -g
DEPS = parser.h symbol_table.h
CFLAGS =  -Wall -c $(DEBUG)
LFLAGS =  -Wall $(DEBUG)

%.o: %.c $(DEPS)
	$(CC) $< $(CFLAGS) $(MODE)

all: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $(TARGET) $(MODE)

clean:
	rm -f *.o hasm