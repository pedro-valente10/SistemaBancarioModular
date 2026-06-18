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
 *
 * Assertiva de entrada:  tipo e id_conta_gerada devem ser ponteiros validos (nao-nulos).
 * Assertiva de saída:
 *   - se 0: uma nova conta é inserida no array encapsulado e *id_conta_gerada recebe o ID gerado.
 *   - se -1: limite atingido, cliente inexistente ou parametros invalidos. O armazenamento nao muda.
 */
int abrir_conta(int id_cliente, const char *tipo, int *id_conta_gerada);

/**
 * @brief Consulta o saldo disponível de uma conta específica.
 * 
 * @param id_conta Número da conta a ser consultada (entrada).
 * @param saldo_retornado Ponteiro de saída que receberá o valor do saldo (saída).
 * @return int Retorna 0 para sucesso, ou -1 caso a conta não seja encontrada.
 *
 * Assertiva de entrada:  saldo_retornado deve ser ponteiro valido (nao-nulo).
 * Assertiva de saída:
 *   - se 0: *saldo_retornado contem o saldo atual da conta.
 *   - se -1: conta nao encontrada ou parametro invalido. O saldo nao e retornado.
 */
int consultar_saldo(int id_conta, double *saldo_retornado);

/**
 * @brief Atualiza o saldo de uma conta específica (somando ou subtraindo um valor).
 * 
 * @param id_conta Número da conta a ser atualizada (entrada).
 * @param valor Quantia a ser somada ou subtraída (entrada).
 * @return int Retorna 0 para sucesso, ou -1 caso a alteração resulte em saldo negativo ou se a conta não existir.
 *
 * Assertiva de entrada:  nenhuma (id_conta é validado internamente).
 * Assertiva de saída:
 *   - se 0: o saldo da conta correspondente é modificado (somado ao valor).
 *   - se -1: a operacao é negada e o saldo da conta permanece inalterado.
 */
int atualiza_saldo(int id_conta, double valor);

/**
 * @brief Exibe no console todas as contas associadas a um determinado cliente.
 * 
 * @param id_cliente ID do cliente titular (entrada).
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:    imprime na tela os dados da conta. O armazenamento e as
 *                        estruturas de dados nao sofrem nenhuma alteracao.
 */
void listar_contas(int id_cliente);

/**
 * @brief Localiza uma conta na estrutura interna pelo seu ID.
 * 
 * @param id_conta ID da conta a ser buscada (entrada).
 * @param conta_retornada Ponteiro de saída que receberá os dados da conta encontrada (saída).
 * @return int Retorna 0 em caso de sucesso, ou -1 se a conta não for encontrada.
 *
 * Assertiva de entrada:  conta_retornada deve ser um ponteiro valido (nao-nulo).
 * Assertiva de saída:
 *   - se 0: *conta_retornada contem uma copia exata da struct da conta encontrada.
 *   - se -1: a conta nao foi encontrada e o conteudo de *conta_retornada é ignorado.
 */
int buscar_conta(int id_conta, Conta *conta_retornada);

/**
 * @brief Salva os dados das contas em um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/escrever o arquivo.
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:
 *   - se 0: os dados encapsulados foram gravados com sucesso no arquivo.
 *   - se -1: falha na criacao do arquivo. Dados em memoria nao sao alterados.
 */
int salvar_contas_arquivo(void);

/**
 * @brief Carrega os dados das contas de um arquivo de texto.
 * 
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver erro ao abrir/ler o arquivo.
 *
 * Assertiva de entrada:  nenhuma.
 * Assertiva de saída:
 *   - se 0: o TAD interno é preenchido com as contas lidas do arquivo.
 *   - se -1: falha na leitura. O estado do sistema para contas é reiniciado para vazio.
 */
int carregar_contas_arquivo(void);

#endif /* CONTAS_H */
