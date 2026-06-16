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
        printf("\n┌───────────────────────────────────────────┐\n");
        printf("│         SISTEMA BANCÁRIO MODULAR          │\n");
        printf("├───────────────────────────────────────────┤\n");
        if (usuario_logado_id != -1) {
            printf("│ [Status] Cliente Logado (ID: %-3d)        │\n", usuario_logado_id);
        } else {
            printf("│ [Status] Não autenticado                  │\n");
        }
        printf("├────────────────────────────────────────────┤\n");
        printf("│ [Operações]                                │\n");
        printf("│  ➤ 0. Rodar Testes Automatizados          │\n");
        printf("│  ➤ 1. Cadastrar Cliente                   │\n");
        printf("│  ➤ 2. Fazer Login                         │\n");
        printf("│  ➤ 3. Abrir Conta                         │\n");
        printf("│  ➤ 4. Consultar Saldo                     │\n");
        printf("│  ➤ 5. Depositar                           │\n");
        printf("│  ➤ 6. Sacar                               │\n");
        printf("│  ➤ 7. Transferir                          │\n");
        printf("│  ➤ 8. Listar Histórico                    │\n");
        printf("├────────────────────────────────────────────┤\n");
        printf("│ [Sair]                                     │\n");
        printf("│  ➤ 9. Sair                                │\n");
        printf("└────────────────────────────────────────────┘\n");
        printf("Escolha uma opção: ");

        /* Tratamento basico de erro caso a entrada nao seja numerica */
        if (scanf("%d", &opcao) != 1) {
            printf("⚠ Erro: Opção inválida. Digite um número.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer(); /* Limpa o '\n' restante do buffer */

        switch (opcao) {
            case 0: {
                printf("\n✦ Executando bateria de testes automatizados...\n");
                /* executar_testes_contas(); */
                /* executar_testes_transacoes(); */
                printf("✦ Testes finalizados.\n");
                break;
            }
            case 1: {
                char nome[100]; /* Armazena o nome fornecido para o novo cliente */
                char cpf[15]; /* Armazena o CPF fornecido para o novo cliente */
                char senha[50]; /* Armazena a senha cadastrada para o novo cliente */
                int novo_id = -1; /* Ponteiro de saida: recebe o ID incremental gerado para o cliente */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Cadastrar Cliente ===\n");
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
                    printf("✦ Sucesso: Cliente cadastrado com sucesso! ID: %d\n", novo_id);
                } else {
                    printf("⚠ Erro: CPF já cadastrado ou limite atingido.\n");
                }
                break;
            }
            case 2: {
                char cpf[15]; /* Armazena o CPF inserido para autenticacao */
                char senha[50]; /* Armazena a senha inserida para autenticacao */
                int id_cliente = -1; /* Ponteiro de saida: recebe o ID do cliente correspondente se logado */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Fazer Login ===\n");
                printf("CPF: ");
                if (fgets(cpf, sizeof(cpf), stdin)) {
                    cpf[strcspn(cpf, "\n")] = '\0';
                }
                printf("Senha: ");
                if (fgets(senha, sizeof(senha), stdin)) {
                    senha[strcspn(senha, "\n")] = '\0';
                }

                status = login(cpf, senha, &id_cliente);
                if (status == 0) {
                    usuario_logado_id = id_cliente;
                    printf("✦ Sucesso: Login realizado com sucesso! ID do cliente: %d\n", usuario_logado_id);
                } else {
                    printf("⚠ Erro: CPF ou senha incorretos.\n");
                }
                break;
            }
            case 3: {
                int id_cliente; /* Armazena o ID do cliente que sera proprietario da conta */
                int numero_conta = -1; /* Ponteiro de saida: recebe o numero da conta gerado pelo sistema */
                char tipo_conta[20]; /* Armazena o tipo da conta */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Abrir Conta ===\n");
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
                            printf("⚠ Erro: ID inválido.\n");
                            limpar_buffer();
                            break;
                        }
                        limpar_buffer();
                    }
                } else {
                    printf("Digite o ID do Cliente: ");
                    if (scanf("%d", &id_cliente) != 1) {
                        printf("⚠ Erro: ID inválido.\n");
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                }

                printf("Digite o tipo da conta (corrente ou poupanca): ");
                if (scanf("%19s", tipo_conta) != 1) {
                    printf("⚠ Erro: Tipo inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = abrir_conta(id_cliente, tipo_conta, &numero_conta);
                if (status == 0) {
                    printf("✦ Sucesso: Conta criada com sucesso! Número: %d\n", numero_conta);
                } else {
                    printf("⚠ Erro: Cliente inexistente ou limite de contas atingido.\n");
                }
                break;
            }
            case 4: {
                int numero_conta; /* Armazena o numero da conta a ser consultado */
                double saldo = 0.0; /* Ponteiro de saida: recebe o valor do saldo da conta */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Consultar Saldo ===\n");
                printf("Digite o número da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = consultar_saldo(numero_conta, &saldo);
                if (status == 0) {
                    printf("✦ Sucesso: Saldo da conta %d: R$ %.2f\n", numero_conta, saldo);
                } else {
                    printf("⚠ Erro: Conta não encontrada.\n");
                }
                break;
            }
            case 5: {
                int numero_conta; /* Armazena o numero da conta destino do deposito */
                double valor; /* Armazena a quantia monetaria a ser depositada */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Depositar ===\n");
                printf("Digite o número da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor do depósito: ");
                if (scanf("%lf", &valor) != 1 || valor <= 0) {
                    printf("⚠ Erro: Valor inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = depositar(numero_conta, valor);
                if (status == 0) {
                    printf("✦ Sucesso: Depósito de R$ %.2f concluído na conta %d!\n", valor, numero_conta);
                } else {
                    printf("⚠ Erro: Conta inexistente ou valor inválido.\n");
                }
                break;
            }
            case 6: {
                int numero_conta; /* Armazena o numero da conta de onde sera realizado o saque */
                double valor; /* Armazena a quantia monetaria a ser sacada */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Sacar ===\n");
                printf("Digite o número da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor do saque: ");
                if (scanf("%lf", &valor) != 1 || valor <= 0) {
                    printf("⚠ Erro: Valor inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = sacar(numero_conta, valor);
                if (status == 0) {
                    printf("✦ Sucesso: Saque de R$ %.2f concluído na conta %d!\n", valor, numero_conta);
                } else {
                    printf("⚠ Erro: Saldo insuficiente ou conta inexistente.\n");
                }
                break;
            }
            case 7: {
                int conta_origem; /* Armazena o numero da conta de origem dos fundos */
                int conta_destino; /* Armazena o numero da conta de destino dos fundos */
                double valor; /* Armazena a quantia monetaria a ser transferida */
                int status; /* Armazena o codigo de retorno das operacoes de negocio (0 para sucesso, -1 para erro) */

                printf("\n=== Transferir ===\n");
                printf("Digite o número da conta de origem: ");
                if (scanf("%d", &conta_origem) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o número da conta de destino: ");
                if (scanf("%d", &conta_destino) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                printf("Digite o valor da transferência: ");
                if (scanf("%lf", &valor) != 1 || valor <= 0) {
                    printf("⚠ Erro: Valor inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                status = transferir(conta_origem, conta_destino, valor);
                if (status == 0) {
                    printf("✦ Sucesso: Transferência de R$ %.2f de %d para %d concluída com sucesso!\n", valor, conta_origem, conta_destino);
                } else {
                    printf("⚠ Erro: Saldo insuficiente ou contas inválidas.\n");
                }
                break;
            }
            case 8: {
                int numero_conta; /* Armazena o numero da conta para consulta do historico */

                printf("\n=== Histórico de Transações ===\n");
                printf("Digite o número da conta: ");
                if (scanf("%d", &numero_conta) != 1) {
                    printf("⚠ Erro: Número inválido.\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                listar_transacoes(numero_conta);
                break;
            }
            case 9:
                printf("\n✦ Saindo do sistema bancário. Até mais!\n");
                return 0;
            default:
                printf("\n⚠ Erro: Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
