#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "arvoreBinaria.h"
#include <json-c/json.h>

char *readFile(const char *filename);

void parseDate(const char *str, int data[3]);

void addCIN(Estado *estados, Node **cpfTree, CIN pessoa);

void parseJSON(const char *filename, Estado *estados, Node **cpfTree);

void salvarDadosTxt(Node *cpfTree, Estado *estados, const char *filename);

void carregarDadosTxt(Node **cpfTree, Estado *estados, const char *filename);

#endif
