


#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

void leituraERGUF(No listaAntigoPadrao[], No listaNovoPadrao[], char *arquivo);


No *inverterLista(No **listaOrdenada);

void gravarERGUF(const char *filename, No *lst);

#endif