#include "arvoreBinaria.h"

Node *novo_node(CIN cin)
{
	Node *novo = (Node *)malloc(sizeof(Node));

	if (novo)
	{
		novo->cin = cin;
		novo->dir = NULL;
		novo->esq = NULL;

		return novo;
	}

	return NULL;
}

Estado *novo_estado(int valor_sigla)
{
	Estado *novo_estado = (Estado *)malloc(sizeof(Estado));

	if (novo_estado)
	{
		novo_estado->node = NULL;
		novo_estado->dir = NULL;
		novo_estado->esq = NULL;
		novo_estado->sigla = valor_sigla;

		return novo_estado;
	}
	return NULL;
}

const char *siglas_estados[] = {
	"AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
	"MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
	"RO", "RR", "RS", "SC", "SE", "SP", "TO"};

// Obter o valor correspondente ao estado
int valor_estado(const char *sigla)
{
	int i;
	for (i = 0; i < 27; i++)
	{
		if (strcmp(siglas_estados[i], sigla) == 0)
		{
			return i;
		}
	}
	return -1;
}

Estado *busca_estado(Estado *estado, int valor_sigla, Estado *anterior)
{
	if (estado == NULL)
	{
		return anterior;
	}

	if (estado->sigla == valor_sigla)
		return estado;
	else if (valor_sigla > estado->sigla)
		return busca_estado(estado->dir, valor_sigla, estado);
	else
		return busca_estado(estado->esq, valor_sigla, estado);
}

Estado *popular_estados()
{
	int i;
	Estado *raiz, *novo, *aux;
	raiz = novo_estado(13);

	if (raiz)
	{
		for (i = 14; i < 27; i = i + 2)
		{
			aux = busca_estado(raiz, i, NULL);
			if (aux != NULL)
			{
				novo = novo_estado(i);
				if (novo->sigla > aux->sigla)
					aux->dir = novo;
				else
					aux->esq = novo;
			}
			else
			{
				return NULL;
			}
		}

		for (i = 15; i < 27; i = i + 2)
		{
			aux = busca_estado(raiz, i, NULL);
			if (aux != NULL)
			{
				novo = novo_estado(i);
				if (novo->sigla > aux->sigla)
					aux->dir = novo;
				else
					aux->esq = novo;
			}
			else
			{
				return NULL;
			}
		}

		for (i = 0; i < 13; i = i + 2)
		{
			aux = busca_estado(raiz, i, NULL);
			if (aux != NULL)
			{
				novo = novo_estado(i);
				if (novo->sigla > aux->sigla)
					aux->dir = novo;
				else
					aux->esq = novo;
			}
			else
			{
				return NULL;
			}
		}

		for (i = 1; i < 13; i = i + 2)
		{
			aux = busca_estado(raiz, i, NULL);
			if (aux != NULL)
			{
				novo = novo_estado(i);
				if (novo->sigla > aux->sigla)
					aux->dir = novo;
				else
					aux->esq = novo;
			}
			else
			{
				return NULL;
			}
		}
	}
	return raiz;
}

Node *busca_cin(Node *raiz, long registro, Node *anterior)
{
	if (raiz == NULL)
		return anterior;

	if (raiz->cin.registro == registro)
		return raiz;

	if (registro > raiz->cin.registro)
		return busca_cin(raiz->dir, registro, raiz);
	else
		return busca_cin(raiz->esq, registro, raiz);
}

Node *busca_nome(Node *raiz, const char *nome, Node *anterior)
{
	if (raiz == NULL)
		return anterior;

	if (strcmp(raiz->cin.nome, nome) == 0)
		return raiz;

	if (strcmp(nome, raiz->cin.nome) > 0)
		return busca_nome(raiz->dir, nome, raiz);
	else
		return busca_nome(raiz->esq, nome, raiz);
}

void inserir_cin(Node **node, CIN cin)
{
	if (*node == NULL)
	{
		*node = novo_node(cin);
		if (*node == NULL)
		{
			printf("Falha ao tentar alocar memória para o registro: %ld\n", cin.registro);
		}
	}
	else if (cin.registro < (*node)->cin.registro)
	{
		inserir_cin(&((*node)->esq), cin);
	}
	else if (cin.registro > (*node)->cin.registro)
	{
		inserir_cin(&((*node)->dir), cin);
	}
}

void inserir_nome(Estado *estados, CIN cin)
{
	int valor_sigla = valor_estado(cin.registros_emetidos[0].estado);
	Estado *estado = busca_estado(estados, valor_sigla, NULL);

	if (estado == NULL)
	{
		printf("Estado não encontrado para o registro: %ld\n", cin.registro);
		return;
	}

	inserir_cin(&(estado->node), cin);
}

Node *maior_ValorEsq(Node *no)
{
	Node *maior = no->esq;

	while (maior && maior->dir != NULL)
		maior = maior->dir;

	return maior;
}

long remover_cin(Node **raiz, Estado *estados, long registro)
{
	Node *p = *raiz, *no_removido = NULL, *maior;
	long reg_removido = -1;

	if (p == NULL)
		return reg_removido;

	if (registro < p->cin.registro)
	{
		reg_removido = remover_cin(&(p->esq), estados, registro);
	}
	else if (registro > p->cin.registro)
	{
		reg_removido = remover_cin(&(p->dir), estados, registro);
	}
	else
	{
		no_removido = p;
		if (p->esq == NULL)
		{
			*raiz = p->dir;
		}
		else if (p->dir == NULL)
		{
			*raiz = p->esq;
		}
		else
		{
			maior = maior_ValorEsq(p);
			p->cin = maior->cin;
			reg_removido = remover_cin(&(p->esq), estados, maior->cin.registro);
			return reg_removido;
		}
		reg_removido = no_removido->cin.registro;
		free(no_removido);
	}
	return reg_removido;
}

void remover_cin_estado(Estado *estados, Node *no_removido)
{
	int valor_sigla = valor_estado(no_removido->cin.registros_emetidos[0].estado);
	Estado *estado = busca_estado(estados, valor_sigla, NULL);

	if (estado == NULL)
	{
		printf("Estado não encontrado para o registro: %ld\n", no_removido->cin.registro);
		return;
	}

	long registro = no_removido->cin.registro;
	remover_cin(&(estado->node), estados, registro);
}

void imprimir_cins_idade(Node *arvore, int anoInicial, int anoFinal)
{
	if (arvore == NULL)
		return;

	imprimir_cins_idade(arvore->esq, anoInicial, anoFinal);

	if (arvore->cin.data[2] >= anoInicial && arvore->cin.data[2] <= anoFinal)
	{
		imprimir_cin(arvore->cin);
	}

	imprimir_cins_idade(arvore->dir, anoInicial, anoFinal);
}

void imprimir_cin(CIN cin)
{
	printf("\"nome\": \"%s\",\n\"cpf\": \"%ld\",\n\"rg\": \"%d\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
		   cin.nome,
		   cin.registro,
		   cin.registros_emetidos[0].rg,
		   cin.data[0],
		   cin.data[1],
		   cin.data[2],
		   cin.registros_emetidos[0].cidade,
		   cin.registros_emetidos[0].estado);
}

void relatorio(Estado *estados, int anoInicial, int anoFinal)
{
	if (estados == NULL)
		return;

	relatorio(estados->esq, anoInicial, anoFinal);
	imprimir_cins_idade(estados->node, anoInicial, anoFinal);
	relatorio(estados->dir, anoInicial, anoFinal);
}

void deleta_arvore(Node *arvore)
{
	if (arvore)
	{
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}
