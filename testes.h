#ifndef TESTES_H
#define TESTES_H

/**
 * @brief Executa a suíte completa de testes automatizados do sistema.
 *
 * Cobre os casos CT-01 a CT-13 definidos no documento T4 (seção 6),
 * incluindo clientes, contas e transações. Imprime o resultado de cada
 * caso (PASS/FAIL) e um relatório final consolidado.
 *
 * Deve ser chamada com as estruturas internas zeradas (sem dados reais),
 * pois cria registros próprios de teste que podem conflitar com dados
 * de produção. Recomenda-se não chamar esta função após o carregamento
 * dos arquivos de persistência.
 *
 * @return int  Retorna 0 se todos os testes passaram, ou 1 se algum falhou.
 */
int executar_testes(void);

#endif /* TESTES_H */