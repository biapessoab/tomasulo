#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#pragma once

typedef struct Instrucao
{
	int posBufferReordenacao; // Posição no buffer de reordenação
	Operation tipo;	// Tipo de instrução
	int emitida;  // Flag para indicar se a instrução foi emitida
	char *instCompleta; // Instrução completa
	char **instDividida;	// Instrução dividida em partes
} Instrucao;

extern Instrucao *inicializadorInstrucoes(char *);	// inicializar as instruções
extern char **lerInstrucoes(char *, int);	// ler as instruções de um arquivo
extern int numeroDeLinhas(char *);	// contar o número de linhas em um arquivo
extern char **dividirInstrucao(char *);// dividir uma instrução em partes
extern Instrucao *clonarInstrucao(Instrucao *instrucao_origem);	 // clonar uma instrução
extern int encontrarNumeroDeSaltos(Instrucao *, int, char *, int);	// encontrar o número de saltos em um programa
extern void imprimirInstrucoes(Instrucao *, char *); // imprimir as instruções