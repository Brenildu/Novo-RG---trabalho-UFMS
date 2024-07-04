#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvoreAVL.h"
#include "arquivos.h"
#include "menu.h"

/*
Compilação: gcc -o main.exe *.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
*/

void startTimer(struct timespec *start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double stopTimer(struct timespec *start, struct timespec *end)
{
	clock_gettime(CLOCK_MONOTONIC, end);
	double elapsed = (end->tv_sec - start->tv_sec) * 1e9; // segundos para nanosegundos
	elapsed += (end->tv_nsec - start->tv_nsec);			  // nanosegundos
	return elapsed;
}

int main(int argc, char *argv[])
{
	struct timespec tstart = {0, 0}, tend = {0, 0};

	int op, anoInicio, anoFim;
	long cpf;
	Estado *estados = popular_estados(); // Árvore de estados
	Node *cpfTree = NULL;				 // Árvore de CPFs
	CIN cin_novo;

	if (estados == NULL)
	{
		fprintf(stderr, "Erro ao popular a árvore de estados.\n");
		return EXIT_FAILURE;
	}

	// Carregar dados existentes
	carregarDadosTxt(cpfTree, estados, "dados.txt");

	// Carregar dados dos arquivos JSON passados como argumentos
	for (int i = 1; i < argc; i++)
	{
		printf("Processando arquivo: %s\n", argv[i]);
		parseJSON(argv[i], estados, &cpfTree);
	}

	do
	{
		op = menu();

		switch (op)
		{
		case 1:
			cpf = menu1();
			if (cpf == -1)
			{
				printf("Verifique se digitou certo o CPF, Operação Inválida!\n");
			}
			else
			{
				startTimer(&tstart);
				Node *result = busca_cin(cpfTree, cpf, NULL);
				if (result != NULL)
				{
					imprimir_cin(result->cin);
				}
				else
				{
					printf("CPF não encontrado.\n");
				}
				double elapsed = stopTimer(&tstart, &tend);
				printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
			}
			break;

		case 2:
			menu2(&anoInicio, &anoFim);

			startTimer(&tstart);
			relatorio(estados, anoInicio, anoFim);
			double elapsed = stopTimer(&tstart, &tend);
			printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
			break;

		case 3:
			printf("Finalizando programa!\n");
			salvarDadosTxt(cpfTree, estados, "dados.txt");
			deleta_arvore(cpfTree);
			deleta_arvore(estados);
			break;

		default:
			printf("Verifique se digitou corretamente, Operação Inválida!\n");
			break;
		}
	} while (op != 5);

	return 0;
}
