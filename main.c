#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "inventory.txt"
#define ARQUIVO_TEMP "temp.txt"
#define MAX_NOME 100

void limparBuffer(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        /* descarta caracteres restantes */
    }
}

void adicionarItem(void) {
    char nome[MAX_NOME];
    int quantidade;

    printf("\nDigite o nome do item: ");
    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        printf("Erro ao ler o nome do item.\n");
        return;
    }

    nome[strcspn(nome, "\n")] = '\0';

    if (strlen(nome) == 0) {
        printf("O nome do item não pode ficar vazio.\n");
        return;
    }

    printf("Digite a quantidade: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        printf("Quantidade inválida!\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }

    fprintf(arquivo, "%s\n%d\n", nome, quantidade);
    fclose(arquivo);

    printf("Item adicionado com sucesso!\n");
}

void removerItem(void) {
    char nome[MAX_NOME];
    char nomeArquivo[MAX_NOME];
    int quantidadeRemover;
    int quantidadeAtual;
    int encontrado = 0;

    printf("\nDigite o nome do item: ");
    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        printf("Erro ao ler o nome do item.\n");
        return;
    }
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite a quantidade a ser removida: ");
    if (scanf("%d", &quantidadeRemover) != 1 || quantidadeRemover <= 0) {
        printf("Quantidade inválida!\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Item não encontrado.\n");
        return;
    }

    FILE *temporario = fopen(ARQUIVO_TEMP, "w");
    if (temporario == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    while (fgets(nomeArquivo, sizeof(nomeArquivo), arquivo) != NULL) {
        nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

        if (fscanf(arquivo, "%d", &quantidadeAtual) != 1) {
            break;
        }
        limparBuffer();

        if (strcmp(nomeArquivo, nome) == 0 && !encontrado) {
            encontrado = 1;

            if (quantidadeRemover > quantidadeAtual) {
                printf("Estoque insuficiente. Quantidade disponível: %d\n",
                       quantidadeAtual);
                fprintf(temporario, "%s\n%d\n", nomeArquivo, quantidadeAtual);
            } else if (quantidadeRemover == quantidadeAtual) {
                printf("Item removido do estoque!\n");
            } else {
                quantidadeAtual -= quantidadeRemover;
                fprintf(temporario, "%s\n%d\n", nomeArquivo, quantidadeAtual);
                printf("Quantidade atualizada com sucesso!\n");
            }
        } else {
            fprintf(temporario, "%s\n%d\n", nomeArquivo, quantidadeAtual);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrado) {
        printf("Item não encontrado.\n");
    }

    if (remove(ARQUIVO) != 0) {
        printf("Erro ao atualizar o arquivo de estoque.\n");
        remove(ARQUIVO_TEMP);
        return;
    }

    if (rename(ARQUIVO_TEMP, ARQUIVO) != 0) {
        printf("Erro ao salvar as alterações no estoque.\n");
    }
}

void listarEstoque(void) {
    char nome[MAX_NOME];
    int quantidade;
    int vazio = 1;

    FILE *arquivo = fopen(ARQUIVO, "r");

    printf("\n=============================\n");
    printf("        Estoque Atual\n");
    printf("=============================\n");

    if (arquivo == NULL) {
        printf("O estoque está vazio.\n");
        return;
    }

    while (fgets(nome, sizeof(nome), arquivo) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';

        if (fscanf(arquivo, "%d", &quantidade) != 1) {
            break;
        }
        limparBuffer();

        printf("Nome: %s\n", nome);
        printf("Quantidade: %d\n\n", quantidade);
        vazio = 0;
    }

    if (vazio) {
        printf("O estoque está vazio.\n");
    }

    fclose(arquivo);
}

int main(void) {
    int opcao;
    char resposta;

    do {
        printf("\n================================\n");
        printf("       Controle de Estoque\n");
        printf("================================\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Tente novamente.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao < 1 || opcao > 4) {
            printf("Opção inválida! Tente novamente.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                adicionarItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarEstoque();
                break;
            case 4:
                printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");
                return 0;
        }

        printf("\nDeseja realizar outra operação? (s/n): ");
        if (scanf(" %c", &resposta) != 1) {
            resposta = 'n';
        }
        limparBuffer();

    } while (resposta == 's' || resposta == 'S');

    printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");
    return 0;
}
