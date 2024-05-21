#include <stdio.h>
#include <stdlib.h>
#include "instrucoes.h"
#include <stdbool.h>

#pragma once

#ifndef linhaBufferReordenacao

typedef struct Linha_Buffer_Reordenacao
{
	bool execInstrucao;   // Indica se a instrução está sendo executada
	int estadoInstrucao;	   // Estado atual da instrução
	char *resultado; // Resultado da instrução
	Instrucao *instrucao;	   // Ponteiro para a instrução
} Linha_Buffer_Reordenacao;

#endif

typedef struct Buffer_Reordenacao
{
	int maxLinhasAlocadasBr;	 // Número máximo de linhas alocadas para o buffer de reordenação
	int linhasPreenchidas;	// Número de linhas preenchidas no buffer de reordenação
	Linha_Buffer_Reordenacao linha[MAX_LINHAS]; // Array de linhas do buffer de reordenação
} Buffer_Reordenacao;

extern Buffer_Reordenacao *inicializadorBufferReordenacao(); // inicializar o buffer de reordenação
extern int inserirInstrucaoBR(Instrucao *, Buffer_Reordenacao *);  // inserir uma instrução no buffer de reordenação
extern void imprimirBufferReordenacao(Buffer_Reordenacao *); // imprimir o buffer de reordenação
extern void liberarLinhaBufferReordenacao(Buffer_Reordenacao *br, int posicaoParaLiberar); // liberar uma linha específica do buffer de reordenação
extern Buffer_Reordenacao *liberarBufferReordenacao(Buffer_Reordenacao *); // liberar todo o buffer de reordenação
