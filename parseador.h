#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

typedef struct {
  int inicio;
  int final;
} Rango;

typedef struct {
  int error;
  int largoAlias;
  bool esExtension;
  int largo;
} Metadatos;

const Metadatos ERROR = {.error = 1};

void remover_espacios(char* entrada);
Metadatos chequeador(char * entrada);
bool procesar(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
