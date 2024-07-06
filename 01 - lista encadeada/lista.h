#ifndef LISTA_H
#define LISTA_H

#include "estrutura.h"

CIN *criar_no(CIN pessoa);

void inserir_no(CIN **lista, CIN pessoa);

CIN *busca_cin(CIN *lista, char registro[12]);

void imprimir_cin(CIN cin);

void insere_ordenado(CIN **lista, CIN pessoa);

CIN *gerar_relatorio(CIN *lista, int anoInicial, int anoFinal);

void imprimir_relatorio(CIN *relatorio);

void deleta_lista(CIN *lista);

#endif