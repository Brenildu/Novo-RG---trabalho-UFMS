#include "hash.h"

void popular_hash(No tabela[])
{
	int i;
	for (i = 0; i < TAM; i++)
	{
		tabela[i].prox = NULL;
	}
}

void popular_estados(Estado estados[]){
	int i;
	for(i = 0; i < 27; i++){
		popular_hash(estados[i].tabela);
	}
}

long valor_sequencia(long registro)
{
	long valor;

	valor = registro % 100 + registro / 100;
	return valor;
}

long funcao_hash(long registro)
{
	long chave;

	chave = valor_sequencia(registro) % 18181817;

	return chave;
}

void insere_tabela(No lista[], No *novo)
{
	long chave;

	chave = funcao_hash(novo->cin.registro);

	inserir_lista_ordem_alfabetica(&lista[chave], novo);
}



long remover_cin(No *lista, long registro)
{
	No *p = lista->prox;
	No *no_removido = NULL;
	long reg_removido = -1;

	if (p)
	{
		if(p->cin.registro == registro){
			no_removido = p->prox;
			p->prox = no_removido->prox;
			reg_removido = no_removido->cin.registro;
			free(no_removido);
		}else{
			while (p->prox != NULL && p->prox->cin.registro != registro)
			{
				p = p->prox;
			}

			if (p->prox != NULL)
			{
				no_removido = p->prox;
				p->prox = no_removido->prox;
				reg_removido = no_removido->cin.registro;
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

void inserir_lista_ordem_alfabetica(No *lista, No *novo)
{
	No *aux;
	if (novo)
	{
		if (lista->prox == NULL)
		{
			novo->prox = NULL;
			lista->prox = novo;
		}
		else if (strcmp(novo->cin.nome, lista->prox->cin.nome) < 0)
		{
			novo->prox = lista->prox;
			lista->prox = novo;
		}
		else
		{
			aux = lista->prox;
			while (aux->prox && strcmp(novo->cin.nome, lista->prox->cin.nome) >= 0)
				aux = aux->prox;

			if (strcmp(novo->cin.nome, lista->prox->cin.nome) != 0 || novo->cin.registro != lista->prox->cin.registro)
			{
				novo->prox = aux->prox;
				aux->prox = novo;
			}
			else{
				/*Ja cadastrado */
			}
		}
	}
}

No *busca_registro(No lista_registro[], long registro)
{
	No *p, *no = NULL;
	int chave;

	chave = funcao_hash(registro);
	if (chave < 0)
		return NULL;

	p = lista_registro[chave].prox;
	while (p != NULL && p->cin.registro != registro)
	{
		p = p->prox;
	}
	if (p)
	{
		no = p;
	}

	return no;
}

void relatorio_intervaloAnos(No lista_registro[], int anoInicial, int anoFinal)
{
	No *no;
	int i, j;

	for (i = 0; i < TAM; i++)
	{
		no = lista_registro[i].prox;
		while (no)
		{
			j = no->cin.registros_emetidos - 1;
			if (no->cin.data[2] >= anoInicial && no->cin.data[2] <= anoFinal)
			{
				printf("%s;%s;%s;%d;%d;%d;%d;%ld;\n",
					no->cin.nome,
					no->cin.Naturalidade->cidade,
					no->cin.Naturalidade->estado,
					no->cin.Naturalidade->rg,
					no->cin.data[0],
					no->cin.data[1],
					no->cin.data[2],
					no->cin.registro);
			}
			while (j >= 0)
			{
				printf("Estados Emitidos: %d", no->cin.registros_emetidos);
			}

			no = no->prox;
		}
	}
}

void relatorio_porEstado(No lista_registro[])
{
	No *no;
	int i, j, estado = 0;

	while (estado <= 25)
	{
		for (i = 0; i < TAM; i++)
		{
			no = lista_registro[i].prox;
			while (no)
			{
				imprimir_registros_estado(lista_registro,"sp");
				no = no->prox;
			}
		}
		estado++;
	}
}

void * imprimir_registros_(No *lista)
{
	No *no = lista->prox;
	if (no)
	{
		printf("----------------\n");
		while (no != NULL)
		{
			printf("\"nome\": \"%s\",\n\"cpf\": \"%ld\",\n\"rg\": \"%d\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\"estado\": \"%s\";\n}\n",
				no->cin.nome,
				no->cin.registro,
				no->cin.Naturalidade[0].rg,
				no->cin.data[0],
				no->cin.data[1],
				no->cin.data[2],
				no->cin.Naturalidade[0].cidade,
				no->cin.Naturalidade[0].estado);

			no = no->prox;
		}
		printf("\n\n");
	}
}

void * imprimir_registros_estado(No *lista, char estado)
{
	No *no = lista->prox, *estados = NULL;
	int j;
	if (no)
	{
		printf("----------------\n");
		while (no != NULL)
		{
			j = 0;
			while(j < no->cin.registros_emetidos){
				if(strcmp(no->cin.Naturalidade[j].estado, estado) == 0 ){
					printf("\"nome\": \"%s\",\n\"cpf\": \"%ld\",\n\"rg\": \"%d\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%d\",\n\"estado\": \"%d\";\n}\n",
						no->cin.nome,
						no->cin.registro,
						no->cin.Naturalidade[j].rg,
						no->cin.data[0],
						no->cin.data[1],
						no->cin.data[2],
						no->cin.Naturalidade[j].cidade,
						no->cin.Naturalidade[j].estado);
				}
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

void deleta_tabela(No tabela[])
{
	int i;

	for (i = 0; i < TAM; i++)
	{
		finaliza_lista(tabela[i]);
	}
}
