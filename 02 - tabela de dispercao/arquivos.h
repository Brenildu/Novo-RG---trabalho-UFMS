#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "hash.h"
#include <json-c/json.h>

char *readFile(const char *filename);

void parseDate(const char *str, int data[3]);

void addCIN(CIN **lst, CIN pessoa);

CIN *parseJSON(const char *filename, CIN *lst);

void salvarDadosTxt(CIN *lst, const char *filename);

CIN *carregarDadosTxt(const char *filename);

#endif
