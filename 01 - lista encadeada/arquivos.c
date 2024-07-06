#include "cJSON.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregarDados(CIN **lista, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *json = cJSON_Parse(data);
    if (!json)
    {
        printf("Erro ao fazer parsing do JSON\n");
        free(data);
        return;
    }

    cJSON *cidadaos = cJSON_GetObjectItem(json, "cidadãos");
    if (!cidadaos)
    {
        printf("Erro ao encontrar o item 'cidadãos' no JSON\n");
        cJSON_Delete(json);
        free(data);
        return;
    }

    cJSON *cidadao = NULL;
    cJSON_ArrayForEach(cidadao, cidadaos)
    {
        CIN pessoa;
        cJSON *nome = cJSON_GetObjectItem(cidadao, "nome");
        cJSON *cpf = cJSON_GetObjectItem(cidadao, "cpf");
        cJSON *rg = cJSON_GetObjectItem(cidadao, "rg");
        cJSON *data_nasc = cJSON_GetObjectItem(cidadao, "data_nasc");
        cJSON *naturalidade = cJSON_GetObjectItem(cidadao, "naturalidade");

        strcpy(pessoa.nome, nome->valuestring);
        strcpy(pessoa.registro, cpf->valuestring);
        sscanf(data_nasc->valuestring, "%d/%d/%d", &pessoa.data[0], &pessoa.data[1], &pessoa.data[2]);
        strcpy(pessoa.registros_emitidos[0].rg, rg->valuestring);
        strcpy(pessoa.registros_emitidos[0].cidade, cJSON_GetObjectItem(naturalidade, "cidade")->valuestring);
        strcpy(pessoa.registros_emitidos[0].estado, cJSON_GetObjectItem(naturalidade, "estado")->valuestring);

        inserir_no(lista, pessoa);
    }

    cJSON_Delete(json);
    free(data);
}

void salvarDados(CIN *lista, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    CIN *p = lista;
    while (p)
    {
        fprintf(file, "Nome: %s\n", p->nome);
        fprintf(file, "CPF: %s\n", p->registro);
        fprintf(file, "Data de Nascimento: %02d/%02d/%04d\n", p->data[0], p->data[1], p->data[2]);
        fprintf(file, "Cidade: %s\n", p->registros_emitidos[0].cidade);
        fprintf(file, "Estado: %s\n", p->registros_emitidos[0].estado);
        fprintf(file, "RG: %s\n\n", p->registros_emitidos[0].rg);
        p = p->prox;
    }

    fclose(file);
}

void imprimir_relatorio_em_arquivo(CIN *relatorio, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    CIN *p = relatorio, cin;
    if(p){
        fprintf(file, "{\"uf\": \"%s\",\n \"cidadãos\": [", (*p).registros_emitidos[0].estado);
    }
    while (p)
    {
        cin = *p;
        fprintf(file, "\n\t{\n\t\"nome\": \"%s\",\n\t\"cpf\": \"%s\",\n\t\"rg\": \"%s\",\n\t\"data_nasc\": \"%d/%d/%d\",\n\t\"naturalidade\":{\n\t\t\"cidade\": \"%s\",\n\t\t\"estado\": \"%s\"\n\t},",
                cin.nome,
                cin.registro,
                cin.registros_emitidos[0].rg,
                cin.data[0],
                cin.data[1],
                cin.data[2],
                cin.registros_emitidos[0].cidade,
                cin.registros_emitidos[0].estado);

        if(p->prox && strcmp(p->prox->registros_emitidos[0].estado, p->registros_emitidos[0].estado) != 0){
            fprintf(file, "\n\t]\n}\n{\"uf\": \"%s\",\n \"cidadãos\": [", p->prox->registros_emitidos[0].estado);
        }
        p = p->prox;
    }
    fprintf(file, "\n\t]\n}");
    fclose(file);
}