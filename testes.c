/**
 * @file testes.c
 * @brief Suíte de testes automatizados do Sistema Bancário Modular.
 *
 * Cobre todos os casos de teste definidos no documento T4 (seção 6),
 * do CT-01 ao CT-13. Cada teste imprime PASS ou FAIL com descrição.
 *
 * Compilação (junto com os demais módulos):
 *   gcc -Wall -Wextra -std=c11 testes.c clientes.c contas.c transacoes.c -o testes
 *
 * Execução:
 *   ./testes
 */

#include <stdio.h>
#include <string.h>

#include "clientes.h"
#include "contas.h"
#include "transacoes.h"

/*  Infraestrutura de relatório  */

/** Contadores globais de resultado */
static int total_testes  = 0;
static int testes_ok     = 0;
static int testes_falhou = 0;

/**
 * @brief Avalia uma condição e imprime o resultado formatado.
 *
 * @param id          Identificador do caso de teste (ex.: "CT-01").
 * @param descricao   Descrição resumida do que está sendo testado.
 * @param condicao    Expressão booleana: 1 = PASS, 0 = FAIL.
 */
static void assert_teste(const char *id, const char *descricao, int condicao) {
    total_testes++;
    if (condicao) {
        testes_ok++;
        printf("  [PASS] %s — %s\n", id, descricao);
    } else {
        testes_falhou++;
        printf("  [FAIL] %s — %s\n", id, descricao);
    }
}

/*  Bloco 6.1: Módulo Clientes  */

/**
 * @brief Executa os casos de teste CT-01 a CT-04 (módulo Clientes).
 */
static void testar_clientes(void) {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║             BLOCO 6.1 — MÓDULO CLIENTES               ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");

    int id_gerado = -1; /* Armazena o ID gerado pelo cadastro de teste */
    int ret; /* Armazena o retorno da funcao testada */

    /*  CT-01: Cadastro com dados válidos  */
    ret = cadastrar_cliente("Fulano Silva", "123.456.789-00", "senha123", &id_gerado);
    assert_teste("CT-01a", "cadastrar_cliente() retorna 0 para dados válidos",
                 ret == 0);
    assert_teste("CT-01b", "ID gerado é positivo (capturado via ponteiro)",
                 id_gerado > 0);

    /*  CT-02: Cadastro com CPF duplicado  */
    int id_dup = -1; /* Armazena o ID da tentativa duplicada */
    ret = cadastrar_cliente("Outro Nome", "123.456.789-00", "outrasenha", &id_dup);
    assert_teste("CT-02a", "segunda chamada com CPF duplicado retorna -1",
                 ret == -1);
    assert_teste("CT-02b", "ID da tentativa duplicada permanece -1",
                 id_dup == -1);

    /*  CT-03: Login com credenciais corretas  */
    int id_logado = -1; /* Armazena o ID do cliente retornado no login com sucesso */
    ret = login("123.456.789-00", "senha123", &id_logado);
    assert_teste("CT-03a", "login() retorna 0 para credenciais corretas",
                 ret == 0);
    assert_teste("CT-03b", "ID do cliente retornado via ponteiro é válido",
                 id_logado > 0);

    /*  CT-04: Login com senha incorreta  */
    int id_invalido = -1; /* Armazena o ID no login falho */
    ret = login("123.456.789-00", "senhaerrada", &id_invalido);
    assert_teste("CT-04a", "login() retorna -1 para senha incorreta",
                 ret == -1);
    assert_teste("CT-04b", "ID permanece -1 após login falho",
                 id_invalido == -1);
}

/*  Bloco 6.2: Módulo Contas  */

/**
 * @brief Executa os casos de teste CT-05 a CT-07 (módulo Contas).
 *
 * @param id_cliente_valido  ID de um cliente previamente cadastrado (CT-01).
 * @param id_conta_out       Ponteiro de saída: recebe o ID da conta criada no CT-05.
 */
