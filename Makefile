CC = gcc
CFLAGS = -g -Wall -Wpedantic

.PHONY: default_target all clean

default_target: Interprete
all: default_target

OBJECTS_ORDENADOR = $(patsubst %.c, compilados/.obj/%.o, $(wildcard avl/*.c))
HEADERS_ORDENADOR = $(wildcard avl/*.h)

OBJECTS_SELECTOR = $(patsubst %.c, compilados/.obj/%.o, $(wildcard trie/*.c))
HEADERS_SELECTOR = $(wildcard trie/*.h)

OBJECTS_INTERPRETE = $(patsubst %.c, compilados/.obj/%.o, $(wildcard *.c))
HEADERS_INTERPRETE = $(wildcard *.h)

compilados/.obj/%.o: %.c $(HEADERS_ORDENADOR) $(HEADERS_SELECTOR) $(HEADERS_INTERPRETE)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: Interprete $(OBJECTS_ORDENADOR) $(OBJECTS_SELECTOR) $(OBJECTS_INTERPRETE)

Interprete: compilados compilados/.obj compilados/.obj/avl compilados/.obj/trie $(OBJECTS_ORDENADOR) $(OBJECTS_SELECTOR) $(OBJECTS_INTERPRETE)
	$(CC) $(OBJECTS_ORDENADOR) $(OBJECTS_SELECTOR) $(OBJECTS_INTERPRETE) $(CFLAGS) -o compilados/$@

compilados:
	mkdir -p $@

compilados/.obj/avl:
	mkdir -p $@

compilados/.obj/trie:
	mkdir -p $@

compilados/.obj:
	mkdir -p $@

clean:
	-rm -rf compilados