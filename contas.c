#include <stdio.h>
#include <string.h>
#include "contas.h"
#include "clientes.h"   /* para validar o titular via buscar_cliente() */

/* Armazenamento encapsulado (TAD) — invisível a outros módulos */
static Conta contas[MAX_CONTAS];
static int total_contas = 0;
static int proximo_id_conta = 1;

int abrir_conta(int id_cliente, const char *tipo, int *id_conta_gerada) {
    Cliente titular; /* Armazena os dados do cliente titular buscado */
    if (tipo == NULL || id_conta_gerada == NULL) return -1;
    if (total_contas >= MAX_CONTAS) return -1;
    /* Integridade referencial: o cliente titular deve existir */
    if (buscar_cliente(id_cliente, &titular) != 0) return -1;
    /* Apenas tipos validos sao aceitos */
    if (strcmp(tipo, "corrente") != 0 && strcmp(tipo, "poupanca") != 0) return -1;
    contas[total_contas].id_conta = proximo_id_conta;
    contas[total_contas].id_cliente = id_cliente;
    strncpy(contas[total_contas].tipo, tipo, sizeof(contas[total_contas].tipo) - 1);
    contas[total_contas].tipo[sizeof(contas[total_contas].tipo) - 1] = '\0';
    contas[total_contas].saldo = 0.0;
    *id_conta_gerada = proximo_id_conta;
    proximo_id_conta++;
    total_contas++;
    return 0;
}

int consultar_saldo(int id_conta, double *saldo_retornado) {
    int i; /* Variavel iteradora para percorrer o array de contas */
    if (saldo_retornado == NULL) return -1;
    if (id_conta <= 0) return -1;
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            *saldo_retornado = contas[i].saldo;
            return 0;
        }
    }
    return -1;
}

int atualiza_saldo(int id_conta, double valor) {
    int i; /* Variavel iteradora para busca da conta */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            if (contas[i].saldo + valor < 0.0) return -1;  /* nunca permite saldo negativo */
            contas[i].saldo += valor;
            return 0;
        }
    }
    return -1;
}

void listar_contas(int id_cliente) {
    int i, encontrou = 0; /* Iterador de busca e flag para cabecalho */
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_cliente == id_cliente) {
            if (!encontrou) {
                printf("\n╔═══════════════════════════════════════════════════════╗\n");
                printf("║                 CONTAS DO CLIENTE %-3d                 ║\n", id_cliente);
                printf("╠═══════════════════════════════════════════════════════╣\n");
                encontrou = 1;
            }
            printf("║ Conta: %-3d | Tipo: %-10s | Saldo: R$ %-9.2f ║\n",
                   contas[i].id_conta, contas[i].tipo, contas[i].saldo);
        }
    }
    if (encontrou) {
        printf("╚═══════════════════════════════════════════════════════╝\n");
    } else {
        printf("Nenhuma conta encontrada para o cliente %d.\n", id_cliente);
    }
}

int buscar_conta(int id_conta, Conta *conta_retornada) {
    int i; /* Variavel iteradora para busca no array de contas */
    if (conta_retornada == NULL) return -1;
    if (id_conta <= 0) return -1;
    for (i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            *conta_retornada = contas[i];
            return 0;
        }
    }
    return -1;
}

int salvar_contas_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    int i; /* Variavel iteradora para gravacao dos registros */
    f = fopen("contas.txt", "w");
    if (f == NULL) return -1;
    for (i = 0; i < total_contas; i++) {
        fprintf(f, "%d;%d;%s;%.2f\n",
                contas[i].id_conta, contas[i].id_cliente,
                contas[i].tipo, contas[i].saldo);
    }
    fclose(f);
    return 0;
}

int carregar_contas_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    char linha[256]; /* Buffer temporario para leitura de cada linha do arquivo */
    int max_id = 0; /* Rastreia o ID maximo carregado para o proximo gerador */
    total_contas = 0;
    proximo_id_conta = 1;
    f = fopen("contas.txt", "r");
    if (f == NULL) return -1;   /* 1a execucao: comeca vazio */
    while (total_contas < MAX_CONTAS &&
           fgets(linha, sizeof(linha), f) != NULL) {
        Conta c; /* Variavel temporaria para os dados parseados na linha atual */
        if (sscanf(linha, "%d;%d;%19[^;];%lf",
                   &c.id_conta, &c.id_cliente, c.tipo, &c.saldo) == 4) {
            contas[total_contas] = c;
            if (c.id_conta > max_id) max_id = c.id_conta;
            total_contas++;
        }
    }
    fclose(f);
    proximo_id_conta = max_id + 1;
    return 0;
}
