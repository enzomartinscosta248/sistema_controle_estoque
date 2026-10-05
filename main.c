```c
#include <stdio.h>
#include "produto.h"


void exibirMenu(void) {
    printf("\n===== CONTROLE DE ESTOQUE =====\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar produto por codigo\n");
    printf("4 - Registrar entrada de estoque\n");
    printf("5 - Registrar saida de estoque\n");
    printf("6 - Remover produto\n");
    printf("0 - Salvar e sair\n");
    printf("Escolha uma opcao: ");
}


int main(void) {
    Produto estoque[MAX_PRODUTOS];

    int total = carregarDados(estoque);
    int opcao;

    printf(
        "Bem-vindo ao controle de estoque! "
        "%d produto(s) carregado(s).\n",
        total
    );

    do {
        /*
         * Usa a mesma funcao de leitura segura utilizada
         * no restante do programa.
         */
        if (!lerInteiro("", &opcao)) {
            continue;
        }

        /*
         * O menu precisa ser mostrado antes da leitura.
         * Por isso fazemos a leitura separadamente abaixo.
         */
        switch (opcao) {

            case 1:
                cadastrarProduto(estoque, &total);
                break;

            case 2:
                listarProdutos(estoque, total);
                break;

            case 3: {
                int codigo;

                if (!lerInteiro(
                        "Codigo: ",
                        &codigo
                    )) {
                    break;
                }

                Produto *encontrado =
                    buscarPorCodigo(
                        estoque,
                        total,
                        codigo
                    );

                if (encontrado != NULL) {
                    printf(
                        "\n--- Produto encontrado ---\n"
                    );

                    printf(
                        "Codigo:   %d\n",
                        encontrado->codigo
                    );

                    printf(
                        "Nome:     %s\n",
                        encontrado->nome
                    );

                    printf(
                        "Qtd:      %d\n",
                        encontrado->quantidade
                    );

                    printf(
                        "Preco:    R$ %.2f\n",
                        encontrado->precoUnitario
                    );
                } else {
                    printf(
                        "Produto nao encontrado.\n"
                    );
                }

                break;
            }

            case 4:
                registrarEntrada(
                    estoque,
                    total
                );
                break;

            case 5:
                registrarSaida(
                    estoque,
                    total
                );
                break;

            case 6: {
                int codigo;

                if (!lerInteiro(
                        "Codigo do produto a remover: ",
                        &codigo
                    )) {
                    break;
                }

                if (removerProduto(
                        estoque,
                        &total,
                        codigo
                    )) {

                    printf(
                        "Produto removido com sucesso.\n"
                    );

                } else {

                    printf(
                        "Produto nao encontrado.\n"
                    );
                }

                break;
            }

            case 0:
                salvarDados(
                    estoque,
                    total
                );

                printf(
                    "Dados salvos. Ate mais!\n"
                );
                break;

            default:
                printf(
                    "Opcao invalida.\n"
                );
        }

    } while (opcao != 0);

    return 0;
}
```
