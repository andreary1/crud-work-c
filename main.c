#include <stdio.h>
#include <stdlib.h>
#include "uf.h"

int main() {

    UF ufs[50];

    FILE *fuf = fopen("uf.data", "rb+");
    //FILE *fpessoa = fopen("pessoas.data", "rb+");
    //FILE *feleicao = fopen("eleicoes.data", "rb+");
    //FILE *fcandidato_eleicao = fopen("candidatos.data", "rb+");
    //FILE *fvoto = fopen("votos.data", "rb+");
    //FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");
    if (fuf == NULL) {
        FILE *fuf = fopen("uf.data", "wb+");
        if (fuf == NULL) {
            printf("erro ao criar arquivo\n");
            return -1;
        }
    }

    int total_ufs = carregarUFs(ufs);

    int codigo_uf_atual = 0;
    while (fread(&ufs[total_ufs], sizeof(UF), 1, fuf) == 1) {
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
                        adicionarUF(ufs, &total_ufs, &codigo_uf_atual);
                        total_ufs = carregarUFs(ufs);
                        break;
                    case 2:
                        total_ufs = carregarUFs(ufs);
                        alterarUF(ufs, total_ufs);
                        break;
                    case 3:
                        excluirUF(ufs, &total_ufs, &codigo_uf_atual);
                        total_ufs = carregarUFs(ufs);
                        break;
                    case 4:
                        total_ufs = carregarUFs(ufs);
                        mostrarDados(total_ufs);
                        break;
                    case 5:
                        total_ufs = carregarUFs(ufs);
                        mostrarUF(ufs, total_ufs);
                        break;
                    case 0:
                        printf("Saindo\n");
                        break;
                    default:
                        printf("Opcao invalida!\n");
                        break;
                }
        }

    } while (opcao != 0);

    return 0;
}