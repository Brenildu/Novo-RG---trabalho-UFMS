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
		Node *novo = novo_node(cin);
		if (novo)
			return novo;
		else
			printf("Falha ao tentar alocar memória para a registro: %s\n", cin.registro);
		return NULL;
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
        // Node with only one child or no child
        if (node->esq == NULL || node->dir == NULL)
        {
            Node *temp = node->esq ? node->esq : node->dir;

            // No child case
            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else // One child case
            {
                *node = *temp; // Copy the contents of the non-empty child
            }
            free(temp);
        }
        else
        {
            // Node with two children: Get the inorder predecessor (largest in the left subtree)
            Node *temp = node->esq;
            while (temp->dir != NULL)
            {
                temp = temp->dir;
            }

            // Copy the inorder predecessor's data to this node
            node->cin = temp->cin;

            // Delete the inorder predecessor
            node->esq = remover_cin(node->esq, temp->cin.registro);
        }
    }

    // If the tree had only one node then return
    if (node == NULL)
    {
        return node;
    }

    // Update the height of the current node
    node->altura = 1 + maior(obter_altura(node->esq), obter_altura(node->dir));

    // Get the balance factor of this node to check
}


Node *busca_registro(Node *arvore, long registro)
{
	if (arvore == NULL || arvore->cin.registro == registro)
		return arvore;

	if (registro < arvore->cin.registro)
		return busca_registro(arvore->esq, registro);

	return busca_registro(arvore->dir, registro);
}

Node *procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro)
{
	Node *no = NULL;

	if (raizNovoPadrao)
	{
		no = busca_registro(raizNovoPadrao, registro);
	}
	else
	{
		no = busca_registro(raizAntigoPadrao, registro);
		if (no == NULL)
		{
			no = busca_registro(raizNovoPadrao, registro);
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
		imprimir_cins(arvore->esq);
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
	if (arvore != NULL)
	{
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}