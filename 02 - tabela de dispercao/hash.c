#include "hash.h"

// Preencher a tabela hash com NULL
void popular_hash(No tabela[], int tamanho)
{
	int i;
	for (i = 0; i < tamanho; i++)
	{
		tabela[i].prox = NULL;
	}
}

// Preencher os estados com tabelas hash
void popular_estados(Estado estados[])
{
	int i;
	for (i = 0; i < 27; i++)
	{
		popular_hash(estados[i].tabela, TAM_ALFABETICO);
	}
}

// Calcular o valor de sequência a partir do registro
long valor_sequencia(long registro)
{
	long valor = registro % 100 + registro / 100;
	return valor;
}

// Array com as siglas dos estados brasileiros em ordem alfabética
const char *siglas_estados[] = {
	"AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
	"MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
	"RO", "RR", "RS", "SC", "SE", "SP", "TO"};

int obter_posicao_alfabetica(char letra)
{
	if (letra == 'a')
	{
		return 0;
	}
	if (letra == 'b')
	{
		return 1;
	}
	if (letra == 'c')
	{
		return 2;
	}
	if (letra == 'd')
	{
		return 3;
	}
	if (letra == 'e')
	{
		return 4;
	}
	if (letra == 'f')
	{
		return 5;
	}
	if (letra == 'g')
	{
		return 6;
	}
	if (letra == 'h' || letra == 'i' || letra == 'n' || letra == 'o')
	{
		return 7;
	}
	if (letra == 'j')
	{
		return 8;
	}
	if (letra == 'k' || letra == 'q' || letra == 'u' || letra == 'x' || letra == 'x' || letra == 'w' || letra == 'y' || letra == 'z')
	{
		return 9;
	}
	if (letra == 'l')
	{
		return 10;
	}
	if (letra == 'm')
	{
		return 11;
	}
	if (letra == 'p')
	{
		return 12;
	}
	if (letra == 'r')
	{
		return 13;
	}
	if (letra == 's')
	{
		return 14;
	}
	if (letra == 't')
	{
		return 15;
	}
	if (letra == 'v')
	{
		return 16;
	}
	return -1;
}

// Obter o valor correspondente ao estado
int valor_estado(const char *sigla)
{
	int i;
	for (i = 0; i < 27; i++)
	{
		if (strcmp(siglas_estados[i], sigla) == 0)
		{
			return i;
		}
	}
	return -1;
}

// Função hash para calcular a chave
long funcao_hash(long registro)
{
	long chave = valor_sequencia(registro) % TAM;
	return chave;
}

// Criar um novo nó
No *criar_no(CIN cin)
{
	No *novo = malloc(sizeof(No));

	if (!novo)
	{
		printf("Falha ao alocar memória.\n");
		return NULL;
	}

	novo->cin = cin;
	novo->prox = NULL;

	return novo;
}

// Criar um novo estado (adicionar inicialização da tabela)
Estado *criar_estado()
{
	Estado *estado = malloc(sizeof(Estado));
	if (!estado)
	{
		printf("Falha ao alocar memória para estado.\n");
		return NULL;
	}
	popular_hash(estado->tabela, TAM_ALFABETICO);
	return estado;
}

/*Inserção para a busca de cin*/
void inserir_ordenado(No *lista, No *novo){
	No *aux;

	if (lista == NULL || novo->cin.registro == lista->cin.registro)
	{
		novo->prox = lista->prox;
		lista->prox = novo;
	}
	else
	{
		aux = lista->prox;
		while (aux != NULL && novo->cin.registro == lista->cin.registro > 0)
		{
			aux = aux->prox;
		}
		novo->prox = aux;
		aux = novo;
	}

}

// Inserir na lista em ordem alfabética
void inserir_lista_ordem_alfabetica(No *lista, No *novo)
{
	No *aux;

	if (lista == NULL || strcmp(novo->cin.nome, lista->cin.nome) < 0)
	{
		novo->prox = lista->prox;
		lista->prox = novo;
	}
	else
	{
		aux = lista->prox;
		while (aux != NULL && strcmp(novo->cin.nome, aux->cin.nome) > 0)
		{
			aux = aux->prox;
		}
		novo->prox = aux;
		aux = novo;
	}
}

