/**
 * @file contas.c
 * @brief Implementação do módulo de contas bancárias do Sistema Bancário Modular.
 *
 * Este módulo gerencia as operações básicas de conta (abertura, consulta e
 * atualização de saldo). As contas são armazenadas em memória através de
 * um array estático (TAD), garantindo encapsulamento.
 */

#include <stdio.h>
#include <string.h>
#include "clientes.h"
#include "contas.h"

/* Armazenamento encapsulado (TAD) — invisível a outros módulos */
static Conta contas[MAX_CONTAS];
static int total_contas = 0;
static int proximo_id   = 1;

/**
 * @brief Abre uma nova conta bancária para um cliente cadastrado.
 * 
 * @param id_cliente ID do cliente proprietário da conta (entrada).
 * @param tipo Ponteiro para a string representando o tipo da conta ("corrente" ou "poupanca") (entrada).
 * @param id_conta_gerada Ponteiro de saída que receberá o número da conta gerado (saída).
 * @return int Retorna 0 para sucesso, ou -1 caso o cliente não seja encontrado ou o limite de contas seja atingido.
 */
int abrir_conta(int id_cliente, const char *tipo, int *id_conta_gerada) {
    Cliente c; /* Armazena os dados do cliente titular buscado */

    /* Checando possíveis erros de nulidade */
    if (tipo == NULL || id_conta_gerada == NULL) return -1;
    if (id_cliente <= 0) return -1;
    if (total_contas >= MAX_CONTAS) return -1;

    /* Verificando se o cliente existe no modulo de clientes */
    if (buscar_cliente(id_cliente, &c) != 0) return -1;

    /* Evita que nomes invalidos de tipos de conta sejam aceitos */
    if (strcmp(tipo, "corrente") != 0 && strcmp(tipo, "poupanca") != 0) return -1;

    /* Inicializa os campos da nova conta */
    contas[total_contas].id_conta = proximo_id;
    contas[total_contas].id_cliente = id_cliente;
    strncpy(contas[total_contas].tipo, tipo, sizeof(contas[total_contas].tipo) - 1);
    contas[total_contas].tipo[sizeof(contas[total_contas].tipo) - 1] = '\0';
    contas[total_contas].saldo = 0.0;

    /* Atualiza os valores indicadores do numero de contas e IDs */
    *id_conta_gerada = proximo_id;
    proximo_id++;
    total_contas++;
    return 0;
}

/**
 * @brief Localiza uma conta na estrutura interna pelo seu ID.
 * 
 * @param id_conta ID da conta a ser buscada (entrada).
 * @param conta_retornada Ponteiro de saída que receberá os dados da conta encontrada (saída).
 * @return int Retorna 0 em caso de sucesso, ou -1 se a conta não for encontrada.
 */
int buscar_conta(int id_conta, Conta *conta_retornada){
    int i; /* Variavel iteradora para busca no array de contas */

    /* Checando possíveis erros de entrada */
    if (conta_retornada == NULL) return -1;
    if (id_conta <= 0) return -1;

    /* Procura a conta linearmente no array */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            *conta_retornada = contas[i]; /* Acha a conta e retorna uma copia para a saida */
            return 0;
        }
    }
    return -1; /* A conta nao foi encontrada */
}

/**
 * @brief Consulta o saldo disponível de uma conta específica.
 * 
 * @param id_conta Número da conta a ser consultada (entrada).
 * @param saldo_retornado Ponteiro de saída que receberá o valor do saldo (saída).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não seja encontrada.
 */
int consultar_saldo(int id_conta, double *saldo_retornado) {
    int i; /* Variavel iteradora para busca no array de contas */

    /* Checando possíveis erros de entrada */
    if (saldo_retornado == NULL) return -1;
    if (id_conta <= 0) return -1;

    /* Procura a conta linearmente */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            *saldo_retornado = contas[i].saldo; /* Acha a conta e atribui o saldo */
            return 0;
        }
    }
    return -1; /* A conta nao foi encontrada */
}

/**
 * @brief Atualiza o saldo de uma conta específica (somando ou subtraindo um valor).
 * 
 * @param id_conta Número da conta a ser atualizada (entrada).
 * @param valor Quantia a ser somada ou subtraída (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a alteração resulte em saldo negativo ou se a conta não existir.
 */
