// arvoreAVL.c
#include "arvoreAVL.h"

// Função para criar um novo node
Node *criar_node(CIN cin) {
    Node *novo = (Node *)malloc(sizeof(Node));
    if (!novo) {
        printf("Falha ao alocar memória.\n");
        return NULL;
    }
    novo->cin = cin;
    novo->dir = NULL;
    novo->esq = NULL;
    novo->ancestral = NULL; // Inicializa o ponteiro ancestral como NULL
    novo->fb = 0; // Inicialmente, o fator de balanceamento é 0
    return novo;
}

// Função auxiliar para obter a altura de um nó
int altura(Node *N) {
    if (N == NULL)
        return 0;
    return N->fb;
}

// Função auxiliar para obter o fator de balanceamento de um nó
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}

// Função auxiliar para rotacionar à direita
Node *rotacao_direita(Node *y) {
    Node *x = y->esq;
    Node *T2 = x->dir;

    // Rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza os ponteiros ancestrais
    if (T2 != NULL)
        T2->ancestral = y;
    x->ancestral = y->ancestral;
    y->ancestral = x;

    // Atualiza alturas
    y->fb = max(altura(y->esq), altura(y->dir)) + 1;
    x->fb = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

// Função auxiliar para rotacionar à esquerda
Node *rotacao_esquerda(Node *x) {
    Node *y = x->dir;
    Node *T2 = y->esq;

    // Rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza os ponteiros ancestrais
    if (T2 != NULL)
        T2->ancestral = x;
    y->ancestral = x->ancestral;
    x->ancestral = y;

    // Atualiza alturas
    x->fb = max(altura(x->esq), altura(x->dir)) + 1;
    y->fb = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

// Função para atualizar o balanceamento de um node e suas subárvores
Node *atualiza_balanceamento(Node *node) {
    node->fb = 1 + max(altura(node->esq), altura(node->dir));
    int balance = getBalance(node);

    // Se o nó não está balanceado, então existem 4 casos

    // Caso Esquerda Esquerda
    if (balance > 1 && getBalance(node->esq) >= 0)
        return rotacao_direita(node);

    // Caso Direita Direita
    if (balance < -1 && getBalance(node->dir) <= 0)
        return rotacao_esquerda(node);

    // Caso Esquerda Direita
    if (balance > 1 && getBalance(node->esq) < 0) {
        node->esq = rotacao_esquerda(node->esq);
        return rotacao_direita(node);
    }

    // Caso Direita Esquerda
    if (balance < -1 && getBalance(node->dir) > 0) {
        node->dir = rotacao_direita(node->dir);
        return rotacao_esquerda(node);
    }

    return node;
}

// Função para inserir um node na árvore AVL
Node* insere_avl(Node *node, CIN cin, Node *ancestral) {
    if (node == NULL) {
        Node *novo = criar_node(cin);
        novo->ancestral = ancestral; // Define o ancestral do novo nó
        return novo;
    }

    if (strcmp(cin.registro, node->cin.registro) < 0) {
        node->esq = insere_avl(node->esq, cin, node);
    } else if (strcmp(cin.registro, node->cin.registro) > 0) {
        node->dir = insere_avl(node->dir, cin, node);
    } else {
        printf("Registro duplicado: %s\n", cin.registro);
        return node;
    }

    // Atualiza altura e balanceamento
    node->fb = 1 + max(altura(node->esq), altura(node->dir));
    return atualiza_balanceamento(node);
}

// Função para buscar um node na árvore AVL
Node *busca_cin(Node *raiz, char registro[12]) {
    if (raiz == NULL || strcmp(raiz->cin.registro, registro) == 0)
        return raiz;

    if (strcmp(registro, raiz->cin.registro) > 0)
        return busca_cin(raiz->dir, registro);
    else
        return busca_cin(raiz->esq, registro);
}

// Função para imprimir os dados de um CIN
void imprimir_cin(CIN cin) {
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
Estado *novo_estado(const char *sigla) {
    Estado *novo_estado = (Estado *)malloc(sizeof(Estado));
    if (novo_estado) {
        novo_estado->node = NULL;
        novo_estado->esq = NULL;
        novo_estado->dir = NULL;
        strcpy(novo_estado->sigla, sigla);
    }
    return novo_estado;
}

// Função para inserir um estado na árvore
Estado *inserir_estado(Estado *raiz, const char *sigla) {
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
    if (inicio > fim)
        return;

    int meio = (inicio + fim) / 2;
    *raiz = inserir_estado(*raiz, siglas_estados[meio]);

    criar_arvore_estados(raiz, inicio, meio - 1);
    criar_arvore_estados(raiz, meio + 1, fim);
}

// Função para inserir um node na árvore em ordem alfabética
Node *inserir_arvore_ordem_alfabetica(Node *arvore, CIN pessoa, Node *ancestral) {
    if (arvore == NULL) {
        Node *novo = criar_node(pessoa);
        novo->ancestral = ancestral; // Define o ancestral do novo nó
        return novo;
    }

    if (strcmp(pessoa.nome, arvore->cin.nome) < 0)
        arvore->esq = inserir_arvore_ordem_alfabetica(arvore->esq, pessoa, arvore);
    else
        arvore->dir = inserir_arvore_ordem_alfabetica(arvore->dir, pessoa, arvore);

    // Atualiza altura e verifica balanceamento a partir do nó inserido até a raiz
    return atualiza_balanceamento(arvore);
}


// Função para buscar um estado na árvore de estados
Estado *busca_estado(Estado *raiz, const char *sigla) {
    if (raiz == NULL || strcmp(raiz->sigla, sigla) == 0)
        return raiz;

    if (strcmp(sigla, raiz->sigla) < 0)
        return busca_estado(raiz->esq, sigla);
    else
        return busca_estado(raiz->dir, sigla);
}


void insere_node_estado(Estado *estados, CIN pessoa) {
    Estado *busca = busca_estado(estados, pessoa.registros_emitidos->estado);

    if (busca) {
        busca->node = inserir_arvore_ordem_alfabetica(busca->node, pessoa, NULL); // A raiz não tem ancestral
    }
}


void gerar_relatorio(Estado *estados, Node *arvore, int anoInicial, int anoFinal)
{
    if (arvore == NULL)
        return;

    if (arvore->cin.data[2] >= anoInicial && arvore->cin.data[2] <= anoFinal)
    {
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

int max(int a, int b)
{
    return (a > b) ? a : b;
}