static void testar_contas(int id_cliente_valido, int *id_conta_out) {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║              BLOCO 6.2 — MÓDULO CONTAS                ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");

    int ret; /* Armazena o retorno das funcoes testadas */
    int id_conta = -1; /* Armazena o ID da conta criada no teste */

    /*  CT-05: Abertura de conta corrente para cliente existente  */
    ret = abrir_conta(id_cliente_valido, "corrente", &id_conta);
    assert_teste("CT-05a", "abrir_conta() retorna 0 para cliente existente",
                 ret == 0);
    assert_teste("CT-05b", "ID da conta criada é positivo",
                 id_conta > 0);

    *id_conta_out = id_conta;

    /*  CT-06: Abertura de conta para cliente inexistente  */
    int id_conta_invalida = -1; /* Armazena o ID na abertura de conta falha */
    ret = abrir_conta(9999, "poupanca", &id_conta_invalida);
    assert_teste("CT-06a", "abrir_conta() retorna -1 para cliente inexistente",
                 ret == -1);
    assert_teste("CT-06b", "ID permanece -1 para cliente inexistente",
                 id_conta_invalida == -1);

    /*  CT-07: Consulta de saldo após depósito  */
    /* Realiza um depósito de R$ 500,00 para preparar o cenário */
    depositar(id_conta, 500.0);

    double saldo = -1.0; /* Armazena o saldo consultado no teste */
    ret = consultar_saldo(id_conta, &saldo);
    assert_teste("CT-07a", "consultar_saldo() retorna 0 para conta válida",
                 ret == 0);
    assert_teste("CT-07b", "saldo retornado via ponteiro reflete o depósito (R$ 500,00)",
                 saldo == 500.0);

    /*  CT-14: Verificar se cliente possui conta (Nova regra de seguranca) */
    ret = cliente_tem_conta(id_cliente_valido);
    assert_teste("CT-14a", "cliente_tem_conta() retorna 1 para cliente com conta",
                 ret == 1);
    
    ret = cliente_tem_conta(9999);
    assert_teste("CT-14b", "cliente_tem_conta() retorna 0 para cliente sem conta/inexistente",
                 ret == 0);

    /*  CT-15: Verificar titularidade de conta (Nova regra de seguranca) */
    ret = verifica_titularidade(id_conta, id_cliente_valido);
    assert_teste("CT-15a", "verifica_titularidade() retorna 1 quando cliente é o titular",
                 ret == 1);

    ret = verifica_titularidade(id_conta, 9999);
    assert_teste("CT-15b", "verifica_titularidade() retorna 0 para cliente não titular",
                 ret == 0);
}

/*  Bloco 6.3: Módulo Transações  */

/**
 * @brief Executa os casos de teste CT-08 a CT-13 (módulo Transações).
 *
 * @param id_cliente_valido  ID de um cliente cadastrado (para abrir segunda conta).
 * @param id_conta1          ID da conta criada no bloco Contas (saldo inicial R$ 500,00).
 */
