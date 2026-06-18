/**
 * @file transacoes.c
 * @brief Implementação do módulo de transações do Sistema Bancário Modular.
 *
 * Implementa as regras de negócio das operações financeiras (depósito, saque
 * e transferência) e o histórico de transações por conta.
 *
 * Dependência direta: módulo Contas (atualiza_saldo, consultar_saldo, buscar_conta).
 *
 * Os dados são encapsulados em arrays estáticos conforme o padrão TAD (RNF-02).
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "contas.h"
#include "transacoes.h"

/* Armazenamento interno (encapsulado — não exposto via .h) */

/** Array estático de transações — acessível apenas neste módulo */
static Transacao transacoes[MAX_TRANSACOES];

/** Contador de transações registradas */
static int total_transacoes = 0;

/* Funções auxiliares internas */

/**
 * @brief Obtém a data e hora atuais no formato DD/MM/AAAA HH:MM.
 *
 * @param buf   Buffer de destino (deve ter pelo menos 20 bytes).
 * @param tam   Tamanho do buffer.
 * @return void Preenche o buffer com a data/hora atual.
 */
static void obter_data_hora(char *buf, int tam) {
    time_t agora = time(NULL); /* Obtem o tempo atual do sistema */
    struct tm *t = localtime(&agora); /* Converte para a estrutura local */
    /* Formato exigido pelo documento: DD/MM/AAAA HH:MM */
    strftime(buf, tam, "%d/%m/%Y %H:%M", t);
}

/**
 * @brief Registra uma transação na estrutura interna.
 *
 * @param id_conta          Conta de origem.
 * @param id_conta_destino  Conta de destino (-1 se não for transferência).
 * @param tipo              "deposito", "saque" ou "transferencia".
 * @param valor             Valor da operação.
 * @return int  0 em caso de sucesso, -1 se o limite de transações foi atingido.
 */
static int registrar_transacao(int id_conta, int id_conta_destino,
                               const char *tipo, double valor) {
    Transacao *t; /* Ponteiro para a nova transacao no array */
    
    if (total_transacoes >= MAX_TRANSACOES) {
        return -1;
    }

    t = &transacoes[total_transacoes];

    t->id_transacao     = total_transacoes + 1;   /* IDs começam em 1 */
    t->id_conta         = id_conta;
    t->id_conta_destino = id_conta_destino;
    t->valor            = valor;
    strncpy(t->tipo, tipo, sizeof(t->tipo) - 1);
    t->tipo[sizeof(t->tipo) - 1] = '\0';
    obter_data_hora(t->data, sizeof(t->data));

    total_transacoes++;
    return 0;
}

/* Interface pública */

/**
 * @brief Registra um depósito em uma conta bancária.
 * 
 * @param id_conta Número da conta de destino (entrada).
 * @param valor Quantia a ser depositada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não exista ou o valor seja inválido.
 */
