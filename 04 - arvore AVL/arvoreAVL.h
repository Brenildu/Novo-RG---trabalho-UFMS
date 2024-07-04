#include "arvoreBinaria.h"

typedef struct Node {
    CIN cin;
    struct Node *esq;
    struct Node *dir;
    int altura;
} Node;

typedef struct Estado {
    int sigla;
    Node *node;
    struct Estado *esq;
    struct Estado *dir;
} Estado;

Node *novo_node(CIN cin) {
    Node *novo = (Node *)malloc(sizeof(Node));

    if (novo) {
        novo->cin = cin;
        novo->dir = NULL;
        novo->esq = NULL;
        novo->altura = 1; // novo nó é inicialmente adicionado na folha
    }

    return novo;
}

Estado *novo_estado(int valor_sigla) {
    Estado *novo_estado = (Estado *)malloc(sizeof(Estado));

    if (novo_estado) {
        novo_estado->node = NULL;
        novo_estado->dir = NULL;
        novo_estado->esq = NULL;
        novo_estado->sigla = valor_sigla;
    }
    return novo_estado;
}

const char *siglas_estados[] = {
    "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
    "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
    "RO", "RR", "RS", "SC", "SE", "SP", "TO"
};

// Obter o valor correspondente ao estado
int valor_estado(const char *sigla) {
    for (int i = 0; i < 27; i++) {
        if (strcmp(siglas_estados[i], sigla) == 0) {
            return i;
        }
    }
    return -1;
}

// Funções auxiliares para AVL
int altura(Node *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node *rotacao_direita(Node *y) {
    Node *x = y->esq;
    Node *T2 = x->dir;

    // Rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza alturas
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    // Retorna nova raiz
    return x;
}

Node *rotacao_esquerda(Node *x) {
    Node *y = x->dir;
    Node *T2 = y->esq;

    // Rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza alturas
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    // Retorna nova raiz
    return y;
}

int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}

Node *inserir_avl(Node *node, CIN cin) {
    if (node == NULL)
        return novo_node(cin);

    if (cin.registro < node->cin.registro)
        node->esq = inserir_avl(node->esq, cin);
    else if (cin.registro > node->cin.registro)
        node->dir = inserir_avl(node->dir, cin);
    else
        return node;

    // Atualiza altura deste ancestral node
    node->altura = 1 + max(altura(node->esq), altura(node->dir));

    // Obtém o fator de balanceamento deste ancestral
    int balance = getBalance(node);

    // Se o nó não está balanceado, então existem 4 casos

    // Caso Esquerda Esquerda
    if (balance > 1 && cin.registro < node->esq->cin.registro)
        return rotacao_direita(node);

    // Caso Direita Direita
    if (balance < -1 && cin.registro > node->dir->cin.registro)
        return rotacao_esquerda(node);

    // Caso Esquerda Direita
    if (balance > 1 && cin.registro > node->esq->cin.registro) {
        node->esq = rotacao_esquerda(node->esq);
        return rotacao_direita(node);
    }

    // Caso Direita Esquerda
    if (balance < -1 && cin.registro < node->dir->cin.registro) {
        node->dir = rotacao_direita(node->dir);
        return rotacao_esquerda(node);
    }

    return node;
}

Estado *busca_estado(Estado *estado, int valor_sigla, Estado *anterior) {
    if (estado == NULL)
        return anterior;

    if (estado->sigla == valor_sigla)
        return estado;
    else if (valor_sigla > estado->sigla)
        return busca_estado(estado->dir, valor_sigla, estado);
    else
        return busca_estado(estado->esq, valor_sigla, estado);
}

Estado *popular_estados() {
    Estado *raiz = novo_estado(13);

    if (raiz) {
        for (int i = 0; i < 27; i++) {
            Estado *novo = novo_estado(i);
            raiz = inserir_estado_avl(raiz, novo);
        }
    }

    return raiz;
}

Estado *inserir_estado_avl(Estado *raiz, Estado *estado) {
    if (raiz == NULL)
        return estado;

    if (estado->sigla < raiz->sigla)
        raiz->esq = inserir_estado_avl(raiz->esq, estado);
    else if (estado->sigla > raiz->sigla)
        raiz->dir = inserir_estado_avl(raiz->dir, estado);
    else
        return raiz;

    return raiz;
}

