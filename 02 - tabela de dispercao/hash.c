#include "hash.h"

void popular_hash(No tabela[TAM])
{
	int i;
	for (i = 0; i < TAM; i++)
	{
		tabela[i].prox = NULL;
	}
}

int valor_sequencia(int registro)
{
	int valor;

	valor = registro % 100 + registro / 100;
	;
	return valor;
}

int funcao_hash(int registro)
{
	int chave;

	chave = valor_sequencia(registro) % 18181817;

	return chave;
}

void insere_tabela(No lista[], No *novo)
{
	int chave;

	chave = funcao_hash(novo->cin.registro);

	inserir_ordenado(&lista[chave], novo);
}

void inserir_no_inicio(No *lista, No *novo)
{
	if (novo)
	{
		novo->prox = lista->prox;
		lista->prox = novo;
	}
}

int remover_registro(No *lista, int registro)
{
	No *p = lista;
	No *no_removido = NULL;
	int reg_removido = -1;

	if (lista->prox)
	{
		if ((lista)->cin.registro == registro)
		{
			no_removido = lista;
			lista = lista->prox;
			free(no_removido);
		}
		else
		{
			while (p->prox != NULL && p->prox->cin.registro != registro)
			{
				p = p->prox;
			}

			if (p->prox != NULL)
			{
				no_removido = p;
				p = p->prox;
				free(no_removido);
			}
		}
	}
	return reg_removido;
}

No *criar_no(CIN cin)
{
	No *novo = malloc(sizeof(No));

	novo->cin = cin;
	novo->prox = NULL;

	if (!novo)
	{
		printf("Falha ao alocar memória.\n");
		return NULL;
	}
	return novo;
}

void inserir_lista_ordenada(No *lista, No *novo)
{
	No *aux;

	if (novo)
	{

		if (lista->prox == NULL)
		{
			novo->prox = NULL;
			lista->prox = novo;
		}
		else if (novo->cin.registro < lista->prox->cin.registro)
		{
			novo->prox = lista->prox;
			lista->prox = novo;
		}
		else
		{
			aux = lista->prox;
			while (aux->prox && novo->cin.registro >= aux->prox->cin.registro)
				aux = aux->prox;

			if (novo->cin.registro != aux->cin.registro)
			{
				novo->prox = aux->prox;
				aux->prox = novo;
			}
		}
	}
}

No *busca_registro(No lista_registro[], int registro)
{
	No *p, *no = NULL;
	int registroS;
	int chave;

	registroS = registro;
	chave = funcao_hash(registroS);
	if (chave < 0 || chave >= TAM)
		return NULL;

	*p = lista_registro[chave];
	while (p != NULL && p->cin.registro != NULL)
	{
		p = p->prox;
	}
	if (p)
	{
		no = p;
	}

	return no;
}

void relatorio_intervaloAnos(No listaAntigoPadrao[], No listaNovoPadrao[], int anoInicial, int anoFinal)
{
	No *no;
	int i, j;

	for (i = 0; i < TAM; i++)
	{
		no = listaAntigoPadrao[i].prox;
		while (no)
		{
			j = no->cin.registros_emetidos - 1;
			if (no->cin.data[2] >= anoInicial && no->cin.data[2] <= anoFinal)
			{
				printf("%s;%s;%d;%d;%d;%s;\n",
					   no->cin.nome,
					   no->cin.naturalidade,
					   no->cin.data[0],
					   no->cin.data[1],
					   no->cin.data[2],
					   no->cin.registro);
			}
			while (j >= 0)
			{
				printf("Estados Emitidos: %s", no->cin.estado[j]);
			}

			no = no->prox;
		}
	}

	for (i = 0; i < TAM; i++)
	{
		no = listaNovoPadrao[i].prox;
		while (no)
		{
			j = no->cin.registros_emetidos - 1;
			if (no->cin.data[2] >= anoInicial && no->cin.data[2] <= anoFinal)
			{
				printf("%s;%s;%d;%d;%d;%s;%d;\n",
					   no->cin.nome,
					   no->cin.naturalidade,
					   no->cin.data[0],
					   no->cin.data[1],
					   no->cin.data[2],
					   no->cin.registro,
					   no->cin.registros_emetidos);
			}

			no = no->prox;
		}
	}
}

void relatorio_porEstado(No listaAntigoPadrao[], No listaNovoPadrao[])
{
	No *no;
	int i, j, estado = 0, estadosEmitidos;

	while (estado <= 25)
	{
		for (i = 0; i < TAM; i++)
		{
			no = listaAntigoPadrao[i].prox;
			while (no)
			{
				j = 0;
				estadosEmitidos = no->cin.registros_emetidos - 1;
				while (j < estadosEmitidos)
				{
					if (no->cin.estado[j] == estado)
					{
						printf("%s;%s;%d;%d;%d;%s;\n",
							   no->cin.nome,
							   no->cin.naturalidade,
							   no->cin.data[0],
							   no->cin.data[1],
							   no->cin.data[2],
							   no->cin.registro);
					}
				}
				j++;
				no = no->prox;
			}
		}
		for (i = 0; i < TAM; i++)
		{
			no = listaNovoPadrao[i].prox;
			while (no)
			{
				/* Vendo como sera feito ainda
				j = 0;
				estadosEmitidos = no->cin.registros_emetidos - 1;
				if (no->cin.estado[j] == estado)
				{
					printf("%s;%s;%d;%d;%d;%s;\n",
						   no->cin.nome,
						   no->cin.naturalidade,
						   no->cin.data[0],
						   no->cin.data[1],
						   no->cin.data[2],
						   no->cin.registro);
				}*/
				no = no->prox;
			}
		}
		estado++;
	}
}

void imprimir_registros(No *lista)
{
	No *no = lista->prox;
	int j;
	if (no)
	{
		printf("----------------\n");
		while (no != NULL)
		{
			j = no->cin.registros_emetidos - 1;

			printf("%s;%s;%d;%d;%d;%s;\n",
				   no->cin.nome,
				   no->cin.naturalidade,
				   no->cin.data[0],
				   no->cin.data[1],
				   no->cin.data[2],
				   no->cin.registro);

			while (j >= 0)
			{
				printf("Estados Emitidos: %s", no->cin.estado[j]);
			}

			no = no->prox;
		}
		printf("\n\n");
	}
}

void imprimir_tabela(No tabela[])
{
	int i;

	for (i = 0; i < TAM; i++)
	{
		imprimir_registros(&tabela[i]);
	}
}

void finaliza_lista(No lista)
{
	No *aux, *remove;

	aux = lista.prox;
	while (aux)
	{
		remove = aux;
		aux = aux->prox;
		free(remove);
	}
	lista.prox = NULL;
}

void deleta_tabela(No tabela[TAM])
{
	int i;

	for (i = 0; i < TAM; i++)
	{
		libera_lista(tabela[i]);
	}
}
