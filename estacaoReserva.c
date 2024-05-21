#include "bufferReordenacao.h"
#include "estacaoReserva.h"
#include "instrucoes.h"
#include <string.h>
#include <stdio.h>

Estacao_Reserva *inicializarEstacaoReserva()
{
	Estacao_Reserva *rs = (Estacao_Reserva *)malloc(sizeof(Estacao_Reserva));

	rs->linha[0].nome = "LOAD1"; // Nome da estação de reserva
	rs->linha[0].tipo = LOAD;	 // Tipo da instrução

	rs->linha[1].nome = "LOAD2";
	rs->linha[1].tipo = LOAD;

	rs->linha[2].nome = "ADD1";
	rs->linha[2].tipo = ADD;

	rs->linha[3].nome = "ADD2";
	rs->linha[3].tipo = ADD;

	rs->linha[4].nome = "ADD3";
	rs->linha[4].tipo = ADD;

	rs->linha[5].nome = "MULT1";
	rs->linha[5].tipo = MULT;

	rs->linha[6].nome = "MULT2";
	rs->linha[6].tipo = MULT;

	for (int i = 0; i < MAX_LINHAS_ER; i++)
	{
		rs->linha[i].isEstacaoOcupada = false; // Define a estação de reserva como não ocupada
		rs->linha[i].operacao = strdup("-");			// Define a operação da instrução como "-"
		rs->linha[i].valorVj = strdup("-");	// Define o valor do registro Vj como "-"
		rs->linha[i].valorVk = strdup("-");	// Define o valor do registro Vk como "-"
		rs->linha[i].dependenciaQj = -1;		// Define a informação de dependência Qj como -1
		rs->linha[i].dependenciaQk = -1;		// Define a informação de dependência Qk como -1
		rs->linha[i].posDestino = -1;				// Define a posição de destino no buffer de reordenação como -1
		rs->linha[i].end = strdup("-");		// Define o endereço de memória como "-"
	}

	return rs;
}

// limpar uma linha da estação de reserva
void limparLinhaER(Estacao_Reserva *rs, int positionRS)
{
	// Limpa os campos da linha da estação de reserva
	strcpy(rs->linha[positionRS].operacao, "");			  // Limpa a operação da instrução
	strcpy(rs->linha[positionRS].valorVj, ""); // Limpa o valor do registro Vj
	strcpy(rs->linha[positionRS].valorVk, ""); // Limpa o valor do registro Vk

	// Libera a memória alocada para o endereço de memória, se houver
	if (rs->linha[positionRS].end != NULL)
	{
		free(rs->linha[positionRS].end);
		rs->linha[positionRS].end = NULL;
	}

	// Define a linha da estação de reserva como não ocupada
	rs->linha[positionRS].isEstacaoOcupada = false;

	// Define as informações de dependência como -1
	rs->linha[positionRS].dependenciaQj = -1;
	rs->linha[positionRS].dependenciaQk = -1;

	// Define a posição de destino no buffer de reordenação como -1
	rs->linha[positionRS].posDestino = -1;
}
// inserir uma instrução na estação de reserva
int inserirInstrucaoER(Instrucao *instruction, Estacao_Reserva *reservationStation, Buffer_Reordenacao *br)
{
	int positionRS = -1, pos = 0;

	// Verifica o tipo de instrução e encontra a posição disponível na estação de reserva correspondente
	if (instruction->tipo == LOAD)
	{
		for (pos = 0; pos < 2; pos++)
		{
			if (!reservationStation->linha[pos].isEstacaoOcupada)
			{
				positionRS = pos;
				break;
			}
		}
	}
	else if (instruction->tipo == ADD)
	{
		for (pos = 2; pos < 5; pos++)
		{
			if (!reservationStation->linha[pos].isEstacaoOcupada)
			{
				positionRS = pos;
				break;
			}
		}
	}
	else if (instruction->tipo == MULT)
	{
		for (pos = 5; pos < 7; pos++)
		{
			if (!reservationStation->linha[pos].isEstacaoOcupada)
			{
				positionRS = pos;
				break;
			}
		}
	}
	else
	{
		printf("A instrução não é suportada neste simulador!\n");
	}

	// Se houver uma posição disponível na estação de reserva, insere a instrução
	if (positionRS != -1)
	{
		reservationStation->linha[positionRS].isEstacaoOcupada = true;
		strcpy(reservationStation->linha[positionRS].operacao, instruction->instDividida[0]);

		// Define as informações de dependência Qj e Qk de acordo com a instrução
		if (instruction->tipo == LOAD)
		{
			if (strcmp(instruction->instDividida[0], "SW") == 0)
			{
				reservationStation->linha[positionRS].dependenciaQj = identificarWAR(instruction->instDividida[1], instruction->posBufferReordenacao, br);
			}
			else
			{
				reservationStation->linha[positionRS].dependenciaQj = -1;
			}

			reservationStation->linha[positionRS].dependenciaQk = identificarWAR(instruction->instDividida[3], instruction->posBufferReordenacao, br);
		}
		else
		{
			reservationStation->linha[positionRS].dependenciaQj = identificarWAR(instruction->instDividida[2], instruction->posBufferReordenacao, br);
			reservationStation->linha[positionRS].dependenciaQk = identificarWAR(instruction->instDividida[3], instruction->posBufferReordenacao, br);
		}

		// Define os valores dos registros Vj e Vk de acordo com as dependências
		if (reservationStation->linha[positionRS].dependenciaQj == -1)
		{
			if (strcmp(instruction->instDividida[0], "SW") == 0)
			{
				strcpy(reservationStation->linha[positionRS].valorVj, instruction->instDividida[1]);
			}
			else
			{
				strcpy(reservationStation->linha[positionRS].valorVj, instruction->instDividida[2]);
			}
		}
		if (reservationStation->linha[positionRS].dependenciaQk == -1)
		{
			strcpy(reservationStation->linha[positionRS].valorVk, instruction->instDividida[3]);
		}

		// Define a posição de destino no buffer de reordenação
		reservationStation->linha[positionRS].posDestino = instruction->posBufferReordenacao;

		// Se for uma instrução do tipo LOAD, define o endereço de memória
		if (instruction->tipo == LOAD)
		{
			char *end = (char *)malloc(SIZE_STR * sizeof(char));
			strcpy(end, "");
			size_t index1 = 0;
			for (; index1 < strlen(instruction->instDividida[2]); index1++)
			{
				end[index1] = instruction->instDividida[2][index1];
			}
			end[index1++] = ' ';
			end[index1++] = '+';
			end[index1++] = ' ';
			end[index1++] = 'R';
			end[index1++] = 'e';
			end[index1++] = 'g';
			end[index1++] = 's';
			end[index1++] = '[';
			size_t index2 = 0;
			for (; index2 < strlen(instruction->instDividida[3]); index2++)
			{
				end[index1++] = instruction->instDividida[3][index2];
			}
			end[index1++] = ']';
			end[index1++] = '\0';
			strcpy(reservationStation->linha[positionRS].end, end);
		}
	}

	return positionRS;
}

