#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "lista.h"

int main(void)
{
    int op, anoInicial, anoFinal;
    char cpf[12];
    CIN *lst, pessoa1;

    pessoa1.data[0] = 1;
    pessoa1.data[1] = 1;
    pessoa1.data[2] = 1;
    strcpy(pessoa1.nome, "Breno");
    strcpy(pessoa1.registro, "12345678911");
    pessoa1.prox = NULL;
    strcpy(pessoa1.registros_emitidos[0].cidade, "Ilha Solteira");
    strcpy(pessoa1.registros_emitidos[0].estado, "SP");
    strcpy(pessoa1.registros_emitidos[0].rg, "111111111");

    printf("%d/%d/%d\n", pessoa1.data[0], pessoa1.data[1], pessoa1.data[2]);
    printf("%s\n", pessoa1.nome);
    printf("%s\n", pessoa1.registro);
    printf("%s\n", pessoa1.registros_emitidos[0].rg);
    printf("%s\n", pessoa1.registros_emitidos[0].cidade);
    printf("%s\naiuu", pessoa1.registros_emitidos[0].estado);

    do
    {
        op = menuPrincipal();

        switch (op)
        {
        case 1:
            menu1(cpf);
            printf("CPF: %s\n", cpf);
            break;

        case 2:
            menu2(&anoInicial, &anoFinal);
            printf("Relatorio de faixa etaria de pessoas que nascem entre: %d e %d", anoInicial, anoFinal);
            break;

        case 3:
            printf("Finalizando programa!!\n\n");
            break;

        default:
            printf("Verifique se digitou corretamente, Operacao Invalida!!\n\n");
            break;
        }

    } while (op != 3);

    return 0;
}