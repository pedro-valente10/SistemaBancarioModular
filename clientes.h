#ifndef CLIENTES_H
#define CLIENTES_H

/**
 * @brief Cadastra um novo cliente no sistema.
 * 
 * @param nome Ponteiro para a string com o nome do cliente (entrada).
 * @param cpf Ponteiro para a string com o CPF do cliente (entrada).
 * @param senha Ponteiro para a string com a senha do cliente (entrada).
 * @param out_id Ponteiro de saida que recebera o ID gerado para o cliente (saida).
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro (CPF ja cadastrado ou limite de clientes atingido).
 */
int cadastrar_cliente(const char* nome, const char* cpf, const char* senha, int* out_id);

/**
 * @brief Realiza a autenticacao de um cliente com CPF e senha.
 * 
 * @param cpf Ponteiro para a string com o CPF do cliente (entrada).
 * @param senha Ponteiro para a string com a senha do cliente (entrada).
 * @param out_id_cliente Ponteiro de saida que recebera o ID do cliente autenticado (saida).
 * @return int Retorna 0 para sucesso no login, ou -1 para credenciais incorretas (erro).
 */
int fazer_login(const char* cpf, const char* senha, int* out_id_cliente);

#endif /* CLIENTES_H */
