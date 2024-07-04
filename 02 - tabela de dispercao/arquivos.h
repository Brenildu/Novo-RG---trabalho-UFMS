#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "hash.h"
#include <json-c/json.h>

char *readFile(const char *filename);

void parseDate(const char *str, int data[3]);

void addCIN(CIN **lst, CIN pessoa);

void addCIN_estados(Estado estados[], CIN pessoa);

void parseJSON(const char *filename, No tabela[], Estado estados[]);

void salvarDadosTxt(No tabela[], const char *filename);

void carregarDadosTxt(No tabela[], Estado estados[], const char *filename);

#endif
