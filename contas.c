#include <stdio.h>
#include <string.h>
#include "clientes.h"
#include "contas.h"

static Conta contas[MAX_CONTAS];
static int total_contas = 0;
static int proximo_id   = 1;

int abrir_conta(int id_cliente, const char *tipo, int *id_conta_gerada) {
    Cliente c;

    //checando possíveis erros
    if (tipo == NULL || id_conta_gerada == NULL) return -1;
    if (id_cliente <= 0 ) return -1;
    if (total_contas >= MAX_CONTAS) return -1;

    //Verificando se o cliente existe
    if (buscar_cliente(id_cliente, &c) != 0) return -1;

    //evita que nomes invalidos sejam aceitos
    if (strcmp(tipo, "corrente") != 0 && strcmp(tipo, "poupanca") != 0) return -1;

    //inicializa a nova conta
    contas[total_contas].id_conta = proximo_id;
    contas[total_contas].id_cliente = id_cliente;
    strncpy(contas[total_contas].tipo, tipo, sizeof(contas[total_contas].tipo) - 1);
    contas[total_contas].tipo[sizeof(contas[total_contas].tipo) - 1] = '\0';
    contas[total_contas].saldo = 0.0;

    //atualiza os valores indicadores do número de contas
    *id_conta_gerada = proximo_id;
    proximo_id++;
    total_contas++;
    return 0;
}

int buscar_conta(int id_conta, Conta *conta_retornada){
    //checando possíveis erros
    if (conta_retornada == NULL) return -1;
    if (id_conta <= 0) return -1;


    //procura a conta
    for (int i = 0; i < total_contas; i++){
        if (contas[i].id_conta == id_conta) {
            *conta_retornada = contas[i]; //acha a conta e retorna o seu endereça
            return 0;
        }
    }
    return -1; //a conta não foi encontrada
}

int consultar_saldo(int id_conta, double *saldo_retornado) {
    //checando possíveis erros
    if (saldo_retornado == NULL) return -1;
    if (id_conta <= 0) return -1;


    //procura a conta
    for (int i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            *saldo_retornado = contas[i].saldo; //acha a conta e retorna o saldo
            return 0;
        }
    }
    return -1;
}

int atualiza_saldo(int id_conta, double valor) {
    //checando possíveis erros
    if (id_conta <= 0) return -1;

    //procura conta
    for (int i = 0; i < total_contas; i++) {
        if (contas[i].id_conta == id_conta) {
            //verifica se o valor da conta esta negativo
            if (contas[i].saldo + valor < 0.0) return -1;

            contas[i].saldo += valor; //adiciona o valor ao saldo
            return 0;
        }
    }
    return -1;
}

void listar_contas(int id_cliente) {
    int encontrou = 0;

    //procura conta do cliente
    for (int i = 0; i < total_contas; i++) {
        if (contas[i].id_cliente == id_cliente) {
            if (!encontrou) {
                printf("=== Contas do cliente %d ===\n", id_cliente);
                encontrou = 1;
            }
            printf("ID: %d | Tipo: %s | Saldo: R$ %.2f\n",
                   contas[i].id_conta, contas[i].tipo, contas[i].saldo); //escreve o id, tipo e saldo da conta
        }
    }

    //em caso de nenhuma conta ter sido encontrada com o id daquele cliente um aviso é enviado
    if (!encontrou) {
        printf("Nenhuma conta encontrada para o cliente %d.\n", id_cliente);
    }
}

int salvar_contas_arquivo(void) {
    FILE *f;

    f = fopen("contas.txt", "w");
    if (f == NULL) return -1;

    //para cada conta existente escreve as informações dela dentro do arquivo
    for (int i = 0; i < total_contas; i++) {
        fprintf(f, "%d;%d;%s;%.2f\n",
                contas[i].id_conta,
                contas[i].id_cliente,
                contas[i].tipo,
                contas[i].saldo);
    }

    fclose(f);
    return 0;
}

int carregar_contas_arquivo(void) {
    FILE *f;
    char linha[256];
    int max_id = 0;
    total_contas = 0;
    proximo_id = 1;

    //abre o arquivo 
    f = fopen("contas.txt", "r");
    if (f == NULL) return -1;

    //escreve as contas enquanto o número máximo não for atingido E se ainda tiverem contas dentro do arquivo
    while (total_contas < MAX_CONTAS && fgets(linha, sizeof(linha), f) != NULL) {
        Conta c;
        if (sscanf(linha, "%d;%d;%19[^;];%lf", &c.id_conta, &c.id_cliente, c.tipo, &c.saldo) == 4) {
            contas[total_contas] = c;
            if (c.id_conta > max_id) max_id = c.id_conta;
            total_contas++;
        }
    }

    fclose(f);
    proximo_id = max_id + 1;
    return 0;
}
