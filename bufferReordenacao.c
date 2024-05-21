#include "bufferReordenacao.h"
#include "instrucoes.h"
#include <string.h>
#include <stdio.h>
// inicializar o buffer de reordenação
Buffer_Reordenacao *inicializadorBufferReordenacao()
{
	Buffer_Reordenacao *br = (Buffer_Reordenacao *)malloc(sizeof(Buffer_Reordenacao));

	br->linhasPreenchidas = 0;
	br->maxLinhasAlocadasBr = 0;
	return br;
}

// inserir uma instrução no buffer de reordenação
int inserirInstrucaoBR(Instrucao *instruction, Buffer_Reordenacao *br)
{
	// Calcula a posição onde a instrução será inserida
	int pos = br->linhasPreenchidas % MAX_LINHAS;

	// Marca a posição como ocupada
	br->linha[pos].execInstrucao = true;

	// Clona a instrução e a armazena na posição do buffer
	br->linha[pos].instrucao = clonarInstrucao(instruction);

	// Define o estado da instrução como ESPERANDO
	br->linha[pos].estadoInstrucao = ESPERANDO;

	// Aloca memória para o resultado da instrução, caso ainda não tenha sido calculado
	if (br->linha[pos].resultado == NULL)
	{
		br->linha[pos].resultado = (char *)malloc(SIZE_STR * sizeof(char));
	}
	strcpy(br->linha[pos].resultado, "-\0");

	// Define a posição do buffer de reordenação na instrução
	br->linha[pos].instrucao->posBufferReordenacao = pos;

	// Incrementa o número de linhas preenchidas no buffer
	br->linhasPreenchidas += 1;

	// Atualiza o número máximo de linhas alocadas no buffer
	if (br->linhasPreenchidas < MAX_LINHAS)
	{
		br->maxLinhasAlocadasBr = br->linhasPreenchidas;
	}
	else
	{
		br->maxLinhasAlocadasBr = MAX_LINHAS;
	}

	// Retorna a posição onde a instrução foi inserida
	return pos;
}

// liberar uma linha específica do buffer de reordenação
void liberarLinhaBufferReordenacao(Buffer_Reordenacao *br, int positionToFree)
{
	// Define o estado da instrução como ESPERANDO
	br->linha[positionToFree].estadoInstrucao = ESPERANDO;
	// Limpa os campos da instrução
	strcpy(br->linha[positionToFree].instrucao->instCompleta, "");
	strcpy(br->linha[positionToFree].instrucao->instDividida[1], "");
	// Limpa o resultado da instrução
	strcpy(br->linha[positionToFree].resultado, "");
	// Marca a posição como não ocupada
	br->linha[positionToFree].execInstrucao = false;
}

Buffer_Reordenacao *liberabufferReordenacao(Buffer_Reordenacao *br)
{
	for (int i = 0; i < br->maxLinhasAlocadasBr; i++)
	{
		free(br->linha[i].instrucao);
		free(br->linha[i].resultado);
	}
	free(br);
	return NULL;
}

void imprimirBufferReordenacao(Buffer_Reordenacao *br) {
    int i = 0;
    int inicio = br->linhasPreenchidas % br->maxLinhasAlocadasBr;
    int fim = ((inicio - 1) < 0 ? (br->maxLinhasAlocadasBr - 1) : (inicio - 1));

    printf("\n\n\tStatus das Instruções\n\n");

    // Imprimir linha superior da tabela
    printf("  ------------------------------------------------------------------------------------------------------\n");

    // Imprimir cabeçalho da tabela
    printf("  | Entrada  |   Instrução        | Estado                           |              Valor               |\n");

    // Imprimir linha separadora entre cabeçalho e conteúdo
    printf("  ------------------------------------------------------------------------------------------------------\n");

    for (i = inicio; i != fim; i = ((i + 1) % br->maxLinhasAlocadasBr)) {
        printf("  |    %d     | ", i);
        printf("%-18s | ", (br->linha[i].instrucao->instCompleta == NULL ? "-" : (strcmp(br->linha[i].instrucao->instCompleta, "") == 0 ? "-" : br->linha[i].instrucao->instCompleta)));

        printf("%-34s | ", (br->linha[i].estadoInstrucao == ESPERANDO ? "ESPERANDO" :
                            (br->linha[i].estadoInstrucao == EXECUTANDO ? "EXECUTANDO" :
                            (br->linha[i].estadoInstrucao == ESCREVER_RESULTADO ? "ESCREVER_RESULTADO" :
                            (br->linha[i].estadoInstrucao == EMITIDO ? "EMITIDO" : "CONFIRMADO")))));

        // Imprimir valor com tamanho maior
        printf("%-34s |\n", (br->linha[i].resultado == NULL ? "-" :
                            (strcmp(br->linha[i].resultado, "") == 0 ? "-" :
                            br->linha[i].resultado)));
    }

    // Imprimir linha inferior da tabela
    printf("  |    %d     | ", fim);
    printf("%-18s | ", (br->linha[fim].instrucao->instCompleta == NULL ? "-" : (strcmp(br->linha[fim].instrucao->instCompleta, "") == 0 ? "-" : br->linha[fim].instrucao->instCompleta)));

    printf("%-34s | ", (br->linha[fim].estadoInstrucao == ESPERANDO ? "ESPERANDO" :
                        (br->linha[fim].estadoInstrucao == EXECUTANDO ? "EXECUTANDO" :
                        (br->linha[fim].estadoInstrucao == ESCREVER_RESULTADO ? "ESCREVER_RESULTADO" :
                        (br->linha[fim].estadoInstrucao == EMITIDO ? "EMITIDO" : "CONFIRMADO")))));

    // Imprimir valor com tamanho maior
    printf("%-34s |\n", (br->linha[fim].resultado == NULL ? "-" :
                        (strcmp(br->linha[fim].resultado, "") == 0 ? "-" :
                        br->linha[fim].resultado)));

    // Imprimir linha inferior da tabela
    printf("  ------------------------------------------------------------------------------------------------------\n");
}
