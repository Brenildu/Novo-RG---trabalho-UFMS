
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM 17476

typedef struct{
	char nome[70];
	int data[3];
	char naturalidade[30];
	int registro;
	int registros_emetidos;
	int estado[27];

}CIN;

typedef struct no{
	Veiculo veiculo;
	struct no *proximo;
}No;

void inicializar_tabela(No tabela[TAM]);

int valor_caractere(char c);

int funcao_hash(char placa[]);

void insere_tabela(No lista[], No *novo);

No* criar_no(Veiculo v);

void inserir_no_inicio(No *lista, No *novo);

No* remover_veiculo(No *lista, char placa[]);

int comparaCategoria(char categoriaAnt[]);

void inserir_ordenado(No *lista, No *novo);

No *buscarPlacaAntiga(No *listaAntigoPadrao, char placa[]);

No *buscarPlacaNova(No *listaNovoPadrao, char placa[]);

No* buscaVeiculo(No listaAntigoPadrao[], No listaNovoPadrao[], char placa[]);

No* alterarPlaca(No listaAntigoPadrao[], No listaNovoPadrao[], const char *placa);

void relatorio_intervaloAnos(No listaAntigoPadrao[], No listaNovoPadrao[], int anoInicial, int anoFinal);

void relatorio_porEstado(No listaAntigoPadrao[], No listaNovoPadrao[], char estado[3]);

void imprimir_veiculos(No *lista);

No* buscaPlaca(No listaAntigoPadrao[], No listaNovoPadrao[], const char *placa);

void imprimir_tabela(No tabela[]);

void libera_lista(No lista);

void deleta_tabela(No tabela[TAM]);

#endif