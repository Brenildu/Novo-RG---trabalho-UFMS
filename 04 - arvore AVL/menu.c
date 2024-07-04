#include "menu.h"


int menu()
{
    int opcao = -1;
    printf("Olá, seja bem-vindo ao Cadastro Nova Carteira de Identidade Nacional\n\nPor favor, selecione a operação desejada:\n-----------------------\n");
    printf("1 - Buscar por Número de Cadastro\n");
    printf("2 - Adicionar um novo cpf\n");
    printf("3 - Remover um cpf\n");
    printf("4 - Gerar Relatório\n");
    printf("5 - Sair\n----------------------\n");
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

CIN menu2()
{
    CIN novo = {0}, vazio = {0};
    printf("Cadastro de CIN\n-------------------\n");
    printf("Digite a data de nascimento completa (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &novo.data[0], &novo.data[1], &novo.data[2]);

    printf("\nDigite o nome completo: ");
    scanf(" %[^\n]", novo.nome);

    printf("\nDigite o cpf (apenas numeros): ");
    scanf("%ld", &novo.registro);

    if (novo.registro < 10000000000 || novo.registro > 99999999999)
    {
        return vazio;
    }

    printf("\nDigite a cidade de naturalidade: ");
    scanf(" %[^\n]", novo.registros_emetidos[0].cidade);

    printf("\nDigite o estado de naturalidade: ");
    scanf(" %[^\n]", novo.registros_emetidos[0].estado);

    printf("\nDigite a rg (apenas numeros): ");
    scanf("%d", &novo.registros_emetidos[0].rg);

    return novo;
}

long menu3()
{
    long cpf;
    printf("Remoção de cpf cadastrados\n-------------------\n");
    printf("\nDigite o cpf(apenas numeros) que deseja remover: ");
    scanf("%ld", &cpf);
    if (cpf < 10000000000 || cpf > 99999999999)
    {
        cpf = -1;
    }

    return cpf;
}

void menu4(int *anoInicio, int *anoFinal)
{
    printf("Relatorio de faixa idade por estados\n-------------------\n");

    printf("\nDigite o ano de inicio: ");
    scanf("%d", anoInicio);

    printf("\nDigite o ano de fim: ");
    scanf("%d", anoFinal);
}
