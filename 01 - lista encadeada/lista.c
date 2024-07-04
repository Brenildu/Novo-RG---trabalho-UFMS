#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    char cidade[31];
    char estado[3];
    int rg;
} Naturalidade;

typedef struct CIN{
    long int num_registro;
    char nome[70];
    int dataNasc;
    Naturalidade naturalidade[27];
    struct CIN *prox;
} CIN;

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

void printCIN(CIN *pessoa) {
    int i;
    printf("Numero de Registro: %ld\n", pessoa->num_registro);
    printf("Nome: %s\n", pessoa->nome);
    printf("Data de Nascimento: %d\n", pessoa->dataNasc);
    
    for(i=0;i<26;i++){
    	if(pessoa->naturalidade[i].rg>0){
    		printf("RGs:\n");
    		printf("RG: %d\n", pessoa->naturalidade[i].rg);
    		printf("Naturalidade: %s, %s\n", pessoa->naturalidade[i].cidade, pessoa->naturalidade[i].estado);
    	}
    }
    
    printf("\n");
}

int parseDate(const char *date_str) {
    int day, month, year;
    sscanf(date_str, "%d/%d/%d", &day, &month, &year);
    return day * 1000000 + month * 10000 + year;
}

void buscaCIN(CIN *lst, long int cpf) {
    CIN *q = lst;
    while (q != NULL) {
        if (q->num_registro == cpf) {
            printf("CIN encontrada:\n");
            printCIN(q);
            return;
        }
        q = q->prox;
    }
    printf("CIN com CPF %ld não encontrada.\n", cpf);
}


void startTimer(struct timespec *start) {
    clock_gettime(CLOCK_MONOTONIC, start);
}

double stopTimer(struct timespec *start, struct timespec *end) {
    clock_gettime(CLOCK_MONOTONIC, end);
    double elapsed = (end->tv_sec - start->tv_sec) * 1e9; // segundos para nanosegundos
    return elapsed += (end->tv_nsec - start->tv_nsec); // nanosegundos
}

CIN *parseJSON(const char *filename, CIN *lst) {
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
        CIN pessoa;
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

	//printCIN(&pessoa);
        addCIN(&lst, pessoa);
    }

    json_object_put(parsed_json);
    free(json_data);

    return lst;
}

void salvarDadosTxt(CIN *lst, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    

    CIN *p = lst;
    while (p != NULL) {
    	
        fprintf(file, "Num Registro: %ld\n", p->num_registro);
        fprintf(file, "Nome: %s\n", p->nome);
        fprintf(file, "Data de Nascimento: %d\n", p->dataNasc);
        fprintf(file, "Naturalidade: %s, %s\n", p->naturalidade[0].cidade, p->naturalidade[0].estado);
        int i = 0;
        while (i < 27 && p->naturalidade[i].rg > 0) {
            fprintf(file, "Cidade %d: %s, %s\n", i + 1, p->naturalidade[i].cidade, p->naturalidade[i].estado);
            fprintf(file, "RG: %d\n", p->naturalidade[i].rg);
            i++;
        }

        fprintf(file, "\n");

        p = p->prox;
    }

    fclose(file);
}

