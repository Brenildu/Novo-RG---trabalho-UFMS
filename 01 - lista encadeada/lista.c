#include "lista.h"

void teste()
{
    printf("Passou!!");
}

/*FUNÇAO INSERE ORDENADO NA LISTA
void addCIN(CIN **lst, CIN pessoa) {
    CIN *novaCIN = (CIN *)malloc(sizeof(CIN));
    if (novaCIN == NULL) {
        perror("Error allocating memory");
        return;
    }
    *novaCIN = pessoa;
    novaCIN->prox = NULL;

    // Se a lista estiver vazia ou a nova pessoa deve ser a primeira
    if (*lst == NULL || (*lst)->num_registro > pessoa.num_registro) {
        novaCIN->prox = *lst;
        *lst = novaCIN;
        return;
    }

    CIN *current = *lst;
    while (current->prox != NULL && current->prox->num_registro < pessoa.num_registro) {
        current = current->prox;
    }

    // Verifica se a pessoa já está na lista
    if (current->num_registro == pessoa.num_registro) {
        // Verifica se o RG já existe para o estado
        for (int i = 0; i < 27; i++) {
            if (strcmp(current->naturalidade[i].estado, pessoa.naturalidade[0].estado) == 0) {
                printf("A pessoa %s já possui um RG para o estado %s.\n", pessoa.nome, pessoa.naturalidade[0].estado);
                free(novaCIN);
                return;
            } else if (current->naturalidade[i].estado[0] == '\0') {
                current->naturalidade[i] = pessoa.naturalidade[0];
                printf("Novo RG adicionado para %s no estado %s.\n", pessoa.nome, pessoa.naturalidade[0].estado);
                free(novaCIN);
                return;
            }
        }
    } else {
        novaCIN->prox = current->prox;
        current->prox = novaCIN;
    }
}
*/
CIN *criar_no()
{
    CIN *novo = (CIN *)malloc(sizeof(CIN));

    if (novo != NULL)
    {
        printf("Erro ao alocar memoria!!");
        return NULL;
    }

    return novo;
}

void inserir_no(CIN **lista, CIN pessoa)
{
    CIN *novoCIN = criar_no;

    if (novoCIN)
    {
        *novoCIN = pessoa;
        novoCIN->prox = *lista;
        *lista = novoCIN;
        printf("No inserido");
    }
}
CIN *busca_cin(CIN *lista, char registro[12])
{
    CIN *cin = NULL;
    return cin;
}

void imprimir_cin(CIN cin)
{
    printf("\"nome\": \"%s\",\n\"cpf\": \"%s\",\n\"rg\": \"%s\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
           cin.nome,
           cin.registro,
           cin.registros_emitidos[0].rg,
           cin.data[0],
           cin.data[1],
           cin.data[2],
           cin.registros_emitidos[0].cidade,
           cin.registros_emitidos[0].estado);
}

void relatorio(CIN *lista, int anoInicial, int anoFinal)
{
    CIN *listaPorEstado[27] = {NULL}; // Array de listas, um para cada estado (0-26)

    // Filtrar pessoas pela faixa etária e agrupar por estado
    while (lista != NULL)
    {
        if (lista->data[2] >= anoInicial && lista->data[2] <= anoFinal)
        {
            // Copiar a pessoa para evitar modificar a lista original
            CIN pessoaCopia = *lista;
            pessoaCopia.prox = NULL;

            // Determinar o índice do estado
            int estadoIdx = -1;
            int i;
            for (i = 0; i < 27; i++)
            {
            }

            if (estadoIdx >= 0 && estadoIdx < 27)
            {
                addCINOrdenadaPorNome(&listaPorEstado[estadoIdx], pessoaCopia);
            }
        }
        lista = lista->prox;
    }

    // Salvar relatório em arquivo de texto
    salvarRelatorioTxt(listaPorEstado, "RelatorioEstado.txt");
    // Destruir listas temporárias
    for (int i = 0; i < 27; i++)
    {
        deleta_lista(listaPorEstado[i]);
    }
}

void deleta_lista(CIN *lista)
{
    CIN *tmp;
    while (lista != NULL)
    {
        tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}