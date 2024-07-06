#include "arquivos.h"

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
void addCIN(No *tabela[], CIN pessoa)
{
    int hash = funcao_hash(pessoa.registro);
    No *novo = criar_no(pessoa);
    if (novo == NULL)
    {
        perror("Error allocating memory");
        return;
    }
    printf("Adicionou!");
    inserir_ordenado(tabela[hash], novo);
}

// Função para analisar um arquivo JSON simples e inserir os dados na tabela hash
void parseJSON(const char *filename, No *tabela[])
{
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

void carregarDadosTxt(No *tabela[], const char *filename)
{
    printf("Entrou!\n");
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    CIN pessoa;
    memset(&pessoa, 0, sizeof(CIN));
    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "Num Registro: ", 14) == 0)
        {
            sscanf(line, "Num Registro: %ld", &pessoa.registro);
        }
        else if (strncmp(line, "Nome: ", 6) == 0)
        {
            sscanf(line, "Nome: %[^\n]", pessoa.nome);
        }
        else if (strncmp(line, "Data de Nascimento: ", 20) == 0)
        {
            sscanf(line, "Data de Nascimento: %d/%d/%d", &pessoa.data[0], &pessoa.data[1], &pessoa.data[2]);
        }
        else if (strncmp(line, "Naturalidade: ", 14) == 0)
        {
            sscanf(line, "Naturalidade: %[^,], %s", pessoa.registros_emetidos[0].cidade, pessoa.registros_emetidos[0].estado);
        }
        else if (strncmp(line, "RG: ", 4) == 0)
        {
            sscanf(line, "RG: %d", &pessoa.registros_emetidos[0].rg);
        }

        if (line[0] == '\n' || line[0] == '\r')
        {
            if (pessoa.nome[0] != '\0')
            {
                addCIN(tabela, pessoa);
                memset(&pessoa, 0, sizeof(CIN));
            }
        }
    }

    if (pessoa.nome[0] != '\0')
    {
        addCIN(tabela, pessoa);
        printf("adicionar!\n");
    }

    fclose(file);
}