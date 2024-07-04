#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct Node
{
	CIN cin;
	struct Node *dir;
	struct Node *esq;
} Node;

typedef struct Estado
{
	Node *node;
	int sigla;
	struct Estado *dir;
	struct Estado *esq;
} Estado;

Node *novo_node(CIN cin);

Estado *novo_estado(int valor_sigla);

int valor_estado(const char *sigla);

Estado *busca_estado(Estado *estado, int valor_sigla, Estado *anterior);

Estado *popular_estados();

Node *busca_cin(Node *raiz, long registro, Node *anterior);

Node *busca_nome(Node *raiz, const char *nome, Node *anterior);

void inserir_cin(Node **node, CIN cin);

void inserir_nome(Estado *estados, CIN cin);

Node *maior_ValorEsq(Node *no);

long remover_cin(Node **raiz, Estado *estados, long registro);

void remover_cin_estado(Estado *estados, Node *no_removido);

void imprimir_cins_idade(Node *arvore, int anoInicial, int anoFinal);

void imprimir_cin(CIN cin);

void relatorio(Estado *estados, int anoInicial, int anoFinal);

void deleta_arvore(Node *arvore);

#endif