Node *busca_cin(Node *raiz, long registro, Node *anterior) {
    if (raiz == NULL)
        return anterior;

    if (raiz->cin.registro == registro)
        return raiz;

    if (registro > raiz->cin.registro)
        return busca_cin(raiz->dir, registro, raiz);
    else
        return busca_cin(raiz->esq, registro, raiz);
}

Node *busca_nome(Node *raiz, const char *nome, Node *anterior) {
    if (raiz == NULL)
        return anterior;

    if (strcmp(raiz->cin.nome, nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->cin.nome) > 0)
        return busca_nome(raiz->dir, nome, raiz);
    else
        return busca_nome(raiz->esq, nome, raiz);
}

void inserir_cin(Node **node, CIN cin) {
    *node = inserir_avl(*node, cin);
}

void inserir_nome(Estado *estados, CIN cin) {
    int valor_sigla = valor_estado(cin.registros_emetidos[0].estado);
    Estado *estado = busca_estado(estados, valor_sigla, NULL);

    if (estado == NULL) {
        printf("Estado não encontrado para o registro: %ld\n", cin.registro);
        return;
    }

    estado->node = inserir_avl(estado->node, cin);
}

Node *maior_ValorEsq(Node *no) {
    Node *maior = no->esq;

    while (maior && maior->dir != NULL)
        maior = maior->dir;

    return maior;
}

long remover_cin(Node **raiz, Estado *estados, long registro) {
    Node *no_removido = NULL;
    *raiz = remover_avl(*raiz, registro, &no_removido);

    if (no_removido != NULL) {
        remover_cin_estado(estados, no_removido);
        long reg_removido = no_removido->cin.registro;
        free(no_removido);
        return reg_removido;
    }

    return -1;
}

Node *remover_avl(Node *root, long registro, Node **no_removido) {
    if (root == NULL)
        return root;

    if (registro < root->cin.registro)
        root->esq = remover_avl(root->esq, registro, no_removido);
    else if (registro > root->cin.registro)
        root->dir = remover_avl(root->dir, registro, no_removido);
    else {
        *no_removido = root;

        if ((root->esq == NULL) || (root->dir == NULL)) {
            Node *temp = root->esq ? root->esq : root->dir;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            Node *temp = maior_ValorEsq(root);

            root->cin = temp->cin;

            root->esq = remover_avl(root->esq, temp->cin.registro, no_removido);
        }
    }

    if (root == NULL)
        return root;

    root->altura = 1 + max(altura(root->esq), altura(root->dir));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->esq) >= 0)
        return rotacao_direita(root);

    if (balance > 1 && getBalance(root->esq) < 0) {
        root->esq = rotacao_esquerda(root->esq);
        return rotacao_direita(root);
    }

    if (balance < -1 && getBalance(root->dir) <= 0)
        return rotacao_esquerda(root);

    if (balance < -1 && getBalance(root->dir) > 0) {
        root->dir = rotacao_direita(root->dir);
        return rotacao_esquerda(root);
    }

    return root;
}

void remover_cin_estado(Estado *estados, Node *no_removido) {
    int valor_sigla = valor_estado(no_removido->cin.registros_emetidos[0].estado);
    Estado *estado = busca_estado(estados, valor_sigla, NULL);

    if (estado == NULL)
        return;

    estado->node = remover_avl(estado->node, no_removido->cin.registro, &no_removido);
}

void imprimir_cins_idade(Node *arvore, int anoInicial, int anoFinal) {
    if (arvore == NULL)
        return;

    imprimir_cins_idade(arvore->esq, anoInicial, anoFinal);

    if (arvore->cin.data[2] >= anoInicial && arvore->cin.data[2] <= anoFinal)
        imprimir_cin(arvore->cin);

    imprimir_cins_idade(arvore->dir, anoInicial, anoFinal);
}

void imprimir_cin(CIN cin) {
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

void relatorio(Estado *estados, int anoInicial, int anoFinal) {
    if (estados == NULL)
        return;

    relatorio(estados->esq, anoInicial, anoFinal);
    imprimir_cins_idade(estados->node, anoInicial, anoFinal);
    relatorio(estados->dir, anoInicial, anoFinal);
}

void deleta_arvore(Node *arvore) {
    if (arvore == NULL)
        return;

    deleta_arvore(arvore->esq);
    deleta_arvore(arvore->dir);
    free(arvore);
}