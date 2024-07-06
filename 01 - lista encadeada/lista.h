#ifndef LISTA_H
#define LISTA_H

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

void teste();

CIN *criar_no();

void inserir_no(CIN *lista, CIN *novo);

/*void inserir_lista_ordem_alfabetica(CIN *lista, CIN *novo);*/

CIN *busca_cin(CIN *lista, char registro[12]);

void imprimir_cin(CIN cin);

void relatorio(CIN *lista, int anoInicial, int anoFinal);

void deleta_lista(CIN *lista);

#endif