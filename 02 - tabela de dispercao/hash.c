#include"hash.h"

void inicializar_tabela(No tabela[TAM])
{
	int i;
	
	for(i = 0; i<TAM; i++)
	{
		tabela[i].proximo = NULL;
	}
}

int valor_caractere(char c)
{
	int r;
	
	if(c == 'A')
		r = 0;
	else if(c == 'B')
		r = 1;
	else if(c == 'C')
		r = 2;
	else if(c == 'D')
		r = 3;
	else if(c == 'E')
		r = 4;
	else if(c == 'F')
		r = 5;
	else if(c == 'G')
		r = 6;
	else if(c == 'H')
		r = 7;
	else if(c == 'I')
		r = 8;
	else if(c == 'J')
		r = 9;
	else if(c == 'K')
		r = 10;
	else if(c == 'L')
		r = 11;
	else if(c == 'M')
		r = 12;
	else if(c == 'N')
		r = 13;
	else if(c == 'O')
		r = 14;
	else if(c == 'P')
		r = 15;
	else if(c == 'Q')
		r = 16;
	else if(c == 'R')
		r = 17;
	else if(c == 'S')
		r = 18;
	else if(c == 'T')
		r = 19;
	else if(c == 'U')
		r = 20;
	else if(c == 'V')
		r = 21;
	else if(c == 'W')
		r = 22;
	else if(c == 'X')
		r = 23;
	else if(c == 'Y')
		r = 24;
	else if(c == 'Z')
		r = 25;
	

	return r;
}

int funcao_hash(char placa[])
{
	int val_char1, val_char2, val_char3, chave = 0;
	
	val_char1 = valor_caractere(placa[0]);
	val_char2 = valor_caractere(placa[1]);
	val_char3 = valor_caractere(placa[2]);
	chave = (val_char1*26*26) + (val_char2*26) + val_char3;
	
	return chave;
}

void insere_tabela(No lista[], No *novo)
{
	int chave;
	
	chave = funcao_hash(novo->veiculo.placa);
	
	inserir_ordenado(&lista[chave],novo);
}

No* criar_no(Veiculo v)
{
	No *novo = malloc(sizeof(No));
	
	novo->veiculo = v;
	novo->proximo = NULL;
	
	if(!novo)
	{
		printf("Falha ao alocar um novo nó.\n");
		return NULL;
	}
	return novo;
}

void inserir_no_inicio(No *lista, No *novo)
{

	if(novo)
	{
		novo->proximo = lista->proximo;
		lista->proximo = novo;
	}
}


No* remover_veiculo(No *lista, char placa[])
{
	No *aux,*remover = NULL;
	
	if(lista->proximo)
	{
		if(strcmp(lista->proximo->veiculo.placa,placa) == 0)
		{
			remover = lista->proximo;
			lista->proximo = remover->proximo;
		}
		else
		{
			aux = lista->proximo;
			while(aux->proximo && strcmp(aux->proximo->veiculo.placa,placa) != 0)
				aux = aux->proximo;
			if(aux->proximo)
			{
				remover = aux->proximo;
				aux->proximo = remover->proximo;
			}
		}
	}
	
	
	return remover;
}

int comparaCategoria(char categoriaAnt[])
{
	int id;
	/*IDs
	0 = Mantem o anterior;
	1 = Comercial
	2 = Especiais
	3 = Oficial e Representação
	4 = Diplomático/Consular
	*/
	
	id = 0;
	
	if(strcmp(categoriaAnt, "Aluguel")==0 || strcmp(categoriaAnt, "Aprendizagem")==0)
	{
		return id = 1;
	}
	else if(strcmp(categoriaAnt, "Experiência/Fabricante")==0)
	{
		return id = 2;
	}
	else if(strcmp(categoriaAnt, "Oficial")==0 || strcmp(categoriaAnt, "Representação")==0)
	{
		return id = 3;
	}
	else if(strcmp(categoriaAnt, "Missão Diplomática")==0 || strcmp(categoriaAnt, "Corpo Consular")==0 || strcmp(categoriaAnt, "Corpo Diplomático")==0)
	{
		return id = 4;
	}
	else if(strcmp(categoriaAnt, "Organismo Consular/Internacional")==0 || strcmp(categoriaAnt, "Acordo Cooperação Internacional")==0)
	{
		return id = 4;
	}
	else
		return id;
}


