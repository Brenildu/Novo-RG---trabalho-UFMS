#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM 1257
#define TAM_ESTADO 27
#define TAM_ALFABETO 14

typedef struct Naturalidade
{
    char cidade[31];
    char estado[3];
    char rg[10];
    struct Naturalidade *prox;

} Naturalidade;

typedef struct CIN
{
    char registro[12];
    char nome[70];
    int data[3];
    Naturalidade *registros_emitidos;
    struct CIN *prox;
} CIN;

typedef struct No
{
    CIN cin;
    struct No *prox;
} No;

typedef struct Estado
{
    struct No tabela[TAM_ALFABETO];
} Estado;

#endif