#include <stdio.h>
#include <stdlib.h>
#include "uf.h"
#include "eleicao.h"

int main() {

    UF ufs[50];
    Eleicao eleicoes[100];

    carregarArquivos();

    int total_ufs = carregarUFs(ufs);
    int total_eleicoes = carregarEleicoes(eleicoes);

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
                menuUF(ufs, &total_ufs);
                break;
            case 3:
                menuEleicao(eleicoes, &total_eleicoes);
                break;
            case 4:
                //menuCandidato();
                break;
        }
    } while (opcao != 0);

    return 0;
}