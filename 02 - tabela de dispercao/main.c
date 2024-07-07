#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "hash.h"
#include "arquivos.h"

/*gcc -Wall -Wextra -g3 main.c hash.c menu.c arquivos.c cJSON.c -o output\main.exe

./output/main.exe nome_do_arquivo.json
*/

void startTimer(clock_t *start)
{
    *start = clock();
}

double stopTimer(clock_t *start, clock_t *end)
{
    *end = clock();
    double elapsed = ((double)(*end - *start)) / CLOCKS_PER_SEC;
    return elapsed;
}

int main(int argc, char *argv[])
{
    printf("Entrou!");
    printf("Legal!\n");
    if (argc != 2)
    {
        printf("Uso: %s <nome_do_arquivo_json>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int op, anoInicial, anoFinal;
    char cpf[12];
    No tabela[TAM];
    Estado relatorio[TAM_ESTADO];
    clock_t tstart, tend;

    // Inicializar a tabela hash
    popular_hash(tabela, TAM);
    popular_estados(relatorio);

    printf("Tabela hash inicializada.\n");

    // Carrega os dados do arquivo JSON
    startTimer(&tstart);
    carregarDados(tabela, filename);
    double elapsed = stopTimer(&tstart, &tend);
    printf("\n-----------\nCarregar dados\nTempo de execucao : %.3f segundos\n", elapsed);

    do
    {
        op = menuPrincipal();

        switch (op)
        {
        case 1:
            menu1(cpf);
            printf("CPF: %s\n", cpf);

            startTimer(&tstart);
            No *busca = busca_cin(tabela, cpf);

            elapsed = stopTimer(&tstart, &tend);

            if (busca)
            {
                imprimir_cin(busca->cin);
                printf("A Busca foi um sucesso\n");
            }
            else
            {
                printf("CPF nao encontrado\n");
            }

            printf("Tempo de execucao: %.3f segundos\n\n", elapsed);
            break;

        case 2:
            menu2(&anoInicial, &anoFinal);
            printf("Relatorio de faixa etaria de pessoas que nascem entre: %d e %d\n", anoInicial, anoFinal);

            startTimer(&tstart);
            popular_estados(relatorio);

            gerar_relatorio(relatorio, tabela, anoInicial, anoFinal);

            imprimir_relatorio_em_arquivo(relatorio, "relatorio.txt");
            deleta_estados(relatorio);

            elapsed = stopTimer(&tstart, &tend);
            printf("Tempo de execucao: %.3f segundos\n\n", elapsed);
            
        case 3:
            printf("Finalizando programa!!\n\n");
            break;

        default:
            printf("Verifique se digitou corretamente, Operacao Invalida!!\n\n");
            break;
        }

    } while (op != 3);

    // Salva os dados no arquivo TXT
    salvarDados(tabela, "saida.txt");

    // Limpa a memória da lista
    deleta_tabela(tabela);

    return 0;
}