#include "bufferReordenacao.h"
#include "instrucoes.h"
#include "status.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// inicializar o Registro_status
Registro_status *inicializadorRegistroStatus()
{
	// Aloca memória para o Registro_status
	Registro_status *rStatus = (Registro_status *)malloc(sizeof(Registro_status));

	// Loop para inicializar cada coluna do Registro_status
	for (int i = 0; i < MAX_REGISTRADORES; i++)
	{
		// Aloca memória para o campo do registro
		char *f = (char *)malloc(4 * sizeof(char));
		char *F = (char *)malloc(4 * sizeof(char));
		F[0] = 'R';
		F[1] = '\0';
		// Converte o número do registro para string e concatena com 'R'
		sprintf(f, "%d", i);
		strcat(F, f);
		strcat(F, "\0");

		// Atribui o campo, numEntrada e isRegistradorOcupado para a coluna atual
		rStatus->coluna[i].campo = F;
		rStatus->coluna[i].numEntrada = -1;
		rStatus->coluna[i].isRegistradorOcupado = false;
	}

	return rStatus;
}

// encontrar a posição de um registro no Registro_status
int getPosicaoRegistro(char *campo, Registro_status *Registro_status)
{
	int pos = -1;

	// Percorre todas as colunas do Registro_status
	for (int i = 0; i < MAX_REGISTRADORES; i++)
	{
		// Verifica se o campo do registro é igual ao campo fornecido
		if (strcmp(campo, Registro_status->coluna[i].campo) == 0)
		{
			pos = i;
			break;
		}
	}

	return pos;
}
// obter a posição de reordenação de um registro no Registro_status
int getPosicaoReordenacao(char *campo, Registro_status *Registro_status)
{
	int reordenar_pos = -1;
	int pos_registrador = getPosicaoRegistro(campo, Registro_status);

	// Verifica se o registro está ocupado e se possui uma posição de reordenação válida
	if (pos_registrador != -1 && Registro_status->coluna[pos_registrador].isRegistradorOcupado == true)
	{
		reordenar_pos = Registro_status->coluna[pos_registrador].numEntrada;
	}

	return reordenar_pos;
}

// obter o nome do registro ou a posição de reordenação
char *posBr(Registro_status *Registro_status, char *name_register, int position_instruction_br)
{
	// Aloca memória para o resultado
	char *result = (char *)malloc(SIZE_STR * sizeof(char));
	// Obtém a posição de reordenação do registro
	int reorder_position_register = getPosicaoReordenacao(name_register, Registro_status);
	// Aloca memória para a posição
	char *position = (char *)malloc(2 * sizeof(char));
	position[0] = '\0';

	// Verifica se a posição de reordenação é válida e se é diferente da posição da instrução no reorder buffer
	if (reorder_position_register != -1 && reorder_position_register != position_instruction_br)
	{
		// Formata o resultado com a posição de reordenação
		sprintf(result, "#%d", reorder_position_register);
	}
	else
	{
		// Copia o nome do registro para o resultado
		strcpy(result, name_register);
	}

	return result;
}

// inserir um registro no Registro_status
int setStatus(char *campoRegister, int entryReorderBuffer, Registro_status *Registro_status)
{
	int Registro_status_pos = -1;
	int controller = -1;

	// Verifica se a posição do reorder buffer é válida
	if (entryReorderBuffer == -1)
	{
		printf("ERRO: Posição inválida do reorder buffer: %d!\n", entryReorderBuffer);
		return controller;
	}

	// Encontra a posição do registro no Registro_status
	Registro_status_pos = getPosicaoRegistro(campoRegister, Registro_status);

	// Verifica se o registro foi encontrado
	if (Registro_status_pos == -1)
	{
		printf("ERRO: Registro campo não encontrado: %s!\n", campoRegister);
		return controller;
	}

	// Insere o registro no Registro_status
	Registro_status->coluna[Registro_status_pos].numEntrada = entryReorderBuffer;
	Registro_status->coluna[Registro_status_pos].isRegistradorOcupado = true;

	return Registro_status_pos;
}

// liberar um registro no Registro_status
int liberarRegistro(char *campoRegister, Registro_status *Registro_status)
{
	// Encontra a posição do registro no Registro_status
	int posRegisterStatus = getPosicaoRegistro(campoRegister, Registro_status);

	// Verifica se o registro foi encontrado
	if (posRegisterStatus == -1)
	{
		printf("** Erro: Registro campo não encontrado: %s! **\n", campoRegister);
	}
	else
	{
		// Libera o registro, atribuindo valores padrão aos campos
		Registro_status->coluna[posRegisterStatus].numEntrada = -1;
		Registro_status->coluna[posRegisterStatus].isRegistradorOcupado = false;
	}

	return posRegisterStatus;
}

