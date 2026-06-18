#include <stdio.h>
#include <string.h>
#include "clientes.h"

/* Armazenamento encapsulado (TAD) — invisível a outros módulos */
static Cliente clientes[MAX_CLIENTES];
static int total_clientes = 0;
static int proximo_id     = 1;

/**
 * @brief Normaliza o CPF para o formato padrão.
 *
 * Aceita com ou sem máscara, exige exatamente 11 dígitos e
 * apenas caracteres válidos (dígitos, '.' e '-'). Escreve "000.000.000-00" na saída.
 *
 * @param entrada String contendo o CPF de origem (entrada).
 * @param saida   Buffer de destino para o CPF normalizado (saída).
 * @return int    0 se válido, -1 caso contrário.
 */
static int normalizar_cpf(const char *entrada, char *saida) {
    char digitos[12]; /* Buffer para os 11 digitos numericos do CPF */
    int nd = 0, i; /* Contadores para numero de digitos e loop de leitura */
    if (entrada == NULL) return -1;
    for (i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];
        if (c >= '0' && c <= '9') {
            if (nd >= 11) return -1;
            digitos[nd++] = c;
        } else if (c != '.' && c != '-') {
            return -1;
        }
    }
    if (nd != 11) return -1;
    sprintf(saida, "%c%c%c.%c%c%c.%c%c%c-%c%c",
            digitos[0], digitos[1], digitos[2],
            digitos[3], digitos[4], digitos[5],
            digitos[6], digitos[7], digitos[8],
            digitos[9], digitos[10]);
    return 0;
}

int cadastrar_cliente(const char *nome, const char *cpf,
                      const char *senha, int *id_cliente_gerado) {
    char cpf_norm[15]; /* Armazena o CPF formatado e normalizado */
    int i; /* Variavel iteradora para percorrer o array de clientes */
    if (nome == NULL || cpf == NULL || senha == NULL || id_cliente_gerado == NULL)
        return -1;
    if (total_clientes >= MAX_CLIENTES) return -1;
    if (strlen(nome) == 0) return -1;
    if (strlen(senha) < 6) return -1;
    if (normalizar_cpf(cpf, cpf_norm) != 0) return -1;
    for (i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_norm) == 0) return -1;
    }
    clientes[total_clientes].id_cliente = proximo_id;
    strncpy(clientes[total_clientes].nome, nome,
            sizeof(clientes[total_clientes].nome) - 1);
    clientes[total_clientes].nome[sizeof(clientes[total_clientes].nome) - 1] = '\0';
    strcpy(clientes[total_clientes].cpf, cpf_norm);
    strncpy(clientes[total_clientes].senha, senha,
            sizeof(clientes[total_clientes].senha) - 1);
    clientes[total_clientes].senha[sizeof(clientes[total_clientes].senha) - 1] = '\0';
    *id_cliente_gerado = proximo_id;
    proximo_id++;
    total_clientes++;
    return 0;
}

int login(const char *cpf, const char *senha, int *id_cliente_retornado) {
    char cpf_norm[15]; /* Armazena o CPF formatado e normalizado */
    int i; /* Variavel iteradora para percorrer o array de clientes */
    if (cpf == NULL || senha == NULL || id_cliente_retornado == NULL) return -1;
    if (normalizar_cpf(cpf, cpf_norm) != 0) return -1;
    for (i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_norm) == 0 &&
            strcmp(clientes[i].senha, senha) == 0) {
            *id_cliente_retornado = clientes[i].id_cliente;
            return 0;
        }
    }
    return -1;
}

int buscar_cliente(int id_cliente, Cliente *cliente_retornado) {
    int i; /* Variavel iteradora para busca no array de clientes */
    if (cliente_retornado == NULL) return -1;
    if (id_cliente <= 0) return -1;
    for (i = 0; i < total_clientes; i++) {
        if (clientes[i].id_cliente == id_cliente) {
            *cliente_retornado = clientes[i];
            return 0;
        }
    }
    return -1;
}

void listar_clientes(void) {
    int i; /* Variavel iteradora para imprimir a lista de clientes */
    if (total_clientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                 CLIENTES CADASTRADOS                  ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    for (i = 0; i < total_clientes; i++) {
        printf("║ ID: %-3d | Nome: %-25s | CPF: %-14s ║\n",
               clientes[i].id_cliente, clientes[i].nome, clientes[i].cpf);
    }
    printf("╚═══════════════════════════════════════════════════════╝\n");
}

int salvar_clientes_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    int i; /* Variavel iteradora para gravacao dos registros */
    f = fopen("clientes.txt", "w");
    if (f == NULL) return -1;
    for (i = 0; i < total_clientes; i++) {
        fprintf(f, "%d;%s;%s;%s\n",
                clientes[i].id_cliente, clientes[i].nome,
                clientes[i].cpf, clientes[i].senha);
    }
    fclose(f);
    return 0;
}

int carregar_clientes_arquivo(void) {
    FILE *f; /* Ponteiro para o arquivo de persistencia */
    char linha[256]; /* Buffer para armazenar cada linha lida do arquivo */
    int max_id = 0; /* Rastreia o maior ID lido para configurar o proximo gerador */
    total_clientes = 0;
    proximo_id = 1;
    f = fopen("clientes.txt", "r");
    if (f == NULL) return -1;   /* 1a execucao: comeca vazio */
    while (total_clientes < MAX_CLIENTES &&
           fgets(linha, sizeof(linha), f) != NULL) {
        Cliente c; /* Variavel temporaria para os dados parseados na linha atual */
        if (sscanf(linha, "%d;%99[^;];%14[^;];%49[^\n]",
                   &c.id_cliente, c.nome, c.cpf, c.senha) == 4) {
            clientes[total_clientes] = c;
            if (c.id_cliente > max_id) max_id = c.id_cliente;
            total_clientes++;
        }
    }
    fclose(f);
    proximo_id = max_id + 1;
    return 0;
}
