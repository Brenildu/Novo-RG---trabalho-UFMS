#include "arvoreDigital.h"

NodeTrie* criarNodeTrie() {
    NodeTrie *no = (NodeTrie *)malloc(sizeof(NodeTrie));
    no->fimPalavra = 0;
    no->cin = NULL;
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

void inserirTrie(NodeTrie *raiz, CIN cin) {
    NodeTrie *no = raiz;
    for (int i = 0; i < strlen(cin.nome); i++) {
        int indice = tolower(cin.nome[i]) - 'a';
        if (!no->filhos[indice]) {
            no->filhos[indice] = criarNodeTrie();
        }
        no = no->filhos[indice];
    }
    no->fimPalavra = 1;
    no->cin = (CIN *)malloc(sizeof(CIN));
    memcpy(no->cin, &cin, sizeof(CIN));
}

CIN* buscarTrie(NodeTrie *raiz, const char *nome) {
    NodeTrie *no = raiz;
    for (int i = 0; i < strlen(nome); i++) {
        int indice = tolower(nome[i]) - 'a';
        if (!no->filhos[indice]) {
            return NULL;
        }
        no = no->filhos[indice];
    }
    if (no != NULL && no->fimPalavra) {
        return no->cin;
    }
    return NULL;
}

void imprimirTrie(NodeTrie *raiz, char *prefixo, int nivel) {
    if (raiz->fimPalavra) {
        printf("%s;%s;%s;%d;%d/%d/%d;%ld\n",
               prefixo,
               raiz->cin->naturalidade.cidade,
               raiz->cin->naturalidade.estado,
               raiz->cin->naturalidade.rg,
               raiz->cin->data[0],
               raiz->cin->data[1],
               raiz->cin->data[2],
               raiz->cin->registro);
    }
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filhos[i]) {
            prefixo[nivel] = 'a' + i;
            prefixo[nivel + 1] = '\0';
            imprimirTrie(raiz->filhos[i], prefixo, nivel + 1);
        }
    }
}

void deletarTrie(NodeTrie *raiz) {
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filhos[i]) {
            deletarTrie(raiz->filhos[i]);
        }
    }
    if (raiz->cin) {
        free(raiz->cin);
    }
    free(raiz);
}
