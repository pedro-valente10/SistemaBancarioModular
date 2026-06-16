#ifndef TRANSACOES_H
#define TRANSACOES_H

/**
 * @brief Realiza a transferencia de valores entre duas contas bancarias.
 * 
 * @param conta_origem Numero da conta de origem (entrada).
 * @param conta_destino Numero da conta de destino (entrada).
 * @param valor Quantia a ser transferida (entrada).
 * @return int Retorna 0 para sucesso, ou -1 em caso de contas invalidas, valor incorreto ou saldo insuficiente.
 */
int transferir(int conta_origem, int conta_destino, float valor);

/**
 * @brief Obtem a quantidade total de transacoes registradas em uma conta.
 * 
 * @param numero_conta Numero da conta (entrada).
 * @param out_total Ponteiro de saida que recebera o total de transacoes registradas (saida).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta nao seja encontrada.
 */
int obter_total_transacoes(int numero_conta, int* out_total);

/**
 * @brief Obtem os detalhes de uma transacao de uma conta pelo seu indice no historico.
 * 
 * @param numero_conta Numero da conta (entrada).
 * @param index Indice da transacao no historico, comecando em 0 (entrada).
 * @param out_tipo Ponteiro de saida para a string que recebera o tipo da transacao (ex: "Deposito", "Saque") (saida).
 * @param out_valor Ponteiro de saida que recebera o valor da transacao (saida).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta nao exista ou o indice seja invalido.
 */
int obter_transacao(int numero_conta, int index, char* out_tipo, float* out_valor);

#endif /* TRANSACOES_H */
