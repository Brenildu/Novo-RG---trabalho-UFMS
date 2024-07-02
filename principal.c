#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char cidade[31];
    char estado[3];
    int rg;
} Naturalidade;

typedef struct Pessoa{
    long int num_registro;
    char nome[70];
    int dataNasc;
    Naturalidade naturalidade[27];
    char estadoNasc;
    struct Pessoa *prox;
} Pessoa;

void addPessoa(Pessoa **lst, Pessoa pessoa) {
    Pessoa *novaPessoa = (Pessoa *)malloc(sizeof(Pessoa));
    if (novaPessoa == NULL) {
        perror("Error allocating memory");
        return;
    }
    *novaPessoa = pessoa;
    novaPessoa->prox = NULL;

    // Se a lista estiver vazia ou a nova pessoa deve ser a primeira
    if (*lst == NULL || (*lst)->num_registro > pessoa.num_registro) {
        novaPessoa->prox = *lst;
        *lst = novaPessoa;
        return;
    }

    Pessoa *current = *lst;
    while (current->prox != NULL && current->prox->num_registro < pessoa.num_registro) {
        current = current->prox;
    }

    // Verifica se a pessoa já está na lista
    if (current->num_registro == pessoa.num_registro) {
        // Verifica se o RG já existe para o estado
        for (int i = 0; i < 27; i++) {
            if (strcmp(current->naturalidade[i].estado, pessoa.naturalidade[0].estado) == 0) {
                printf("A pessoa %s já possui um RG para o estado %s.\n", pessoa.nome, pessoa.naturalidade[0].estado);
                free(novaPessoa);
                return;
            } else if (current->naturalidade[i].estado[0] == '\0') {
                current->naturalidade[i] = pessoa.naturalidade[0];
                printf("Novo RG adicionado para %s no estado %s.\n", pessoa.nome, pessoa.naturalidade[0].estado);
                free(novaPessoa);
                return;
            }
        }
    } else {
        novaPessoa->prox = current->prox;
        current->prox = novaPessoa;
    }
}

char *readFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
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
    printf("CPF/Num Registro: %ld\n", pessoa->num_registro);
    printf("Nome: %s\n", pessoa->nome);
    printf("Data de Nascimento: %d\n", pessoa->dataNasc);
    printf("Naturalidade: %s, %s\n", pessoa->naturalidade->cidade, pessoa->naturalidade->estado);
    printf("RG: %d\n", pessoa->naturalidade->rg);
    printf("Estado de Nascimento: %c\n", pessoa->estadoNasc);
    printf("\n");
}

int parseDate(const char *date_str) {
    int day, month, year;
    sscanf(date_str, "%d/%d/%d", &day, &month, &year);
    return day * 1000000 + month * 10000 + year;
}

void buscaPessoa(Pessoa *lst, long int cpf) {
    Pessoa *q = lst;
    while (q != NULL) {
        if (q->num_registro == cpf) {
            printf("Pessoa encontrada:\n");
            printPessoa(q);
            return;
        }
        q = q->prox;
    }
    printf("Pessoa com CPF %ld não encontrada.\n", cpf);
}

Pessoa *parseJSON(const char *filename, Pessoa *lst) {
    char *json_data = readFile(filename);
    if (json_data == NULL) {
        return NULL;
    }

    struct json_object *parsed_json = json_tokener_parse(json_data);
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON data\n");
        free(json_data);
        return NULL;
    }

    struct json_object *estado;
    struct json_object *cidadaos;

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

    for (size_t i = 0; i < json_object_array_length(cidadaos); i++) {
        struct json_object *cidadao = json_object_array_get_idx(cidadaos, i);
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

        if (nome && json_object_is_type(nome, json_type_string)) {
            strncpy(pessoa.nome, json_object_get_string(nome), sizeof(pessoa.nome)-1);
            pessoa.nome[sizeof(pessoa.nome)-1] = '\0';
        }

        if (data_nasc && json_object_is_type(data_nasc, json_type_string)) {
            pessoa.dataNasc = parseDate(json_object_get_string(data_nasc));
        }

        if (rg && json_object_is_type(rg, json_type_string)) {
            pessoa.naturalidade->rg = atoi(json_object_get_string(rg));
        }

        if (cidade && json_object_is_type(cidade, json_type_string)) {
            strncpy(pessoa.naturalidade->cidade, json_object_get_string(cidade), sizeof(pessoa.naturalidade->cidade)-1);
            pessoa.naturalidade->cidade[sizeof(pessoa.naturalidade->cidade)-1] = '\0';
        }

        if (estado_nat && json_object_is_type(estado_nat, json_type_string)) {
            strncpy(pessoa.naturalidade->estado, json_object_get_string(estado_nat), sizeof(pessoa.naturalidade->estado)-1);
            pessoa.naturalidade->estado[sizeof(pessoa.naturalidade->estado)-1] = '\0';
        }

        if (cpf && json_object_is_type(cpf, json_type_string)) {
            //printf("CPF (antes da conversão): %s\n", json_object_get_string(cpf));  // Debug: Verificar valor do CPF
            pessoa.num_registro = atol(json_object_get_string(cpf));
            //printf("CPF (depois da conversão): %ld\n", pessoa.num_registro);  // Debug: Verificar valor do CPF convertido
        }

        pessoa.estadoNasc = json_object_get_string(estado)[0];

	//printPessoa(&pessoa);
        addPessoa(&lst, pessoa);
    }

    json_object_put(parsed_json);
    free(json_data);

    return lst;
}

