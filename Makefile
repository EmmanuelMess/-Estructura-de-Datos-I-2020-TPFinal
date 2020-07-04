CC = gcc
CFLAGS = -g -Wall -Wpedantic

.PHONY: default_target all clean

default_target: ArbolAvl
all: default_target

OBJECTS_ARBOLAVL = $(patsubst %.c, compilados/.obj/%.o, $(wildcard *.c))
HEADERS_ARBOLAVL = $(wildcard *.h)

compilados/.obj/%.o: %.c $(HEADERS_ARBOLAVL)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: ArbolAvl $(OBJECTS_ARBOLAVL)

ArbolAvl: compilados/.obj $(OBJECTS_ARBOLAVL)
	$(CC) $(OBJECTS_ARBOLAVL) $(CFLAGS) -o compilados/$@

compilados/.obj:
	mkdir -p $@

clean:
	-rm -rf compilados