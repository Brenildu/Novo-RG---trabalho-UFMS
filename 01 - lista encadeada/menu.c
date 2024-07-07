#include "menu.h"

int menuPrincipal()
{
    int opcao = -1;
    printf("\n\nMenu principal:\n-----------------------\n");
    printf("1 - Buscar por Numero de Cadastro\n");
    printf("2 - Gerar Relatorio\n");
    printf("3 - Sair\n----------------------\n");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 3)
    {
        opcao = -1;
    }
    return opcao;
}

void menu1(char *cpf)
{
    printf("\n-------------------\nBusca por cpf cadastrados\n-------------------\n");
    printf("\nDigite o cpf(apenas numeros) que deseja buscar: ");
    scanf("%s", cpf);
}

void menu2(int *anoInicio, int *anoFinal)
{
    printf("\n-------------------\nRelatorio de faixa idade por estados\n-------------------\n");

    printf("\nDigite o ano de inicio: ");
    scanf("%d", anoInicio);

    printf("\nDigite o ano de fim: ");
    scanf("%d", anoFinal);
}
