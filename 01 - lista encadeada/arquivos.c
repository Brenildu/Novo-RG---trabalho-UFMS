#include "arquivos.h"

void teste1()
{
    printf("PAssou do testee1\n");
}

char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
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

CIN *parseJSON(const char *filename, CIN **lst)
{
    CIN *lst = NULL;

    return lst;
}

CIN *carregarDadosTxt(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    CIN *lst = NULL;
    char line[1024]; // Buffer para armazenar cada linha lida do arquivo

    CIN pessoa;
    memset(&pessoa, 0, sizeof(CIN)); // Inicializa a estrutura com zeros

    int naturalidadeIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "Num Registro: ", 14) == 0)
        {
            sscanf(line, "Num Registro: %s", &pessoa.registro);
        }
        else if (strncmp(line, "Nome: ", 6) == 0)
        {
            sscanf(line, "Nome: %[^\n]", pessoa.nome);
        }
        else if (strncmp(line, "Data de Nascimento: ", 20) == 0)
        {
            sscanf(line, "Data de Nascimento: %d/%d/%d", &pessoa.data[0], &pessoa.data[1], &pessoa.data[2]);
        }
        else if (strncmp(line, "Cidade ", 7) == 0)
        {
            sscanf(line, "Cidade %*d: %[^,], %s", pessoa.registros_emitidos[0].cidade, pessoa.registros_emitidos[0].estado);
        }
        else if (strncmp(line, "RG: ", 4) == 0)
        {
            sscanf(line, "RG: %d", &pessoa.registros_emitidos[0].rg);
        }

        // Verifica se uma nova pessoa deve ser adicionada
        if (line[0] == '\n' || line[0] == '\r')
        {
            if (pessoa.nome[0] != '\0')
            {
                addCIN(&lst, pessoa);
                memset(&pessoa, 0, sizeof(CIN)); // Reseta a estrutura para o próximo bloco de dados
                naturalidadeIndex = 0;
            }
        }
    }

    // Adiciona a última pessoa, caso o arquivo não termine com uma nova linha
    if (pessoa.nome[0] != '\0')
    {
        addCIN(&lst, pessoa);
    }

    fclose(file);
    return lst;
}

void salvarRelatorioTxt(CIN *lstPorEstado[], const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 27; i++)
    {
        if (lstPorEstado[i] != NULL)
        {
            fprintf(file, "Estado :\n");
            CIN *p = lstPorEstado[i];
            while (p != NULL)
            {
                fprintf(file, "Num Registro: %s\n", p->registro);
                fprintf(file, "Nome: %s\n", p->nome);
                fprintf(file, "Data de Nascimento: %d/%d/%d\n", p->data[0], p->data[1], p->data[2]);
                // Imprimir outros dados conforme necessário
                fprintf(file, "Cidade: %s, %s\n", p->registros_emitidos[0].cidade, p->registros_emitidos[0].estado);
                fprintf(file, "RG: %s\n", p->registros_emitidos[0].rg);

                p = p->prox;
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

void salvarDadosTxt(CIN *lst, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    CIN *p = lst;
    while (p != NULL)
    {

        fprintf(file, "Num Registro: %ld\n", p->registro);
        fprintf(file, "Nome: %s\n", p->nome);
        fprintf(file, "Data de Nascimento: %d/%d/%d\n", p->data[0], p->data[1], p->data[2]);
        fprintf(file, "Naturalidade: %s, %s\n", p->registros_emitidos[0].cidade, p->registros_emitidos[0].estado);

        fprintf(file, "\n");

        p = p->prox;
    }

    fclose(file);
}
