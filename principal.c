#include<stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
	char cidade[31];
	char estado[3];
	int rg;
}Naturalidade;

typedef struct Pessoa{
	int num_registro;
	char nome[70];
	int dataNasc;
	Naturalidade naturalidade[27];
	char estadoNasc;
	struct Pessoa *prox;
}Pessoa;

void addPessoa(Pessoa **lst, Pessoa novaPessoa){
	Pessoa *q = *lst;

	Pessoa *novo = (Pessoa*)malloc(sizeof(Pessoa));
	if(novo == NULL){
		printf("Erro ao alocar a memória");
		return;
	}
	
	*novo = novaPessoa;
	novo->prox = NULL;
	
	if(*lst == NULL || (*lst)->num_registro >= novo->num_registro){
		novo->prox = *lst;
		*lst = novo;
	} else{
		while(q->prox != NULL && (q->prox)->num_registro < novo->num_registro){
			q = q->prox;
		}
		novo->prox = q->prox;
		q->prox = novo;
	}
	
	//printf("Estou no add");
}

char *readFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file, estou aqui");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);
    return data;
}

void printPessoa(Pessoa *pessoa) {
    // Implementação da impressão de pessoa
    // Substitua ou implemente conforme necessário
    printf("Nome: %s\n", pessoa->nome);
    printf("Data de Nascimento: %d\n", pessoa->dataNasc);
    printf("Naturalidade: %s, %s\n", pessoa->naturalidade->cidade, pessoa->naturalidade->estado);
    printf("RG: %d\n", pessoa->naturalidade->rg);
    printf("CPF/Num Registro: %d\n", pessoa->num_registro);
    printf("Estado de Nascimento: %c\n", pessoa->estadoNasc);
    printf("\n");
}

int parseDate(const char *date_str) {
    int day, month, year;
    sscanf(date_str, "%d/%d/%d", &day, &month, &year);
    return day * 1000000 + month * 10000 + year;
}

void *buscaPessoa(Pessoa *lst, int cpf){
	Pessoa *q;
	q = lst;
	while(q!=NULL || q->num_registro==cpf){
		if (q->num_registro == cpf) { // Considerando que o CPF está armazenado em 'num_registro'
		    printf("Busca\n");
		    printPessoa(q);
		    break;
		}
		q = q->prox;
        }
}

Pessoa *parseJSON(const char *filename, Pessoa *lst) {
    printf("Opening file: %s\n", filename);
    char *json_data = readFile(filename);
    if (json_data == NULL) {
        return NULL;
    }

    // Parsear o JSON
    struct json_object *parsed_json = json_tokener_parse(json_data);
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON data\n");
        free(json_data);
        return NULL;
    }

    // Extrair informações do JSON
    struct json_object *estado;
    struct json_object *cidadaos;
    struct json_object *cidadao;

    if (!json_object_object_get_ex(parsed_json, "uf", &estado)) {
        fprintf(stderr, "Error: missing 'uf' field\n");
        json_object_put(parsed_json);
        free(json_data);
        return NULL;
    }

    if (!json_object_object_get_ex(parsed_json, "cidadãos", &cidadaos) || !json_object_is_type(cidadaos, json_type_array)) {
        fprintf(stderr, "Error: 'cidadãos' is not an array\n");
        json_object_put(parsed_json);
        free(json_data);
        return NULL;
    }

    // Iterar sobre cada cidadão no array
    for (size_t i = 0; i < json_object_array_length(cidadaos); i++) {
        cidadao = json_object_array_get_idx(cidadaos, i);
        Pessoa pessoa;
        pessoa.prox = NULL;

        struct json_object *nome;
        struct json_object *data_nasc;
        struct json_object *rg;
        struct json_object *naturalidade;
        struct json_object *cidade;
        struct json_object *estado_nat;
        struct json_object *cpf;

        json_object_object_get_ex(cidadao, "nome", &nome);
        json_object_object_get_ex(cidadao, "data_nasc", &data_nasc);
        json_object_object_get_ex(cidadao, "rg", &rg);
        json_object_object_get_ex(cidadao, "naturalidade", &naturalidade);
        json_object_object_get_ex(cidadao, "cpf", &cpf);

        json_object_object_get_ex(naturalidade, "cidade", &cidade);
        json_object_object_get_ex(naturalidade, "estado", &estado_nat);

        // Preencher a estrutura Pessoa
        if (nome && json_object_is_type(nome, json_type_string)) {
            strncpy(pessoa.nome, json_object_get_string(nome), sizeof(pessoa.nome)-1);
            pessoa.nome[sizeof(pessoa.nome)-1] = '\0'; // Garantir terminação nula
        }

        if (data_nasc && json_object_is_type(data_nasc, json_type_string)) {
            pessoa.dataNasc = parseDate(json_object_get_string(data_nasc));
        }

        if (rg && json_object_is_type(rg, json_type_string)) {
            pessoa.naturalidade[0].rg = atoi(json_object_get_string(rg)); // Exemplo: armazenar o RG na primeira posição do array
        }

        if (cidade && json_object_is_type(cidade, json_type_string)) {
            strncpy(pessoa.naturalidade[0].cidade, json_object_get_string(cidade), sizeof(pessoa.naturalidade[0].cidade)-1);
            pessoa.naturalidade[0].cidade[sizeof(pessoa.naturalidade[0].cidade)-1] = '\0'; // Garantir terminação nula
        }

        if (estado_nat && json_object_is_type(estado_nat, json_type_string)) {
            strncpy(pessoa.naturalidade[0].estado, json_object_get_string(estado_nat), sizeof(pessoa.naturalidade[0].estado)-1);
            pessoa.naturalidade[0].estado[sizeof(pessoa.naturalidade[0].estado)-1] = '\0'; // Garantir terminação nula
        }

        if (cpf && json_object_is_type(cpf, json_type_string)) {
            pessoa.num_registro = atoi(json_object_get_string(cpf));  // Converter string para int
        }
        
        pessoa.estadoNasc = json_object_get_string(estado)[0];

        // Adicionar pessoa à lista
        addPessoa(&lst, pessoa);
    }

    json_object_put(parsed_json);
    free(json_data);

    return lst;
}


int main(){
	printf("Hello Wolrd\n");
	int aux;
	
	Pessoa *lst = NULL;
	lst = parseJSON("TO.erguf", lst);

	Pessoa *p = lst;
	while (p != NULL) {
		printPessoa(p);
		p = p->prox;
	}
    
	printf("Digite o numero de registro");
	scanf("%d",&aux);
	
	buscaPessoa(lst, aux);

    return 0;
}
