#include <stdio.h>
#include <stdlib.h>
#include "UF/uf.h"
#include "eleicao/eleicao.h"
#include "candidato/candidato_eleicao.h"

int main() {

    UF *ufs[30];
    Eleicao *eleicoes[100];
    Candidato *candidatos[200];

    carregarArquivos();

    carregarUFs(ufs, 30);
    carregarEleicoes(eleicoes, 100);
    carregarCandidatos(candidatos, 200);

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
                menuUF(ufs);
                break;
            case 3:
                menuEleicao(eleicoes);
                break;
            case 4:
                menuCandidatos(candidatos);
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    liberarUFs(ufs, 50);
    liberarEleicoes(eleicoes, 100);
    liberarCandidatos(candidatos, 200);

    return 0;
}