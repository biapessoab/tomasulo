#include "instrucoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// inicializar as instruções a partir de um arquivo
// A função lê as instruções do arquivo e inicializa a estrutura de dados Instrucao com as informações lidas
Instrucao *inicializadorInstrucoes(char *filename)
{
	// Obtém o número total de linhas no arquivo
	int numero_linhas = numeroDeLinhas(filename);

	// Aloca memória para o array de instruções
	Instrucao *instrucao = (Instrucao *)malloc(numero_linhas * sizeof(Instrucao));

	// Lê as instruções do arquivo
	char **ler_instrucoes = lerInstrucoes(filename, numero_linhas);

	// Inicializa cada instrução no array
	for (int i = 0; i < numero_linhas; i++)
	{
		// Aloca memória para a instrução completa e copia o valor lido do arquivo
		instrucao[i].instCompleta = (char *)malloc(SIZE_STR * sizeof(char));
		strcpy(instrucao[i].instCompleta, ler_instrucoes[i]);

		// Divide a instrução em partes separadas e armazena no array de strings instDividida
		instrucao[i].instDividida = dividirInstrucao(ler_instrucoes[i]);
	}

	return instrucao;
}

// contar o número de linhas em um arquivo
// A função abre o arquivo em modo de leitura e conta o número de linhas lendo cada linha do arquivo
int numeroDeLinhas(char *file_path)
{
	char foo[MAX_SIZE];
	int linhas = 0;
	FILE *file_pointer;

	file_pointer = fopen(file_path, "r");

	if (file_pointer == NULL)
	{
		printf("Erro ao abrir o arquivo.\n");
		return -1;
	}

	while (fgets(foo, MAX_SIZE, file_pointer) != NULL)
	{
		linhas++;
	}

	fclose(file_pointer);
	return linhas;
}

// ler as instruções de um arquivo
// A função abre o arquivo em modo de leitura e lê cada linha do arquivo, armazenando as instruções em um array de strings
char **lerInstrucoes(char *file_path, int numero_linhas)
{
	char **inputs_file = NULL;
	FILE *file_pointer;

	file_pointer = fopen(file_path, "r");

	if (file_pointer != NULL)
	{
		int i, line_size = 0;
		inputs_file = (char **)malloc((numero_linhas + 1) * sizeof(char *));

		for (i = 0; i < numero_linhas; i++)
		{
			inputs_file[i] = (char *)malloc(MAX_SIZE * sizeof(char));
			fgets(inputs_file[i], MAX_SIZE, file_pointer);

			line_size = strlen(inputs_file[i]);
			inputs_file[i][line_size - 1] = '\0';
		}

		inputs_file[numero_linhas] = (char *)malloc(sizeof(char));
		strcpy(inputs_file[numero_linhas], "\0");
	}
	fclose(file_pointer);
	return inputs_file;
}

// encontrar o número de saltos em um array de instruções a partir de uma determinada posição
// A função percorre o array de instruções a partir da posição inicial e conta quantas instruções contêm o label procurado
int encontrarNumeroDeSaltos(Instrucao *instrucao, int numero_instruc, char *label_instruc, int pos)
{
	int resp = 0;
	for (int i = pos + 1; i < numero_instruc; i++)
	{
		if (strstr(instrucao[i].instDividida[0], label_instruc) != NULL)
		{
			resp++;
		}
	}
	return resp;
}

// dividir uma instrução em partes separadas
// A função utiliza um delimitador para separar a instrução em partes
// Retorna um array de strings contendo as partes da instrução
char **dividirInstrucao(char *instrucao)
{
	char **splitted_string = (char **)malloc((5 * (sizeof(char *))));
	char *delimitador = " ,():";
	char *token;
	int aux = 0;

	token = strtok(instrucao, delimitador);
	while (token != NULL)
	{
		splitted_string[aux] = (char *)malloc(10 * sizeof(char));
		strcpy(splitted_string[aux], token);
		aux++;
		token = strtok(NULL, delimitador);
	}

	splitted_string[aux] = (char *)malloc(sizeof(char));
	strcpy(splitted_string[aux], "\0");

	return splitted_string;
}

// clonar uma instrução
// A função aloca memória para uma nova instrução e copia os valores da instrução de origem para a nova instrução
Instrucao *clonarInstrucao(Instrucao *origem_instrucao)
{
	Instrucao *instrucao = (Instrucao *)malloc(sizeof(Instrucao));

	instrucao->instCompleta = (char *)malloc(SIZE_STR * sizeof(char));
	strcpy(instrucao->instCompleta, origem_instrucao->instCompleta);
	instrucao->emitida = origem_instrucao->emitida;
	instrucao->posBufferReordenacao = origem_instrucao->posBufferReordenacao;
	instrucao->tipo = origem_instrucao->tipo;

	instrucao->instDividida = (char **)malloc(5 * sizeof(char *));
	for (int i = 0; i < 5; i++)
	{
		if (origem_instrucao->instDividida[i] != NULL)
		{
			instrucao->instDividida[i] = (char *)malloc(10 * sizeof(char));
			strcpy(instrucao->instDividida[i], origem_instrucao->instDividida[i]);
		}
		else
		{
			instrucao->instDividida[i] = NULL;
		}
	}

	return instrucao;
}


// imprimir as instruções
void imprimirInstrucoes(Instrucao *it, char *filePath)
{
	int size = numeroDeLinhas(filePath);
	for (int i = 0; i < size; i++)
	{
		printf("%s\n", it[i].instCompleta);
	}
}