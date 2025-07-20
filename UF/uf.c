//#define _GNU_SOURCE
#include <stdio.h>
#include "uf.h"
#include "../eleicao/eleicao.h"
#include <stdlib.h>
#include <string.h>

extern UF **ufs;
extern Eleicao **eleicoes;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler(char sentenca[], int tamanho) {
    do {
        fflush(stdin);
        //__fpurge(stdin);
        fgets(sentenca, tamanho, stdin);
        sentenca[strcspn(sentenca, "\n")] = '\0';

        if (strlen(sentenca) == tamanho - 1 && sentenca[tamanho - 2] != '\n') {
            limparBuffer();
        }

    } while (strlen(sentenca) == 0 || strspn(sentenca, " ") == strlen(sentenca));
}

void lerNaoObrigatorio(char sentenca[], int tamanho) {
    fflush(stdin);
    fgets(sentenca, tamanho, stdin);
    sentenca[strcspn(sentenca, "\n")] = '\0';

    if (strlen(sentenca) == tamanho - 1 && sentenca[tamanho - 2] != '\n') {
        limparBuffer();
    }
}

int verificarCodigo(int codigo_uf, int num_ufs) {

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            return 1;
        }
    }

    return 0;
}

int verificarSigla(char sig[], int num_ufs) {

    for (int i = 0; i < num_ufs; i++) {
        if (strcasecmp(ufs[i]->sigla, sig) == 0) {
            return 1;
        }
    }

    return 0;
}

int carregarUFs(int *capacidade_ufs) {

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf == NULL) return 0;

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    while (num_ufs > *capacidade_ufs) {
        *capacidade_ufs *= 2;
    }

    ufs = malloc(*capacidade_ufs * sizeof(UF *));
    if (ufs == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 0;
    }

    fseek(fuf, 0, SEEK_SET);
    for (int i = 0; i < num_ufs; i++) {
        UF *uf = (UF *)malloc(sizeof(UF));
        if (uf == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(uf, sizeof(UF), 1, fuf);

        ufs[i] = uf;
    }

    for (int i = num_ufs; i < *capacidade_ufs; i++) {
        ufs[i] = NULL;
    }

    fclose(fuf);
    return num_ufs;
}

void liberarUFs(int total_uf) {
    for (int i = 0; i < total_uf; i++) {
        if (ufs[i] != NULL) {
            free(ufs[i]);
            ufs[i] = NULL;
        }
    }
    free(ufs);
    ufs = NULL;
}

void adicionarUF(int *num_ufs, int *capacidade_ufs) {

    if (*num_ufs >= *capacidade_ufs) {
        *capacidade_ufs *= 2;
        ufs = realloc(ufs, *capacidade_ufs * sizeof(UF *));
        if (ufs == NULL) {
            printf("erro na alocacao de memoria\n");
            return;
        }
        for (int i = *num_ufs; i < *capacidade_ufs; i++) {
            ufs[i] = NULL;
        }
    }

    int codigo_uf;
    do {
        printf("Digite um codigo valido pra UF: ");
        scanf("%d", &codigo_uf);
        limparBuffer();
    } while (verificarCodigo(codigo_uf, *num_ufs) || codigo_uf == 0);

    char desc[30];
    printf("codigo atribuido: %d\n", codigo_uf);
    printf("Digite o nome da UF: ");
    ler(desc, sizeof(desc));

    char sig[3];
    do {
        printf("Digite a sigla da UF: ");
        ler(sig, sizeof(sig));
    } while (verificarSigla(sig, *num_ufs));


    ufs[*num_ufs] = (UF *)malloc(sizeof(UF));
    if (ufs[*num_ufs] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        return;
    }

    ufs[*num_ufs]->codigo = codigo_uf;
    strcpy(ufs[*num_ufs]->descricao, desc);
    strcpy(ufs[*num_ufs]->sigla, sig);

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf != NULL) {
        fseek(fuf, 0, SEEK_END);
        fwrite(ufs[*num_ufs], sizeof(UF), 1, fuf);
        fclose(fuf);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        free(ufs[*num_ufs]);
        return;
    }

    (*num_ufs)++;
    printf("UF adicionada!\n");
}

void alterarUF(int num_ufs) {

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
                        char sig[3];
                        do {
                            ler(sig, sizeof(sig));
                        } while (verificarSigla(sig, num_ufs));
                        strcpy(ufs[i]->sigla, sig);
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

void mostrarDadosDasUFs(int num_ufs) {

    if (num_ufs == 0) {
        printf("Nao ha ufs cadastradas\n");
        return;
    }

    printf("=====================================================\n");
    printf("| %-7s | %-30s | %-6s |\n",
           "Codigo", "Nome", "Sigla");
    printf("=====================================================\n");

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] == NULL) continue;
        printf("| %-7d | %-30s | %-6s |\n", ufs[i]->codigo, ufs[i]->descricao, ufs[i]->sigla);
    }
    printf("=====================================================\n");

}

void mostrarUF(int num_ufs) {

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

    printf("=====================================================\n");
    printf("| %-7s | %-30s | %-6s |\n",
           "Codigo", "Nome", "Sigla");
    printf("=====================================================\n");

    printf("| %-7d | %-30s | %-6s |\n", ufs[encontrado]->codigo, ufs[encontrado]->descricao,
        ufs[encontrado]->sigla);
    printf("=====================================================\n");

}

void excluirUF(int *num_ufs, int *num_eleicoes, int *num_candidatos, int *num_votos,int *num_comparecimentos) {

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
            exclusaoEleicoesPorUF(num_eleicoes, num_candidatos, num_votos, num_comparecimentos, codigo_uf);
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

void exclusaoEleicoesPorUF(int *num_eleicoes, int *num_candidatos, int *num_votos, int *num_comparecimentos, int codigo) {

    int encontrado = -1;

    for (int i = 0; i < *num_eleicoes; i++) {
        if (eleicoes[i] != NULL && codigo == eleicoes[i]->codigo_uf) {
            Eleicao e = *eleicoes[i];
            free(eleicoes[i]);
            eleicoes[i] = NULL;

            encontrado = i;

            for (int j = encontrado; j < *num_eleicoes - 1; j++) {
                eleicoes[j] = eleicoes[j + 1];
            }

            eleicoes[*num_eleicoes - 1] = NULL;

            (*num_eleicoes)--;
            i--;
            exclusaoCandidatoPelaEleicao(num_candidatos, num_votos, num_comparecimentos, e);
        }
    }

    if (encontrado == -1) {
        return;
    }

    FILE *feleicao = fopen("eleicao.data", "wb+");

    if (feleicao == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *num_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
        }
    }

    fclose(feleicao);
}