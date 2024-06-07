#include "arquivos.h"

void carregamentoERNV(No **listaAntigoPadrao, No **listaNovoPadrao, char *arquivo)
{
	FILE *ptarq;
	Veiculo v;
	No *novo;
	
	ptarq = fopen(arquivo,"r");
	if(ptarq != NULL)
	{
		
		while(fscanf(ptarq,"%[^;];%[^;];%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^\n]\n",
			v.placa,
			v.renavam,
			v.marca,
			v.modelo,
			&v.ano,
			v.cor,
			v.categoria,
			v.estado,
			v.cidade)==9)
		{
			novo = buscaPlaca(listaAntigoPadrao, listaNovoPadrao, v.placa);
			if(novo == NULL)
			{
				novo = criar_no(v);
				if(novo)
				{
					if(v.placa[4]>='A' && v.placa[4]<='Z')
					{
						inserir_ordenado(listaNovoPadrao,novo);
					}
					else if(v.placa[4]>='0' && v.placa[4]<='9')
					{
						inserir_ordenado(listaAntigoPadrao,novo);
					}
				}
				else
					printf("Falha ao tentar alocar memoria para a placa: %s\n",v.placa);;
			}
		}
			
			
			
		fclose(ptarq);
	}
	else
	{
		printf("Não foi possivel abrir o arquivo %s\n",arquivo);
		printf("Forma correta de execução: ./main.exe arquivo.ernv\n");
	}
	
}

void carregamentoBinario(No **listaAntigoPadrao, No **listaNovoPadrao)
{
	FILE *ptarq;
	Veiculo v;
	No *novo;
	
	ptarq = fopen("antigoPadrao.bin","rb");
	if(ptarq != NULL)
	{
		while(fread(&v, sizeof(Veiculo), 1, ptarq)==1)
		{
			novo = criar_no(v);
			if(novo)
				inserir_no_inicio(listaAntigoPadrao,novo);
			else
				printf("Falha ao tentar alocar memoria para a placa: %s\n",v.placa);
		}
			
			
			
		fclose(ptarq);
	}
	else
		printf("Falha ao tentar abrir o arquivo antigoPadrao.bin\n");
	
	ptarq = fopen("novoPadrao.bin","rb");
	if(ptarq != NULL)
	{
		while(fread(&v, sizeof(Veiculo), 1, ptarq)==1)
		{
			novo = criar_no(v);
			if(novo)
				inserir_no_inicio(listaNovoPadrao,novo);
			else
				printf("Falha ao tentar alocar memoria para a placa: %s\n",v.placa);
		}
			
			
			
		fclose(ptarq);
	}
	else
		printf("Falha ao tentar abrir o arquivo novoPadrao.bin\n");
	
}

void carregamento_arquivo(int argc, char *argv[], No **listaAntigoPadrao, No **listaNovoPadrao)
{
	if(argc<=1)
	{
		carregamentoBinario(listaAntigoPadrao,listaNovoPadrao);
	}
	else
	{
		carregamentoBinario(listaAntigoPadrao,listaNovoPadrao);
		carregamentoERNV(listaAntigoPadrao,listaNovoPadrao, argv[1]);
	}
}

No *inverterLista(No **listaOrdenada)
{
	No *no, *aux, *novo;
	No *listaInvertida = NULL;
	

	if(*listaOrdenada == NULL)
		return NULL;


	no = *listaOrdenada;
	while(no)
	{
		novo = criar_no(no->veiculo);
		if(novo)
			inserir_no_inicio(&listaInvertida,novo);
		else
			printf("Falha ao tentar alocar memoria para a placa: %s\n",no->veiculo.placa);
		aux = no;
		no = no->proximo;
		free(aux);
	}
	
	return listaInvertida;
}

void gravacaoBinario(No **listaAntigoPadrao, No **listaNovoPadrao)
{
	FILE *ptarq;
	No *no;
	
	*listaAntigoPadrao = inverterLista(listaAntigoPadrao);
	*listaNovoPadrao = inverterLista(listaNovoPadrao);
	ptarq = fopen("antigoPadrao.bin","wb");
	if(ptarq != NULL)
	{
	
		no = *listaAntigoPadrao;
		while(no)
		{
			fwrite(&no->veiculo, sizeof(Veiculo), 1, ptarq);
			
			
			no = no->proximo;
			
		}
		
		
			
		fclose(ptarq);
	}
	else
		printf("Falha ao tentar abrir o arquivo antigoPadrao.bin\n");
	
	ptarq = fopen("novoPadrao.bin","wb");
	if(ptarq != NULL)
	{		
		no = *listaNovoPadrao;
		while(no)
		{
			fwrite(&no->veiculo, sizeof(Veiculo), 1, ptarq);
			
			
			no = no->proximo;
		}
		
			
		fclose(ptarq);
	}
	else
		printf("Falha ao tentar abrir o arquivo novoPadrao.bin\n");
}