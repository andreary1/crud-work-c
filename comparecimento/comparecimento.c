#include "comparecimento.h"
#include "../uf/uf.h"
#include <stdio.h>
#include <stdlib.h>

#include "../eleicao/eleicao.h"
#include "../voto/voto.h"

extern Comparecimento **comparecimentos;
extern Eleicao **eleicoes;
extern UF **ufs;
extern Voto **votos;

int carregarComparecimentos(int *capacidade_comp) {
    FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");
    if (fcomparecimento == NULL) return 0;

    fseek(fcomparecimento, 0, SEEK_END);
    long int num_comparecimentos = ftell(fcomparecimento);
    num_comparecimentos /= sizeof(Comparecimento);

    while (num_comparecimentos >= *capacidade_comp) {
        *capacidade_comp *= 2;
    }

    comparecimentos = malloc(*capacidade_comp * sizeof(Comparecimento *));
    if (comparecimentos == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 0;
    }

    fseek(fcomparecimento, 0, SEEK_SET);
    for (int i = 0; i < num_comparecimentos; i++) {
        Comparecimento *comparecimento = (Comparecimento *)malloc(sizeof(Comparecimento));
        if (comparecimento == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(comparecimento, sizeof(Comparecimento), 1, fcomparecimento);

        comparecimentos[i] = comparecimento;
    }
    for (int i = num_comparecimentos; i < *capacidade_comp; i++) {
        comparecimentos[i] = NULL;
    }

    fclose(fcomparecimento);
    return num_comparecimentos;
}

void liberarComparecimentos(int capacidade_comp) {
    for (int i = 0; i < capacidade_comp; i++) {
        free(comparecimentos[i]);
        comparecimentos[i] = NULL;
    }
    free(comparecimentos);
    comparecimentos = NULL;
}

void mostrarComparecimentosEleicao(int total_comparecimentos) {

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

void mostrarTodosOsComparecimentos(int num_comparecimentos, int num_ufs, int num_eleicoes) {

    if (num_comparecimentos == 0) {
        printf("Nao ha votos cadastrados\n");
        return;
    }

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] == NULL) continue;
        printf("---- %s (%s) ----\n", ufs[i]->descricao, ufs[i]->sigla);
        for (int j = 0; j < num_eleicoes; j++) {
            if (eleicoes[j] == NULL) continue;
            if (eleicoes[j]->codigo_uf == ufs[i]->codigo) {
                int num = 1;
                printf("Ano %d:\n", eleicoes[j]->ano);
                for (int k = 0; k < num_comparecimentos; k++) {
                    if (votos[k] == NULL) continue;
                    if (eleicoes[j]->ano == comparecimentos[k]->ano && comparecimentos[k]->codigo_uf == eleicoes[j]->codigo_uf) {
                        printf("%d. CPF: %s\n", num, comparecimentos[k]->CPF);
                        num++;
                    }
                }
            }
        }
    }
}