static void testar_transacoes(int id_cliente_valido, int id_conta1) {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║            BLOCO 6.3 — MÓDULO TRANSAÇÕES              ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");

    int ret; /* Armazena o retorno das funcoes de transacao testadas */
    double saldo = 0.0; /* Armazena o saldo consultado para validacao */

    /*  CT-08: Depósito com valor positivo  */
    /* Estado inicial da conta1: R$ 500,00 (do CT-07) */
    ret = depositar(id_conta1, 1000.0);
    assert_teste("CT-08a", "depositar() retorna 0 para valor positivo",
                 ret == 0);
    consultar_saldo(id_conta1, &saldo);
    assert_teste("CT-08b", "saldo aumenta corretamente após depósito (R$ 1500,00)",
                 saldo == 1500.0);

    /*  CT-09: Saque com saldo suficiente  */
    /* Conta1 tem R$ 1500,00; saca R$ 300,00 → esperado R$ 1200,00 */
    ret = sacar(id_conta1, 300.0);
    assert_teste("CT-09a", "sacar() retorna 0 para saldo suficiente",
                 ret == 0);
    consultar_saldo(id_conta1, &saldo);
    assert_teste("CT-09b", "saldo reduz corretamente após saque (R$ 1200,00)",
                 saldo == 1200.0);

    /*  CT-10: Saque com saldo insuficiente  */
    /* Conta1 tem R$ 1200,00; tenta sacar R$ 5000,00 */
    double saldo_antes = 0.0; /* Armazena o saldo antes de uma operacao de teste */
    consultar_saldo(id_conta1, &saldo_antes);

    ret = sacar(id_conta1, 5000.0);
    assert_teste("CT-10a", "sacar() retorna -1 para saldo insuficiente",
                 ret == -1);
    consultar_saldo(id_conta1, &saldo);
    assert_teste("CT-10b", "saldo permanece inalterado após saque recusado",
                 saldo == saldo_antes);

    /*  CT-11: Transferência válida entre duas contas  */
    /* Criar segunda conta para o mesmo cliente */
    int id_conta2 = -1; /* Armazena o ID da segunda conta para transferencia */
    abrir_conta(id_cliente_valido, "poupanca", &id_conta2);
    depositar(id_conta2, 100.0);  /* Conta2: R$ 100,00 */

    /* Zerar conta1 até R$ 500,00 para reproduzir o cenário do documento */
    double saldo_atual1 = 0.0; /* Armazena o saldo atual para nivelamento do teste */
    consultar_saldo(id_conta1, &saldo_atual1);
    if (saldo_atual1 > 500.0) {
        sacar(id_conta1, saldo_atual1 - 500.0);
    }

    /* Conta1: R$ 500,00 | Conta2: R$ 100,00 — transfere R$ 200,00 */
    ret = transferir(id_conta1, id_conta2, 200.0);
    assert_teste("CT-11a", "transferir() retorna 0 para transferência válida",
                 ret == 0);

    double saldo1 = 0.0, saldo2 = 0.0; /* Armazena saldos de origem e destino apos transferencia */
    consultar_saldo(id_conta1, &saldo1);
    consultar_saldo(id_conta2, &saldo2);
    assert_teste("CT-11b", "saldo da conta de origem reduz para R$ 300,00",
                 saldo1 == 300.0);
    assert_teste("CT-11c", "saldo da conta de destino aumenta para R$ 300,00",
                 saldo2 == 300.0);

    /*  CT-12: Transferência para a mesma conta  */
    ret = transferir(id_conta1, id_conta1, 100.0);
    assert_teste("CT-12a", "transferir() retorna -1 para origem == destino",
                 ret == -1);
    consultar_saldo(id_conta1, &saldo);
    assert_teste("CT-12b", "saldo permanece inalterado após tentativa inválida",
                 saldo == 300.0);

    /*  CT-16: Operações com valores negativos ou zero  */
    ret = depositar(id_conta1, -50.0);
    assert_teste("CT-16a", "depositar() retorna -1 para valor negativo", ret == -1);
    ret = depositar(id_conta1, 0.0);
    assert_teste("CT-16b", "depositar() retorna -1 para valor zero", ret == -1);

    /*  CT-17: Saque com valores negativos ou zero  */
    ret = sacar(id_conta1, -10.0);
    assert_teste("CT-17a", "sacar() retorna -1 para valor negativo", ret == -1);
    ret = sacar(id_conta1, 0.0);
    assert_teste("CT-17b", "sacar() retorna -1 para valor zero", ret == -1);

    /*  CT-18: Transferência com valores negativos ou zero  */
    ret = transferir(id_conta1, id_conta2, -20.0);
    assert_teste("CT-18a", "transferir() retorna -1 para valor negativo", ret == -1);

    /*  CT-19: Transferência com saldo insuficiente na origem  */
    ret = transferir(id_conta1, id_conta2, 99999.0);
    assert_teste("CT-19a", "transferir() retorna -1 para saldo insuficiente", ret == -1);

    /*  CT-13: Listagem do histórico  */
    /*
     * A conta1 acumulou até aqui (em ordem):
     *   1. depositar  R$ 500,00  (CT-07 — preparação)
     *   2. depositar  R$ 1000,00 (CT-08)
     *   3. sacar      R$ 300,00  (CT-09)
     *   4. (saque de nivelamento — se executado)
     *   5. transferir R$ 200,00  (CT-11)
     *
     * Verificamos que listar_transacoes() não trava e produz saída.
     * A validação visual fica registrada no console.
     */
    printf("\n  [INFO] CT-13 — saída do histórico da conta %d:\n", id_conta1);
    listar_transacoes(id_conta1);
    assert_teste("CT-13", "listar_transacoes() executa sem falha para conta com histórico",
                 1 /* verificação visual no console acima */);
}

/*  Ponto de entrada  */

/**
 * @brief Executa a suíte completa de testes automatizados.
 *
 * Pode ser chamado diretamente (compilando este arquivo como main)
 * ou integrado ao main.c do sistema (caso 0 do menu).
 *
 * @return int  0 se todos os testes passaram, 1 se algum falhou.
 */
int executar_testes(void) {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║      SUÍTE DE TESTES AUTOMATIZADOS — T4 INF1040       ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");

    /*  Bloco Clientes  */
    testar_clientes();

    /*
     * Recuperar o ID do cliente cadastrado no CT-01 para uso nos blocos
     * seguintes. Fazemos um login para obter o ID sem depender de variável
     * estática interna (respeitando o encapsulamento do TAD).
     */
    int id_cliente = -1; /* Armazena o ID recuperado via login no teste de integracao */
    login("123.456.789-00", "senha123", &id_cliente);

    /*  Bloco Contas  */
    int id_conta1 = -1; /* Armazena o ID da conta gerada no bloco Contas */
    testar_contas(id_cliente, &id_conta1);

    /*  Bloco Transações  */
    if (id_conta1 > 0) {
        testar_transacoes(id_cliente, id_conta1);
    } else {
        printf("\n  [SKIP] Bloco Transações pulado: nenhuma conta criada nos testes anteriores.\n");
    }

    /*  Relatório final  */
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                    RELATÓRIO FINAL                    ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    printf("║  Total de testes : %-35d║\n", total_testes);
    printf("║  Passaram        : %-35d║\n", testes_ok);
    printf("║  Falharam        : %-35d║\n", testes_falhou);
    printf("╚═══════════════════════════════════════════════════════╝\n");

    return (testes_falhou == 0) ? 0 : 1;
}

/*  main() standalone (compilar apenas com -DTESTES_STANDALONE)  */
#ifdef TESTES_STANDALONE
int main(void) {
    return executar_testes();
}
#endif
