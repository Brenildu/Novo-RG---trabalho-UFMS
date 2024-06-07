#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"
#include "arquivos.h"

/*
Compilação: gcc -o main.exe *.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
*/

int main(int argc, char *argv[])
{
	mystruct estrutura;
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	char tempo[50];
	
	
	inicializar_tabela(estrutura.listaAnt);
	inicializar_tabela(estrutura.listaNov);
	
	carregamento_arquivo(argc, argv, estrutura.listaAnt, estrutura.listaNov);
	
	clock_gettime(CLOCK_MONOTONIC, &tend);
	sprintf(tempo, "Tempo de Carregamento: %.5f segundos",((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) 
		- ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	
	
	
	gravacaoBinario(estrutura.listaAnt, estrutura.listaNov);
	
	deleta_tabela(estrutura.listaAnt);
	deleta_tabela(estrutura.listaNov);
	
	return 0;
}