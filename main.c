#include <stdio.h>
#include <stdlib.h>
#include "uf.h"
#include "eleicao.h"
#include "candidato_eleicao.h"

int main() {

    UF *ufs[50];
    Eleicao *eleicoes[100];
    //Candidato candidatos[200];

    carregarArquivos();

    carregarUFs(ufs, 50);
    carregarEleicoes(eleicoes, 100);
    //int total_candidatos = carregarCandidatos(candidatos);

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
                //menuCandidato(candidatos, &total_candidatos);
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    liberarUFs(ufs, 50);
    liberarEleicoes(eleicoes, 100);

    return 0;
}