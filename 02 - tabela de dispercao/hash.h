
#ifndef HASH_H
#define HASH_H

#include "estrutura.h"

void popular_hash(No tabela[], int tamanho);

int valor_sequencia(char registro[12]);

int funcao_hash(char registro[12]);

No *criar_no(CIN cin);

void inserir_no(No **lista, CIN pessoa);

void insere_tabela(No tabela[], CIN pessoa);

No *busca_cin(No tabela[TAM], char registro[12]);

void imprimir_cin(CIN cin);

void popular_estados(Estado estados[]);

int obter_posicao_alfabetica(char inicial);

int valor_estado(const char *sigla);

void inserir_lista_ordem_alfabetica(No **lista, CIN pessoa);

void insere_estado(Estado estados[], CIN pessoa, const char *sigla);

void gerar_relatorio(Estado estados[], No tabela[], int anoInicial, int anoFinal);

void deleta_naturalidade(Naturalidade **nat);

void deleta_lista(No **lista);

void deleta_tabela(No tabela[]);

void deleta_estados(Estado estados[]);

#endif