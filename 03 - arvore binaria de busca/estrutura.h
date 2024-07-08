#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
} CIN;

typedef struct Node
{
    CIN cin;
    struct Node *dir;
    struct Node *esq;
} Node;

typedef struct Estado
{
    Node *node;
    char sigla[3];
    struct Estado *dir;
    struct Estado *esq;
} Estado;

#endif