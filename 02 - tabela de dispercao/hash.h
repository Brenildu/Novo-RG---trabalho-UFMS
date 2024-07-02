
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM (long)18181817

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

typedef struct no
{
	CIN cin;
	struct no *prox;
} No;

void popular_hash(No tabela[TAM]);

int valor_sequencia(int registro);

int funcao_hash(int registro);

void insere_tabela(No lista[], No *novo);

void inserir_no_inicio(No *lista, No *novo);

int remover_registro(No *lista, int registro);

No *criar_no(CIN cin);

void inserir_ordenado(No *lista, No *novo);

No *busca_registroAntigos(No *listaAntigoPadrao, int registro);

No *busca_registroNovos(No *listaNovoPadrao, int registro);

No *busca_registro(No lista_registro[], int registro);

No *alterarRegistro(No listaAntigoPadrao[], No listaNovoPadrao[], const char *placa);

void relatorio_intervaloAnos(No listaAntigoPadrao[], No listaNovoPadrao[], int anoInicial, int anoFinal);

void relatorio_porEstado(No listaAntigoPadrao[], No listaNovoPadrao[]);

void imprimir_registros(No *lista);

No *buscaRegistro(No listaAntigoPadrao[], No listaNovoPadrao[], const int *registro);

void imprimir_tabela(No tabela[]);

void finaliza_lista(No lista);

void deleta_tabela(No tabela[TAM]);

#endif