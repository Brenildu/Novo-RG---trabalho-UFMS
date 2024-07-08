#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include "estrutura.h"

Node *criar_node(CIN cin);

void insere_arvore(Node **arvore, CIN cin);

Node *busca_cin(Node *arvore, char registro[12]);

void imprimir_cin(CIN cin);

Estado *novo_estado(const char *sigla);

Estado *inserir_estado(Estado *raiz, const char *sigla);

void criar_arvore_estados(Estado **raiz, int inicio, int fim);

void inserir_arvore_ordem_alfabetica(Node **arvore, CIN pessoa);

Estado *busca_estado(Estado *estado, char *sigla);

void insere_node_estado(Estado *estados, CIN pessoa);

void gerar_relatorio(Estado *estados, Node *arvore, int anoInicial, int anoFinal);

void deleta_naturalidade(Naturalidade *nat);

void deleta_arvore(Node *arvore);

void deleta_estados(Estado *estados);

#endif