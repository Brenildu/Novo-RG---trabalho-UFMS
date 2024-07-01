#include "arvoreBinaria.h"

Node *novo_node(CIN cin)
{
	Node *novo = (Node *)malloc(sizeof(Node));
	
	if(novo)
	{
		novo->cin = cin;
		novo->dir = NULL;
		novo->esq = NULL;	
		
		return novo;
	}
	
	return NULL;
}

Node* inserir_cin(Node* node, CIN cin) 
{
	Node *novo;

	if (node == NULL) 
	{
		novo = new_node(cin);
		if(novo)
			return novo;
		else
			printf("Falha ao tentar alocar memória para o registro: %d\n",cin.registro);
		return NULL;
	}
	if(cin.registro < node->cin.registro)
	{
		node->esq = inserir_cin(node->esq, cin);
	}
	else if (cin.registro > node->cin.registro)
	{
		node->dir = inserir_cin(node->dir, cin);
	}
	return node;
}

Node* maiorValorEsq(Node* no)
{
    Node* maior = no;

    while (maior && maior->esq != NULL)
        maior = maior->esq;
  
    return maior;
}

Node* remover_cin(Node* raiz, long registro)
{
	Node* temp;

    if (raiz == NULL)
        return raiz;
  
    if (strcmp(registro, raiz->cin.registro) < 0)
        raiz->esq = remover_cin(raiz->esq, registro);
  

    else if (strcmp(registro, raiz->cin.registro) > 0)
        raiz->dir = remover_cin(raiz->dir, registro);

    else {
        if (raiz->esq == NULL) {
            temp = raiz->esq;
            free(raiz);
            return temp;
        }
        else if (raiz->dir == NULL) {
            temp = raiz->dir;
            free(raiz);
            return temp;
        }
  
        temp = maiorValorEsq(raiz->esq);
        raiz->cin = temp->cin;
        raiz->esq = remover_cin(raiz->esq, temp->cin.registro);
    }
    return raiz;
}

Node *buscaregistro(Node *arvore, long registro) 
{
	if(!arvore) 
	{
		return NULL;
	}
	
	if(registro == arvore->cin.registro) 
	{
		return arvore;
	} 
	else if(registro < arvore->cin.registro) 
	{
		return buscaregistro(arvore->esq, registro);
	} 
	else if(registro > arvore->cin.registro)
	{
		return buscaregistro(arvore->dir, registro);
	}
	return NULL;
}


Node* procurar_cin(Node *raizAntigoPadrao, Node *raizNovoPadrao, const long *registro)
{
	Node *no = NULL;
	
	if(0)
	{
		no = buscaregistro(raizNovoPadrao,registro);
	}
	else
	{
		no = buscaregistro(raizAntigoPadrao,registro);
		if(no==NULL)
		{
			no = buscaregistro(raizNovoPadrao,registro);
			
		}
	}
	
	
	return no;
}

Node* buscaCin(Node *raizAntigoPadrao, Node *raizNovoPadrao, long registro)
{
	Node *no = NULL;
	
	if()
	{
		no = buscaregistro(raizNovoPadrao,registro);
	}
	else
	{
		no = buscaregistro(raizAntigoPadrao,registro);
		if(no==NULL)
		{
			no = buscaregistro(raizNovoPadrao,registro);
			if(no)
			{
				printf("registro encontrada, ele ja foi atualizado para o novo padrão!\n");
			}
			
		}
	}
	
	
	return no;
}


Node* alterarRegistro(Node **raizAntigoPadrao, Node **raizNovoPadrao, const long *registro)
{
	Node *alterar, *novo = NULL;
	CIN cin;
	int retorno;
    long aux = registro;
	
	
}

void relatorio_anos(Node *raiz, int anoInicial, int anoFinal)
{
	
	if(raiz)
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
	
	if(raiz)
		relatorio_anos(raiz->dir, anoInicial, anoFinal);

}

void relatorio_intervaloAnos(Node *raizAntigoPadrao, Node *raizNovoPadrao, int anoInicial, int anoFinal)
{
	relatorio_anos(raizAntigoPadrao, anoInicial, anoFinal);
	
	relatorio_anos(raizNovoPadrao, anoInicial, anoFinal);
}

void relatorio_estado(Node *raiz, char estado[3], char finalP)
{
	
}

void relatorio_porEstado(Node *rootAntigoPadrao, Node *rootNovoPadrao, char estado[3])
{
	
	
}

void imprimir_cin(Node *arvore) 
{
	if(arvore) 
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
	if(arvore) 
	{
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}
