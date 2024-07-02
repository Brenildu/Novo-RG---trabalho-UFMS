#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

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
	int altura;
} Node;

int maior(int a, int b);

int obter_altura(Node *N);

short fatorDeBalanceamento(Node *node);

Node *rotacaoSE(Node *node);

Node *rotacaoSD(Node *ramo);

Node *rotacaoDD(Node *ramo);

Node *rotacaoDE(Node *ramo);

Node *balancear(Node *raiz);

Node *novo_Node(CIN cin);

Node *inserir_cin(Node *node, CIN cin);

Node *maior_ValorNode(Node *no);

Node *remover_cin(Node *raiz, long registro);

Node *busca_registro(Node *arvore, long registro);

Node *procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro);

void imprimir_cins(Node *arvore);

int calcular_idade(int anoNascimento, int anoAtual);

void relatorio_faixaEtaria(Node *raiz, int idadeInicial, int idadeFinal, int anoAtual, Node **pessoas);

void agruparPorEstado(Node *raiz, Node **pessoas);

void relatorio_completo(Node *raiz, int idadeInicial, int idadeFinal, int anoAtual);

void deleta_arvore(Node *arvore);

#endif