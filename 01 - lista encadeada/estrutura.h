#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char cidade[31];
    char estado[3];
    char rg[10];
} Naturalidade;

typedef struct CIN
{
    char registro[12];
    char nome[70];
    int data[3];
    Naturalidade registros_emitidos[27];
    struct CIN *prox;
} CIN;

#endif