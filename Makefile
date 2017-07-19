OBJS = hasm.o parser.o symbol_table.o hdr.o
TARGET = hasm
HVM_TARGET = hvm
HVM_OBJ = hvm.o
CC = gcc
MODE = -std=c99
DEBUG = -g
DEPS = parser.h symbol_table.h hdr.h hvm.h
CFLAGS =  -Wall -c $(DEBUG)
LFLAGS =  -Wall $(DEBUG)

%.o: %.c $(DEPS)
	$(CC) $< $(CFLAGS) $(MODE)

all: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $(TARGET) $(MODE)

hvm: $(HVM_OBJ)
	$(CC) $^ $(LFLAGS) -o $(HVM_TARGET) $(MODE)

.PHONY: clean
clean:
	rm -f  *.o hasm hvm core asms/*.hex asms/*.int
