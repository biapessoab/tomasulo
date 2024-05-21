#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bufferReordenacao.h"
#include "instrucoes.h"
#include "estacaoReserva.h"
#include "status.h"

// decodificar a operação e retornar o tipo correspondente
int decodificacaoOperacao(char *operation)
{
	int resp = -1;
	// Verifica se a operação é load ou store
	if (strcmp(operation, "lwd") == 0 || strcmp(operation, "swd") == 0)
	{
		resp = LOAD;
	}
	// Verifica se a operação é add ou sub
	else if (strcmp(operation, "addd") == 0 || strcmp(operation, "subd") == 0)
	{
		resp = ADD;
	}
	// Verifica se a operação é mul ou div
	else if (strcmp(operation, "muld") == 0 || strcmp(operation, "divd") == 0)
	{
		resp = MULT;
	}
	return resp;
}

void imprimirCiclos(int ciclo)
{
	printf("\nCiclos: %d\n", ciclo);
}

void inicializarPipeline(char *filename)
{
	system("cls || clear ");
	Buffer_Reordenacao *br = inicializadorBufferReordenacao();				// Inicializa o buffer de reordenação
	Instrucao *isntrucoes = inicializadorInstrucoes(filename);				// Inicializa as instruções a partir de um arquivo
	Estacao_Reserva *rs = inicializarEstacaoReserva();						// Inicializa a estação de reserva
	Registro_status *Renomerar_registrador = inicializadorRegistroStatus(); // Inicializa o registro de status

	imprimirInstrucoes(isntrucoes, filename); // Imprime as instruções
	fflush(stdin);

	getchar();
	system("cls || clear");

	int numero_instrucoes = numeroDeLinhas(filename); // Obtém o número de instruções do arquivo
	int inicio = br->linhasPreenchidas % MAX_LINHAS, fim = ((inicio - 1) % MAX_LINHAS < 0) ? MAX_LINHAS - 1 : (inicio - 1);
	int i, j, aux, pos = 0;
	int *position_run = (int *)calloc(MAX_LINHAS_ER, sizeof(int));

	// Inicializa variáveis e aloca memória para o vetor position_run
	for (i = 0; i < MAX_LINHAS_ER; i++)
	{
		position_run[i] = -1;
	}

	int ciclo = 0;

	while (br->linha[fim].estadoInstrucao != CONFIRMADO || pos != numero_instrucoes)
	{
		// Calcula o índice inicial e final do buffer de reordenação
		inicio = br->linhasPreenchidas % (br->maxLinhasAlocadasBr == 0 ? MAX_LINHAS : br->maxLinhasAlocadasBr);
		fim = ((inicio - 1) % (br->maxLinhasAlocadasBr == 0 ? MAX_LINHAS : br->maxLinhasAlocadasBr) < 0) ? ((br->maxLinhasAlocadasBr == 0 ? MAX_LINHAS : br->maxLinhasAlocadasBr) - 1) : (inicio - 1);

		// Percorre o buffer de reordenação e insere as instruções
		for (i = inicio; i != fim && pos < numero_instrucoes; i = (i + 1) % MAX_LINHAS)
		{ // Verifica se a instrução possui um rótulo
			if (strstr(isntrucoes[pos].instCompleta, ":") != NULL)
			{
				pos++;
			}

			// Verifica se a posição atual é menor que o número total de instruções e se a execução da instrução atual não está ocupada
			if (pos < numero_instrucoes && br->linha[i].execInstrucao != true)
			{
				// Libera o registro se a instrução anterior foi confirmada e não precisa fazer escrita
				if (br->linha[i].estadoInstrucao == CONFIRMADO && naoFazerEscrita(br->linha[i].instrucao->instDividida[0]) == 0)
				{
					liberarRegistro(br->linha[i].instrucao->instDividida[1], Renomerar_registrador);
				}

				// Insere a instrução no buffer de reordenação
				int reordenar_pos = inserirInstrucaoBR(&isntrucoes[pos], br);
				br->linha[i].instrucao->posBufferReordenacao = reordenar_pos;
				br->linha[i].instrucao->tipo = decodificacaoOperacao(br->linha[i].instrucao->instDividida[0]);
				pos++;
			}
		}

		// Verifica se a instrução possui um rótulo
		if (strstr(isntrucoes[pos].instCompleta, ":") != NULL)
		{
			pos++;
		}

		// Verifica se a posição atual é menor que o número total de instruções e se a execução da instrução atual não está ocupada
		if (pos < numero_instrucoes && br->linha[i].execInstrucao != true)
		{
			// Libera o registro se a instrução anterior foi confirmada e não precisa fazer escrita
			if (br->linha[i].estadoInstrucao == CONFIRMADO && naoFazerEscrita(br->linha[i].instrucao->instDividida[0]) == 0)
			{
				liberarRegistro(br->linha[i].instrucao->instDividida[1], Renomerar_registrador);
			}
			// Insere a instrução no buffer de reordenação
			int reordenar_pos = inserirInstrucaoBR(&isntrucoes[pos], br);
			br->linha[i].instrucao->posBufferReordenacao = reordenar_pos;
			br->linha[i].instrucao->tipo = decodificacaoOperacao(br->linha[i].instrucao->instDividida[0]);
			pos++;
		}

		ciclo++; // Incrementa o ciclo
		// Imprime o buffer de reordenação, a estação de reserva e o status do registro

		imprimirBufferReordenacao(br);
		imprimirEstacaoReserva(rs);
		imprimirRegistradorStatus(Renomerar_registrador);
		imprimirCiclos(ciclo);

		fflush(stdin);
		getchar();
		system("cls || clear");

		inicio = br->linhasPreenchidas % br->maxLinhasAlocadasBr;

		fim = (((inicio - 1) < 0 ? (br->maxLinhasAlocadasBr - 1) : (inicio - 1)) % br->maxLinhasAlocadasBr);
		for (i = inicio; i != fim; i = (i + 1) % br->maxLinhasAlocadasBr)
		{
			if (br->linha[i].estadoInstrucao == ESPERANDO)
			{
				// Insere a instrução na estação de reserva
				int logico = inserirInstrucaoER(br->linha[i].instrucao, rs, br);
				if (logico != -1)
				{
					br->linha[i].estadoInstrucao = EMITIDO;
				}
			}
		}

		// Verifica se a instrução está esperando para ser emitida
		if (br->linha[i].estadoInstrucao == ESPERANDO)
		{
			// Tenta inserir a instrução na estação de reserva
			int logico = 0;
			if ((logico = inserirInstrucaoER(br->linha[i].instrucao, rs, br)) != -1)
			{
				// Se a inserção for bem-sucedida, atualiza o estado da instrução para EMITIDO
				br->linha[i].estadoInstrucao = EMITIDO;
			}
		}

		ciclo++; // Incrementa o ciclo
		// Imprime o buffer de reordenação, a estação de reserva e o status do registro
		imprimirBufferReordenacao(br);
		imprimirEstacaoReserva(rs);
		imprimirRegistradorStatus(Renomerar_registrador);
		imprimirCiclos(ciclo);

		fflush(stdin);

		getchar();
		system("cls || clear");

		aux = 0;

		// Percorre a estação de reserva e verifica se as instruções podem ser executadas
		for (i = 0; i < MAX_LINHAS_ER; i++)
		{
			if (rs->linha[i].isEstacaoOcupada == false)
				continue;

			// Verifica se não há dependências e se a instrução de destino foi emitida
			if ((rs->linha[i].dependenciaQj == -1 && rs->linha[i].dependenciaQk == -1) && br->linha[rs->linha[i].posDestino].estadoInstrucao == EMITIDO)
			{
				int inst_position = rs->linha[i].posDestino;
				br->linha[inst_position].estadoInstrucao = EXECUTANDO;
				position_run[aux] = i;
				aux++;
			}
		}

		ciclo++; // Incrementa o ciclo
		// Imprime o buffer de reordenação, a estação de reserva e o status do registro
		imprimirBufferReordenacao(br);
		imprimirEstacaoReserva(rs);
		imprimirRegistradorStatus(Renomerar_registrador);
		imprimirCiclos(ciclo);

		// Aguarda a entrada do usuário
		fflush(stdin);
		getchar();
		system("cls || clear");

		inicio = br->linhasPreenchidas % br->maxLinhasAlocadasBr;
		fim = (((inicio - 1) < 0 ? (br->maxLinhasAlocadasBr - 1) : (inicio - 1)) % br->maxLinhasAlocadasBr);

		// Percorre o buffer de reordenação e verifica as instruções em execução
		for (i = inicio; i != fim; i = (i + 1) % br->maxLinhasAlocadasBr)
		{
			if (br->linha[i].estadoInstrucao == EXECUTANDO)
			{
				// Atualiza o estado da instrução para ESCREVER_RESULTADO
				br->linha[i].estadoInstrucao = ESCREVER_RESULTADO;

				// Calcula e armazena o resultado da instrução
				strcpy(br->linha[i].resultado, calcularResultado(br->linha[i].instrucao, Renomerar_registrador));
			}
		}
		// Verifica se a última instrução está em execução
		if (br->linha[i].estadoInstrucao == EXECUTANDO)
		{
			// Atualiza o estado da instrução para ESCREVER_RESULTADO
			br->linha[i].estadoInstrucao = ESCREVER_RESULTADO;

			// Calcula e armazena o resultado da instrução
			strcpy(br->linha[i].resultado, calcularResultado(br->linha[i].instrucao, Renomerar_registrador));
		}

		aux = 0;
		while (position_run[aux] != -1)
		{
			i = position_run[aux];
			int inst_position = rs->linha[i].posDestino;

			// Percorre a estação de reserva e atualiza os valores lidos dos registradores de acordo com as dependências
			for (j = 0; j < MAX_LINHAS_ER; j++)
			{
				if (rs->linha[j].isEstacaoOcupada == false)
					continue;

				if (rs->linha[j].dependenciaQj == inst_position)
				{
					strcpy(rs->linha[j].valorVj, br->linha[inst_position].instrucao->instDividida[1]);
					rs->linha[j].dependenciaQj = -1;
				}
				if (rs->linha[j].dependenciaQk == inst_position)
				{
					strcpy(rs->linha[j].valorVk, br->linha[inst_position].instrucao->instDividida[1]);
					rs->linha[j].dependenciaQk = -1;
				}
			}

			// Limpa a linha da estação de reserva e incrementa o auxiliar
			limparLinhaER(rs, i);
			aux++;
		}

		for (i = 0; i < MAX_LINHAS_ER; i++) // Limpa o vetor position_run
		{
			position_run[i] = -1;
		}
		ciclo++; // Incrementa o ciclo
		// Imprime o buffer de reordenação e a estação de reserva
		imprimirBufferReordenacao(br);
		imprimirEstacaoReserva(rs);
		imprimirRegistradorStatus(Renomerar_registrador);
		imprimirCiclos(ciclo);

		fflush(stdin);

		getchar();
		system("cls || clear");

		inicio = br->linhasPreenchidas % br->maxLinhasAlocadasBr;
		fim = (((inicio - 1) < 0 ? (br->maxLinhasAlocadasBr - 1) : (inicio - 1)) % br->maxLinhasAlocadasBr);
		int controll_commit = 0;

		// Percorre o buffer de reordenação e confirma as instruções que estão prontas para serem confirmadas
		for (i = inicio; i != fim && controll_commit == 0; i = (i + 1) % br->maxLinhasAlocadasBr)
		{
			if (br->linha[i].estadoInstrucao == ESCREVER_RESULTADO && br->linha[i].execInstrucao == true)
			{
				br->linha[i].estadoInstrucao = CONFIRMADO;
				// Verifica se a instrução não é de escrita e insere o registro de
				if (naoFazerEscrita(br->linha[i].instrucao->instDividida[0]) == 0)
				{
					setStatus(br->linha[i].instrucao->instDividida[1], i, Renomerar_registrador);
				}
				br->linha[i].execInstrucao = false;
			}
			else if (br->linha[i].estadoInstrucao != CONFIRMADO)
			{
				controll_commit = 1;
			}
		}
		// Verifica se a última instrução está pronta para ser confirmada
		if (controll_commit == 0 && br->linha[fim].estadoInstrucao == ESCREVER_RESULTADO && br->linha[i].execInstrucao == true)
		{
			br->linha[fim].estadoInstrucao = CONFIRMADO;
			// Verifica se a instrução não é de escrita e insere o registro de status
			if (naoFazerEscrita(br->linha[i].instrucao->instDividida[0]) == 0)
			{
				setStatus(br->linha[i].instrucao->instDividida[1], i, Renomerar_registrador);
			}
			br->linha[fim].execInstrucao = false;
		}
		ciclo++;
		// Imprime o buffer de reordenação, a estação de reserva e o status do registro
		imprimirBufferReordenacao(br);
		imprimirEstacaoReserva(rs);
		imprimirRegistradorStatus(Renomerar_registrador);
		imprimirCiclos(ciclo);

		fflush(stdin);

		getchar();
		system("cls || clear");
	}
	printf("Fim!\n");
}


int main(void)
{
	char nome_arquivo[100];
	printf("Digite o número do exemplo: ");
	scanf("%s", nome_arquivo);

	char caminho[100] = "./inputs/input";
	strcat(caminho, nome_arquivo);
	strcat(caminho, ".txt");

	inicializarPipeline(caminho);
	return 0;
}
