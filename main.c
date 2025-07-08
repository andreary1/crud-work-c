#include <stdio.h>
#include <stdlib.h>
#include "uf.h"
#include "eleicao.h"

int main() {

    UF ufs[50];
    Eleicao eleicoes[100];

    FILE *fuf = fopen("uf.data", "rb+");
    //FILE *fpessoa = fopen("pessoas.data", "rb+");
    FILE *feleicao = fopen("eleicoes.data", "rb+");
    //FILE *fcandidato_eleicao = fopen("candidatos.data", "rb+");
    //FILE *fvoto = fopen("votos.data", "rb+");
    //FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");
    if (fuf == NULL) {
        FILE *fuf = fopen("uf.data", "wb+");
        if (fuf == NULL) {
            printf("erro ao criar arquivo da uf\n");
            return -1;
        }
    }

    if (feleicao == NULL) {
        FILE *feleicao = fopen("eleicao.data", "wb+");
        if (feleicao == NULL) {
            printf("erro ao criar arquivo da eleicao\n");
            return -1;
        }
    }

    int total_ufs = carregarUFs(ufs);
    int total_eleicoes = carregarEleicoes(eleicoes);

    int codigo_uf_atual = 0;
    while (fread(ufs, sizeof(UF), 1, fuf) == 1) {
        codigo_uf_atual++;
    }

    int opcao;
    do {
        printf("--------MENU--------\n");
        printf("Selecione uma opcao:\n");
        printf("1 - Unidades Federativas\n");
        printf("2 - Pessoas\n");
        printf("3 - Eleicoes\n");
        printf("4 - Candidatos\n");
        printf("5 - Votos\n");
        printf("6 - Comparecimentos\n");
        printf("0 - Sair\n");
        printf("--------------------\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                menuUF(ufs, &total_ufs, &codigo_uf_atual);
                break;
            case 3:
                menuEleicao(eleicoes, &total_eleicoes, &codigo_uf_atual);
                break;
        }
    } while (opcao != 0);

    return 0;
}