#include <stdio.h>
#include <stdlib.h>
#include "UF/uf.h"
#include "eleicao/eleicao.h"
#include "candidato/candidato_eleicao.h"

int main() {

    UF *ufs[35];
    Eleicao *eleicoes[100];
    Candidato *candidatos[200];

    carregarArquivos();

    int num_ufs = carregarUFs(ufs, 35);
    int num_eleicoes = carregarEleicoes(eleicoes, 100);
    int num_candidatos = carregarCandidatos(candidatos, 200);

    int opcao;
    do {
        printf("----------MENU----------\n");
        printf("Selecione uma opcao:\n");
        printf("1 - Unidades Federativas\n");
        printf("2 - Pessoas\n");
        printf("3 - Eleicoes\n");
        printf("4 - Candidatos\n");
        printf("5 - Votos\n");
        printf("6 - Comparecimentos\n");
        printf("0 - Sair\n");
        printf("------------------------\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                menuUF(ufs, &num_ufs, &num_eleicoes);
                break;
            case 3:
                menuEleicao(eleicoes, &num_eleicoes);
                break;
            case 4:
                //menuCandidatos(candidatos, ufs, &num_candidatos);
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    liberarUFs(ufs, 35);
    liberarEleicoes(eleicoes, 100);
    liberarCandidatos(candidatos, 200);

    return 0;
}