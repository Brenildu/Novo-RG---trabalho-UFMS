#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "hash.h"

char *readFile(const char *filename);

void parseDate(const char *str, int data[3]);

void addCIN(No *tabela[], CIN pessoa);

void parseJSON(const char *filename, No *tabela[]);

void salvarDadosTxt(No *tabela[], const char *filename);

void carregarDadosTxt(No *tabela[], const char *filename);

#endif