// Buscar um CIN na tabela hash
No *busca_cin(No tabela_cin[], long registro)
{
	No *p, *q;
	int chave = funcao_hash(registro);

	p = tabela_cin[chave].prox;
	q = tabela_cin[chave].prox;
	while (p && p->cin.registro > registro)
	{
		q = p;
		p = p->prox;
	}

	return q;
}

// Inserir na tabela hash
void insere_tabela(No tabela[], No *novo)
{
	long chave = funcao_hash(novo->cin.registro);
	inserir_ordenado(tabela[chave].prox, novo);
}

// Inserir no estado
void insere_estado(Estado estados[], No *novo, const char *sigla)
{
	int i = valor_estado(sigla), alf = obter_posicao_alfabetica(novo->cin.nome[0]);

	if (i >= 0 && alf >= 0)
	{
		inserir_lista_ordem_alfabetica(estados[i].tabela[alf].prox, novo);
	}
}

// Remover da tabela de CIN
long remover_cin(No tabela[], Estado estados[],long registro)
{
	No *p = tabela->prox;
	No *no_removido = NULL;
	long reg_removido = -1;

	p = busca_cin(tabela, registro);

	if (p != tabela->prox && p->prox->cin.registro == registro)
	{
		no_removido = p->prox;
		p->prox = no_removido->prox;
		reg_removido = no_removido->cin.registro;

		remover_cin_estado(estados, no_removido);
		free(no_removido);
	}else{
		if(p == tabela->prox){
			no_removido = tabela->prox;
			tabela->prox = no_removido->prox;
			reg_removido = no_removido->cin.registro;

			remover_cin_estado(estados, no_removido);
			free(no_removido);
		}
	}

	return reg_removido;
}

// Remover da tabela de estados
void remover_cin_estado(Estado estados[], No *no_removido)
{
	int i, valor_sigla;
	No *p;

	if(no_removido != NULL){
		for(i = 0; i < no_removido->cin.num_registros_emetidos; i++){
			valor_sigla = valor_estado(no_removido->cin.registros_emetidos[i].estado);
			p = busca_cin(estados[valor_sigla].tabela, no_removido->cin.registro);

			if (p && p->prox->cin.registro == no_removido->cin.registro)
			{
				no_removido = p->prox;
				p->prox = no_removido->prox;
				free(no_removido);
			}else{
				if(p == estados[i].tabela->prox){
					no_removido = estados[i].tabela->prox;
					estados[i].tabela->prox = no_removido->prox;
					free(no_removido);
				}
			}
		}
	}
}


// Imprimir os cidadaos com certa idade
void imprimir_cins_idade(No *lista, int anoInicial, int anoFinal)
{
	No *no = lista;

	while (no)
	{
		if (no->cin.data[2] >= anoInicial && no->cin.data[2] <= anoFinal)
		{
			imprimir_cin(no->cin);
		}

		no = no->prox;
	}
}

// Imprimir uma lista de CINs
void imprimir_cin(CIN cin)
{
	printf("\"nome\": \"%s\",\n\"cpf\": \"%ld\",\n\"rg\": \"%d\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
		   cin.nome,
		   cin.registro,
		   cin.Naturalidade.rg,
		   cin.data[0],
		   cin.data[1],
		   cin.data[2],
		   cin.Naturalidade.cidade,
		   cin.Naturalidade.estado);
}

/*Relatorio faixa etária divido por estados*/
void relatorio(Estado estados[], int anoInicial, int anoFinal)
{
	int i, j;

	for (i = 0; i < 27; i++)
	{
		printf("\"uf\": \"%s\",\n\"cidadaos\n: [\n", siglas_estados[i]);
		for (j = 0; j < 17; j++)
		{
			imprimir_cins_idade(estados[i].tabela[j].prox, anoInicial, anoFinal);
		}
		printf("]\n");
	}
}

// Deletar uma lista
void deleta_lista(No *lista)
{
	No *aux, *remove;

	aux = lista->prox;
	while (aux)
	{
		remove = aux;
		aux = aux->prox;
		free(remove);
	}
	lista->prox = NULL;
}

// Deletar a tabela hash
void deleta_tabela(No tabela[])
{
	int i;

	for (i = 0; i < TAM; i++)
	{
		deleta_lista(&tabela[i]);
	}
}

// Deletar os estados
void deleta_estados(Estado estados[])
{
	int i;

	for (i = 0; i < 27; i++)
	{
		deleta_tabela(estados[i].tabela);
	}
}
