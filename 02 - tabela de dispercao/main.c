#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Compilação: gcc -o main.exe *.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
*/
/*=================================================================================*/
#define TAM 1297

typedef struct
{
    char cidade[31];
    char estado[3];
    int rg;
} Naturalidade;

typedef struct
{
    char nome[70];
    int data[3];
    long registro;
    Naturalidade registros_emetidos[27];
} CIN;

typedef struct No
{
    CIN cin;
    struct No *prox;
} No;

typedef struct Estado
{
    struct No tabela[17];
} Estado;

// Array com as siglas dos estados brasileiros em ordem alfabética
const char *siglas_estados[] = {
    "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
    "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
    "RO", "RR", "RS", "SC", "SE", "SP", "TO"};

void popular_hash(No tabela[TAM]);

void popular_estados(Estado estados[], No tabela[]);

long valor_sequencia(long registro);

int obter_posicao_alfabetica(char letra);

int valor_estado(const char *sigla);

long funcao_hash(long registro);

No *criar_no(CIN cin);

Estado *criar_estado();

void inserir_ordenado(No *lista, No *novo);

void inserir_lista_ordem_alfabetica(No *lista, No *novo);

No *busca_cin(No tabela_cin[], long registro);

void insere_tabela(No tabela[], No *novo);

void insere_estado(Estado estados[], No *novo, const char *sigla);

long remover_cin(No tabela[], Estado estados[], long registro);

void remover_cin_estado(Estado estados[], No *no_removido);

void imprimir_cins_idade(No *lista, int anoInicial, int anoFinal);

void imprimir_cin(CIN cin);

void relatorio(Estado estados[], int anoInicial, int anoFinal);

void deleta_lista(No *lista);

void deleta_tabela(No tabela[]);

void deleta_estados(Estado estados[]);

/*=================================================================================*/

char *readFile(const char *filename);

void parseDate(const char *str, int data[3]);

void addCIN(No tabela[], CIN pessoa);

void parseJSON(const char *filename, No *tabela[]);

void salvarDadosTxt(No *tabela[], const char *filename);

void carregarDadosTxt(No *tabela[], const char *filename);

/*=================================================================================*/

int menuPrincipal();

long menu1();

void menu2(int *anoInicio, int *anoFinal);

/*=================================================================================*/

/*Função para iniciar o timer*/
void startTimer(clock_t *start)
{
    *start = clock();
}

// Função para parar o timer e calcular o tempo decorrido em segundos
double stopTimer(clock_t *start, clock_t *end)
{
    *end = clock();
    double elapsed = ((double)(*end - *start)) / CLOCKS_PER_SEC;
    return elapsed;
}

int main(int argc, char *argv[])
{
    clock_t tstart, tend;

    int op, anoInicio, anoFim, i;
    No tabela[TAM], *ant;
    long teste, cpf;
    Estado estados[27];

    popular_hash(tabela);
    // Carregar dados existentes
    carregarDadosTxt(tabela, "dados.txt");

    // Carregar dados dos arquivos de texto passados como argumento
    for (int i = 1; i < argc; i++)
    {
        printf("Processing file: %s\n", argv[i]);
        parseJSON(argv[i], tabela);
    }
    do
    {
        op = menuPrincipal();

        switch (op)
        {
        case 1:
            cpf = menu1();
            if (cpf == -1)
            {
                printf("Verifique se digitou certo o cpf, Operacao Invalida!!\n");
            }
            else
            {
                startTimer(&tstart);
                ant = busca_cin(tabela, cpf);
                if (ant != NULL && ant->prox != NULL && ant->prox->cin.registro == cpf)
                {
                    imprimir_cin(ant->prox->cin);
                }
                else
                {
                    printf("CPF nao encontrado\n");
                }
                double elapsed = stopTimer(&tstart, &tend);
                printf("Tempo de execucao: %.3f segundos\n\n", elapsed);
            }
            break;

        case 2:
        {
            menu2(&anoInicio, &anoFim);
            startTimer(&tstart);

            popular_estados(estados, tabela);

            relatorio(estados, anoInicio, anoFim);
            double elapsed = stopTimer(&tstart, &tend);

            deleta_estados(estados);
            printf("Tempo de execução: %.3f segundos\n\n", elapsed);
            break;
        }

        case 3:
            printf("Finalizando programa!!\n");
            salvarDadosTxt(tabela, "dados.txt");
            deleta_tabela(tabela);
            break;

        default:
            printf("Verifique se digitou corretamente, Operacao Invalida!!\n\n");
            break;
        }
    } while (op != 3);
    return 0;
}

// Preencher a tabela hash com NULL
void popular_hash(No tabela[TAM])
{
    int i;
    for (i = 0; i < TAM; i++)
    {
        tabela[i].prox = NULL;
    }
}

