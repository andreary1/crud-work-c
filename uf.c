#include <stdio.h>
#include "uf.h"

void adicionarUF(UF *ufs, int *total_ufs) {
    FILE *fuf = fopen("uf.data", "rb+");
    if (*total_ufs > 50) {
        printf("maximo de UFs atingido\n");
        return;
    }

    UF uf;
    printf("Digite o codigo da UF: ");
    scanf("%d", &uf.codigo);
    printf("Digite o nome da UF: ");
    scanf("%s", uf.descricao);
    printf("Digite a sigla da UF: ");
    scanf("%s", uf.sigla);

    fseek(fuf, 0, SEEK_END);
    fwrite(&uf, sizeof(UF), 1, fuf);
    fclose(fuf);
    printf("UF adicionada!\n");
    (*total_ufs)++;
}

void alterarUF(UF *ufs, const int total_ufs) {
    FILE *fuf = fopen("uf.data", "rb+");
    int codigo_uf;
    int opcao_uf;
    printf("Digite o codigo da UF a ser alterada: ");
    scanf("%d", &codigo_uf);
    for (int i = 0; i < total_ufs; i++) {
        if (ufs[i].codigo == codigo_uf) {
            do {
                printf("O que gostaria de alterar nessa UF?\n");
                printf("1. Nome (atual: %s)\n", ufs[i].descricao);
                printf("2. Sigla (atual: %s)\n", ufs[i].sigla);
                printf("0. Nada\n");
                scanf("%d", &opcao_uf);
                switch (opcao_uf) {
                    case 1:
                        printf("Novo nome da UF: ");
                        scanf("%s", ufs[i].descricao);
                        fwrite(ufs, sizeof(UF), total_ufs, fuf);
                        printf("Nome da UF alterado!\n");
                        return;
                    case 2:
                        printf("Nova sigla da UF: ");
                        scanf("%s", ufs[i].sigla);
                        fwrite(ufs, sizeof(UF), total_ufs, fuf);
                        printf("\n");
                        return;
                    case 0:
                        fclose(fuf);
                        return;
                    default:
                        printf("opcao invalida!\n");
                        break;
                }
            } while (opcao_uf != 0);
        }
    }
    printf("Nao existe UF com esse codigo\n");
}

void mostrarDados(UF *ufs, const int total_ufs) {
    FILE *fuf = fopen("uf.data", "rb+");
    if (total_ufs == 0) {
        printf("Nao ha UFs cadastradas\n");
        return;
    }
    UF uf;
    while (fread(&uf, sizeof(UF), 1, fuf)) {
        printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", uf.codigo, uf.descricao, uf.sigla);
    }
    fclose(fuf);
}