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

/* ─── Armazenamento interno (encapsulado — não exposto via .h) ─────────────── */

/** Array estático de transações — acessível apenas neste módulo */
static Transacao transacoes[MAX_TRANSACOES];

/** Contador de transações registradas */
static int total_transacoes = 0;

/* ─── Funções auxiliares internas ───────────────────────────────────────────── */

/**
 * @brief Obtém a data e hora atuais no formato DD/MM/AAAA HH:MM.
 *
 * @param buf   Buffer de destino (deve ter pelo menos 20 bytes).
 * @param tam   Tamanho do buffer.
 */
static void obter_data_hora(char *buf, int tam) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
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
    if (total_transacoes >= MAX_TRANSACOES) {
        fprintf(stderr, "⚠ Erro: limite máximo de transações atingido.\n");
        return -1;
    }

    Transacao *t = &transacoes[total_transacoes];

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

/* ─── Interface pública ─────────────────────────────────────────────────────── */

/**
 * @brief Registra um depósito em uma conta bancária.
 *
 * Valida os parâmetros, credita o valor via atualiza_saldo() e registra
 * a transação na estrutura interna com tipo "deposito".
 *
 * @param id_conta  Número da conta de destino (entrada).
 * @param valor     Quantia a ser depositada — deve ser > 0 (entrada).
 * @return int  0 em caso de sucesso, -1 se a conta não existir, o valor for
 *              inválido ou o limite de transações tiver sido atingido.
 */
