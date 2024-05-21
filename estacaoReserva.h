#include <stdio.h>
#include <stdlib.h>
#include "instrucoes.h"
#include "bufferReordenacao.h"
#include <stdbool.h>

#pragma once

#ifndef linhaEstacaoReserva

typedef struct Linha_Estacao_Reserva
{
	bool isEstacaoOcupada; // Indica se a estação de reserva está ocupada ou não

	char *nome;	// Nome da estação de reserva
	Operation tipo;	// Tipo da estação de reserva
	char *operacao;	 // Operação da instrução
	char *valorVj;  // Valor do registro lido Vj
	char *valorVk;  // Valor do registro lido Vk
	int dependenciaQj; // Informação de dependência Qj
	int dependenciaQk; // Informação de dependência Qk
	int posDestino;		   // Posição de destino no buffer de reordenação
	char *end;		   // Endereço de memória
} Linha_Estacao_Reserva;

#endif


typedef struct Estacao_Reserva
{
	Linha_Estacao_Reserva linha[MAX_LINHAS_ER]; // Array de linhas da estação de reserva
} Estacao_Reserva;


extern Estacao_Reserva *inicializarEstacaoReserva();		// inicializar a estação de reserva
extern int inserirInstrucaoER(Instrucao *instrucao, Estacao_Reserva *estacaoReserva, Buffer_Reordenacao *br); // inserir uma instrução na estação de reserva
extern int searchLinhaByBr(Estacao_Reserva *er, int posicaoBR);	// encontrar a linha da estação de reserva de acordo com a posição no buffer de reordenação
extern int naoFazerEscrita(char *nomeInstrucao);			// verificar se a escrita não deve ser feita
extern int identificarWAR(char *, int, Buffer_Reordenacao *);	// identificar dependência WAR
extern void imprimirEstacaoReserva(Estacao_Reserva *);		// imprimir a estação de reserva
extern void limparLinhaER(Estacao_Reserva *estacaoReserva, int posicaoER);	// limpar uma linha da estação de reserva
