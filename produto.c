```c
#include <stdio.h>
#include <string.h>
#include "produto.h"


/*
 * Descarta tudo que ainda ficou no buffer de entrada.
 */
void limparBufferEntrada(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres restantes */
    }
}


/*
 * Le um inteiro de forma segura.
 *
 * Retorna:
 * 1 -> leitura realizada com sucesso
 * 0 -> entrada invalida
 */
int lerInteiro(const char *mensagem, int *valor) {
    char linha[100];
    char extra;

    printf("%s", mensagem);

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    /*
     * O "%d %c" permite verificar se existe alguma coisa alem
     * do numero digitado.
     */
    if (sscanf(linha, "%d %c", valor, &extra) != 1) {
        printf("Entrada invalida. Digite um numero inteiro.\n");
        return 0;
    }

    return 1;
}


/*
 * Le um numero decimal de forma segura.
 */
int lerFloat(const char *mensagem, float *valor) {
    char linha[100];
    char extra;

    printf("%s", mensagem);

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    if (sscanf(linha, "%f %c", valor, &extra) != 1) {
        printf("Entrada invalida. Digite um numero valido.\n");
        return 0;
    }

    return 1;
}


/*
 * Descobre o proximo codigo disponivel.
 *
 * Procura o maior codigo existente e soma 1.
 */
static int proximoCodigo(const Produto lista[], int total) {
    int maior = 0;

    for (int i = 0; i < total; i++) {
        if (lista[i].codigo > maior) {
            maior = lista[i].codigo;
        }
    }

    return maior + 1;
}


/*
 * Cadastro de produto.
 */
void cadastrarProduto(Produto lista[], int *total) {
    if (*total >= MAX_PRODUTOS) {
        printf(
            "Estoque cheio! Limite de %d produtos cadastrados.\n",
            MAX_PRODUTOS
        );
        return;
    }

    Produto *novo = &lista[*total];

    novo->codigo = proximoCodigo(lista, *total);

    printf("Nome do produto: ");

    if (fgets(novo->nome, TAM_NOME, stdin) == NULL) {
        printf("Erro ao ler o nome.\n");
        return;
    }

    /*
     * Se o usuario digitou mais caracteres que cabem no buffer,
     * descarta o restante da linha.
     */
    if (strchr(novo->nome, '\n') == NULL) {
        limparBufferEntrada();
    }

    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    if (novo->nome[0] == '\0') {
        printf("O nome do produto nao pode ficar vazio.\n");
        return;
    }

    if (!lerInteiro("Quantidade inicial: ", &novo->quantidade)) {
        return;
    }

    if (novo->quantidade < 0) {
        printf("A quantidade nao pode ser negativa.\n");
        return;
    }

    if (!lerFloat("Preco unitario: R$ ", &novo->precoUnitario)) {
        return;
    }

    if (novo->precoUnitario < 0.0f) {
        printf("O preco nao pode ser negativo.\n");
        return;
    }

    (*total)++;

    printf(
        "Produto '%s' cadastrado com o codigo %d.\n",
        novo->nome,
        novo->codigo
    );
}


/*
 * Lista todos os produtos.
 */
void listarProdutos(const Produto lista[], int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf(
        "\n%-6s %-25s %-10s %-10s\n",
        "Cod",
        "Nome",
        "Qtd",
        "Preco"
    );

    printf(
        "--------------------------------------------------\n"
    );

    for (int i = 0; i < total; i++) {
        printf(
            "%-6d %-25s %-10d R$%-8.2f\n",
            lista[i].codigo,
            lista[i].nome,
            lista[i].quantidade,
            lista[i].precoUnitario
        );
    }
}


/*
 * Procura um produto pelo codigo.
 *
 * Retorna o endereco do produto encontrado ou NULL.
 */
Produto *buscarPorCodigo(
    Produto lista[],
    int total,
    int codigo
) {
    for (int i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return &lista[i];
        }
    }

    return NULL;
}


/*
 * Registra entrada de estoque.
 */
void registrarEntrada(Produto lista[], int total) {
    int codigo;
    int quantidade;

    if (!lerInteiro("Codigo do produto: ", &codigo)) {
        return;
    }

    Produto *produto = buscarPorCodigo(lista, total, codigo);

    if (produto == NULL) {
        printf(
            "Produto com codigo %d nao encontrado.\n",
            codigo
        );
        return;
    }

    if (!lerInteiro(
            "Quantidade a adicionar: ",
            &quantidade
        )) {
        return;
    }

    if (quantidade <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    produto->quantidade += quantidade;

    printf(
        "Nova quantidade de '%s': %d\n",
        produto->nome,
        produto->quantidade
    );
}


/*
 * Registra saida de estoque.
 */
void registrarSaida(Produto lista[], int total) {
    int codigo;
    int quantidade;

    if (!lerInteiro("Codigo do produto: ", &codigo)) {
        return;
    }

    Produto *produto = buscarPorCodigo(lista, total, codigo);

    if (produto == NULL) {
        printf(
            "Produto com codigo %d nao encontrado.\n",
            codigo
        );
        return;
    }

    if (!lerInteiro(
            "Quantidade a retirar: ",
            &quantidade
        )) {
        return;
    }

    if (quantidade <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    if (quantidade > produto->quantidade) {
        printf(
            "Estoque insuficiente! Ha somente %d "
            "unidade(s) de '%s'.\n",
            produto->quantidade,
            produto->nome
        );
        return;
    }

    produto->quantidade -= quantidade;

    printf(
        "Nova quantidade de '%s': %d\n",
        produto->nome,
        produto->quantidade
    );
}


/*
 * Remove um produto e desloca os elementos seguintes
 * uma posicao para tras.
 */
int removerProduto(
    Produto lista[],
    int *total,
    int codigo
) {
    for (int i = 0; i < *total; i++) {
        if (lista[i].codigo == codigo) {

            for (int j = i; j < *total - 1; j++) {
                lista[j] = lista[j + 1];
            }

            (*total)--;

            return 1;
        }
    }

    return 0;
}


/*
 * Salva os dados em arquivo binario.
 */
void salvarDados(const Produto lista[], int total) {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar os dados.\n");
        return;
    }

    /*
     * Primeiro salva a quantidade de produtos.
     */
    if (fwrite(&total, sizeof(int), 1, arquivo) != 1) {
        printf("Erro ao salvar a quantidade de produtos.\n");
        fclose(arquivo);
        return;
    }

    /*
     * Depois salva todas as structs.
     */
    if (total > 0) {
        size_t gravados = fwrite(
            lista,
            sizeof(Produto),
            (size_t)total,
            arquivo
        );

        if (gravados != (size_t)total) {
            printf("Erro ao salvar os produtos.\n");
            fclose(arquivo);
            return;
        }
    }

    fclose(arquivo);

    printf("Dados salvos com sucesso.\n");
}


/*
 * Carrega os dados do arquivo binario.
 *
 * Retorna:
 * - quantidade de produtos carregados
 * - 0 se nao existir arquivo ou houver erro
 */
int carregarDados(Produto lista[]) {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "rb");

    if (arquivo == NULL) {
        return 0;
    }

    int total;

    /*
     * Tenta ler a quantidade de produtos.
     */
    if (fread(&total, sizeof(int), 1, arquivo) != 1) {
        printf("Arquivo de estoque invalido ou vazio.\n");
        fclose(arquivo);
        return 0;
    }

    /*
     * Protecao contra arquivo corrompido.
     */
    if (total < 0 || total > MAX_PRODUTOS) {
        printf(
            "Arquivo de estoque invalido: quantidade de produtos "
            "fora do limite permitido.\n"
        );
        fclose(arquivo);
        return 0;
    }

    /*
     * Se nao existem produtos, nao precisa fazer outro fread.
     */
    if (total == 0) {
        fclose(arquivo);
        return 0;
    }

    /*
     * Carrega as structs.
     */
    size_t lidos = fread(
        lista,
        sizeof(Produto),
        (size_t)total,
        arquivo
    );

    if (lidos != (size_t)total) {
        printf(
            "Erro ao carregar os produtos. "
            "Arquivo pode estar incompleto.\n"
        );
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    return total;
}
```

