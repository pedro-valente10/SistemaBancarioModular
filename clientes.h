#ifndef CLIENTES_H
#define CLIENTES_H

#define MAX_CLIENTES 50

/**
 * @brief Estrutura que representa um cliente do banco.
 */
typedef struct {
    int id_cliente;         /**< Identificador único (PK) */
    char nome[100];         /**< Nome completo */
    char cpf[15];           /**< CPF no formato 000.000.000-00 */
    char senha[50];         /**< Senha de acesso */
} Cliente;

/**
 * @brief Cadastra um novo cliente no sistema.
 * 
 * @param nome Ponteiro para a string com o nome do cliente (entrada).
 * @param cpf Ponteiro para a string com o CPF do cliente (entrada).
 * @param senha Ponteiro para a string com a senha do cliente (entrada).
 * @param id_cliente_gerado Ponteiro de saída que receberá o ID gerado para o cliente (saída).
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro (CPF já cadastrado ou limite de clientes atingido).
 */
int cadastrar_cliente(const char *nome, const char *cpf, const char *senha, int *id_cliente_gerado);

/**
 * @brief Realiza a autenticação de um cliente com CPF e senha.
 * 
 * @param cpf Ponteiro para a string com o CPF do cliente (entrada).
 * @param senha Ponteiro para a string com a senha do cliente (entrada).
 * @param id_cliente_retornado Ponteiro de saída que receberá o ID do cliente autenticado (saída).
 * @return int Retorna 0 para sucesso no login, ou -1 para credenciais incorretas (erro).
 */
int login(const char *cpf, const char *senha, int *id_cliente_retornado);

/**
 * @brief Localiza um cliente na estrutura interna pelo seu ID único.
 * 
 * @param id_cliente ID do cliente a ser buscado (entrada).
 * @param cliente_retornado Ponteiro de saída que receberá os dados do cliente encontrado (saída).
 * @return int Retorna 0 em caso de sucesso, ou -1 se o ID não for encontrado.
 */
int buscar_cliente(int id_cliente, Cliente *cliente_retornado);

/**
 * @brief Exibe no console todos os clientes cadastrados no sistema.
 * 
 * Apenas realiza saída para stdout e não exibe o campo senha.
 */
void listar_clientes(void);

/**
 * @brief Salva os dados dos clientes em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 */
int salvar_clientes_arquivo(void);

/**
 * @brief Carrega os dados dos clientes de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 */
int carregar_clientes_arquivo(void);

#endif /* CLIENTES_H */
