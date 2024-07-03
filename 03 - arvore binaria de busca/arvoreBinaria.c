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

Estado *novo_estado(int valor_sigla){
	Estado *novo_estado = (Estado *)malloc(sizeof(Estado));

	if(novo_estado){
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

Estado *busca_estado(Estado *estado, int valor_sigla, Estado *anterior){
	if(estado == NULL){
		return anterior;
	}

	if(estado->sigla == valor_sigla)
		return estado;
	else if(valor_sigla > estado->sigla )
		return busca_estado(estado->dir, valor_sigla, estado);
	else
		return busca_estado(estado->esq, valor_sigla, estado);
}

Estado *popular_estados(){
	int i;
	Estado *raiz, *novo, *aux;
	raiz = novo_estado(13); 

	if(raiz){
		for(i = 14; i < 27; i = i+2){
			aux = busca_estado(raiz, i, raiz);
			if(aux != NULL){
				novo = novo_estado(i);
				if(novo->sigla > aux->sigla)
					aux->dir = novo;
			}else{
				return NULL;
			}
		}

		for(i = 15; i < 27; i = i+2){
			aux = busca_estado(raiz, i, raiz);
			if(aux != NULL){
				novo = novo_estado(i);
				if(novo->sigla > aux->sigla)
					aux->dir = novo;
			}else{
				return NULL;
			}
		}

		for(i = 0; i < 13; i = i+2){
			aux = busca_estado(raiz, i, raiz);
			if(aux != NULL){
				novo = novo_estado(i);
				if(novo->sigla > aux->sigla)
					aux->dir = novo;
			}else{
				return NULL;
			}
		}

		for(i = 1; i < 13; i = i+2){
			aux = busca_estado(raiz, i, raiz);
			if(aux != NULL){
				novo = novo_estado(i);
				if(novo->sigla > aux->sigla)
					aux->dir = novo;
			}else{
				return NULL;
			}
		}
	}
}


void inserir_cin(Node **node, CIN cin)
{
	Node *novo, *p;

	p = *node;
	novo = novo_node(cin);
	if (p == NULL)
	{
		if (novo)
			*node = novo;
		else
			printf("Falha ao tentar alocar memória para o registro: %ld\n", cin.registro);

	}
	else if (cin.registro < p->cin.registro)
	{
		inserir_cin(p->esq, cin);
	}
	else if (cin.registro > p->cin.registro)
	{
		inserir_cin(p->dir, cin);
	} else{
		free(novo);
	}
}

void insere_nome_ordem_alfabetica(Node **node ,CIN cin){
	Node *novo, *p;

	p = *node;
	novo = novo_node(cin);
	if (p == NULL)
	{
		if (novo)
			*node = novo;
		else
			printf("Falha ao tentar alocar memória para o registro: %ld\n", cin.registro);

	}
	else if (strcmp(cin.nome, p->cin.nome) < 0)
	{
		insere_nome(p->esq, cin);
	}
	else if (strcmp(cin.nome, p->cin.nome) > 0)
	{
		insere_nome(p->dir, cin);
	} else{
		if(cin.registro != p->cin.registro){
			insere_nome(p->esq, cin);
		}else{
			free(novo);
		}
	}

}

void insere_nome_estados(Estado *estados, Node *node, CIN cin, int valor_sigla){

}

Node *maior_ValorEsq(Node *no)
{
	Node *maior = no;

	while (maior && maior->esq != NULL)
		maior = maior->esq;

	return maior;
}

Node *remover_cin(Node *raiz, long registro)
{
	Node *temp;

	if (raiz == NULL)
		return raiz;

	if (registro < raiz->cin.registro)
		raiz->esq = remover_cin(raiz->esq, registro);

	else if (registro > raiz->cin.registro)
		raiz->dir = remover_cin(raiz->dir, registro);

	else
	{
		if (raiz->esq == NULL)
		{
			temp = raiz->esq;
			free(raiz);
			return temp;
		}
		else if (raiz->dir == NULL)
		{
			temp = raiz->dir;
			free(raiz);
			return temp;
		}

		temp = maior_ValorEsq(raiz->esq);
		raiz->cin = temp->cin;
		raiz->esq = remover_cin(raiz->esq, temp->cin.registro);
	}
	return raiz;
}

Node *busca_registro(Node *arvore, long registro)
{
	if (!arvore)
	{
		return NULL;
	}

	if (registro == arvore->cin.registro)
	{
		return arvore;
	}
	else if (registro < arvore->cin.registro)
	{
		return busca_registro(arvore->esq, registro);
	}
	else if (registro > arvore->cin.registro)
	{
		return busca_registro(arvore->dir, registro);
	}
	return NULL;
}

Node *procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, const long *registro)
{
	Node *no = NULL;

	if (0)
	{
		no = busca_registro(raizNovoPadrao, *registro);
	}
	else
	{
		no = busca_registro(raizAntigoPadrao, *registro);
		if (no == NULL)
		{
			no = busca_registro(raizNovoPadrao, *registro);
		}
	}

	return no;
}

Node *busca_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro)
{
	Node *no = NULL;

	if (0)
	{
		no = busca_registro(raizNovoPadrao, registro);
	}
	else
	{
		no = busca_registro(raizAntigoPadrao, registro);
		if (no == NULL)
		{
			no = busca_registro(raizNovoPadrao, registro);
			if (no)
			{
				printf("registro encontrada, ele ja foi atualizado para o novo padrão!\n");
			}
		}
	}

	return no;
}

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal)
{

	if (raiz)
		relatorio_anos(raiz->esq, anoInicial, anoFinal);

	if (raiz->cin.data[2] >= anoInicial && raiz->cin.data[2] <= anoFinal)
	{
		printf("%s;%s;%s;%d;%d;%d;%d;%ld;\n",
			   raiz->cin.nome,
			   raiz->cin.Naturalidade->cidade,
			   raiz->cin.Naturalidade->estado,
			   raiz->cin.Naturalidade->rg,
			   raiz->cin.data[0],
			   raiz->cin.data[1],
			   raiz->cin.data[2],
			   raiz->cin.registro);
	}

	if (raiz)
		relatorio_anos(raiz->dir, anoInicial, anoFinal);
}

void relatorio_intervaloAnos(Node *raizAntigoPadrao, Node *raizNovoPadrao, int anoInicial, int anoFinal)
{
	relatorio_anos(raizAntigoPadrao, anoInicial, anoFinal);

	relatorio_anos(raizNovoPadrao, anoInicial, anoFinal);
}

void relatorio_porEstado(Node *rootAntigoPadrao, Node *rootNovoPadrao, char estado[3])
{
}

void imprimir_cins(Node *arvore)
{
	if (arvore)
	{
		imprimir_cins(arvore->esq);
		printf("%s;%s;%s;%d;%d;%d;%d;%ld;\n",
			   arvore->cin.nome,
			   arvore->cin.Naturalidade->cidade,
			   arvore->cin.Naturalidade->estado,
			   arvore->cin.Naturalidade->rg,
			   arvore->cin.data[0],
			   arvore->cin.data[1],
			   arvore->cin.data[2],
			   arvore->cin.registro);
		imprimir_cins(arvore->dir);
	}
}

void deleta_arvore(Node *arvore)
{
	if (arvore)
	{
		if (arvore->esq)
			deleta_arvore(arvore->esq);
		if (arvore->dir)
			deleta_arvore(arvore->dir);

		free(arvore);
	}
}