// encontrar a linha da estação de reserva de acordo com a posição no buffer de reordenação
int encontrarLinhaERDeAcordoComPosicaobr(Estacao_Reserva *rs, int positionbr)
{
	int linha_rs_posicao = -1;
	// Percorre todas as linhas da estação de reserva
	for (int i = 0; i < MAX_LINHAS_ER; i++)
	{
		// Verifica se a posição de destino no buffer de reordenação é igual à posiçãobr
		if (rs->linha[i].posDestino == positionbr)
		{
			linha_rs_posicao = i; // Armazena a linha da estação de reserva correspondente
			break;				  // Sai do loop
		}
	}

	return linha_rs_posicao; // Retorna a linha da estação de reserva encontrada ou -1 se não encontrou
}

int naoFazerEscrita(char *nomeInstruction)
{
	return (strcmp(nomeInstruction, "SW") == 0);
}

int identificarWAR(char *registro_analisado, int posicao, Buffer_Reordenacao *br)
{
	int resultado = -1;
	int inicio = br->linhasPreenchidas % br->maxLinhasAlocadasBr;
	if (posicao >= 0)
	{
		for (int i = inicio; i != posicao; i = (i + 1) % br->maxLinhasAlocadasBr)
		{
			char *operacao_instrucao = br->linha[i].instrucao->instDividida[0];

			// Se a instrução já tiver seu resultado, ou se ela não escrever no registro, não há necessidade de analisar a dependência
			if (br->linha[i].estadoInstrucao == ESCREVER_RESULTADO || br->linha[i].estadoInstrucao == CONFIRMADO || naoFazerEscrita(operacao_instrucao))
			{
				continue;
			}

			/* Dependência RAW (Read After Write) */
			if (strcmp(br->linha[i].instrucao->instDividida[1], registro_analisado) == 0)
			{
				resultado = i;
			}
		}
	}
	return resultado;
}

// imprimir a estação de reserva com colunas fixas e substituindo -1 por -
void imprimirEstacaoReserva(Estacao_Reserva *reservationStation)
{
    int i = 0;
    printf("\n\n\n\tEstação de Reserva\n\n");
    printf("  ------------------------------------------------------------------------------------\n");
    printf("  |  Nome            | Ocupado |  Vj  |  Vk  |  Qj  |  Qk  | Destino| Endereço       |\n");
    printf("  ------------------------------------------------------------------------------------\n");
    for (i = 0; i < MAX_LINHAS_ER; i++)
    {
        char *ocupado = (reservationStation->linha[i].isEstacaoOcupada == false) ? "Não" : "Sim";
        char vj[5], vk[5], qj[5], qk[5], destino[8];

        if (reservationStation->linha[i].valorVj == NULL || strcmp(reservationStation->linha[i].valorVj, "") == 0 || strcmp(reservationStation->linha[i].operacao, "LW") == 0)
            strcpy(vj, "-");
        else
            strcpy(vj, reservationStation->linha[i].valorVj);

        if (reservationStation->linha[i].valorVk == NULL || strcmp(reservationStation->linha[i].valorVk, "") == 0)
            strcpy(vk, "-");
        else
            strcpy(vk, reservationStation->linha[i].valorVk);

        if (reservationStation->linha[i].dependenciaQj == -1)
            strcpy(qj, "-");
        else
            sprintf(qj, "%d", reservationStation->linha[i].dependenciaQj);

        if (reservationStation->linha[i].dependenciaQk == -1)
            strcpy(qk, "-");
        else
            sprintf(qk, "%d", reservationStation->linha[i].dependenciaQk);

        if (reservationStation->linha[i].posDestino == -1)
            strcpy(destino, "-");
        else
            sprintf(destino, "%d", reservationStation->linha[i].posDestino);

        char *endereco = (reservationStation->linha[i].end == NULL || strcmp(reservationStation->linha[i].end, "") == 0) ? "-" : reservationStation->linha[i].end;

        printf("  |  %-15s | %-7s | %-4s | %-4s | %-4s | %-4s | %-6s | %-14s |\n",
               reservationStation->linha[i].nome,
               ocupado,
               vj,
               vk,
               qj,
               qk,
               destino,
               endereco
        );
    }
    printf("  ------------------------------------------------------------------------------------\n");
}