CIN *carregarDadosTxt(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    CIN *lst = NULL;
    char line[1024];  // Buffer para armazenar cada linha lida do arquivo

    CIN pessoa;
    memset(&pessoa, 0, sizeof(CIN));  // Inicializa a estrutura com zeros

    int naturalidadeIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Num Registro: ", 14) == 0) {
            sscanf(line, "Num Registro: %ld", &pessoa.num_registro);
        } else if (strncmp(line, "Nome: ", 6) == 0) {
            sscanf(line, "Nome: %[^\n]", pessoa.nome);
        } else if (strncmp(line, "Data de Nascimento: ", 20) == 0) {
            sscanf(line, "Data de Nascimento: %d", &pessoa.dataNasc);
        } else if (strncmp(line, "Cidade ", 7) == 0) {
            sscanf(line, "Cidade %*d: %[^,], %s", pessoa.naturalidade[naturalidadeIndex].cidade, pessoa.naturalidade[naturalidadeIndex].estado);
        } else if (strncmp(line, "RG: ", 4) == 0) {
            sscanf(line, "RG: %d", &pessoa.naturalidade[naturalidadeIndex].rg);
            naturalidadeIndex++;
        }

        // Verifica se uma nova pessoa deve ser adicionada
        if (line[0] == '\n' || line[0] == '\r') {
            if (pessoa.nome[0] != '\0') {
                addCIN(&lst, pessoa);
                memset(&pessoa, 0, sizeof(CIN));  // Reseta a estrutura para o próximo bloco de dados
                naturalidadeIndex = 0;
            }
        }
    }

    // Adiciona a última pessoa, caso o arquivo não termine com uma nova linha
    if (pessoa.nome[0] != '\0') {
        addCIN(&lst, pessoa);
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

const char* estadosSigla[27] = {
    "AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", 
    "PB", "PR", "PE", "PI", "RJ", "RN", "RS", "RO", "RR", "SC", "SP", "SE", "TO"
};

void addCINOrdenadaPorNome(CIN **lstTemp, CIN pessoa) {
    CIN *novo = (CIN *)malloc(sizeof(CIN));
    *novo = pessoa;
    novo->prox = NULL;

    if (*lstTemp == NULL || strcmp(novo->nome, (*lstTemp)->nome) < 0) {
        novo->prox = *lstTemp;
        *lstTemp = novo;
    } else {
        CIN *atual = *lstTemp;
        while (atual->prox != NULL && strcmp(novo->nome, atual->prox->nome) >= 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

void destruirLista(CIN *lstTemp) {
    CIN *tmp;
    while (lstTemp != NULL) {
        tmp = lstTemp;
        lstTemp = lstTemp->prox;
        free(tmp);
    }
}

int calcularIdade(int anoAtual, int anoNascimento) {
    anoNascimento = anoNascimento%10000;
    return anoAtual - anoNascimento;
}

void salvarRelatorioTxt(CIN *lstPorEstado[], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 27; i++) {
        if (lstPorEstado[i] != NULL) {
            fprintf(file, "Estado %s:\n", estadosSigla[i]);
            CIN *p = lstPorEstado[i];
            while (p != NULL) {
                fprintf(file, "Num Registro: %ld\n", p->num_registro);
                fprintf(file, "Nome: %s\n", p->nome);
                fprintf(file, "Data de Nascimento: %d\n", p->dataNasc);
                // Imprimir outros dados conforme necessário
                int j = 0;
                while (j < 27 && p->naturalidade[j].rg > 0) {
                    fprintf(file, "Cidade %d: %s, %s\n", j + 1, p->naturalidade[j].cidade, p->naturalidade[j].estado);
                    fprintf(file, "RG: %d\n", p->naturalidade[j].rg);
                    j++;
                }
                p = p->prox;
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}


void gerarRelatorioIdadePorEstado(CIN *lst, int idadeMin, int idadeMax, int anoAtual) {
    CIN *lstPorEstado[27] = {NULL}; // Array de listas, um para cada estado (0-26)

    // Filtrar pessoas pela faixa etária e agrupar por estado
    while (lst != NULL) {
        int idade = calcularIdade(anoAtual, lst->dataNasc);
        if (idade >= idadeMin && idade <= idadeMax) {
            // Copiar a pessoa para evitar modificar a lista original
            CIN pessoaCopia = *lst;
            pessoaCopia.prox = NULL;

            // Determinar o índice do estado
            int estadoIdx = -1;
            int i;
            for (i = 0; i < 27; i++) {
                if (strcmp(lst->naturalidade[0].estado, estadosSigla[i]) == 0) {
                    estadoIdx = i;
                    break;
                }
            }

            if (estadoIdx >= 0 && estadoIdx < 27) {
                addCINOrdenadaPorNome(&lstPorEstado[estadoIdx], pessoaCopia);
            }
        }
        lst = lst->prox;
    }

    // Salvar relatório em arquivo de texto
    salvarRelatorioTxt(lstPorEstado, "RelatorioEstado.txt");
    // Destruir listas temporárias
    for (int i = 0; i < 27; i++) {
        destruirLista(lstPorEstado[i]);
    }
}

int main(int argc, char *argv[]) {

	struct timespec start, end;
	long long  time_used;
	long int aux;
	int op, idadeMin, idadeMax;

	CIN *lst = carregarDadosTxt("dados.txt");
    
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
			startTimer(&start);
    			buscaCIN(lst, aux);
    			time_used = stopTimer(&start, &end);
    			printf("Tempo utilizado: %lld nanosegundos\n",time_used);
		}
		if(op==2){
			printf("Digite a idade minima: ");
			scanf("%d", &idadeMin);
			printf("Digite a idade maxima: ");
			scanf("%d", &idadeMax);
			startTimer(&start);
			gerarRelatorioIdadePorEstado(lst, idadeMin, idadeMax, 2024);
			time_used = stopTimer(&start, &end);
		}
	}
    
	salvarDadosTxt(lst, "dados.txt");

	return 0;
}

