
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM (long)181817

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
	Naturalidade Naturalidade[26];
	long registro;
	int registros_emetidos;

} CIN;

typedef struct No
{
	CIN cin;
	struct No *prox;
} No;

typedef struct Estado
{
	struct No tabela[TAM];
} Estado;

void popular_hash(No tabela[]);

void popular_estados(Estado estados[]);

long valor_sequencia(long registro);

long funcao_hash(long registro);

void insere_tabela(No lista_registro[], No *novo);

void inserir_no_inicio(No *lista, No *novo);

long remover_registro(No *lista, long registro);

No *criar_no(CIN cin);

void inserir_lista_ordem_alfabetica(No *lista, No *novo);

No *busca_registro(No lista_registro[], long registro);

void relatorio_intervaloAnos(No lista_registro[], int anoInicial, int anoFinal);

void relatorio_porEstado(No lista_registro[]);

void imprimir_registros(No *lista);

void * imprimir_registros_estado(No *lista, char estado);

void imprimir_tabela(No tabela[]);

void finaliza_lista(No lista);

void deleta_tabela(No tabela[]);

#endif