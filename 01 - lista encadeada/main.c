#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  // Incluir o cabeçalho para clock_gettime
#include "menu.h"
#include "lista.h"
#include "arquivos.h"

void startTimer(struct timespec *start)
{
    clock_gettime(CLOCK_MONOTONIC, start);
}

double stopTimer(struct timespec *start, struct timespec *end)
{
    clock_gettime(CLOCK_MONOTONIC, end);
    double elapsed = (end->tv_sec - start->tv_sec) * 1e9; // segundos para nanosegundos
    elapsed += (end->tv_nsec - start->tv_nsec);           // nanosegundos
    return elapsed;
}

int main(void) {
    int op, anoInicial, anoFinal;
    char cpf[12];
    CIN *lista = NULL;
    CIN *busca, *relatorio;
    struct timespec tstart = {0, 0}, tend = {0, 0};

    // Carrega os dados do arquivo JSON
    carregarDados(&lista, "dados.json");

    do {
        op = menuPrincipal();

        switch (op) {
            case 1:
                menu1(cpf);
                printf("CPF: %s\n", cpf);

                startTimer(&tstart);
                busca = busca_cin(lista, cpf);

                double elapsed = stopTimer(&tstart, &tend);

                if (busca) {
                    imprimir_cin(*busca);
                    printf("A Busca foi um sucesso\n");
                } else {
                    printf("CPF nao encontrado\n");
                }

                printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
                break;

            case 2:
                menu2(&anoInicial, &anoFinal);
                printf("Relatorio de faixa etaria de pessoas que nascem entre: %d e %d\n", anoInicial, anoFinal);

                startTimer(&tstart);

                relatorio = gerar_relatorio(lista, anoInicial, anoFinal);
                imprimir_relatorio_em_arquivo(relatorio, "relatorio.txt");
                deleta_lista(relatorio); // Exclui o relatório gerado

                printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
                break;

            case 3:
                printf("Finalizando programa!!\n\n");
                break;

            default:
                printf("Verifique se digitou corretamente, Operacao Invalida!!\n\n");
                break;
        }

    } while (op != 3);

    // Salva os dados no arquivo TXT
    salvarDados(lista, "saida.txt");

    // Limpa a memória da lista
    deleta_lista(lista);

    return 0;
}
