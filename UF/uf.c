#include <stdio.h>
#include "uf.h"
#include "../eleicao/eleicao.h"
#include <stdlib.h>
#include <string.h>

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler(char sentenca[], int tamanho) {
    do {
        fflush(stdin);
        fgets(sentenca, tamanho, stdin);
        sentenca[strcspn(sentenca, "\n")] = '\0';

        if (strlen(sentenca) == tamanho - 1 && sentenca[tamanho - 2] != '\n') {
            limparBuffer();
        }

    } while (strlen(sentenca) == 0);
}

void lerNaoObrigatorio(char sentenca[], int tamanho) {
        fflush(stdin);
        fgets(sentenca, tamanho, stdin);
        sentenca[strcspn(sentenca, "\n")] = '\0';

        if (strlen(sentenca) == tamanho - 1 && sentenca[tamanho - 2] != '\n') {
            limparBuffer();
        }
}

int verificarCodigo(int codigo_uf) {

    FILE *fuf = fopen("uf.data", "rb+");
    UF uf;
    while (fread(&uf, sizeof(UF), 1, fuf) == 1) {
        if (uf.codigo == codigo_uf) {
            fclose(fuf);
            return 1;
        }
    }
    fclose(fuf);
    return 0;
}

int carregarUFs(UF *ufs[], int total_ufs) {

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf == NULL) return 0;

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    for (int i = 0; i < total_ufs; i++) {
        ufs[i] = NULL;
    }
    fseek(fuf, 0, SEEK_SET);
    for (int i = 0; i < num_ufs; i++) {
        UF *uf = (UF *)malloc(sizeof(UF));
        fread(uf, sizeof(UF), 1, fuf);

        ufs[i] = uf;
    }

    fclose(fuf);
    return num_ufs;
}

void liberarUFs(UF *ufs[], int total_uf) {
    for (int i = 0; i < total_uf; i++) {
        if (ufs[i] != NULL) {
            free(ufs[i]);
            ufs[i] = NULL;
        }
    }
}

void menuUF(UF *ufs[], int *num_ufs) {
    char opcao_uf;
    do {
        printf("----OPCOES PARA UNIDADES FEDERATIVAS----\n");
        printf("1. Inserir UF\n");
        printf("2. Alterar UF\n");
        printf("3. Excluir UF\n");
        printf("4. Mostrar dados de todas as UFs\n");
        printf("5. Mostrar dados de uma UF\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        scanf("%c", &opcao_uf);
        limparBuffer();
        switch (opcao_uf) {
            case '1':
                adicionarUF(ufs, num_ufs);
                break;
            case '2':
                alterarUF(ufs, *num_ufs);
                break;
            case '3':
                excluirUF(ufs, num_ufs);
                break;
            case '4':
                mostrarDadosDasUFs(ufs, *num_ufs);
                break;
            case '5':
                mostrarUF(ufs, *num_ufs);
                break;
            case '0':
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_uf != '0');
}

void adicionarUF(UF *ufs[], int *num_ufs) {

    if (*num_ufs >= 35) {
        printf("maximo de UFs atingido\n");
        return;
    }

    ufs[*num_ufs] = (UF *)malloc(sizeof(UF));
    if (ufs[*num_ufs] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        return;
    }

    do {
        printf("Digite um codigo valido pra UF\n");
        scanf("%d", &ufs[*num_ufs]->codigo);
        limparBuffer();
    } while (verificarCodigo(ufs[*num_ufs]->codigo));


    printf("codigo atribuido: %d\n", ufs[*num_ufs]->codigo);
    printf("Digite o nome da UF: ");
    ler(ufs[*num_ufs]->descricao, sizeof(ufs[*num_ufs]->descricao));

    printf("Digite a sigla da UF: ");
    ler(ufs[*num_ufs]->sigla, sizeof(ufs[*num_ufs]->sigla));

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf != NULL) {
        fseek(fuf, 0, SEEK_END);
        fwrite(ufs[*num_ufs], sizeof(UF), 1, fuf);
        fclose(fuf);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }

    (*num_ufs)++;
    printf("UF adicionada!\n");
}

void alterarUF(UF *ufs[], int num_ufs) {

    int codigo_uf;
    char opcao_alterar_uf;

    printf("Digite o codigo da UF a ser alterada: ");
    scanf("%d", &codigo_uf);
    limparBuffer();
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            do {
                FILE *fuf = fopen("uf.data", "rb+");
                if (fuf == NULL) {
                    printf("Erro ao abrir arquivo para alterar\n");
                    return;
                }
                printf("O que gostaria de alterar nessa UF?\n");
                printf("1. Nome (atual: %s)\n", ufs[i]->descricao);
                printf("2. Sigla (atual: %s)\n", ufs[i]->sigla);
                printf("0. Nada\n");
                scanf("%d", &opcao_alterar_uf);
                limparBuffer();
                switch (opcao_alterar_uf) {
                    case 1:
                        printf("Novo nome da UF: ");
                        ler(ufs[i]->descricao, sizeof(ufs[i]->descricao));
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(ufs[i], sizeof(UF), 1, fuf);
                        printf("Nome da UF alterado!\n");
                        fclose(fuf);
                        break;
                    case 2:
                        printf("Nova sigla da UF: ");
                        ler(ufs[i]->sigla, sizeof(ufs[i]->sigla));
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(ufs[i], sizeof(UF), 1, fuf);
                        printf("Sigla da UF alterada!\n");
                        fclose(fuf);
                        break;
                    case 0:
                        fclose(fuf);
                        break;
                    default:
                        printf("opcao invalida!\n");
                        fclose(fuf);
                        break;
                }
            } while (opcao_alterar_uf != 0);

            return;
        }
    }

    printf("Nao existe UF com esse codigo\n");
}

void mostrarDadosDasUFs(UF *ufs[], int num_ufs) {

    if (num_ufs == 0) {
        printf("Nao ha ufs cadastradas\n");
        return;
    }

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] == NULL) continue;
        printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", ufs[i]->codigo, ufs[i]->descricao, ufs[i]->sigla);
    }

}

void mostrarUF(UF *ufs[], int num_ufs) {

    if (num_ufs == 0) {
        printf("Nao ha UFs cadastradas\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF que deseja observar: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int encontrado = -1;
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", ufs[encontrado]->codigo, ufs[encontrado]->descricao,
        ufs[encontrado]->sigla);

}

void excluirUF(UF *ufs[], int *num_ufs) {

    int codigo_uf;
    printf("Digite o codigo da UF que deseja excluir: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int encontrado = -1;
    for (int i = 0; i < *num_ufs; i++) {
        if (ufs[i] != NULL && ufs[i]->codigo == codigo_uf) {
            free(ufs[i]);
            ufs[i] = NULL;
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    for (int i = encontrado; i < *num_ufs - 1; i++) {
        ufs[i] = ufs[i + 1];
    }
    ufs[*num_ufs - 1] = NULL;
    (*num_ufs)--;

    FILE *fuf = fopen("uf.data", "wb+");
    if (fuf == NULL) {
        printf("erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *num_ufs; i++) {
        if (ufs[i] != NULL) {
            fwrite(ufs[i], sizeof(UF), 1, fuf);
        }
    }

    fclose(fuf);
    printf("UF removida!\n");
}