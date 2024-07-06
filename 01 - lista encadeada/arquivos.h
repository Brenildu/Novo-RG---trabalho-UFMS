#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "lista.h"

void carregarDados(CIN **lista, const char *filename);

void salvarDados(CIN *lista, const char *filename);

void imprimir_relatorio_em_arquivo(CIN *relatorio, const char *filename);

#endif
