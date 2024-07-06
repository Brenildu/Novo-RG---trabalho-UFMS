#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


CIN *criar_no(CIN cin)
{
    CIN *novo = (CIN *)malloc(sizeof(CIN));
    if (!novo)
    {
        printf("Falha ao alocar memória.\n");
        return NULL;
    }
    *novo = cin;
    novo->prox = NULL;
    return novo;
}

void inserir_no(CIN **lista, CIN pessoa)
{
    CIN *novoCIN = criar_no(pessoa);
    if (novoCIN)
    {
        novoCIN->prox = *lista;
        *lista = novoCIN;
    }
}

CIN *busca_cin(CIN *lista, char registro[12])
{
    CIN *p = lista;
    while (p && strcmp(p->registro, registro) != 0)
    {
        p = p->prox;
    }
    return p;
}

void imprimir_cin(CIN cin)
{
    printf("\"nome\": \"%s\",\n\"cpf\": \"%s\",\n\"rg\": \"%s\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
           cin.nome,
           cin.registro,
           cin.registros_emitidos[0].rg,
           cin.data[0],
           cin.data[1],
           cin.data[2],
           cin.registros_emitidos[0].cidade,
           cin.registros_emitidos[0].estado);
}

void insere_ordenado(CIN **lista, CIN pessoa)
{
    CIN *novo = criar_no(pessoa);
    if (!novo)
    {
        return;
    }

    CIN *p = *lista;
    CIN *q = NULL;

    // Encontra o ponto de inserção baseado no estado
    while (p != NULL && strcmp(p->registros_emitidos[0].estado, pessoa.registros_emitidos[0].estado) < 0)
    {
        q = p;
        p = p->prox;
    }

    // Se encontrou um estado igual, insere ordenado por nome
    if (p != NULL && strcmp(p->registros_emitidos[0].estado, pessoa.registros_emitidos[0].estado) == 0)
    {
        while (p != NULL && strcmp(p->registros_emitidos[0].estado, pessoa.registros_emitidos[0].estado) == 0 && strcmp(p->nome, pessoa.nome) < 0)
        {
            q = p;
            p = p->prox;
        }
    }

    // Insere o novo nó na posição correta
    if (q == NULL)
    {
        // Inserir no início da lista
        novo->prox = *lista;
        *lista = novo;
    }
    else
    {
        // Inserir após q
        q->prox = novo;
        novo->prox = p;
    }
}


CIN *gerar_relatorio(CIN *lista, int anoInicial, int anoFinal)
{
    CIN *relat = NULL;
    CIN *p = lista;
    while (p)
    {
        if (p->data[2] >= anoInicial && p->data[2] <= anoFinal)
        {
            insere_ordenado(&relat, *p);
        }
        p = p->prox;
    }
    return relat;
}

void imprimir_relatorio(CIN *relatorio)
{
    CIN *p = relatorio;
    while (p)
    {
        imprimir_cin(*p);
        p = p->prox;
    }
}

void deleta_lista(CIN *lista)
{
    CIN *tmp;
    while (lista != NULL)
    {
        tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}