// Preencher os estados com tabelas hash
void popular_estados(Estado estados[], No tabela[])
{
    int i;
    No *p, *novo;
    for (i = 0; i < TAM; i++)
    {
        p = tabela[i].prox;
        while (p != NULL)
        {
            novo = criar_no(p->cin);
            insere_estado(estados, novo, novo->cin.registros_emetidos[0].estado);
            p = p->prox;
        }
    }
}

// Calcular o valor de sequência a partir do registro
long valor_sequencia(long registro)
{
    long valor = registro % 100 + registro / 100;
    return valor;
}

int obter_posicao_alfabetica(char letra)
{
    if (letra == 'a')
    {
        return 0;
    }
    if (letra == 'b')
    {
        return 1;
    }
    if (letra == 'c')
    {
        return 2;
    }
    if (letra == 'd')
    {
        return 3;
    }
    if (letra == 'e')
    {
        return 4;
    }
    if (letra == 'f')
    {
        return 5;
    }
    if (letra == 'g')
    {
        return 6;
    }
    if (letra == 'h' || letra == 'i' || letra == 'n' || letra == 'o')
    {
        return 7;
    }
    if (letra == 'j')
    {
        return 8;
    }
    if (letra == 'k' || letra == 'q' || letra == 'u' || letra == 'x' || letra == 'w' || letra == 'y' || letra == 'z')
    {
        return 9;
    }
    if (letra == 'l')
    {
        return 10;
    }
    if (letra == 'm')
    {
        return 11;
    }
    if (letra == 'p')
    {
        return 12;
    }
    if (letra == 'r')
    {
        return 13;
    }
    if (letra == 's')
    {
        return 14;
    }
    if (letra == 't')
    {
        return 15;
    }
    if (letra == 'v')
    {
        return 16;
    }
    return -1;
}

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

// Função hash para calcular a chave
long funcao_hash(long registro)
{
    long chave = valor_sequencia(registro) % TAM;
    return chave;
}

// Criar um novo nó
No *criar_no(CIN cin)
{
    No *novo = malloc(sizeof(No));

    if (!novo)
    {
        printf("Falha ao alocar memória.\n");
        return NULL;
    }

    novo->cin = cin;
    novo->prox = NULL;

    return novo;
}

// Criar um novo estado (adicionar inicialização da tabela)
Estado *criar_estado()
{
    Estado *estado = malloc(sizeof(Estado));
    if (!estado)
    {
        printf("Falha ao alocar memória para estado.\n");
        return NULL;
    }
    popular_hash(estado->tabela);
    return estado;
}

/*Inserção para a busca de cin*/
void inserir_ordenado(No *lista, No *novo)
{
    No *aux;

    if (lista == NULL || novo->cin.registro == lista->cin.registro)
    {
        novo->prox = lista->prox;
        lista->prox = novo;
    }
    else
    {
        aux = lista->prox;
        while (aux != NULL && novo->cin.registro > lista->cin.registro)
        {
            aux = aux->prox;
        }
        novo->prox = aux;
        aux = novo;
    }
    printf("Adicionado!\n");
}

// Inserir na lista em ordem alfabética
void inserir_lista_ordem_alfabetica(No *lista, No *novo)
{
    No *aux;

    if (lista == NULL || strcmp(novo->cin.nome, lista->cin.nome) < 0)
    {
        novo->prox = lista->prox;
        lista->prox = novo;
    }
    else
    {
        aux = lista->prox;
        while (aux != NULL && strcmp(novo->cin.nome, aux->cin.nome) > 0)
        {
            aux = aux->prox;
        }
        novo->prox = aux;
        aux = novo;
    }
}

// Buscar um CIN na tabela hash
No *busca_cin(No tabela_cin[], long registro)
{
    No *p, *q;
    int chave = funcao_hash(registro);

    p = tabela_cin[chave].prox;
    q = tabela_cin[chave].prox;
    while (p && p->cin.registro > registro)
    {
        q = p;
        p = p->prox;
    }

    return q;
}
/*=============================================================*/
// Inserir na tabela hash
void insere_tabela(No tabela[TAM], No *novo)
{
    long chave = funcao_hash(novo->cin.registro);
    inserir_ordenado(tabela[chave].prox, novo);
}

// Inserir no estado
void insere_estado(Estado estados[], No *novo, const char *sigla)
{
    int i = valor_estado(sigla), alf = obter_posicao_alfabetica(novo->cin.nome[0]);

    if (i >= 0 && alf >= 0)
    {
        inserir_lista_ordem_alfabetica(estados[i].tabela[alf].prox, novo);
    }
}

