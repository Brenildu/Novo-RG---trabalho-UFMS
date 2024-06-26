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
	Naturalidade Naturalidade[26];
	long registro;
	int registros_emetidos;

} CIN;

typedef struct Node
{
	CIN cin;
	struct Node *dir;
	struct Node *esq;
} Node;

Node *novoNode(CIN cin);

Node *inserir_cin(Node *node, CIN cin);

Node *maiorValorNode(Node *no);

Node *removerCin(Node *raiz, long registro);

Node *buscaRegistro(Node *arvore, long registro);

Node *procurarCin(Node *raizAntigoPadrao, Node *raizNovoPadrao, const long *registro);

Node *buscaCIN(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro);

void imprimirCin(Node *arvore);

void exclui_arvore(Node *arvore);

int quantidade_de_cin(Node *raiz);

Node *alterarRegistro(Node **raizAntigoPadrao, Node **raizNovoPadrao, const long *registro);

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal);

void relatorio_intervaloAnos(Node *raizAntigoPadrao, Node *raizNovoPadrao, int anoInicial, int anoFinal);

void relatorio_estado(Node *raiz, char estado[3], char finalP);

void relatorio_porEstado(Node *raizAntigoPadrao, Node *raizNovoPadrao, char estado[3]);

#endif