void salvarDadosTxt(Pessoa *lst, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Pessoa *p = lst;
    while (p != NULL) {
        fprintf(file, "Num Registro: %ld\n", p->num_registro);
        fprintf(file, "Nome: %s\n", p->nome);
        fprintf(file, "Data de Nascimento: %d\n", p->dataNasc);
        fprintf(file, "Naturalidade: %s, %s\n", p->naturalidade->cidade, p->naturalidade->estado);
        fprintf(file, "RG: %d\n", p->naturalidade->rg);
        fprintf(file, "Estado de Nascimento: %c\n", p->estadoNasc);
        fprintf(file, "\n");

        p = p->prox;
    }

    fclose(file);
}

Pessoa *carregarDadosTxt(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    Pessoa *lst = NULL;
    char line[1024];  // Buffer para armazenar cada linha lida do arquivo

    Pessoa pessoa;
    memset(&pessoa, 0, sizeof(Pessoa));  // Inicializa a estrutura com zeros

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Num Registro: ", 14) == 0) {
            sscanf(line, "Num Registro: %ld", &pessoa.num_registro);
        } else if (strncmp(line, "Nome: ", 6) == 0) {
            sscanf(line, "Nome: %[^\n]", pessoa.nome);
        } else if (strncmp(line, "Data de Nascimento: ", 20) == 0) {
            sscanf(line, "Data de Nascimento: %d", &pessoa.dataNasc);
        } else if (strncmp(line, "Naturalidade: ", 14) == 0) {
            sscanf(line, "Naturalidade: %[^,], %s", pessoa.naturalidade->cidade, pessoa.naturalidade->estado);
        } else if (strncmp(line, "RG: ", 4) == 0) {
            sscanf(line, "RG: %d", &pessoa.naturalidade->rg);
        } else if (strncmp(line, "Estado de Nascimento: ", 21) == 0) {
            sscanf(line, "Estado de Nascimento: %c", &pessoa.estadoNasc);
        }

        if (line[0] == '\n' || line[0] == '\r') {
            if (pessoa.nome[0] != '\0') {
                addPessoa(&lst, pessoa);
                memset(&pessoa, 0, sizeof(Pessoa));  // Reseta a estrutura para o próximo bloco de dados
            }
        }
    }

    // Adiciona a última pessoa, caso o arquivo não termine com uma nova linha
    if (pessoa.nome[0] != '\0') {
        addPessoa(&lst, pessoa);
    }

    fclose(file);
    return lst;
}

void menu(){
	printf("Olá, seja bem-vindo ao Cadastro Nova Carteira de Identidade Nacional\n\nPor favor, selecione a operação desejada:\n");
	printf("1 - Buscar por Número de Cadastro\n");
	printf("2 - Gerar Relatório\n");
	printf("3 - Sair\n");
}

int main(int argc, char *argv[]) {
	//printf("Hello World\n");
	long int aux;
	int op;

	Pessoa *lst = carregarDadosTxt("dados.txt");
    
    
    	for (int i = 1; i < argc; i++) {
        	printf("Processing file: %s\n", argv[i]);
        	lst = parseJSON(argv[i], lst);
   	}
	while(op!=3)
	{
		menu();
		scanf("%d", &op);
		if(op==1){
			printf("Digite o numero de registro: ");
    			scanf("%ld", &aux);

    			buscaPessoa(lst, aux);
		}
		if(op==2){
			printf("Relatorio");
		}
	}
    
	salvarDadosTxt(lst, "dados.txt");

	return 0;
}

