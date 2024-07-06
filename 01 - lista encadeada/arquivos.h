#ifndef ARQUIVOS_H
#define ARQUIVOS_H
#include "lista.h"

void teste1();

char *readFile(const char *filename);

CIN *parseJSON(const char *filename, CIN **lst);

CIN *carregarDadosTxt(const char *filename);

void salvarRelatorioTxt(CIN *lstPorEstado[], const char *filename);

void salvarDadosTxt(CIN *lst, const char *filename);

#endif