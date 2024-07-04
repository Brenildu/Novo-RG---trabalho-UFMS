#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int menu()
{
    int opcao = -1;
    printf("Olá, seja bem-vindo ao Cadastro Nova Carteira de Identidade Nacional\n\nPor favor, selecione a operação desejada:\n-----------------------\n");
    printf("1 - Buscar por Número de Cadastro\n");
    printf("2 - Gerar Relatório\n");
    printf("3 - Sair\n----------------------\n");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 5)
    {
        opcao = -1;
    }
    return opcao;
}

long menu1()
{
    long cpf;
    printf("Busca por cpf cadastrados\n-------------------\n");
    printf("\nDigite o cpf(apenas numeros) que deseja buscar: ");
    scanf("%ld", &cpf);
    if (cpf < 10000000000 || cpf > 99999999999)
    {
        cpf = -1;
    }

    return cpf;
}


void menu2(int *anoInicio, int *anoFinal)
{
    printf("Relatorio de faixa idade por estados\n-------------------\n");

    printf("\nDigite o ano de inicio: ");
    scanf("%d", anoInicio);

    printf("\nDigite o ano de fim: ");
    scanf("%d", anoFinal);
}