// calcular o resultado de uma instrução
char *calcularResultado(Instrucao *instrucao, Registro_status *Registro_status)
{
	// Aloca memória para o resultado
	char *result = (char *)malloc(SIZE_STR * sizeof(char));
	// Copia uma string vazia para o resultado
	strcpy(result, "");

	// Verifica o tipo de instrução
	if (instrucao->tipo == ADD)
	{
		// Concatena o nome do primeiro registro ou a posição de reordenação
		strcat(result, posBr(Registro_status, instrucao->instDividida[2], instrucao->posBufferReordenacao));
		// Concatena o operador de adição ou subtração
		strcat(result, (strcmp(instrucao->instDividida[0], "ADD") == 0) ? " + " : " - ");
		// Concatena o nome do segundo registro ou a posição de reordenação
		strcat(result, posBr(Registro_status, instrucao->instDividida[3], instrucao->posBufferReordenacao));
	}
	else if (instrucao->tipo == LOAD)
	{
		// Verifica se a instrução é do tipo LW
		if (strcmp(instrucao->instDividida[0], "LW") == 0)
		{
			// Concatena o nome do registro de destino
			strcat(result, posBr(Registro_status, instrucao->instDividida[1], instrucao->posBufferReordenacao));
			// Concatena a string " = Mem[""
			strcat(result, " = Mem[");
			// Concatena o valor imediato
			strcat(result, instrucao->instDividida[2]);
			// Concatena a string " + Regs["
			strcat(result, " + Regs[");
			// Concatena o nome do registro de origem
			strcat(result, posBr(Registro_status, instrucao->instDividida[3], instrucao->posBufferReordenacao));
			// Concatena a string "]]"
			strcat(result, "]]");
		}
		else
		{
			// Concatena a string "Mem["
			strcat(result, "Mem[");
			// Concatena o valor imediato
			strcat(result, instrucao->instDividida[2]);
			// Concatena a string " + Regs["
			strcat(result, " + Regs[");
			// Concatena o nome do registro de origem
			strcat(result, posBr(Registro_status, instrucao->instDividida[3], instrucao->posBufferReordenacao));
			// Concatena a string "]] = "
			strcat(result, "]] = ");
			// Concatena o nome do registro de destino
			strcat(result, posBr(Registro_status, instrucao->instDividida[1], instrucao->posBufferReordenacao));
		}
	}
	else if (instrucao->tipo == MULT)
	{
		// Concatena o nome do primeiro registro ou a posição de reordenação
		strcat(result, posBr(Registro_status, instrucao->instDividida[2], instrucao->posBufferReordenacao));
		// Verifica o tipo de instrução de multiplicação
		if (strcmp(instrucao->instDividida[0], "MUL") == 0)
		{
			// Concatena o operador de multiplicação
			strcat(result, " * ");
		}
		else if (strcmp(instrucao->instDividida[0], "DIV") == 0)
		{
			// Concatena o operador de divisão
			strcat(result, " / ");
		}
		else
		{
			// Concatena o operador de módulo
			strcat(result, " % ");
		}
		// Concatena o nome do segundo registro ou a posição de reordenação
		strcat(result, posBr(Registro_status, instrucao->instDividida[3], instrucao->posBufferReordenacao));
	}

	return result;
}

void imprimirRegistradorStatus(Registro_status* registro_status) {
    int i = 0;
    printf("\n\n\tStatus dos Registradores\n\n");

    // Imprime o cabeçalho dos registradores
    printf("  Registrador:\t");
    for (i = 0; i < MAX_REGISTRADORES; ++i) {
        printf("%s\t", registro_status->coluna[i].campo);
    }
    printf("\n");

    // Imprime a reordenação dos registradores
    printf("  Reordenação:\t");
    for (i = 0; i < MAX_REGISTRADORES; ++i) {
        if (registro_status->coluna[i].numEntrada == -1) {
            printf("-\t");
        } else {
            printf("%d\t", registro_status->coluna[i].numEntrada);
        }
    }
    printf("\n");

    // Imprime o status de ocupação dos registradores
    printf("  Ocupado:\t");
    for (i = 0; i < MAX_REGISTRADORES; ++i) {
        printf("%s\t", (registro_status->coluna[i].isRegistradorOcupado == false) ? "x" : "✔");
    }
    printf("\n");
}

