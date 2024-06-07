#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


void carregamentoERNV(No **listaAntigoPadrao, No **listaNovoPadrao, char *arquivo);

void carregamentoBinario(No **listaAntigoPadrao, No **listaNovoPadrao);

void carregamento_arquivo(int argc, char *argv[], No **listaAntigoPadrao, No **listaNovoPadrao);

No *inverterLista(No **listaOrdenada);

void gravacaoBinario(No **listaAntigoPadrao, No **listaNovoPadrao);




#endif