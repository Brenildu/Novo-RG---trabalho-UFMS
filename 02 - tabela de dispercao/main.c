#include <time.h>
#include "hash.h"
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
	Estado estados[27];
	No tabela[TAM];
	CIN cin_novo;

	popular_hash(tabela, TAM);
	popular_estados(estados);

	// Carregar dados existentes
	carregarDadosTxt(tabela, "dados.txt");

	// Carregar dados dos arquivos JSON passados como argumentos
	for (int i = 1; i < argc; i++)
	{
		printf("Processing file: %s\n", argv[i]);
		parseJSON(argv[i], tabela);
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
				printf("Verifique se digitou certo o cpf, Operacao Invalida!!\n");
			}
			else
			{
				startTimer(&tstart);
				busca_cin(tabela, cpf);
				double elapsed = stopTimer(&tstart, &tend);
				printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
			}
			break;

		case 2:
		{
			cin_novo = menu2();
			if (cin_novo.registro == 0)
			{
				printf("Verifique se digitou certo o cpf, Operacao Invalida!!\n");
			}
			else
			{
				No *novo = criar_no(cin_novo);
				if (novo)
				{
					insere_tabela(tabela, novo);
					insere_estado(estados, novo, novo->cin.registros_emetidos[0]);
				}
			}
			break;
		}

		case 3:
			cpf = menu3();
			if (cpf == -1)
			{
				printf("Verifique se digitou certo o cpf, Operacao Invalida!!\n");
			}
			else
			{
				remover_cin(tabela, estados, cpf);
			}
			break;

		case 4:
			menu4(&anoInicio, &anoFim);

			startTimer(&tstart);
			relatorio(estados, anoInicio, anoFim);
			double elapsed = stopTimer(&tstart, &tend);
			printf("Tempo de execução: %.3f segundos\n", elapsed / 1e9);
			break;

		case 5:
			printf("Finalizando programa!!\n");
			salvarDadosTxt(tabela, "dados.txt");
			deleta_estados(estados);
			deleta_tabela(tabela);
			break;

		default:
			printf("Verifique se digitou corretamente, Operacao Invalida!!\n");
			break;
		}
	} while (op != 5);

	return 0;
}
