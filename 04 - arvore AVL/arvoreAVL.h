#ifndef ARVOREAVL_H
#define ARVOREAVL_H

#include "estrutura.h"

Node *criar_node(CIN cin);

int altura(Node *N);

int getBalance(Node *N);

Node *rotacao_direita(Node *y);

Node *rotacao_esquerda(Node *x);

Node *atualiza_balanceamento(Node *node);

Node *insere_avl(Node *node, CIN cin, Node *ancestral);

Node *busca_cin(Node *raiz, char registro[12]);

Node *busca_cin(Node *arvore, char registro[12]);

void imprimir_cin(CIN cin);

Estado *novo_estado(const char *sigla);

Estado *inserir_estado(Estado *raiz, const char *sigla);

void criar_arvore_estados(Estado **raiz, int inicio, int fim);

Node *inserir_arvore_ordem_alfabetica(Node *arvore, CIN pessoa, Node *ancestral);

Estado *busca_estado(Estado *raiz, const char *sigla);

void insere_node_estado(Estado *estados, CIN pessoa);

void gerar_relatorio(Estado *estados, Node *arvore, int anoInicial, int anoFinal);

void deleta_naturalidade(Naturalidade *nat);

void deleta_arvore(Node *arvore);

void deleta_estados(Estado *estados);

int max(int a, int b);

#endif