int depositar(int id_conta, double valor) {
    double saldo_atual = 0.0; /* Armazena o saldo atual da conta destino */

    /* RF-09: validar valor */
    if (valor <= 0.0) {
        return -1;
    }

    /* Verificar existência da conta (consultar_saldo retorna -1 se inexistente) */
    if (consultar_saldo(id_conta, &saldo_atual) != 0) {
        return -1;
    }

    /* Creditar valor */
    if (atualiza_saldo(id_conta, valor) != 0) {
        return -1;
    }

    /* Registrar transação na estrutura interna */
    if (registrar_transacao(id_conta, -1, "deposito", valor) != 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief Registra um saque em uma conta bancária.
 * 
 * @param id_conta Número da conta de origem (entrada).
 * @param valor Quantia a ser sacada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não exista, o valor seja inválido ou o saldo seja insuficiente.
 */
int sacar(int id_conta, double valor) {
    double saldo_atual = 0.0; /* Armazena o saldo atual da conta de origem */

    /* RF-10: validar valor */
    if (valor <= 0.0) {
        return -1;
    }

    /* Verificar existência da conta e obter saldo atual */
    if (consultar_saldo(id_conta, &saldo_atual) != 0) {
        return -1;
    }

    /* RF-10: verificar saldo suficiente */
    if (saldo_atual < valor) {
        return -1;
    }

    /* Debitar valor (negativo para subtração) */
    if (atualiza_saldo(id_conta, -valor) != 0) {
        return -1;
    }

    /* Registrar transação na estrutura interna */
    if (registrar_transacao(id_conta, -1, "saque", valor) != 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief Realiza a transferência de valores entre duas contas bancárias.
 * 
 * @param id_conta_origem Número da conta de origem (entrada).
 * @param id_conta_destino Número da conta de destino (entrada).
 * @param valor Quantia a ser transferida (entrada).
 * @return int Retorna 0 para sucesso, ou -1 em caso de contas inválidas, valor incorreto ou saldo insuficiente.
 */
int transferir(int id_conta_origem, int id_conta_destino, double valor) {
    double saldo_origem = 0.0; /* Armazena o saldo da conta de origem */
    double saldo_destino = 0.0; /* Armazena o saldo da conta de destino */

    /* RF-11: validar valor */
    if (valor <= 0.0) {
        return -1;
    }

    /* CT-12: origem e destino não podem ser iguais */
    if (id_conta_origem == id_conta_destino) {
        return -1;
    }

    /* RF-11: verificar existência da conta de origem (via interface pública) */
    if (consultar_saldo(id_conta_origem, &saldo_origem) != 0) {
        return -1;
    }

    /* RF-11: verificar existência da conta de destino (via interface pública) */
    if (consultar_saldo(id_conta_destino, &saldo_destino) != 0) {
        return -1;
    }

    /* RF-11: verificar saldo suficiente na origem */
    if (saldo_origem < valor) {
        return -1;
    }

    /* Debitar da origem */
    if (atualiza_saldo(id_conta_origem, -valor) != 0) {
        return -1;
    }

    /* Creditar no destino */
    if (atualiza_saldo(id_conta_destino, valor) != 0) {
        /* Tentar reverter o débito para manter consistência */
        atualiza_saldo(id_conta_origem, valor);
        return -1;
    }

    /* Registrar uma única transação com ambas as contas */
    if (registrar_transacao(id_conta_origem, id_conta_destino, "transferencia", valor) != 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief Exibe no console o histórico de todas as transações associadas a uma conta.
 * 
 * Exibe as transações em ordem cronológica de registro.
 * 
 * @param id_conta ID da conta (entrada).
 */
void listar_transacoes(int id_conta) {
    int i; /* Variavel iteradora para percorrer o array de transacoes */
    int encontrou = 0; /* Flag booleana indicando se foram encontradas transacoes */

    printf("\n╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                   HISTÓRICO DE TRANSAÇÕES - Conta %-4d                    ║\n", id_conta);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");

    for (i = 0; i < total_transacoes; i++) {
        Transacao *t = &transacoes[i]; /* Ponteiro auxiliar para a transacao atual */
        char extra_info[30] = ""; /* Armazena detalhes sobre transferencia (origem/destino) */
        char linha_completa[100]; /* Buffer para formatacao da linha para ajuste da borda */

        /* RF-12: exibir transações da conta (origem ou destino) */
        if (t->id_conta != id_conta && t->id_conta_destino != id_conta) {
            continue;
        }

        encontrou = 1;

        /* Para transferências, usar ascii puro para garantir o alinhamento em %-73s */
        if (strcmp(t->tipo, "transferencia") == 0) {
            if (t->id_conta == id_conta) {
                sprintf(extra_info, " | -> Destino: %-3d", t->id_conta_destino);
            } else {
                sprintf(extra_info, " | <- Origem:  %-3d", t->id_conta);
            }
        }

        /* Monta a string contendo as informacoes e imprime no formato da tabela */
        sprintf(linha_completa, "[#%03d] %-14s | R$ %-9.2f | %s%s", 
                t->id_transacao, t->tipo, t->valor, t->data, extra_info);
        
        printf("║ %-73s ║\n", linha_completa);
    }

    if (!encontrou) {
        printf("║               Nenhuma transação registrada para esta conta.               ║\n");
    }
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

/* Persistência */

/**
 * @brief Salva os dados das transações em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_transacoes_arquivo(void) {
    int i; /* Variavel iteradora para gravacao dos registros */
    FILE *f = fopen("transacoes.txt", "w"); /* Ponteiro para criar e gravar o arquivo de persistencia */
    
    if (f == NULL) {
        return -1;
    }

    fprintf(f, "%d\n", total_transacoes);

    for (i = 0; i < total_transacoes; i++) {
        Transacao *t = &transacoes[i]; /* Ponteiro auxiliar para a transacao atual */
        fprintf(f, "%d %d %d %s %.2f %s\n",
                t->id_transacao,
                t->id_conta,
                t->id_conta_destino,
                t->tipo,
                t->valor,
                t->data);
    }

    fclose(f);
    return 0;
}

/**
 * @brief Carrega os dados das transações de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_transacoes_arquivo(void) {
    int i; /* Variavel iteradora de loops */
    int total = 0; /* Armazena o total de registros do arquivo */
    FILE *f = fopen("transacoes.txt", "r"); /* Ponteiro para leitura do arquivo de persistencia */
    
    if (f == NULL) {
        /* Arquivo inexistente na primeira execução — não é erro crítico */
        return -1;
    }

    if (fscanf(f, "%d\n", &total) != 1) {
        fclose(f);
        return -1;
    }

    for (i = 0; i < total && i < MAX_TRANSACOES; i++) {
        Transacao *t = &transacoes[i]; /* Ponteiro para a transacao do iterador */
        char hora[10]; /* Buffer temporario para ler a segunda parte do datetime */
        
        /* data contém espaço — lida com dois tokens separados por scanf */
        if (fscanf(f, "%d %d %d %19s %lf %10s %9s",
                   &t->id_transacao,
                   &t->id_conta,
                   &t->id_conta_destino,
                   t->tipo,
                   &t->valor,
                   t->data,
                   hora) != 7) {
            fclose(f);
            return -1;
        }
        /* Reagrupar data e hora (separados pelo scanf) */
        strncat(t->data, " ", sizeof(t->data) - strlen(t->data) - 1);
        strncat(t->data, hora, sizeof(t->data) - strlen(t->data) - 1);
    }

    total_transacoes = total;
    fclose(f);
    return 0;
}