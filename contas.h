#ifndef CONTAS_H
#define CONTAS_H

#define MAX_CONTAS 150

/**
 * @brief Estrutura que representa uma conta bancária associada a um cliente.
 */
typedef struct {
    int id_conta;           /**< Identificador único (PK) */
    int id_cliente;         /**< Referência ao cliente titular (FK) */
    char tipo[20];          /**< Tipo da conta: "corrente" ou "poupanca" */
    double saldo;           /**< Saldo atual */
} Conta;

/**
 * @brief Abre uma nova conta bancária para um cliente cadastrado.
 * 
 * @param id_cliente ID do cliente proprietário da conta (entrada).
 * @param tipo Ponteiro para a string representando o tipo da conta ("corrente" ou "poupanca") (entrada).
 * @param id_conta_gerada Ponteiro de saída que receberá o número da conta gerado (saída).
 * @return int Retorna 0 para sucesso, ou -1 caso o cliente não seja encontrado ou o limite de contas seja atingido.
 */
int abrir_conta(int id_cliente, const char *tipo, int *id_conta_gerada);

/**
 * @brief Consulta o saldo disponível de uma conta específica.
 * 
 * @param id_conta Número da conta a ser consultada (entrada).
 * @param saldo_retornado Ponteiro de saída que receberá o valor do saldo (saída).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não seja encontrada.
 */
int consultar_saldo(int id_conta, double *saldo_retornado);

/**
 * @brief Atualiza o saldo de uma conta específica (somando ou subtraindo um valor).
 * 
 * @param id_conta Número da conta a ser atualizada (entrada).
 * @param valor Quantia a ser somada ou subtraída (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a alteração resulte em saldo negativo ou se a conta não existir.
 */
int atualiza_saldo(int id_conta, double valor);

/**
 * @brief Exibe no console todas as contas associadas a um determinado cliente.
 * 
 * @param id_cliente ID do cliente titular (entrada).
 */
void listar_contas(int id_cliente);

/**
 * @brief Localiza uma conta na estrutura interna pelo seu ID.
 * 
 * @param id_conta ID da conta a ser buscada (entrada).
 * @param conta_retornada Ponteiro de saída que receberá os dados da conta encontrada (saída).
 * @return int Retorna 0 em caso de sucesso, ou -1 se a conta não for encontrada.
 */
int buscar_conta(int id_conta, Conta *conta_retornada);

/**
 * @brief Salva os dados das contas em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_contas_arquivo(void);

/**
 * @brief Carrega os dados das contas de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_contas_arquivo(void);

#endif /* CONTAS_H */