/*
      Procedimento para inserir ordenado
*/
void inserir_ordenado(No *lista, No *novo)
{
    No *aux;

    if(novo)
    {
        
        if(lista->proximo == NULL)
        { /* a lista está vazia?*/
            novo->proximo = NULL;
            lista->proximo = novo;
        }
        else if(strcmp(novo->veiculo.placa,lista->proximo->veiculo.placa)<0)
        { /* é o menor?*/
            novo->proximo = lista->proximo;
            lista->proximo = novo;
        }
        else
        { /* inserção no meio ou no final da lista*/
            aux = lista->proximo;
            while(aux->proximo && strcmp(novo->veiculo.placa,aux->proximo->veiculo.placa) >= 0)
                aux = aux->proximo;
            if(strcmp(novo->veiculo.placa,aux->veiculo.placa)!=0)
            {
		    novo->proximo = aux->proximo;
		    aux->proximo = novo;
            }
        }
    }
}

No *buscarPlacaAntiga(No *listaAntigoPadrao, char placa[])
{
	No *no = NULL;

	no = listaAntigoPadrao->proximo;
	while(no)
	{
		if(strcmp(no->veiculo.placa,placa)==0)
		{
			return no;
		}
		
		no = no->proximo;
	}
	
	return no;
}

No *buscarPlacaNova(No *listaNovoPadrao, char placa[])
{
	No *no = NULL;

	no = listaNovoPadrao->proximo;
	while(no)
	{
		if(strcmp(no->veiculo.placa,placa)==0)
		{
			return no;
		}
		
		no = no->proximo;
	}
	
	return no;
}

No* buscaPlaca(No listaAntigoPadrao[], No listaNovoPadrao[], const char *placa)
{
	No *no = NULL;
	char placaS[8];
	int chave;
	
	strcpy(placaS, placa);
	chave = funcao_hash(placaS);
	if(chave < 0 || chave>=TAM)
		return NULL;
	
	if(placaS[4]>='A' && placaS[4]<='Z')
	{
		
		no = buscarPlacaNova(&listaNovoPadrao[chave],placaS);
	}
	else
	{
		no = buscarPlacaAntiga(&listaAntigoPadrao[chave],placaS);
		if(no==NULL)
		{
			placaS[4] = placaS[4] + 17;
			chave = funcao_hash(placaS);
			if(chave < 0 || chave>=TAM)
				return NULL;
			
			no = buscarPlacaNova(&listaNovoPadrao[chave],placaS);
			
		}
	}
	
	
	return no;
}


No* alterarPlaca(No listaAntigoPadrao[], No listaNovoPadrao[], const char *placa)
{
	No *alterar, *novo = NULL;
	Veiculo v;
	int retorno, chave;
	char placaS[8];
	strcpy(placaS,placa);
	
	chave = funcao_hash(placaS);
	if(chave < 0 || chave>=TAM)
		return NULL;
		
	alterar = remover_veiculo(&listaAntigoPadrao[chave],placaS);
	if(alterar)
	{
		strcpy(v.placa,alterar->veiculo.placa);
		strcpy(v.renavam,alterar->veiculo.renavam);
		strcpy(v.marca,alterar->veiculo.marca);
		strcpy(v.modelo,alterar->veiculo.modelo);
		v.ano = alterar->veiculo.ano;
		strcpy(v.cor,alterar->veiculo.cor);
		retorno = comparaCategoria(alterar->veiculo.categoria);
		if(retorno>0)
		{
			if(retorno==1)
			{
				strcpy(v.categoria,"Comercial");
			}
			else if(retorno==2)
			{
				strcpy(v.categoria,"Especiais");
			}
			else if(retorno==3)
			{
				strcpy(v.categoria,"Oficial e Representação");
			}
			else if(retorno==4)
			{
				strcpy(v.categoria,"Diplomático/Consular");
			}
		}
		else
		{
			strcpy(v.categoria,alterar->veiculo.categoria);
		}
		strcpy(v.estado,alterar->veiculo.estado);
		strcpy(v.cidade,alterar->veiculo.cidade);
		
		free(alterar);
		
		v.placa[4] = v.placa[4] + 17;
		
		novo = criar_no(v);
		if(novo)
		{
			chave = funcao_hash(v.placa);
			inserir_ordenado(&listaNovoPadrao[chave],novo);
			return novo;
		}
		else
			printf("Falha ao tentar alocar memória para a placa: %s\n",v.placa);
		return NULL;
	}
	else
		return NULL;
}

