#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "hash.h"

void carregarDados(No tabela[TAM], const char *filename);

void salvarDados(No tabela[], const char *filename);

void imprimir_relatorio_em_arquivo(Estado relatorio[], const char *filename);
#endif
