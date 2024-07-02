#include "arvoreAVL.h"

int maior(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

short fatorDeBalanceamento(Node *node)
{
	if (node)
		return (node->dir->altura - node->esq->altura);
	else
		return 0;
}

Node *rotacaoSE(Node *node)
{
	Node *p, *q;

	p = node->dir;
	q = p->esq;

	p->esq = node;
	node->dir = q;

	node->altura = maior(node->esq->altura, node->dir->altura) + 1;
	p->altura = maior(p->esq->altura, p->dir->altura) + 1;

	return p;
}

Node *rotacaoSD(Node *node)
{
	Node *p, *q;

	p = node->esq;
	q = p->dir;

	p->dir = node;
	node->esq = q;

	node->altura = maior(node->esq->altura, node->dir->altura) + 1;
	p->altura = maior(p->esq->altura, p->dir->altura) + 1;

	return p;
}

Node *rotacaoDD(Node *node)
{
	node->esq = rotacaoSE(node->esq);
	return rotacaoSD(node);
}

Node *rotacaoDE(Node *node)
{
	node->dir = rotacaoSD(node->dir);
	return rotacaoSE(node);
}

Node *balancear(Node *node)
{
	short fb = fatorDeBalanceamento(node);

	if (fb > 1 && fatorDeBalanceamento(node->dir) >= 0)
		node = rotacaoEsquerda(node);

	else if (fb < -1 && fatorDeBalanceamento(node->esq) <= 0)
		node = rotacaoDireita(node);

	else if (fb < -1 && fatorDeBalanceamento(node->esq) > 0)
		node = rotacaoEsquerdaDireita(node);

	else if (fb > 1 && fatorDeBalanceamento(node->dir) < 0)
		node = rotacaoDireitaEsquerda(node);

	return node;
}

Node *novo_node(CIN cin)
{
	Node *novo = (Node *)malloc(sizeof(Node));

	if (novo)
	{
		novo->cin = cin;
		novo->esq = NULL;
		novo->dir = NULL;
		novo->altura = 1;
 
		return novo;
	}

	return NULL;
}

Node *inserir_cin(Node *node, CIN cin)
{
	Node *novo;

	if (node == NULL)
	{
		novo = novo_node(cin);
		if (novo)
			return novo;
		else
			printf("Falha ao tentar alocar memória para a registro: %s\n", cin.registro);
		return NULL;
	}
	else
	{
		if (strcmp(cin.nome, node->cin.nome) < 0)
			node->esq = inserir_cin(node->esq, cin);
		else if (strcmp(cin.nome, node->cin.nome) > 0)
			node->dir = inserir_cin(node->dir, cin);
	}
	node->altura = maior(alturaNode(node->esq), alturaNode(node->dir)) + 1;

	node = balancear(node);

	return node;
}

Node *remover_cin(Node *node, long registro)
{
	if (node == NULL)
	{
		return NULL;
	}
	else
	{
		if (registro == node->cin.registro)
		{
			if (node->esq == NULL && node->dir == NULL)
			{
				free(node);
				return NULL;
			}
			else
			{
				if (node->esq != NULL && node->dir != NULL)
				{
					Node *aux = node->esq;
					while (aux->dir != NULL)
						aux = aux->dir;
					node->cin = aux->cin;
					node->esq = remover_cin(node->esq, aux->cin.registro);
					return node;
				}
				else
				{
					Node *aux;
					if (node->esq != NULL)
						aux = node->esq;
					else
						aux = node->dir;
					free(node);
					return aux;
				}
			}
		}
		else
		{
			if (registro == node->cin.registro)
				node->esq = remover_cin(node->esq, registro);
			else
				node->dir = remover_cin(node->dir, registro);
		}
		node->altura = maior(alturaNode(node->esq), alturaNode(node->dir)) + 1;

		node = balancear(node);

		return node;
	}
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
		return buscaRegistro(arvore->esq, registro);
	}
	else if (registro > arvore->cin.registro)
	{
		return buscaRegistro(arvore->dir, registro);
	}
	return NULL;
}

Node *procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, const long *registro)
{
	Node *no = NULL;

	if (0)
	{
		no = buscaregistro(raizNovoPadrao, registro);
	}
	else
	{
		no = buscaregistro(raizAntigoPadrao, registro);
		if (no == NULL)
		{
			no = buscaregistro(raizNovoPadrao, registro);
		}
	}

	return no;
}

Node *alterar_registro(Node **raizAntigoPadrao, Node **raizNovoPadrao, const long *registro)
{
	Node *alterar, *novo = NULL;
	CIN cin;
	int retorno;
	long aux = registro;
}

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal)
{

	if (raiz)
		relatorio_anos(raiz->esq, anoInicial, anoFinal);

	if (raiz->cin.data[2] >= anoInicial && raiz->cin.data[2] <= anoFinal)
	{
		printf("%s;%s;%s;%d;%d;%d;%d;%d;\n",
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

void relatorio_porEstado(Node *rootAntigoPadrao, Node *rootNovoPadrao)
{
}

void imprimir_cins(Node *arvore)
{
	if (arvore)
	{
		imprimir_veiculos(arvore->esq);
		printf("%s;%s;%s;%d;%d;%d;%d;%d;\n",
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
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}
	void imprimir_cins(Node *arvore)
	{
		if (arvore)
		{
			imprimir_veiculos(arvore->esq);
			printf("%s;%s;%s;%d;%d;%d;%d;%d;\n",
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
