#include "cJSON.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregarDados(No tabela[TAM], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
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
    if (!json) {
        printf("Erro ao fazer parsing do JSON\n");
        free(data);
        return;
    }

    cJSON *cidadaos = cJSON_GetObjectItem(json, "cidadãos");
    if (!cidadaos) {
        printf("Erro ao encontrar o item 'cidadaos' no JSON\n");
        cJSON_Delete(json);
        free(data);
        return;
    }

    cJSON *cidadao = NULL;
    cJSON_ArrayForEach(cidadao, cidadaos) {
        CIN pessoa;
        cJSON *nome = cJSON_GetObjectItem(cidadao, "nome");
        cJSON *cpf = cJSON_GetObjectItem(cidadao, "cpf");
        cJSON *rg = cJSON_GetObjectItem(cidadao, "rg");
        cJSON *data_nasc = cJSON_GetObjectItem(cidadao, "data_nasc");
        cJSON *naturalidade = cJSON_GetObjectItem(cidadao, "naturalidade");

        pessoa.registros_emitidos = malloc(sizeof(Naturalidade));
        if (pessoa.registros_emitidos == NULL) {
            printf("Erro ao alocar memória para Naturalidade\n");
            continue;
        }

        strcpy(pessoa.nome, nome->valuestring);
        strcpy(pessoa.registro, cpf->valuestring);
        sscanf(data_nasc->valuestring, "%d/%d/%d", &pessoa.data[0], &pessoa.data[1], &pessoa.data[2]);
        strcpy(pessoa.registros_emitidos->rg, rg->valuestring);
        strcpy(pessoa.registros_emitidos->cidade, cJSON_GetObjectItem(naturalidade, "cidade")->valuestring);
        strcpy(pessoa.registros_emitidos->estado, cJSON_GetObjectItem(naturalidade, "estado")->valuestring);

        insere_tabela(tabela, pessoa);
    }

    cJSON_Delete(json);
    free(data);
}   


void salvarDados(No tabela[TAM], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "{\n\t\"cidadãos\": [\n");
    for (int i = 0; i < TAM; i++) {
        No *p = tabela[i].prox;
        while (p) {
            fprintf(file, "\t\t{\n");
            fprintf(file, "\t\t\t\"nome\": \"%s\",\n", p->cin.nome);
            fprintf(file, "\t\t\t\"cpf\": \"%s\",\n", p->cin.registro);
            fprintf(file, "\t\t\t\"rg\": \"%s\",\n", p->cin.registros_emitidos->rg);
            fprintf(file, "\t\t\t\"data_nasc\": \"%02d/%02d/%04d\",\n", p->cin.data[0], p->cin.data[1], p->cin.data[2]);
            fprintf(file, "\t\t\t\"naturalidade\": {\n");
            fprintf(file, "\t\t\t\t\"cidade\": \"%s\",\n", p->cin.registros_emitidos->cidade);
            fprintf(file, "\t\t\t\t\"estado\": \"%s\"\n", p->cin.registros_emitidos->estado);
            fprintf(file, "\t\t\t}\n");
            fprintf(file, "\t\t}%s\n", (p->prox) ? "," : "");
            p = p->prox;
        }
    }
    fprintf(file, "\t]\n}\n");

    fclose(file);
}

void imprimir_relatorio_em_arquivo(Estado relatorio[], const char *filename)
{
    int i, j;
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    No *p;
    CIN cin;
    for (i = 0; i < TAM_ESTADO; i++)
    {
        for (j = 0; j < TAM_ALFABETO; j++)
        {
            p = relatorio[i].tabela[j].prox;
            if(p){
                fprintf(file, "{\"uf\": \"%s\",\n \"cidadãos\": [", p->cin.registros_emitidos->estado);
                while (p)
                {
                    cin = p->cin;
                    fprintf(file, "\n\t{\n\t\"nome\": \"%s\",\n\t\"cpf\": \"%s\",\n\t\"rg\": \"%s\",\n\t\"data_nasc\": \"%d/%d/%d\",\n\t\"naturalidade\":{\n\t\t\"cidade\": \"%s\",\n\t\t\"estado\": \"%s\"\n\t},",
                            cin.nome,
                            cin.registro,
                            cin.registros_emitidos->rg,
                            cin.data[0],
                            cin.data[1],
                            cin.data[2],
                            cin.registros_emitidos->cidade,
                            cin.registros_emitidos->estado);

                    if(p->prox && strcmp(p->prox->cin.registros_emitidos->estado, p->cin.registros_emitidos->estado) != 0){
                        fprintf(file, "\n\t]\n}\n{\"uf\": \"%s\",\n \"cidadãos\": [", p->prox->cin.registros_emitidos->estado);
                    }
                    p = p->prox;
                }
                fprintf(file, "\n\t]\n}");  
            }
        }
    }

    fclose(file);
}