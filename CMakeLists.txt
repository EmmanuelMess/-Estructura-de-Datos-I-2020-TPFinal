cmake_minimum_required(VERSION 3.16)
project(Estructura_de_Datos_I_2020_TpFinal C)

set(CMAKE_C_STANDARD 99)

add_executable(Estructura_de_Datos_I_2020_TpFinal main.c tests.c tests.h main.h avl/arbol_intervalos.c avl/arbol_intervalos.h avl/deque.c avl/deque.h trie/trie.c trie/trie.h trie/mapa.c trie/mapa.h parseador.c parseador.h avl/matematica.c avl/matematica.h)