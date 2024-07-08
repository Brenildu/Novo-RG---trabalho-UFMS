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

// Calcular o valor de sequência a partir do registro
int valor_sequencia(char registro[12])
{
	// Converter a string para um número inteiro
	long long numero = atoll(registro);

	// Extrair os últimos 4 dígitos
	int ultimos4 = numero % 10000;

	// Dividir o número por 1000
	int divididoPor1000 = numero / 1000;

	// Somar os últimos 4 dígitos com o número dividido por 1000
	int valor = ultimos4 + divididoPor1000;

	return valor;
}

// Função hash para calcular a chave
int funcao_hash(char registro[])
{
	int chave = valor_sequencia(registro) % TAM;
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

// Inserção para a busca de cin
void inserir_no(No **lista, CIN pessoa)
{
	No *novoNo = criar_no(pessoa);
	if (novoNo)
	{
		novoNo->prox = *lista;
		*lista = novoNo;
	}
}

// Inserir na tabela hash
void insere_tabela(No tabela[], CIN pessoa)
{
	int chave = funcao_hash(pessoa.registro);
	inserir_no(&tabela[chave].prox, pessoa);
}

// Buscar um CIN na tabela hash
No *busca_cin(No tabela[TAM], char registro[12])
{
	No *p;
	int chave = funcao_hash(registro), i;

	p = tabela[chave].prox;
	while (p && strcmp(p->cin.registro, registro) != 0)
	{
		i++;
		p = p->prox;
	}
	return p;
}

void imprimir_cin(CIN cin)
{
	printf("\"nome\": \"%s\",\n\"cpf\": \"%s\",\n\"rg\": \"%s\",\n\"data_nasc\": \"%d/%d/%d\",\n\"naturalidade\":{\n\t\"cidade\": \"%s\",\n\t\"estado\": \"%s\"\n}\n",
		   cin.nome,
		   cin.registro,
		   cin.registros_emitidos->rg,
		   cin.data[0],
		   cin.data[1],
		   cin.data[2],
		   cin.registros_emitidos->cidade,
		   cin.registros_emitidos->estado);
}

// Criar um novo estado (adicionar inicialização da tabela)
void popular_estados(Estado estados[])
{
	int i;
	for (i = 0; i < TAM_ESTADO; i++)
	{
		popular_hash(estados[i].tabela, TAM_ALFABETO);
	}
}

// Array com as siglas dos estados brasileiros em ordem alfabética
int obter_posicao_alfabetica(char inicial)
{
	if (inicial == 'A')
	{
		return 0;
	}
	if (inicial == 'B')
	{
		return 1;
	}
	if (inicial == 'C')
	{
		return 2;
	}
	if (inicial == 'D')
	{
		return 3;
	}
	if (inicial == 'E')
	{
		return 4;
	}
	if (inicial == 'F')
	{
		return 5;
	}
	if (inicial == 'G')
	{
		return 6;
	}
	if (inicial == 'H' || inicial == 'I' || inicial == 'J' || inicial == 'K')
	{
		return 7;
	}
	if (inicial == 'L')
	{
		return 8;
	}
	if (inicial == 'M' || inicial == 'N' || inicial == 'O')
	{
		return 9;
	}
	if (inicial == 'P' || inicial == 'Q')
	{
		return 10;
	}
	if (inicial == 'R')
	{
		return 11;
	}
	if (inicial == 'S')
	{
		return 12;
	}
	if (inicial == 'T')
	{
		return 13;
	}
	if (inicial == 'U' || inicial == 'V' || inicial == 'X' || inicial == 'W' || inicial == 'Y' || inicial == 'Z')
	{
		return 14;
	}

	return -1;
}

const char *siglas_estados[] = {
	"AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA",
	"MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN",
	"RO", "RR", "RS", "SC", "SE", "SP", "TO"};

// Obter o valor correspondente ao estado
int valor_estado(const char *sigla)
{
	int i;
	for (i = 0; i < TAM_ESTADO; i++)
	{
		if (strcmp(siglas_estados[i], sigla) == 0)
		{
			return i;
		}
	}
	return -1;
}

// Inserir na lista em ordem alfabética
void inserir_lista_ordem_alfabetica(No **lista, CIN pessoa)
{
	No *p, *novoNo = criar_no(pessoa), *q;

	if (novoNo)
	{

		if (*lista == NULL || strcmp(pessoa.nome, (*lista)->cin.nome) < 0)
		{
			novoNo->prox = *lista;
			*lista = novoNo;
		}
		else
		{
			q = *lista;
			p = (*lista)->prox;
			while (p != NULL && strcmp(novoNo->cin.nome, p->cin.nome) > 0)
			{
				q = p;
				p = p->prox;
			}
			novoNo->prox = p;
			q->prox = novoNo;
		}
	}
}

// Inserir no estado
void insere_estado(Estado estados[], CIN pessoa, const char *sigla)
{
	int i = valor_estado(sigla), alf = obter_posicao_alfabetica(pessoa.nome[0]);

	if (i >= 0 && alf >= 0)
	{
		inserir_lista_ordem_alfabetica(&estados[i].tabela[alf].prox, pessoa);
	}
}

// Preencher os estados com tabelas hash
void gerar_relatorio(Estado estados[], No tabela[], int anoInicial, int anoFinal)
{
	int i;
	No *p;
	for (i = 0; i < TAM; i++)
	{
		p = tabela[i].prox;
		while (p != NULL)
		{
			if (p->cin.data[2] >= anoInicial && p->cin.data[2] <= anoFinal)
			{
				insere_estado(estados, p->cin, p->cin.registros_emitidos->estado);
			}
			p = p->prox;
		}
	}
}

void deleta_naturalidade(Naturalidade **nat)
{
	Naturalidade *aux, *remove;

	aux = *nat;
	while (aux)
	{
		remove = aux;
		aux = aux->prox;
		free(remove);
	}
}

void deleta_lista(No **lista)
{
	No *aux, *remove;

	aux = *lista;
	while (aux)
	{
		remove = aux;
		aux = aux->prox;
		free(remove);
	}
}

// Deletar a tabela hash
void deleta_tabela(No tabela[])
{
	int i;

	for (i = 0; i < TAM; i++)
	{
		deleta_naturalidade(&tabela[i].prox->cin.registros_emitidos);
		deleta_lista(&tabela[i].prox);
	}
}

void deleta_estados(Estado estados[])
{
	int i, j;

	for (i = 0; i < TAM_ESTADO; i++)
	{
		for (j = 0; j < TAM_ALFABETO; j++)
		{
			deleta_lista(&estados[i].tabela[j].prox);
			estados[i].tabela[j].prox = NULL;
		}
	}
}