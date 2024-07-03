#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
	Naturalidade Naturalidade;
	long registro;
	Naturalidade registros_emetidos[26];

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

void inserir_cin(Node **node, CIN cin);

void insere_nome(Node **node ,CIN cin);

Node* maior_ValorEsq(Node* no);

Node* remover_cin(Node* raiz, long registro);

Node *busca_registro(Node *binary_tree, long registro);

Node* procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, const long *registro);

Node* busca_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro);

void imprimir_cins(Node *binary_tree);

void exclui_arvore(Node *binary_tree);

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal);

void relatorio_intervaloAnos(Node *raizAntigoPadrao, Node *raizNovoPadrao, int anoInicial, int anoFinal);

void relatorio_porEstado(Node *raizAntigoPadrao, Node *raizNovoPadrao, char estado[3]);

#endif