# Simulador do Algoritmo de Tomasulo

##### Ana Beatriz Pessoa Braz, Beatriz Fulgêncio da Cunha Menezes, Letícia Guimarães Pedrosa

Este simulador do algoritmo de Tomasulo foi desenvolvido para a disciplina de Arquitetura de Computadores 3 da PUC Minas. Implementado em C, o objetivo deste projeto é simular o funcionamento do algoritmo, permitindo a execução fora de ordem das instruções enquanto mantém a consistência do estado do sistema.

## Arquivos e Estrutura do Projeto
O projeto é organizado da seguinte forma:

- **main.c**: Contém a função main e a lógica principal do simulador.

- **bufferReordenacao.h, instrucoes.h, estacaoReserva.h, status.h**: Arquivos de cabeçalho para as estruturas e funções relacionadas ao buffer de reordenação, instruções, estação de reserva e status de registro, respectivamente.

- **bufferReordenacao.c, instrucoes.c, estacaoReserva.c, status.c**: Implementações das funções definidas nos arquivos de cabeçalho correspondentes.

## Explicação
### 1. Status das Instruções

O Status das Instruções acompanha o progresso de cada instrução no pipeline de execução.

- **Entrada**: Índice da instrução na lista.
- **Instrução**: O código da instrução que está sendo processada.
- **Estado**: O estado atual da instrução, que pode ser:
  - **ESPERANDO**: A instrução está aguardando operandos ou outras condições para ser emitida.
  - **EMITIDO**: A instrução foi emitida para uma estação de reserva.
  - **EXECUTANDO**: A instrução está sendo executada.
  - **ESCREVER_RESULTADO**: A instrução está pronta para escrever o resultado.
  - **CONFIRMADO**: A instrução foi confirmada e seu resultado foi escrito no registrador de destino.
- **Valor**: O valor calculado pela instrução ou "AINDA NÃO CALCULADO" se a execução não foi completada.

### 2. Estação de Reserva

A Estação de Reserva mantém as instruções enquanto aguardam a disponibilidade de operandos ou recursos para execução.

- **Nome**: Nome da estação de reserva (por exemplo, LOAD1, ADD1, MULT1).
- **Ocupado**: Indica se a estação está ocupada (`Sim`) ou não (`Não`).
- **Vj, Vk**: Valores dos operandos da instrução, se disponíveis.
- **Qj, Qk**: Identificadores das estações de reserva que fornecerão os operandos.
- **Destino**: Registrador de destino para o resultado da instrução.
- **Endereço**: Endereço de memória associado à operação de load/store, se aplicável.

### 3. Status dos Registradores

O Status dos Registradores rastreia o estado atual de cada registrador no sistema.

- **Registrador**: Nome dos registradores (por exemplo, R0, R1, R2, etc.).
- **Reordenação**: Número da entrada no Reorder Buffer onde o valor do registrador será gravado quando a instrução for completada.
- **Ocupado**: Indica se o registrador está sendo usado (`Sim`) ou não (`Não`).

## Como rodar
Este projeto inclui um arquivo makefile, tornando a execução simples e direta. Abra o terminal na pasta do projeto e digite `make all` para compilar o código-fonte, gerando um executável chamado `tomasulo`.

Em seguida, você poderá selecionar o input a ser utilizado. O repositório já contém dois arquivos de input: 1 e 2. Para adicionar novas instruções, basta criar um novo arquivo com as instruções desejadas.

Ao iniciar a execução, a cada pressionamento da tecla Enter, um novo ciclo é gerado. É possível visualizar as mudanças nas estações de reserva e no buffer de reordenação diretamente no terminal a cada novo ciclo.

Certifique-se de ter um compilador C instalado em seu sistema para compilar o código-fonte.
