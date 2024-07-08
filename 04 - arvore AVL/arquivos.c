#include "cJSON.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregarDados(Node **arvore, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    if (!data) {
        perror("Erro ao alocar memória para dados");
        fclose(file);
        return;
    }
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

        *arvore = insere_avl(*arvore, pessoa, NULL); 
    }

    cJSON_Delete(json);
    free(data);
}


void salvarDados(Node *arvore, FILE *file) {
    if (arvore == NULL) {
        return;
    }

    salvarDados(arvore->esq, file);

    fprintf(file, "\t\t{\n");
    fprintf(file, "\t\t\t\"nome\": \"%s\",\n", arvore->cin.nome);
    fprintf(file, "\t\t\t\"cpf\": \"%s\",\n", arvore->cin.registro);
    fprintf(file, "\t\t\t\"rg\": \"%s\",\n", arvore->cin.registros_emitidos->rg);
    fprintf(file, "\t\t\t\"data_nasc\": \"%02d/%02d/%04d\",\n", arvore->cin.data[0], arvore->cin.data[1], arvore->cin.data[2]);
    fprintf(file, "\t\t\t\"naturalidade\": {\n");
    fprintf(file, "\t\t\t\t\"cidade\": \"%s\",\n", arvore->cin.registros_emitidos->cidade);
    fprintf(file, "\t\t\t\t\"estado\": \"%s\"\n", arvore->cin.registros_emitidos->estado);
    fprintf(file, "\t\t\t}\n");
    fprintf(file, "\t\t}%s\n", (arvore->dir || arvore->esq) ? "," : "");

    salvarDados(arvore->dir, file);
}

void salvarDadosEmArquivo(Node *arvore, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "{\n\t\"cidadãos\": [\n");
    salvarDados(arvore, file);
    fprintf(file, "\t]\n}\n");

    fclose(file);
}

void imprimir_estado(Estado *estado, FILE *file) {
    if (estado == NULL) return;

    imprimir_estado(estado->esq, file);

    if(estado->node){
        fprintf(file, "{\n\t\"uf\": \"%s\",\n\t\"cidadãos\": [", estado->sigla);
        salvarDados(estado->node, file);
        fprintf(file, "\n\t]\n}");
    }

    imprimir_estado(estado->dir, file);
}

void imprimir_relatorio_em_arquivo(Estado *relatorio, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "[\n");
    imprimir_estado(relatorio, file);
    fprintf(file, "\n]");

    fclose(file);
}