void relatorio_intervaloAnos(No listaAntigoPadrao[], No listaNovoPadrao[], int anoInicial, int anoFinal)
{
	No *no;
	int i;
	
	for(i=0;i<TAM;i++)
	{
		no = listaAntigoPadrao[i].proximo;
		while(no)
		{
			if(no->veiculo.ano >= anoInicial && no->veiculo.ano <= anoFinal)
			{	
				printf("%s;%s;%s;%s;%d;%s;%s;%s;%s\n",
				no->veiculo.placa,
				no->veiculo.renavam,
				no->veiculo.marca,
				no->veiculo.modelo,
				no->veiculo.ano,
				no->veiculo.cor,
				no->veiculo.categoria,
				no->veiculo.estado,
				no->veiculo.cidade);
			}
			
			no = no->proximo;
		}
	}
	
	for(i=0;i<TAM;i++)
	{
		no = listaNovoPadrao[i].proximo;
		while(no)
		{
			if(no->veiculo.ano >= anoInicial && no->veiculo.ano <= anoFinal)
			{	
				printf("%s;%s;%s;%s;%d;%s;%s;%s;%s\n",
				no->veiculo.placa,
				no->veiculo.renavam,
				no->veiculo.marca,
				no->veiculo.modelo,
				no->veiculo.ano,
				no->veiculo.cor,
				no->veiculo.categoria,
				no->veiculo.estado,
				no->veiculo.cidade);
			}
			
			no = no->proximo;
		}
	}
}

void relatorio_porEstado(No listaAntigoPadrao[], No listaNovoPadrao[], char estado[3])
{
	No *no;
	char finalP = '0';
	int i=0, j;
	
	while(finalP<='9')
	{
		for(j=0;j<TAM;j++)
		{
			no = listaAntigoPadrao[j].proximo;
			while(no)
			{
				if(strcmp(no->veiculo.estado,estado)==0 && no->veiculo.placa[6] == finalP)
				{	
					printf("%s;%s;%s;%s;%d;%s;%s;%s;%s\n",
					no->veiculo.placa,
					no->veiculo.renavam,
					no->veiculo.marca,
					no->veiculo.modelo,
					no->veiculo.ano,
					no->veiculo.cor,
					no->veiculo.categoria,
					no->veiculo.estado,
					no->veiculo.cidade);
				}
				
				no = no->proximo;
			}
		}
		
		for(j=0;j<TAM;j++)
		{
			no = listaNovoPadrao[j].proximo;
			while(no)
			{
				if(strcmp(no->veiculo.estado,estado)==0 && no->veiculo.placa[6] == finalP)
				{	
					printf("%s;%s;%s;%s;%d;%s;%s;%s;%s\n",
					no->veiculo.placa,
					no->veiculo.renavam,
					no->veiculo.marca,
					no->veiculo.modelo,
					no->veiculo.ano,
					no->veiculo.cor,
					no->veiculo.categoria,
					no->veiculo.estado,
					no->veiculo.cidade);
				}
				
				no = no->proximo;
			}
		}
		
		i++;
		finalP = '0' + i;
	}	
	
}


void imprimir_veiculos(No *lista)
{
	No *no = lista->proximo;
	if(no)
	{
		printf("----------------\n");
		while(no!=NULL)
		{
			printf("%s;%s;%s;%s;%d;%s;%s;%s;%s;\n",
			no->veiculo.placa,
			no->veiculo.renavam,
			no->veiculo.marca,
			no->veiculo.modelo,
			no->veiculo.ano,
			no->veiculo.cor,
			no->veiculo.categoria,
			no->veiculo.estado,
			no->veiculo.cidade);

			no = no->proximo;
		}
		printf("\n\n");
	}
}

void imprimir_tabela(No tabela[])
{
	int i;
	
	for(i=0;i<TAM;i++)
	{
		imprimir_veiculos(&tabela[i]);
	}
	
}


void libera_lista(No lista)
{
	No *aux, *remove;
	
	aux = lista.proximo;
	while(aux)
	{
		remove = aux;
		aux = aux->proximo;
		free(remove);
	}
	lista.proximo = NULL;
}

void deleta_tabela(No tabela[TAM])
{
	int i;
	
	for(i = 0; i<TAM; i++)
	{
		libera_lista(tabela[i]);
	}

}