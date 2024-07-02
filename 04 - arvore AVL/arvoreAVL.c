#include "arvoreAVL.h"

int maior(int a, int b)
{
	return (a > b) ? a : b;
}

int obter_altura(Node *N)
{
	if (N == NULL)
		return 0;
	return N->altura;
}

short fatorDeBalanceamento(Node *node)
{
	if (node == NULL)
		return 0;
	return obter_altura(node->dir) - obter_altura(node->esq);
}

Node *rotacaoSE(Node *node)
{
	Node *p = node->dir;
	Node *q = p->esq;

	p->esq = node;
	node->dir = q;

	node->altura = maior(obter_altura(node->esq), obter_altura(node->dir)) + 1;
	p->altura = maior(obter_altura(p->esq), obter_altura(p->dir)) + 1;

	return p;
}

Node *rotacaoSD(Node *node)
{
	Node *p = node->esq;
	Node *q = p->dir;

	p->dir = node;
	node->esq = q;

	node->altura = maior(obter_altura(node->esq), obter_altura(node->dir)) + 1;
	p->altura = maior(obter_altura(p->esq), obter_altura(p->dir)) + 1;

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
	if (node == NULL)
		return NULL;

	short fb = fatorDeBalanceamento(node);

	if (fb > 1 && fatorDeBalanceamento(node->dir) >= 0)
		return rotacaoSE(node);

	if (fb < -1 && fatorDeBalanceamento(node->esq) <= 0)
		return rotacaoSD(node);

	if (fb < -1 && fatorDeBalanceamento(node->esq) > 0)
		return rotacaoDE(node);

	if (fb > 1 && fatorDeBalanceamento(node->dir) < 0)
		return rotacaoDD(node);

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
	if (node == NULL)
	{
		return novo_node(cin);
	}

	if (strcmp(cin.nome, node->cin.nome) < 0)
	{
		node->esq = inserir_cin(node->esq, cin);
	}
	else if (strcmp(cin.nome, node->cin.nome) > 0)
	{
		node->dir = inserir_cin(node->dir, cin);
	}
	else
	{
		if (cin.registro < node->cin.registro)
		{
			node->esq = inserir_cin(node->esq, cin);
		}
		else if (cin.registro > node->cin.registro)
		{
			node->dir = inserir_cin(node->dir, cin);
		}
		else
		{
			return node;
		}
	}

	node->altura = 1 + maior(obter_altura(node->esq), obter_altura(node->dir));

	return balancear(node);
}

Node *remover_cin(Node *node, long registro)
{
	if (node == NULL)
	{
		return NULL;
	}

	if (registro < node->cin.registro)
	{
		node->esq = remover_cin(node->esq, registro);
	}
	else if (registro > node->cin.registro)
	{
		node->dir = remover_cin(node->dir, registro);
	}
	else
	{
		if (node->esq == NULL)
		{
			Node *temp = node->dir;
			free(node);
			return temp;
		}
		else if (node->dir == NULL)
		{
			Node *temp = node->esq;
			free(node);
			return temp;
		}

		Node *temp = node->esq;
		while (temp->dir != NULL)
		{
			temp = temp->dir;
		}
		node->cin = temp->cin;
		node->esq = remover_cin(node->esq, temp->cin.registro);
	}

	if (node == NULL)
		return NULL;

	node->altura = 1 + maior(obter_altura(node->esq), obter_altura(node->dir));

	return balancear(node);
}

Node *busca_registro(Node *arvore, long registro)
{
	if (arvore == NULL || arvore->cin.registro == registro)
		return arvore;

	if (registro < arvore->cin.registro)
		return busca_registro(arvore->esq, registro);

	return busca_registro(arvore->dir, registro);
}

void imprimir_cins(Node *arvore)
{
	if (arvore != NULL)
	{
		imprimir_cins(arvore->esq);
		printf("Registro: %ld, Nome: %s, Data de Nascimento: %d/%d/%d, Cidade: %s, Estado: %s\n",
			   arvore->cin.registro, arvore->cin.nome, arvore->cin.data[0], arvore->cin.data[1],
			   arvore->cin.data[2], arvore->cin.Naturalidade->cidade, arvore->cin.Naturalidade->estado);
		imprimir_cins(arvore->dir);
	}
}

void deleta_arvore(Node *arvore)
{
	if (arvore != NULL)
	{
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}

int calcular_idade(int anoNascimento, int anoAtual)
{
	return anoAtual - anoNascimento;
}

void relatorio_faixaEtaria(Node *raiz, int idadeInicial, int idadeFinal, int anoAtual, Node **pessoas)
{
	if (raiz != NULL)
	{
		relatorio_faixaEtaria(raiz->esq, idadeInicial, idadeFinal, anoAtual, pessoas);

		int idade = calcular_idade(raiz->cin.data[2], anoAtual);
		if (idade >= idadeInicial && idade <= idadeFinal)
		{
			inserir_cin(*pessoas, raiz->cin);
		}

		relatorio_faixaEtaria(raiz->dir, idadeInicial, idadeFinal, anoAtual, pessoas);
	}
}

void agruparPorEstado(Node *raiz, Node **pessoas)
{
	if (raiz != NULL)
	{
		agruparPorEstado(raiz->esq, pessoas);
		inserir_cin(*pessoas, raiz->cin);
		agruparPorEstado(raiz->dir, pessoas);
	}
}

void relatorio_completo(Node *raiz, int idadeInicial, int idadeFinal, int anoAtual)
{
	Node *pessoas = NULL, *pessoasAgrupadas = NULL;

	relatorio_faixaEtaria(raiz, idadeInicial, idadeFinal, anoAtual, &pessoas);

	agruparPorEstado(pessoas, &pessoasAgrupadas);

	imprimir_cins(pessoasAgrupadas);

	deleta_arvore(pessoas);
	deleta_arvore(pessoasAgrupadas);
}

int main()
{
	Node *raiz = NULL;
	CIN cin1 = {"Maria Silva", {15, 5, 2005}, {"Sao Paulo", "SP", 123456}, 1, 1};
	CIN cin2 = {"Joao Souza", {10, 3, 2006}, {"Rio de Janeiro", "RJ", 654321}, 2, 1};
	CIN cin3 = {"Ana Costa", {25, 7, 2007}, {"Belo Horizonte", "MG", 111111}, 3, 1};

	raiz = inserir_cin(raiz, cin1);
	raiz = inserir_cin(raiz, cin2);
	raiz = inserir_cin(raiz, cin3);

	int anoAtual = 2024;
	relatorio_porFaixaEtaria(raiz, 16, 18, anoAtual);

	deleta_arvore(raiz);
	return 0;
}
