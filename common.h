#pragma once

#define MAX_SIZE 20
#define MAX_LINHAS_ER 7 // Número máximo de linhas na estacao de reserva
#define MAX_LINHAS 6
#define MAX_REGISTRADORES 16   // Número máximo de registradores 
#define SIZE_STR 40 

// Definições para os estados das instruções
#define ESPERANDO 0
#define EMITIDO 1
#define EXECUTANDO 2
#define ESCREVER_RESULTADO 3
#define CONFIRMADO 4

// Enumeração para as operações das instruções
typedef enum
{
    LOAD,
    ADD,
    MULT
} Operation;
