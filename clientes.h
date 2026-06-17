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
 * @brief Cadastra um novo cliente, gerando um ID único e incremental.
 * @param nome  String não-vazia com o nome do cliente (entrada).
 * @param cpf   String com 11 dígitos, com ou sem máscara; será normalizada
 *              para o formato 000.000.000-00 antes de armazenar (entrada).
 * @param senha String com no mínimo 6 caracteres (entrada).
 * @param id_cliente_gerado  Endereço onde o ID gerado será escrito (saída).
 * @return int  0 em sucesso; -1 em erro.
 *
 * Assertiva de entrada:  nome, cpf, senha e id_cliente_gerado são ponteiros
 *                        válidos (não-nulos).
 * Assertiva de saída:
 *   - se 0: existe no armazenamento um cliente com os dados informados (CPF
 *     em formato canônico), *id_cliente_gerado > 0 e o total aumentou em 1;
 *   - se -1: o armazenamento NÃO foi alterado. Causas: ponteiro nulo, limite
 *     MAX_CLIENTES atingido, nome vazio, senha < 6, CPF inválido (caracteres
 *     não permitidos ou quantidade de dígitos != 11) ou CPF já cadastrado.
 */
int cadastrar_cliente(const char *nome, const char *cpf, const char *senha, int *id_cliente_gerado);

/**
 * @brief Autentica um cliente por CPF e senha.
 * @param cpf   String com o CPF (com ou sem máscara; é normalizada) (entrada).
 * @param senha String com a senha (comparação case-sensitive) (entrada).
 * @param id_cliente_retornado  Endereço que recebe o ID autenticado (saída).
 * @return int  0 em sucesso; -1 para credenciais inválidas.
 *
 * Assertiva de entrada:  cpf, senha e id_cliente_retornado são ponteiros
 *                        válidos (não-nulos).
 * Assertiva de saída:
 *   - se 0: *id_cliente_retornado contém o ID do cliente cujo CPF e senha
 *     conferem; o armazenamento não é alterado;
 *   - se -1: nenhum cliente confere; o armazenamento não é alterado. Por
 *     segurança, não se distingue CPF inexistente de senha incorreta.
 */
int login(const char *cpf, const char *senha, int *id_cliente_retornado);

/**
 * @brief Localiza um cliente pelo ID e copia seus dados para o chamador.
 * @param id_cliente  ID do cliente buscado (entrada).
 * @param cliente_retornado  Endereço de um Cliente que recebe a cópia (saída).
 * @return int  0 em sucesso; -1 se não encontrado ou parâmetro inválido.
 *
 * Assertiva de entrada:  cliente_retornado é um ponteiro válido (não-nulo).
 * Assertiva de saída:
 *   - se 0: *cliente_retornado é uma cópia do registro cujo id == id_cliente;
 *   - se -1: *cliente_retornado não é modificado de forma confiável. Causas:
 *     ponteiro nulo, id_cliente <= 0 ou ID inexistente.
 */
int buscar_cliente(int id_cliente, Cliente *cliente_retornado);

/**
 * @brief Exibe no console todos os clientes (ID, nome e CPF).
 * @return void. Apenas escreve em stdout.
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:    o campo senha NUNCA é exibido; o armazenamento não é
 *                        alterado; se vazio, informa que não há clientes.
 */
void listar_clientes(void);

/**
 * @brief Grava todos os clientes em memória no arquivo texto clientes.txt.
 * @return int  0 em sucesso; -1 se não for possível abrir o arquivo p/ escrita.
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:
 *   - se 0: clientes.txt contém uma linha por cliente no formato
 *     id;nome;cpf;senha;
 *   - se -1: o arquivo não pôde ser aberto; o armazenamento não é alterado.
 */
int salvar_clientes_arquivo(void);

/**
 * @brief Recarrega os clientes a partir de clientes.txt, substituindo o
 *        conteúdo em memória.
 * @return int  0 em sucesso; -1 se o arquivo não existir (1ª execução).
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:
 *   - o armazenamento interno é reinicializado antes da leitura; o gerador de
 *     ID passa a ser (maior_id_lido + 1);
 *   - se 0: o armazenamento reflete o conteúdo do arquivo (até MAX_CLIENTES);
 *   - se -1: arquivo inexistente; o armazenamento fica vazio (não é erro fatal).
 */
int carregar_clientes_arquivo(void);

#endif /* CLIENTES_H */
