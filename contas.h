#ifndef CONTAS_H
#define CONTAS_H

/**
 * @brief Abre uma nova conta bancaria para um cliente cadastrado.
 * 
 * @param id_cliente ID do cliente proprietario da conta (entrada).
 * @param out_numero_conta Ponteiro de saida que recebera o numero da conta gerado (saida).
 * @return int Retorna 0 para sucesso, ou -1 caso o cliente nao seja encontrado ou o limite de contas seja atingido.
 */
int abrir_conta(int id_cliente, int* out_numero_conta);

/**
 * @brief Consulta o saldo disponivel de uma conta especifica.
 * 
 * @param numero_conta Numero da conta a ser consultada (entrada).
 * @param out_saldo Ponteiro de saida que recebera o valor do saldo (saida).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta nao seja encontrada.
 */
int consultar_saldo(int numero_conta, float* out_saldo);

/**
 * @brief Realiza um deposito em uma conta bancaria.
 * 
 * @param numero_conta Numero da conta destino (entrada).
 * @param valor Quantia a ser depositada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta nao exista ou o valor seja invalido.
 */
int depositar(int numero_conta, float valor);

/**
 * @brief Realiza um saque em uma conta bancaria.
 * 
 * @param numero_conta Numero da conta de origem (entrada).
 * @param valor Quantia a ser sacada (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta nao exista, o valor seja invalido ou o saldo seja insuficiente.
 */
int sacar(int numero_conta, float valor);

#endif /* CONTAS_H */
