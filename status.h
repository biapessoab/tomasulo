#include <stdio.h>
#include <stdlib.h>
#include "instrucoes.h"
#include "bufferReordenacao.h"
#include <stdbool.h>

#ifndef COLUNA_STATUS_REGISTRADORES_

typedef struct colunaStatusRegistrador
{
	bool isRegistradorOcupado; // Indica se o registrador está ocupado ou não
	char *campo; // Nome do registrador
	int numEntrada; // Número da entrada na ordem de reordenação
} colunaStatusRegistrador;
#endif

typedef struct Registro_status
{
	colunaStatusRegistrador coluna[MAX_REGISTRADORES]; // Array de colunas de status de registradores
} Registro_status;

// Declaração das funções
extern Registro_status *inicializadorRegistroStatus();
extern int setStatus(char *campoRegistro, int entradaBufferReordenacao, Registro_status *registro_status);
extern int getPosicaoRegistro(char *campo, Registro_status *registro_status);
extern int getPosicaoReordenacao(char *campo, Registro_status *registro_status);
extern int liberarRegistro(char *campoRegistro, Registro_status *registro_status);
extern char *posBr(Registro_status *registro_status, char *nome_registro, int posicao_instrucao_br);
extern char *calcularResultado(Instrucao *instrucao, Registro_status *registro_status);
extern void imprimirRegistradorStatus(Registro_status* registro_status);