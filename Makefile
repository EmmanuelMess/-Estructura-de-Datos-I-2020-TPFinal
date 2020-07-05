CC = gcc
CFLAGS = -g -Wall -Wpedantic -O3

.PHONY: default_target all clean

default_target: Interprete
all: default_target

OBJECTS_AVL = $(patsubst %.c, compilados/.obj/%.o, $(wildcard avl/*.c))
HEADERS_AVL = $(wildcard avl/*.h)

OBJECTS_TRIE = $(patsubst %.c, compilados/.obj/%.o, $(wildcard trie/*.c))
HEADERS_TRIE = $(wildcard trie/*.h)

OBJECTS_INTERPRETE = $(patsubst %.c, compilados/.obj/%.o, $(wildcard *.c))
HEADERS_INTERPRETE = $(wildcard *.h)

compilados/.obj/%.o: %.c $(HEADERS_AVL) $(HEADERS_TRIE) $(HEADERS_INTERPRETE)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: Interprete $(OBJECTS_AVL) $(OBJECTS_TRIE) $(OBJECTS_INTERPRETE)

Interprete: compilados compilados/.obj compilados/.obj/avl compilados/.obj/trie $(OBJECTS_AVL) $(OBJECTS_TRIE) $(OBJECTS_INTERPRETE)
	$(CC) $(OBJECTS_AVL) $(OBJECTS_TRIE) $(OBJECTS_INTERPRETE) $(CFLAGS) -o compilados/$@

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