// Remover da tabela de CIN
long remover_cin(No tabela[], Estado estados[], long registro)
{
    No *p = tabela->prox;
    No *no_removido = NULL;
    long reg_removido = -1;

    p = busca_cin(tabela, registro);

    if (p != tabela->prox && p->prox->cin.registro == registro)
    {
        no_removido = p->prox;
        p->prox = no_removido->prox;
        reg_removido = no_removido->cin.registro;

        remover_cin_estado(estados, no_removido);
        free(no_removido);
    }
    else
    {
        if (p == tabela->prox)
        {
            no_removido = tabela->prox;
            tabela->prox = no_removido->prox;
            reg_removido = no_removido->cin.registro;

            remover_cin_estado(estados, no_removido);
            free(no_removido);
        }
    }

    return reg_removido;
}

// Remover da tabela de estados
void remover_cin_estado(Estado estados[], No *no_removido)
{
    int i, valor_sigla;
    No *p;

    if (no_removido != NULL)
    {
        valor_sigla = valor_estado(no_removido->cin.registros_emetidos[0].estado);
        p = busca_cin(estados[valor_sigla].tabela, no_removido->cin.registro);

        if (p && p->prox->cin.registro == no_removido->cin.registro)
        {
            no_removido = p->prox;
            p->prox = no_removido->prox;
            free(no_removido);
        }
        else
        {
            if (p == estados[i].tabela->prox)
            {
                no_removido = estados[i].tabela->prox;
                estados[i].tabela->prox = no_removido->prox;
                free(no_removido);
            }
        }
    }
}

// Imprimir os cidadaos com certa idade
void imprimir_cins_idade(No *lista, int anoInicial, int anoFinal)
{
    No *no = lista;

    while (no)
    {
        if (no->cin.data[2] >= anoInicial && no->cin.data[2] <= anoFinal)
        {
            imprimir_cin(no->cin);
        }

        no = no->prox;
    }
}

// Imprimir uma lista de CINs
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

/*Relatorio faixa etária divido por estados*/
void relatorio(Estado estados[], int anoInicial, int anoFinal)
{
    int i, j;

    for (i = 0; i < 27; i++)
    {
        printf("\"uf\": \"%s\",\n\"cidadaos\n: [\n", siglas_estados[i]);
        for (j = 0; j < 17; j++)
        {
            imprimir_cins_idade(estados[i].tabela[j].prox, anoInicial, anoFinal);
        }
        printf("]\n");
    }
}

// Deletar uma lista
void deleta_lista(No *lista)
{
    No *aux, *remove;

    aux = lista->prox;
    while (aux)
    {
        remove = aux;
        aux = aux->prox;
        free(remove);
    }
    lista->prox = NULL;
}

// Deletar a tabela hash
void deleta_tabela(No tabela[])
{
    int i;

    for (i = 0; i < TAM; i++)
    {
        deleta_lista(&tabela[i]);
    }
}

// Deletar os estados
void deleta_estados(Estado estados[])
{
    int i;

    for (i = 0; i < 27; i++)
    {
        deleta_tabela(estados[i].tabela);
    }
}

/*=============================================================*/

char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (data == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);
    return data;
}

// Função para converter uma string de data no formato "dd/mm/aaaa" para um array de inteiros
void parseDate(const char *str, int data[3])
{
    sscanf(str, "%d/%d/%d", &data[0], &data[1], &data[2]);
}

// Função para adicionar um novo CIN na tabela hash
void addCIN(No tabela[TAM], CIN pessoa)
{
    int hash = funcao_hash(pessoa.registro);
    No *novo = criar_no(pessoa);
    if (novo == NULL)
    {
        perror("Error allocating memory");
        return;
    }
    printf("addCin!\n");
    inserir_ordenado(tabela[hash].prox, novo);
}

// Função para analisar um arquivo JSON simples e inserir os dados na tabela hash
void parseJSON(const char *filename, No *tabela[])
{
    printf("ppp - ParseJson\n");
    char *json_data = readFile(filename);
    if (json_data == NULL)
    {
        return;
    }

    // Simulação de parsing manual de JSON
    char *ptr = strtok(json_data, "{\":,[]}");
    CIN pessoa;

    while (ptr != NULL)
    {
        if (strcmp(ptr, "nome") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            strncpy(pessoa.nome, ptr, sizeof(pessoa.nome) - 1);
            pessoa.nome[sizeof(pessoa.nome) - 1] = '\0';
        }
        else if (strcmp(ptr, "data_nasc") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            parseDate(ptr, pessoa.data);
        }
        else if (strcmp(ptr, "rg") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            pessoa.registros_emetidos[0].rg = atoi(ptr);
        }
        else if (strcmp(ptr, "cidade") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            strncpy(pessoa.registros_emetidos[0].cidade, ptr, sizeof(pessoa.registros_emetidos[0].cidade) - 1);
            pessoa.registros_emetidos[0].cidade[sizeof(pessoa.registros_emetidos[0].cidade) - 1] = '\0';
        }
        else if (strcmp(ptr, "estado") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            strncpy(pessoa.registros_emetidos[0].estado, ptr, sizeof(pessoa.registros_emetidos[0].estado) - 1);
            pessoa.registros_emetidos[0].estado[sizeof(pessoa.registros_emetidos[0].estado) - 1] = '\0';
        }
        else if (strcmp(ptr, "cpf") == 0)
        {
            ptr = strtok(NULL, "{\":,[]}");
            pessoa.registro = atol(ptr);
            // Adiciona o CIN à tabela de dispersão e aos estados
            addCIN(tabela, pessoa);
        }

        ptr = strtok(NULL, "{\":,[]}");
    }

    free(json_data);
}

