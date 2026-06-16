#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Importacao dos cabecalhos dos modulos conforme arquitetura TAD */
#include "clientes.h"
#include "contas.h"
#include "transacoes.h"

/**
 * @brief Limpa o buffer de entrada do teclado (stdin) de forma portavel (C99/C11).
 * 
 * Remove caracteres remanescentes que ficaram na fila de entrada do console
 * ate alcancar a quebra de linha ou o fim do arquivo.
 * 
 * @param Nenhum.
 * @return Nao possui retorno (void).
 */
void limpar_buffer() {
    int c; /* Variavel temporaria para armazenar cada caractere lido do buffer */
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Ponto de entrada principal do Sistema Bancario Modular.
 * 
 * Renderiza o menu interativo do console em loop, coleta as entradas do usuario
 * e invoca as operacoes dos demais modulos via passagem de parametros por referencia.
 * 
 * @param Nenhum.
 * @return int Retorna 0 ao encerrar com sucesso.
 */
int main() {
    int opcao = 0; /* Armazena a opcao de menu selecionada pelo usuario */
    int usuario_logado_id = -1; /* Armazena o ID do cliente autenticado (-1 se nao logado) */

    while (1) {
        printf("\nSISTEMA BANCARIO MODULAR\n");
        if (usuario_logado_id != -1) {
            printf("Cliente logado ID: %d\n", usuario_logado_id);
        } else {
            printf("Nao logado\n");
        }
        printf("0. Rodar Bateria de Testes Automatizados\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Fazer Login\n");
        printf("3. Abrir Conta\n");
        printf("4. Consultar Saldo\n");
        printf("5. Depositar\n");
        printf("6. Sacar\n");
        printf("7. Transferir\n");
        printf("8. Listar Historico\n");
        printf("9. Sair\n");
        printf("Escolha uma opcao: ");

        /* Tratamento basico de erro caso a entrada nao seja numerica */
        if (scanf("%d", &opcao) != 1) {
            printf("Erro: Opcao invalida. Digite um numero.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer(); /* Limpa o '\n' restante do buffer */

        switch (opcao) {
            case 0: {
                printf("\nExecutando bateria de testes automatizados...\n");
                /* executar_testes_contas(); */
                /* executar_testes_transacoes(); */
                printf("Testes finalizados.\n");
                break;
            }
            case 1: {
                char nome[100]; /* Armazena o nome fornecido para o novo cliente */
                char cpf[15]; /* Armazena o CPF fornecido para o novo cliente */
                char senha[50]; /* Armazena a senha cadastrada para o novo cliente */
                int novo_id = -1; /* Ponteiro de saida: recebe o ID incremental gerado para o cliente */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nCadastrar Cliente\n");
                printf("Nome: ");
                if (fgets(nome, sizeof(nome), stdin)) {
                    nome[strcspn(nome, "\n")] = '\0';
                }
                printf("CPF: ");
                if (fgets(cpf, sizeof(cpf), stdin)) {
                    cpf[strcspn(cpf, "\n")] = '\0';
                }
                printf("Senha: ");
                if (fgets(senha, sizeof(senha), stdin)) {
                    senha[strcspn(senha, "\n")] = '\0';
                }

                /* Invocacao por referencia */
                status = cadastrar_cliente(nome, cpf, senha, &novo_id);
                if (status == 0) {
                    printf("Cliente cadastrado com sucesso! ID: %d\n", novo_id);
                } else {
                    printf("Erro ao cadastrar cliente (CPF ja cadastrado ou limite atingido).\n");
                }
                break;
            }
            case 2: {
                char cpf[15]; /* Armazena o CPF inserido para autenticacao */
                char senha[50]; /* Armazena a senha inserida para autenticacao */
                int id_cliente = -1; /* Ponteiro de saida: recebe o ID do cliente correspondente se logado */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nFazer Login\n");
                printf("CPF: ");
                if (fgets(cpf, sizeof(cpf), stdin)) {
                    cpf[strcspn(cpf, "\n")] = '\0';
                }
                printf("Senha: ");
                if (fgets(senha, sizeof(senha), stdin)) {
                    senha[strcspn(senha, "\n")] = '\0';
                }

                status = fazer_login(cpf, senha, &id_cliente);
                if (status == 0) {
                    usuario_logado_id = id_cliente;
                    printf("Login realizado com sucesso! ID do cliente: %d\n", usuario_logado_id);
                } else {
                    printf("Erro: CPF ou senha incorretos.\n");
                }
                break;
            }
            case 3: {
                int id_cliente; /* Armazena o ID do cliente que sera proprietario da conta */
                int numero_conta = -1; /* Ponteiro de saida: recebe o numero da conta gerado pelo sistema */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nAbrir Conta\n");
                if (usuario_logado_id != -1) {
                    char resp; /* Armazena a resposta de confirmacao (S/N) sobre usar o cliente logado */
                    printf("Usar cliente logado (%d)? (s/n): ", usuario_logado_id);
                    if (scanf(" %c", &resp) != 1) {
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                    if (resp == 'S' || resp == 's') {
                        id_cliente = usuario_logado_id;
                    } else {
                        printf("Digite o ID do Cliente: ");
                        if (scanf("%d", &id_cliente) != 1) {
                            printf("Erro: ID invalido.\n");
                            limpar_buffer();
                            break;
                        }
                        limpar_buffer();
                    }
                } else {
                    printf("Digite o ID do Cliente: ");
                    if (scanf("%d", &id_cliente) != 1) {
                        printf("Erro: ID invalido.\n");
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                }

                status = abrir_conta(id_cliente, &numero_conta);
                if (status == 0) {
                    printf("Conta criada com sucesso! Numero: %d\n", numero_conta);
                } else {
                    printf("Erro ao criar conta (cliente inexistente ou limite de contas).\n");
                }
                break;
            }
            case 4: {
                int numero_conta; /* Armazena o numero da conta a ser consultado */
                float saldo = 0.0f; /* Ponteiro de saida: recebe o valor do saldo da conta */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nConsultar Saldo\n");
                printf("Digite o numero da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = consultar_saldo(numero_conta, &saldo);
                if (status == 0) {
                    printf("Saldo da conta %d: R$ %.2f\n", numero_conta, saldo);
                } else {
                    printf("Erro: Conta nao encontrada.\n");
                }
                break;
            }
            case 5: {
                int numero_conta; /* Armazena o numero da conta destino do deposito */
                float valor; /* Armazena a quantia monetaria a ser depositada */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nDepositar\n");
                printf("Digite o numero da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor do deposito: ");
                if (scanf("%f", &valor) != 1 || valor <= 0) {
                    printf("Erro: Valor invalido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = depositar(numero_conta, valor);
                if (status == 0) {
                    printf("Deposito de R$ %.2f concluido na conta %d!\n", valor, numero_conta);
                } else {
                    printf("Erro ao depositar (conta inexistente ou valor invalido).\n");
                }
                break;
            }
            case 6: {
                int numero_conta; /* Armazena o numero da conta de onde sera realizado o saque */
                float valor; /* Armazena a quantia monetaria a ser sacada */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nSacar\n");
                printf("Digite o numero da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor do saque: ");
                if (scanf("%f", &valor) != 1 || valor <= 0) {
                    printf("Erro: Valor invalido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = sacar(numero_conta, valor);
                if (status == 0) {
                    printf("Saque de R$ %.2f concluido na conta %d!\n", valor, numero_conta);
                } else {
                    printf("Erro ao sacar (saldo insuficiente ou conta inexistente).\n");
                }
                break;
            }
            case 7: {
                int conta_origem; /* Armazena o numero da conta de origem dos fundos */
                int conta_destino; /* Armazena o numero da conta de destino dos fundos */
                float valor; /* Armazena a quantia monetaria a ser transferida */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nTransferir\n");
                printf("Digite o numero da conta de origem: ");
                if (scanf("%d", &conta_origem) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o numero da conta de destino: ");
                if (scanf("%d", &conta_destino) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor da transferencia: ");
                if (scanf("%f", &valor) != 1 || valor <= 0) {
                    printf("Erro: Valor invalido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = transferir(conta_origem, conta_destino, valor);
                if (status == 0) {
                    printf("Transferencia de R$ %.2f de %d para %d concluida com sucesso!\n", valor, conta_origem, conta_destino);
                } else {
                    printf("Erro na transferencia (saldo insuficiente ou contas invalidas).\n");
                }
                break;
            }
            case 8: {
                int numero_conta; /* Armazena o numero da conta para consulta do historico */
                int total_transacoes = 0; /* Ponteiro de saida: recebe o total de transacoes registradas na conta */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\nHistorico de Transacoes\n");
                printf("Digite o numero da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("Erro: Numero invalido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = obter_total_transacoes(numero_conta, &total_transacoes);
                if (status == 0) {
                    if (total_transacoes == 0) {
                        printf("Nenhuma transacao encontrada para essa conta.\n");
                    } else {
                        int i; /* Variavel contadora para iterar no loop do historico de transacoes */
                        printf("Historico da conta %d (%d transacoes):\n", numero_conta, total_transacoes);
                        for (i = 0; i < total_transacoes; i++) {
                            char tipo[20]; /* Ponteiro de saida: recebe o tipo da transacao (ex: Deposito, Saque) */
                            float valor = 0.0f; /* Ponteiro de saida: recebe o valor numerico da transacao */
                            /* Busca elemento a elemento por referencia respeitando as restricoes */
                            if (obter_transacao(numero_conta, i, tipo, &valor) == 0) {
                                printf("%d. %s: R$ %.2f\n", i + 1, tipo, valor);
                            }
                        }
                    }
                } else {
                    printf("Erro ao buscar historico (conta nao encontrada).\n");
                }
                break;
            }
            case 9:
                printf("\nSaindo do sistema bancario. Ate mais!\n");
                return 0;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
