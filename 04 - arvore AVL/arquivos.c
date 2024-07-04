#include "arquivos.h"

// Função para ler o conteúdo de um arquivo e retornar como uma string
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

// Função para converter uma string de data no formato "dd/mm/aaaa" para um array de inteiros
void parseDate(const char *str, int data[3])
{
    sscanf(str, "%d/%d/%d", &data[0], &data[1], &data[2]);
}

// Função para adicionar um novo CIN na árvore binária de busca
void addCIN(Estado *estados, Node **cpfTree, CIN pessoa)
{
    inserir_nome(estados, pessoa);
    inserir_cin(cpfTree, pessoa);
}

// Função para analisar um arquivo JSON e inserir os dados nas duas árvores binárias
void parseJSON(const char *filename, Estado *estados, Node **cpfTree)
{
    char *json_data = readFile(filename);
    if (json_data == NULL)
    {
        return;
    }

    struct json_object *parsed_json = json_tokener_parse(json_data);
    if (parsed_json == NULL)
    {
        fprintf(stderr, "Error parsing JSON data\n");
        free(json_data);
        return;
    }

    struct json_object *estado;
    struct json_object *cidadaos;

    if (!json_object_object_get_ex(parsed_json, "uf", &estado))
    {
        fprintf(stderr, "Error: missing 'uf' field\n");
        json_object_put(parsed_json);
        free(json_data);
        return;
    }

    if (!json_object_object_get_ex(parsed_json, "cidadaos", &cidadaos) || !json_object_is_type(cidadaos, json_type_array))
    {
        fprintf(stderr, "Error: 'cidadaos' is not an array\n");
        json_object_put(parsed_json);
        free(json_data);
        return;
    }

    for (size_t i = 0; i < json_object_array_length(cidadaos); i++)
    {
        struct json_object *cidadao = json_object_array_get_idx(cidadaos, i);
        CIN pessoa;

        struct json_object *nome, *data_nasc, *rg, *naturalidade, *cidade, *estado_nat, *cpf;

        json_object_object_get_ex(cidadao, "nome", &nome);
        json_object_object_get_ex(cidadao, "data_nasc", &data_nasc);
        json_object_object_get_ex(cidadao, "rg", &rg);
        json_object_object_get_ex(cidadao, "naturalidade", &naturalidade);
        json_object_object_get_ex(cidadao, "cpf", &cpf);

        json_object_object_get_ex(naturalidade, "cidade", &cidade);
        json_object_object_get_ex(naturalidade, "estado", &estado_nat);

        if (nome && json_object_is_type(nome, json_type_string))
        {
            strncpy(pessoa.nome, json_object_get_string(nome), sizeof(pessoa.nome) - 1);
            pessoa.nome[sizeof(pessoa.nome) - 1] = '\0';
        }

        if (data_nasc && json_object_is_type(data_nasc, json_type_string))
        {
            parseDate(json_object_get_string(data_nasc), pessoa.data);
        }

        if (rg && json_object_is_type(rg, json_type_int))
        {
            pessoa.registros_emetidos[0].rg = json_object_get_int(rg);
        }

        if (cidade && json_object_is_type(cidade, json_type_string))
        {
            strncpy(pessoa.registros_emetidos[0].cidade, json_object_get_string(cidade), sizeof(pessoa.registros_emetidos[0].cidade) - 1);
            pessoa.registros_emetidos[0].cidade[sizeof(pessoa.registros_emetidos[0].cidade) - 1] = '\0';
        }

        if (estado_nat && json_object_is_type(estado_nat, json_type_string))
        {
            strncpy(pessoa.registros_emetidos[0].estado, json_object_get_string(estado_nat), sizeof(pessoa.registros_emetidos[0].estado) - 1);
            pessoa.registros_emetidos[0].estado[sizeof(pessoa.registros_emetidos[0].estado) - 1] = '\0';
        }

        if (cpf && json_object_is_type(cpf, json_type_string))
        {
            pessoa.registro = atol(json_object_get_string(cpf));
        }

        addCIN(estados, cpfTree, pessoa);
    }

    json_object_put(parsed_json);
    free(json_data);
}

// Função para salvar dados em um arquivo de texto
void salvarDadosTxt(Node *cpfTree, Estado *estados, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Função para salvar cada nó em um arquivo
    void salvarNodeTxt(Node * arvore, FILE * file)
    {
        if (arvore == NULL)
            return;

        fprintf(file, "Num Registro: %ld\n", arvore->cin.registro);
        fprintf(file, "Nome: %s\n", arvore->cin.nome);
        fprintf(file, "Data de Nascimento: %d/%d/%d\n", arvore->cin.data[0], arvore->cin.data[1], arvore->cin.data[2]);
        fprintf(file, "Naturalidade: %s, %s\n", arvore->cin.registros_emetidos[0].cidade, arvore->cin.registros_emetidos[0].estado);
        fprintf(file, "RG: %d\n", arvore->cin.registros_emetidos[0].rg);
        fprintf(file, "\n");

        salvarNodeTxt(arvore->esq, file);
        salvarNodeTxt(arvore->dir, file);
    }

    // Função para salvar cada estado em um arquivo
    void salvarEstadoTxt(Estado * estados, FILE * file)
    {
        if (estados == NULL)
            return;

        salvarNodeTxt(estados->node, file);
        salvarEstadoTxt(estados->esq, file);
        salvarEstadoTxt(estados->dir, file);
    }

    salvarEstadoTxt(estados, file);

    fclose(file);
}

// Função para carregar dados de um arquivo de texto
void carregarDadosTxt(Node **cpfTree, Estado *estados, const char *filename)
{
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
                addCIN(estados, cpfTree, pessoa);
                memset(&pessoa, 0, sizeof(CIN));
            }
        }
    }

    if (pessoa.nome[0] != '\0')
    {
        addCIN(estados, cpfTree, pessoa);
    }

    fclose(file);
}
