```c
#ifndef PRODUTO_H
#define PRODUTO_H

#define MAX_PRODUTOS 200
#define TAM_NOME 50
#define ARQUIVO_ESTOQUE "estoque.dat"

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    int quantidade;
    float precoUnitario;
} Produto;

/* Cadastro e consulta */
void cadastrarProduto(Produto lista[], int *total);
void listarProdutos(const Produto lista[], int total);
Produto *buscarPorCodigo(Produto lista[], int total, int codigo);

/* Movimentacao de estoque */
void registrarEntrada(Produto lista[], int total);
void registrarSaida(Produto lista[], int total);

/* Remocao */
int removerProduto(Produto lista[], int *total, int codigo);

/* Persistencia */
void salvarDados(const Produto lista[], int total);
int carregarDados(Produto lista[]);

/* Utilidades */
void limparBufferEntrada(void);
int lerInteiro(const char *mensagem, int *valor);
int lerFloat(const char *mensagem, float *valor);

#endif
```
