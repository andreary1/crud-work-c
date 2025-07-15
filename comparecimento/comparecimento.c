#include "comparecimento.h"
#include "../uf/uf.h"
#include <stdio.h>
#include <stdlib.h>

#include "../eleicao/eleicao.h"

void menuComparecimento(Comparecimento *comparecimentos[], int *num_comparecimentos, UF *ufs[], int *num_ufs,
    Candidato *candidatos[], int *num_candidatos) {
    char opcao_comparecimento;
    do {
        printf("--------------OPCOES PARA COMPARECIMENTO--------------\n");
        printf("1. Mostrar comparecimentos por UF e ano\n");
        printf("2. Mostrar todos os comparecimentos\n");
        printf("0. Sair\n");
        printf("------------------------------------------------------\n");
        scanf("%c", &opcao_comparecimento);
        limparBuffer();
        switch (opcao_comparecimento) {
            case '1':
                mostrarComparecimentosEleicao(comparecimentos, *num_comparecimentos);
                break;
            case '2':
                //mostrarVotosPorCandidato(votos, *num_votos, ufs, *num_ufs, candidatos, *num_candidatos);
                break;
            case '0':
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_comparecimento != '0');
}

int carregarComparecimentos(Comparecimento *comparecimentos[], int total_comparecimentos) {
    FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");
    if (fcomparecimento == NULL) return 0;

    fseek(fcomparecimento, 0, SEEK_END);
    long int num_comparecimentos = ftell(fcomparecimento);
    num_comparecimentos /= sizeof(Comparecimento);

    for (int i = 0; i < total_comparecimentos; i++) {
        comparecimentos[i] = NULL;
    }
    fseek(fcomparecimento, 0, SEEK_SET);
    for (int i = 0; i < num_comparecimentos; i++) {
        Comparecimento *comparecimento = (Comparecimento *)malloc(sizeof(Comparecimento));
        fread(comparecimento, sizeof(Comparecimento), 1, fcomparecimento);

        comparecimentos[i] = comparecimento;
    }

    fclose(fcomparecimento);
    return num_comparecimentos;
}

void liberarComparecimentos(Comparecimento *comparecimentos[], int total_comparecimentos) {
    for (int i = 0; i < total_comparecimentos; i++) {
        free(comparecimentos[i]);
        comparecimentos[i] = NULL;
    }
}

void mostrarComparecimentosEleicao(Comparecimento *comparecimentos[], int total_comparecimentos) {

    int codigo_uf;
    printf("Digite o codigo da UF em que ocorreu a eleicao: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano dessa eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    int num_comparecimentos_eleicao = 0;
    for (int i = 0; i < total_comparecimentos; i++) {
        if (comparecimentos[i]->codigo_uf == codigo_uf && comparecimentos[i]->ano == ano) {
            num_comparecimentos_eleicao++;
        }
    }

    printf("Numero de comparecimentos nessa eleicao: %d\n", num_comparecimentos_eleicao);
    int contagem = 1;
    for (int i = 0; i < total_comparecimentos; i++) {
        if (comparecimentos[i]->codigo_uf == codigo_uf && comparecimentos[i]->ano == ano) {
            printf("%d. CPF: %s\n", contagem, comparecimentos[i]->CPF);
            contagem++;
        }
    }
}

