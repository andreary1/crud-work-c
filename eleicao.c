#include "eleicao.h"
#include <stdio.h>
#include <stdlib.h>
#include "uf.h"

int carregarEleicoes(Eleicao *eleicoes) {
    FILE *feleicao = fopen("eleicao.data", "rb+");
    if (feleicao == NULL) return 0;

    int total = 0;
    while (fread(&eleicoes, sizeof(Eleicao), 1, feleicao) == 1) {
        total++;
    }

    fclose(feleicao);
    return total;
}

void menuEleicao(Eleicao *eleicoes, int *total_eleicoes, int *codigo_uf_atual) {
    int opcao_eleicao;
    printf("----OPCOES DE ELEICOES----\n");
    printf("1. Inserir Eleicao\n");
    printf("2. Alterar Eleicao\n");
    printf("3. Excluir Eleicao\n");
    printf("4. Mostrar dados de todas as eleicoes\n");
    printf("5. Mostrar dados de uma eleicao\n");
    printf("0. Sair\n");
    printf("-------------------------------------\n");
    scanf("%d", &opcao_eleicao);
    switch (opcao_eleicao) {
        case 1:
            break;
    }
}