int depositar(int id_conta, double valor) {
    /* RF-09: validar valor */
    if (valor <= 0.0) {
        fprintf(stderr, "⚠ Erro: valor de depósito deve ser maior que zero.\n");
        return -1;
    }

    /* Verificar existência da conta (consultar_saldo retorna -1 se inexistente) */
    double saldo_atual = 0.0;
    if (consultar_saldo(id_conta, &saldo_atual) != 0) {
        fprintf(stderr, "⚠ Erro: conta %d não encontrada.\n", id_conta);
        return -1;
    }

    /* Creditar valor */
    if (atualiza_saldo(id_conta, valor) != 0) {
        fprintf(stderr, "⚠ Erro: não foi possível atualizar o saldo da conta %d.\n", id_conta);
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
 * Verifica saldo disponível via consultar_saldo() antes de debitar.
 * Chama atualiza_saldo() com valor negativo e registra a transação
 * com tipo "saque".
 *
 * @param id_conta  Número da conta de origem (entrada).
 * @param valor     Quantia a ser sacada — deve ser > 0 e <= saldo (entrada).
 * @return int  0 em caso de sucesso, -1 se a conta não existir, o valor for
 *              inválido, o saldo for insuficiente ou o limite de transações
 *              tiver sido atingido.
 */
int sacar(int id_conta, double valor) {
    /* RF-10: validar valor */
    if (valor <= 0.0) {
        fprintf(stderr, "⚠ Erro: valor de saque deve ser maior que zero.\n");
        return -1;
    }

    /* Verificar existência da conta e obter saldo atual */
    double saldo_atual = 0.0;
    if (consultar_saldo(id_conta, &saldo_atual) != 0) {
        fprintf(stderr, "⚠ Erro: conta %d não encontrada.\n", id_conta);
        return -1;
    }

    /* RF-10: verificar saldo suficiente */
    if (saldo_atual < valor) {
        fprintf(stderr, "⚠ Erro: saldo insuficiente. Saldo disponível: R$ %.2f\n", saldo_atual);
        return -1;
    }

    /* Debitar valor (negativo para subtração) */
    if (atualiza_saldo(id_conta, -valor) != 0) {
        fprintf(stderr, "⚠ Erro: não foi possível atualizar o saldo da conta %d.\n", id_conta);
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
 * Valida a existência de ambas as contas e o saldo disponível (via
 * consultar_saldo()), debita da origem e credita no destino via
 * atualiza_saldo(). Registra uma única transação com ambos os IDs.
 *
 * @param id_conta_origem   Número da conta de origem (entrada).
 * @param id_conta_destino  Número da conta de destino (entrada).
 * @param valor             Quantia a ser transferida — deve ser > 0 (entrada).
 * @return int  0 em caso de sucesso, -1 em caso de validação falha, saldo
 *              insuficiente ou limite de transações atingido.
 */
int transferir(int id_conta_origem, int id_conta_destino, double valor) {
    /* RF-11: validar valor */
    if (valor <= 0.0) {
        fprintf(stderr, "⚠ Erro: valor de transferência deve ser maior que zero.\n");
        return -1;
    }

    /* CT-12: origem e destino não podem ser iguais */
    if (id_conta_origem == id_conta_destino) {
        fprintf(stderr, "⚠ Erro: conta de origem e destino não podem ser iguais.\n");
        return -1;
    }

    /* RF-11: verificar existência da conta de origem (via interface pública) */
    double saldo_origem = 0.0;
    if (consultar_saldo(id_conta_origem, &saldo_origem) != 0) {
        fprintf(stderr, "⚠ Erro: conta de origem %d não encontrada.\n", id_conta_origem);
        return -1;
    }

    /* RF-11: verificar existência da conta de destino (via interface pública) */
    double saldo_destino = 0.0;
    if (consultar_saldo(id_conta_destino, &saldo_destino) != 0) {
        fprintf(stderr, "⚠ Erro: conta de destino %d não encontrada.\n", id_conta_destino);
        return -1;
    }

    /* RF-11: verificar saldo suficiente na origem */
    if (saldo_origem < valor) {
        fprintf(stderr, "⚠ Erro: saldo insuficiente na conta de origem. Disponível: R$ %.2f\n",
                saldo_origem);
        return -1;
    }

    /* Debitar da origem */
    if (atualiza_saldo(id_conta_origem, -valor) != 0) {
        fprintf(stderr, "⚠ Erro: falha ao debitar da conta de origem %d.\n", id_conta_origem);
        return -1;
    }

    /* Creditar no destino */
    if (atualiza_saldo(id_conta_destino, valor) != 0) {
        /* Tentar reverter o débito para manter consistência */
        atualiza_saldo(id_conta_origem, valor);
        fprintf(stderr, "⚠ Erro: falha ao creditar na conta de destino %d. Operação revertida.\n",
                id_conta_destino);
        return -1;
    }

    /* Registrar uma única transação com ambas as contas */
    if (registrar_transacao(id_conta_origem, id_conta_destino, "transferencia", valor) != 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief Exibe no console o histórico de transações de uma conta.
 *
 * Percorre a estrutura interna e imprime todas as transações onde
 * id_conta == id_conta_origem OU id_conta_destino == id_conta (para
 * que transferências recebidas também apareçam no extrato).
 * As transações são exibidas em ordem de registro (cronológica).
 *
 * @param id_conta  ID da conta cujo histórico será exibido (entrada).
 */
void listar_transacoes(int id_conta) {
    int encontrou = 0;

    printf("\n┌──────────────────────────────────────────────────────┐\n");
    printf("│          HISTÓRICO DE TRANSAÇÕES — Conta %-4d        │\n", id_conta);
    printf("└──────────────────────────────────────────────────────┘\n");

    for (int i = 0; i < total_transacoes; i++) {
        Transacao *t = &transacoes[i];

        /* RF-12: exibir transações da conta (origem ou destino) */
        if (t->id_conta != id_conta && t->id_conta_destino != id_conta) {
            continue;
        }

        encontrou = 1;

        printf("  [#%03d] %-14s | R$ %10.2f | %s",
               t->id_transacao, t->tipo, t->valor, t->data);

        /* Para transferências, indicar sentido e conta envolvida */
        if (strcmp(t->tipo, "transferencia") == 0) {
            if (t->id_conta == id_conta) {
                printf(" | → Destino: %d", t->id_conta_destino);
            } else {
                printf(" | ← Origem:  %d", t->id_conta);
            }
        }

        printf("\n");
    }

    if (!encontrou) {
        printf("  Nenhuma transação registrada para a conta %d.\n", id_conta);
    }

    printf("──────────────────────────────────────────────────────────\n");
}

/* ─── Persistência ──────────────────────────────────────────────────────────── */

/**
 * @brief Salva os dados das transações em um arquivo de texto.
 *
 * Formato de cada linha:
 *   id_transacao id_conta id_conta_destino tipo valor data
 *
 * @return int  0 em caso de sucesso, -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_transacoes_arquivo(void) {
    FILE *f = fopen("transacoes.txt", "w");
    if (f == NULL) {
        fprintf(stderr, "⚠ Erro: não foi possível abrir transacoes.txt para escrita.\n");
        return -1;
    }

    fprintf(f, "%d\n", total_transacoes);

    for (int i = 0; i < total_transacoes; i++) {
        Transacao *t = &transacoes[i];
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
 * Lê o arquivo gravado por salvar_transacoes_arquivo() e restaura
 * a estrutura interna de transações.
 *
 * @return int  0 em caso de sucesso, -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_transacoes_arquivo(void) {
    FILE *f = fopen("transacoes.txt", "r");
    if (f == NULL) {
        /* Arquivo inexistente na primeira execução — não é erro crítico */
        return -1;
    }

    int total = 0;
    if (fscanf(f, "%d\n", &total) != 1) {
        fclose(f);
        return -1;
    }

    for (int i = 0; i < total && i < MAX_TRANSACOES; i++) {
        Transacao *t = &transacoes[i];
        /* data contém espaço — lida com dois tokens separados por scanf */
        char hora[10];
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