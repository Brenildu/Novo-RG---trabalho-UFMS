#include "arvoreBinaria.h"

Node *criar_node(CIN cin)
{
	Node *novo = (Node *)malloc(sizeof(Node));

	if (!novo)
	{
		printf("Falha ao alocar memória.\n");
		return NULL;
	}

	novo->cin = cin;
	novo->dir = NULL;
	novo->esq = NULL;

	return novo;
	
}

void insere_arvore(Node **arvore, CIN cin) {
    if (*arvore == NULL) {
        *arvore = criar_node(cin);
    } else if (strcmp(cin.registro, (*arvore)->cin.registro) < 0) {
        insere_arvore(&((*arvore)->esq), cin);
    } else if (strcmp(cin.registro, (*arvore)->cin.registro) > 0) {
        insere_arvore(&((*arvore)->dir), cin);
    } else {
        // Caso em que o registro já existe, podemos decidir não fazer nada
        // ou tratar de alguma forma, por exemplo, atualizar o nó existente
        // neste exemplo, estamos apenas ignorando a inserção de duplicatas
        printf("Registro duplicado: %s\n", cin.registro);
    }
}


Node *busca_cin(Node *raiz, char registro[12])
{
	if (raiz == NULL || strcmp(raiz->cin.registro, registro) == 0)
		return raiz;


	if (strcmp(registro, raiz->cin.registro) > 0)
		return busca_cin(raiz->dir, registro);
	else
		return busca_cin(raiz->esq, registro);
}

void imprimir_cin(CIN cin)
{
	printf("\"nome\": \"%s\",\n\"cpf\": \"%s\",\n\"rg\": \"%s\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
		   cin.nome,
		   cin.registro,
		   cin.registros_emitidos->rg,
		   cin.data[0],
		   cin.data[1],
		   cin.data[2],
		   cin.registros_emitidos->cidade,
		   cin.registros_emitidos->estado);
}

// Função para criar um novo estado
Estado* novo_estado(const char* sigla) {
    Estado* novo_estado = (Estado*)malloc(sizeof(Estado));
    if (novo_estado) {
        novo_estado->node = NULL;
        novo_estado->esq = NULL;
        novo_estado->dir = NULL;
        strcpy(novo_estado->sigla, sigla);
    }
    return novo_estado;
}

// Função para inserir um estado na árvore
Estado* inserir_estado(Estado* raiz, const char* sigla) {
    if (raiz == NULL) {
        return novo_estado(sigla);
    }
    if (strcmp(sigla, raiz->sigla) < 0) {
        raiz->esq = inserir_estado(raiz->esq, sigla);
    } else {
        raiz->dir = inserir_estado(raiz->dir, sigla);
    }
    return raiz;
}
const char *siglas_estados[] = {
	"AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
	"MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
	"RO", "RR", "RS", "SC", "SE", "SP", "TO"
};

// Função para criar uma árvore balanceada de estados
void criar_arvore_estados(Estado **raiz, int inicio, int fim) {
    if (inicio > fim) return;

    int meio = (inicio + fim) / 2;
    *raiz = inserir_estado(*raiz, siglas_estados[meio]);

    criar_arvore_estados(raiz, inicio, meio - 1);
    criar_arvore_estados(raiz, meio + 1, fim);
}


// Função para inserir um node na árvore de forma alfabética
void inserir_arvore_ordem_alfabetica(Node **arvore, CIN pessoa) {
    Node *novoNode = criar_node(pessoa);

    if (novoNode) {
        if (*arvore == NULL) {
            *arvore = novoNode;
        } else {
            if (strcmp(pessoa.nome, (*arvore)->cin.nome) < 0) {
                inserir_arvore_ordem_alfabetica(&((*arvore)->esq), pessoa);
            } else {
                inserir_arvore_ordem_alfabetica(&((*arvore)->dir), pessoa);
            }
        }
    }
}

// Função para buscar um estado na árvore
Estado *busca_estado(Estado *estado, char *sigla) {
    if (estado == NULL)
        return estado;

    if (strcmp(estado->sigla, sigla) == 0)
        return estado;
    else if (strcmp(sigla, estado->sigla) > 0)
        return busca_estado(estado->dir, sigla);
    else
        return busca_estado(estado->esq, sigla);
}

void insere_node_estado(Estado *estados, CIN pessoa) {
    Estado *busca = busca_estado(estados, pessoa.registros_emitidos->estado);

    if (busca) {
        inserir_arvore_ordem_alfabetica(&(busca->node), pessoa);
    }
}


void gerar_relatorio(Estado *estados, Node *arvore, int anoInicial, int anoFinal) {
    if (arvore == NULL) return;

    if (arvore->cin.data[2] >= anoInicial && arvore->cin.data[2] <= anoFinal) {
        insere_node_estado(estados, arvore->cin);
    }
    gerar_relatorio(estados, arvore->esq, anoInicial, anoFinal);
    gerar_relatorio(estados, arvore->dir, anoInicial, anoFinal);
}

void deleta_naturalidade(Naturalidade *nat)
{
	Naturalidade *aux, *remove;

	aux = nat;
	while (aux)
	{
		remove = aux;
		aux = aux->prox;
		free(remove);
	}
}

void deleta_arvore(Node *arvore)
{
	if (arvore)
	{
		deleta_naturalidade(arvore->cin.registros_emitidos);
		deleta_arvore(arvore->esq);
		deleta_arvore(arvore->dir);
		free(arvore);
	}
}

void deleta_estados(Estado *estados)
{
	if (estados)
	{
		deleta_estados(estados->esq);
		deleta_estados(estados->dir);
		free(estados);
	}
}