int atualiza_saldo(int id_conta, double valor) {
    int i; /* Variavel iteradora para busca da conta */

    /* Checando possíveis erros na entrada */
    if (id_conta <= 0) return -1;

    /* Procura a conta linearmente */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            /* Verifica se a deducao deixara o saldo negativo (nao permitido) */
            if (contas[i].saldo + valor < 0.0) return -1;

            contas[i].saldo += valor; /* Atualiza efetivamente o saldo */
            return 0;
        }
    }
    return -1; /* A conta nao foi encontrada */
}

/**
 * @brief Exibe no console todas as contas associadas a um determinado cliente.
 * 
 * @param id_cliente ID do cliente titular (entrada).
 */
void listar_contas(int id_cliente) {
    int i; /* Variavel iteradora para listagem de contas */
    int encontrou = 0; /* Flag booleana para imprimir cabecalho apenas uma vez */

    /* Procura contas associadas ao cliente */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_cliente == id_cliente) {
            if (!encontrou) {
                printf("\n╔═══════════════════════════════════════════════════════╗\n");
                printf("║                 CONTAS DO CLIENTE %-3d                 ║\n", id_cliente);
                printf("╠═══════════════════════════════════════════════════════╣\n");
                encontrou = 1;
            }
            /* Escreve o id, tipo e saldo da conta com formatacao visual tabular */
            printf("║ Conta: %-3d | Tipo: %-10s | Saldo: R$ %-9.2f   ║\n",
                   contas[i].id_conta, contas[i].tipo, contas[i].saldo);
        }
    }

    if (encontrou) {
        printf("╚═══════════════════════════════════════════════════════╝\n");
    } else {
        /* Caso nenhuma conta tenha sido encontrada, uma mensagem envolta em borda e exibida */
        printf("\n╔═══════════════════════════════════════════════════════╗\n");
        printf("║                 CONTAS DO CLIENTE %-3d                 ║\n", id_cliente);
        printf("╠═══════════════════════════════════════════════════════╣\n");
        printf("║      Nenhuma conta encontrada para este cliente.      ║\n");
        printf("╚═══════════════════════════════════════════════════════╝\n");
    }
}

/**
 * @brief Verifica se um determinado cliente possui alguma conta cadastrada.
 * 
 * @param id_cliente ID do cliente a ser verificado (entrada).
 * @return int Retorna 1 se possuir pelo menos uma conta, 0 caso contrário.
 */
int cliente_tem_conta(int id_cliente) {
    int i; /* Variavel iteradora para procurar as contas do cliente no array */
    if (id_cliente <= 0) return 0;
    
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_cliente == id_cliente) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Verifica se uma conta pertence a um cliente específico.
 * 
 * @param id_conta ID da conta bancária (entrada).
 * @param id_cliente ID do cliente titular a validar (entrada).
 * @return int Retorna 1 se o cliente for titular da conta, 0 caso contrário.
 */
int verifica_titularidade(int id_conta, int id_cliente) {
    int i; /* Variavel iteradora para procurar as contas no array */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta && contas[i].id_cliente == id_cliente) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Salva os dados das contas em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_contas_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    int i; /* Variavel iteradora para gravacao dos registros */

    f = fopen("contas.txt", "w");
    if (f == NULL) return -1;

    /* Para cada conta existente, escreve as informacoes serializadas no arquivo */
    for (i = 0; i < total_contas; i++) {
        fprintf(f, "%d;%d;%s;%.2f\n",
                contas[i].id_conta,
                contas[i].id_cliente,
                contas[i].tipo,
                contas[i].saldo);
    }

    fclose(f);
    return 0;
}

/**
 * @brief Carrega os dados das contas de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_contas_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    char linha[256]; /* Buffer temporario para leitura de cada linha do arquivo */
    int max_id = 0; /* Rastreia o ID maximo carregado para o proximo gerador */
    total_contas = 0;
    proximo_id = 1;

    /* Abre o arquivo para leitura */
    f = fopen("contas.txt", "r");
    if (f == NULL) return -1; /* 1a execucao: arquivo nao existe, comeca vazio */

    /* Le as linhas do arquivo ate o EOF ou ate atingir o limite MAX_CONTAS */
    while (total_contas < MAX_CONTAS && fgets(linha, sizeof(linha), f) != NULL) {
        Conta c; /* Variavel temporaria para parseamento da linha atual */
        if (sscanf(linha, "%d;%d;%19[^;];%lf", &c.id_conta, &c.id_cliente, c.tipo, &c.saldo) == 4) {
            contas[total_contas] = c;
            if (c.id_conta > max_id) max_id = c.id_conta;
            total_contas++;
        }
    }

    fclose(f);
    proximo_id = max_id + 1;
    return 0;
}
