#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_ALFABETO 26

typedef struct {
    char cidade[31];
    char estado[3];
    int rg;
} Naturalidade;

typedef struct {
    char nome[70];
    int data[3];
    Naturalidade naturalidade;
    long registro;
    int registros_emetidos;
} CIN;

typedef struct NodeTrie {
    struct NodeTrie *filhos[TAMANHO_ALFABETO];
    int fimPalavra;
    CIN *cin;
} NodeTrie;

NodeTrie* criarNodeTrie();
void inserirTrie(NodeTrie *raiz, CIN cin);
CIN* buscarTrie(NodeTrie *raiz, const char *nome);
void imprimirTrie(NodeTrie *raiz, char *prefix, int nivel);
void deletarTrie(NodeTrie *raiz);

#endif