void salvarDadosTxt(No *tabela[], const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < TAM; i++)
    {
        No *p = tabela[i]->prox;
        while (p != NULL)
        {
            fprintf(file, "Num Registro: %ld\n", p->cin.registro);
            fprintf(file, "Nome: %s\n", p->cin.nome);
            fprintf(file, "Data de Nascimento: %d/%d/%d\n", p->cin.data[0], p->cin.data[1], p->cin.data[2]);
            fprintf(file, "Naturalidade: %s, %s\n", p->cin.registros_emetidos[0].cidade, p->cin.registros_emetidos[0].estado);
            fprintf(file, "RG: %d\n", p->cin.registros_emetidos[0].rg);
            fprintf(file, "\n");
            p = p->prox;
        }
    }

    fclose(file);
}

// Função para carregar dados de um arquivo de texto JSON
void carregarDadosTxt(No *tabela[], const char *filename)
{
    FILE *file;
    CIN pessoa;
    int inCidadao = 0;
    char line[1024];

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    memset(&pessoa, 0, sizeof(CIN));

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Verifica se está dentro do bloco "cidadãos"
        if (strstr(line, "\"cidadãos\": ["))
        {
            inCidadao = 1;
            continue;
        }

        if (inCidadao)
        {
            // Verifica o final do bloco "cidadãos"
            if (strstr(line, "]"))
            {
                break;
            }

            // Parse "nome"
            if (strstr(line, "\"nome\":"))
            {
                sscanf(line, " \"nome\": \"%[^\"]\"", pessoa.nome);
            }
            // Parse "cpf"
            else if (strstr(line, "\"cpf\":"))
            {
                char cpf_str[12];
                sscanf(line, " \"cpf\": \"%[^\"]\"", cpf_str);
                pessoa.registro = atol(cpf_str);
            }
            // Parse "rg"
            else if (strstr(line, "\"rg\":"))
            {
                sscanf(line, " \"rg\": \"%d\"", &pessoa.registros_emetidos[0].rg);
            }
            // Parse "data_nasc"
            else if (strstr(line, "\"data_nasc\":"))
            {
                sscanf(line, " \"data_nasc\": \"%d/%d/%d\"", &pessoa.data[0], &pessoa.data[1], &pessoa.data[2]);
            }
            // Parse "naturalidade"
            else if (strstr(line, "\"naturalidade\":"))
            {
                // Skip to next lines to parse city and state
                fgets(line, sizeof(line), file);
                if (strstr(line, "\"cidade\":"))
                {
                    sscanf(line, " \"cidade\": \"%[^\"]\"", pessoa.registros_emetidos[0].cidade);
                }
                fgets(line, sizeof(line), file);
                if (strstr(line, "\"estado\":"))
                {
                    sscanf(line, " \"estado\": \"%[^\"]\"", pessoa.registros_emetidos[0].estado);
                }

                // Adiciona a pessoa na tabela
                addCIN(tabela, pessoa);

                // Limpa a estrutura para a próxima entrada
                memset(&pessoa, 0, sizeof(CIN));
            }
        }
    }

    fclose(file);
}

/*=============================================================*/

int menuPrincipal()
{
    int opcao = -1;
    printf("Menu Principal:\n-----------------------\n");
    printf("1 - Buscar por Numero de Cadastro\n");
    printf("2 - Gerar Relatorio\n");
    printf("3 - Sair\n----------------------\n");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 3)
    {
        opcao = -1;
    }
    return opcao;
}

long menu1()
{
    long cpf;
    printf("Busca por cpf cadastrados\n-------------------\n");
    printf("\nDigite o cpf(apenas numeros) que deseja buscar: ");
    scanf("%ld", &cpf);
    return cpf;
}

void menu2(int *anoInicio, int *anoFinal)
{
    printf("Relatorio de faixa idade por estados\n-------------------\n");

    printf("\nDigite o ano de inicio: ");
    scanf("%d", anoInicio);

    printf("\nDigite o ano de fim: ");
    scanf("%d", anoFinal);
}

/*=============================================================*/