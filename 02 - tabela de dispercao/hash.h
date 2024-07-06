
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM 1297

typedef struct
{
	char cidade[31];
	char estado[3];
	int rg;
} Naturalidade;

typedef struct
{
	char nome[70];
	int data[3];
	long registro;
	Naturalidade registros_emetidos[27];
} CIN;

typedef struct No
{
	CIN cin;
	struct No *prox;
} No;

typedef struct Estado
{
	struct No tabela[17];
} Estado;

void popular_hash(No tabela[TAM]);

void popular_estados(Estado estados[], No tabela[]);

long valor_sequencia(long registro);

int obter_posicao_alfabetica(char letra);

int valor_estado(const char *sigla);

long funcao_hash(long registro);

No *criar_no(CIN cin);

Estado *criar_estado();

void inserir_ordenado(No *lista, No *novo);

void inserir_lista_ordem_alfabetica(No *lista, No *novo);

No *busca_cin(No tabela_cin[], long registro);

void insere_tabela(No *tabela[], No *novo);

void insere_estado(Estado *estados[], No *novo, const char *sigla);

void imprimir_cins_idade(No *lista, int anoInicial, int anoFinal);

void imprimir_cin(CIN cin);

void relatorio(Estado estados[], int anoInicial, int anoFinal);

void deleta_lista(No *lista);

void deleta_tabela(No tabela[]);

void deleta_estados(Estado estados[]);

#endif