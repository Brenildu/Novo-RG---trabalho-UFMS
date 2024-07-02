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

short fatorDeBalanceamento(Node *node);

Node *rotacaoSE(Node *node);

Node *rotacaoSD(Node *ramo);

Node *rotacaoDD(Node *ramo);

Node *rotacaoDE(Node *ramo);

Node *balancear(Node *raiz);

Node *novoNode(CIN cin);

Node *inserir_CIN(Node *node, CIN cin);

Node *maiorValorNode(Node *no);

Node *removerCIN(Node *raiz, long registro);

Node *buscaRegistro(Node *arvore, long registro);

Node *procurar_CIN(Node *raizAntigoPadrao, Node *raizNovoPadrao, const char *placa);

Node *buscaCIN(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro);

void imprimirCINs(Node *arvore);

void deleta_arvore(Node *arvore);

int quantidade_de_cin(Node *raiz);

Node *alterarRegistro(Node **raizAntigoPadrao, Node **raizNovoPadrao, const long *registro);

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal);

void relatorio_intervaloAnos(Node *raizAntigoPadrao, Node *raizNovoPadrao, int anoInicial, int anoFinal);

void relatorio_porEstado(Node listaAntigoPadrao[], Node listaNovoPadrao[]);

#endif