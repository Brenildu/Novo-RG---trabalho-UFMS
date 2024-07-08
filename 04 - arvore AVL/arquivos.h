#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "arvoreAVL.h"

void carregarDados(Node **arvore, const char *filename);

void salvarDados(Node *arvore, FILE *file);

void salvarDadosEmArquivo(Node *arvore, const char *filename);

void imprimir_estado(Estado *estado, FILE *file);

void imprimir_relatorio_em_arquivo(Estado *relatorio, const char *filename);
#endif
