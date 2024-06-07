#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arquivos.h"
#include "gui.h"

/*
Compilação: gcc -o main.exe *.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
*/

GtkBuilder *builder;
GtkWidget *window;
GtkStack *stack;
GtkLabel *lab;


int main(int argc, char *argv[])
{
	No *listaAntigoPadrao = NULL, *listaNovoPadrao = NULL, *no;
	char , estado[3];
    int CIN;
	int anoInicio, anoFim,i;
	mystruct estrutura;
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	char tempo[50];
	
	
	carregamento_arquivo(argc, argv, &listaAntigoPadrao, &listaNovoPadrao);
	
    

	estrutura.listaAnt = &listaAntigoPadrao;
	estrutura.listaNov = &listaNovoPadrao;
	
	clock_gettime(CLOCK_MONOTONIC, &tend);
	sprintf(tempo, "Tempo de Carregamento: %.5f segundos",((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) 
		- ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	
	
	
	gravacaoBinario(&listaAntigoPadrao, &listaNovoPadrao);
	
	libera_lista(&listaAntigoPadrao);
	libera_lista(&listaNovoPadrao);
	
	return 0;
}