#include <stdio.h>
#include "uf.h"
#include <stdlib.h>
#include <string.h>

void menuUF(UF *ufs, int *total_ufs, int *codigo_uf_atual) {
    int opcao_uf;
    printf("----OPCOES DE UNIDADES FEDERATIVA----\n");
    printf("1. Inserir UF\n");
    printf("2. Alterar UF\n");
    printf("3. Excluir UF\n");
    printf("4. Mostrar dados de todas as UFs\n");
    printf("5. Mostrar dados de uma UF\n");
    printf("0. Sair\n");
    printf("-------------------------------------\n");
    scanf("%d", &opcao_uf);
    switch (opcao_uf) {
        case 1:
            adicionarUF(total_ufs, codigo_uf_atual);
            *total_ufs = carregarUFs(ufs);
            break;
        case 2:
            *total_ufs = carregarUFs(ufs);
            alterarUF(ufs, *total_ufs);
            break;
        case 3:
            excluirUF(ufs, total_ufs, codigo_uf_atual);
            *total_ufs = carregarUFs(ufs);
            break;
        case 4:
            *total_ufs = carregarUFs(ufs);
            mostrarDadosDasUFs(*total_ufs);
            break;
        case 5:
            *total_ufs = carregarUFs(ufs);
            mostrarUF(ufs, *total_ufs);
            break;
        case 0:
            printf("Saindo\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
}

int carregarUFs(UF *ufs) {
    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf == NULL) return 0;

    int total = 0;
    while (fread(&ufs[total], sizeof(UF), 1, fuf)) {
        total++;
    }
    fclose(fuf);
    return total;
}

void adicionarUF(int *total_ufs, int *codigo_uf) {
    FILE *fuf = fopen("uf.data", "rb+");
    if (*total_ufs > 50) {
        printf("maximo de UFs atingido\n");
        return;
    }

    UF uf;

    uf.codigo = (*codigo_uf)++;

    printf("Digite o nome da UF: ");
    fflush(stdin);
    gets(uf.descricao);

    printf("Digite a sigla da UF: ");
    fflush(stdin);
    gets(uf.sigla);

    fseek(fuf, 0, SEEK_END);
    fwrite(&uf, sizeof(UF), 1, fuf);
    fclose(fuf);
    printf("UF adicionada!\n");
    (*total_ufs)++;
}

void alterarUF(UF *ufs, const int total_ufs) {
    FILE *fuf = fopen("uf.data", "rb+");
    int codigo_uf;
    int opcao_alterar_uf;
    printf("Digite o codigo da UF a ser alterada: ");
    scanf("%d", &codigo_uf);
    for (int i = 0; i < total_ufs; i++) {
        if (ufs[i].codigo == codigo_uf) {
            do {
                printf("O que gostaria de alterar nessa UF?\n");
                printf("1. Nome (atual: %s)\n", ufs[i].descricao);
                printf("2. Sigla (atual: %s)\n", ufs[i].sigla);
                printf("0. Nada\n");
                scanf("%d", &opcao_alterar_uf);
                switch (opcao_alterar_uf) {
                    case 1:
                        printf("Novo nome da UF: ");
                        fflush(stdin);
                        gets(ufs[i].descricao);
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(&ufs[i], sizeof(UF), 1, fuf);
                        printf("Nome da UF alterado!\n");
                        break;
                    case 2:
                        printf("Nova sigla da UF: ");
                        fflush(stdin);
                        gets(ufs[i].sigla);
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(&ufs[i], sizeof(UF), 1, fuf);
                        printf("Sigla da UF alterada!\n");
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        break;
                }
            } while (opcao_alterar_uf != 0);

            fclose(fuf);
            return;
        }
    }
    printf("Nao existe UF com esse codigo\n");
    fclose(fuf);
}

void mostrarDadosDasUFs(const int total_ufs) {
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

void mostrarUF(UF *ufs, const int total_ufs) {

    FILE *fuf = fopen("uf.data", "rb+");

    if (total_ufs == 0) {
        printf("Nao ha UFs cadastradas\n");
        return;
    }
    UF uf;
    int codigo_uf;
    printf("Digite o codigo da UF que deseja observar: ");
    scanf("%d", &codigo_uf);

    int encontrado = 0;
    for (int i = 0; i < total_ufs; i++) {
        if (ufs[i].codigo == codigo_uf) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", ufs[codigo_uf].codigo, ufs[codigo_uf].descricao,
        ufs[codigo_uf].sigla);

    fclose(fuf);
}

void excluirUF(UF *ufs, int *total_ufs, int *codigo_uf_seguinte) {

    int codigo_uf;
    printf("Digite o codigo da UF que deseja excluir: ");
    scanf("%d", &codigo_uf);

    int encontrado = 0;
    for (int i = 0; i < *total_ufs; i++) {
        if (ufs[i].codigo == codigo_uf) {
            for (int j = i; j < *total_ufs - 1; j++) {
                *(ufs + j) = *(ufs + j + 1);
            }
            (*total_ufs)--;
            (*codigo_uf_seguinte)--;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    for (int i = 0; i < *total_ufs; i++) {
        ufs[i].codigo = i;
    }

    FILE *fuf = fopen("uf.data", "wb+");
    fwrite(ufs, sizeof(UF), *total_ufs, fuf);
    fclose(fuf);
    printf("UF removida!\n");
}