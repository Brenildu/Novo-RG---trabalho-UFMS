#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	char cidade[31];
	char estado[3];
	int rg;
}Naturalidade;

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

Node *new_node(CIN cin);

Node* inserir_cin(Node* node, CIN cin);

Node* maxValueNode(Node* no);

Node* remover_cin(Node* raiz, long registro);

Node *buscaRegistro(Node *binary_tree, long registro);

Node* procurar_cin(Node *rootAntigoPadrao, Node *rootNovoPadrao, const long *registro);

Node* buscaCIN(Node *rootAntigoPadrao, Node *rootNovoPadrao, long registro);

void imprimir_cin(Node *binary_tree);

void exclui_arvore(Node *binary_tree);

int quantidade_de_cin(Node *root);

Node* alterarRegistro(Node **rootAntigoPadrao, Node **rootNovoPadrao, const long *registro);

void relatorio_anos(Node *root, int anoInicial, int anoFinal);

void relatorio_intervaloAnos(Node *rootAntigoPadrao, Node *rootNovoPadrao, int anoInicial, int anoFinal);

void relatorio_estado(Node *root, char estado[3], char finalP);

void relatorio_porEstado(Node *rootAntigoPadrao, Node *rootNovoPadrao, char estado[3]);

#endif