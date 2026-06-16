#ifndef TRANSACOES_H
#define TRANSACOES_H

#define MAX_TRANSACOES 300

/**
 * @brief Estrutura que representa uma operação financeira registrada no sistema.
 */
typedef struct {
    int id_transacao;       /**< Identificador único (PK) */
    int id_conta;           /**< Conta de origem (FK) */
    int id_conta_destino;   /**< Conta destino — para transferências (-1 caso contrário) */
    char tipo[20];          /**< Tipo da transação: "deposito", "saque" ou "transferencia" */
    double valor;           /**< Valor da operação */
    char data[20];          /**< Data e hora no formato DD/MM/AAAA HH:MM */
} Transacao;

/**
 * @brief Registra um depósito em uma conta bancária.
 * 
 * @param id_conta Número da conta de destino (entrada).
 * @param valor Quantia a ser depositada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não exista ou o valor seja inválido.
 */
int depositar(int id_conta, double valor);

/**
 * @brief Registra um saque em uma conta bancária.
 * 
 * @param id_conta Número da conta de origem (entrada).
 * @param valor Quantia a ser sacada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não exista, o valor seja inválido ou o saldo seja insuficiente.
 */
int sacar(int id_conta, double valor);

/**
 * @brief Realiza a transferência de valores entre duas contas bancárias.
 * 
 * @param id_conta_origem Número da conta de origem (entrada).
 * @param id_conta_destino Número da conta de destino (entrada).
 * @param valor Quantia a ser transferida (entrada).
 * @return int Retorna 0 para sucesso, ou -1 em caso de contas inválidas, valor incorreto ou saldo insuficiente.
 */
int transferir(int id_conta_origem, int id_conta_destino, double valor);

/**
 * @brief Exibe no console o histórico de todas as transações associadas a uma conta.
 * 
 * Exibe as transações em ordem cronológica de registro.
 * 
 * @param id_conta ID da conta (entrada).
 */
void listar_transacoes(int id_conta);

/**
 * @brief Salva os dados das transações em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_transacoes_arquivo(void);

/**
 * @brief Carrega os dados das transações de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_transacoes_arquivo(void);

#endif /* TRANSACOES_H */
