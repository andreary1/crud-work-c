#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UF/uf.h"
#include "eleicao/eleicao.h"
#include "candidato/candidato_eleicao.h"
#include "pessoa/pessoa.h"
#include "voto/voto.h"

void carregarArquivos();

int main() {

    UF *ufs[35];
    Pessoa *pessoas[400];
    Eleicao *eleicoes[100];
    Candidato *candidatos[200];
    Voto *votos[1000];
    Comparecimento *comparecimentos[1000];


    carregarArquivos();

    int num_ufs = carregarUFs(ufs, 35);
    int num_pessoas = carregarPessoas(pessoas, 400);
    int num_eleicoes = carregarEleicoes(eleicoes, 100);
    int num_candidatos = carregarCandidatos(candidatos, 200);
    int num_votos = carregarVotos(votos, 1000);
    int num_comparecimentos = carregarComparecimentos(comparecimentos, 1000);

    char opcao;
    do {
        printf("----------MENU----------\n");
        printf("1 - Unidades Federativas\n");
        printf("2 - Pessoas\n");
        printf("3 - Eleicoes\n");
        printf("4 - Candidatos\n");
        printf("5 - Votos\n");
        printf("6 - Comparecimentos\n");
        printf("0 - Sair\n");
        printf("------------------------\n");
        scanf("%c", &opcao);
        limparBuffer();
        switch (opcao) {
            case '1':
                menuUF(ufs, &num_ufs);
                break;
            case '2':
                menuPessoas(pessoas, &num_pessoas);
                break;
            case '3':
                menuEleicao(eleicoes, &num_eleicoes);
                break;
            case '4':
                menuCandidatos(candidatos, ufs, &num_candidatos, &num_ufs);
                break;
            case '5':
                menuVotos(votos, &num_votos, ufs, &num_ufs, candidatos, &num_candidatos, comparecimentos ,&num_comparecimentos);
                break;
            case '6':
                menuComparecimento(comparecimentos, &num_comparecimentos, ufs, &num_ufs, candidatos, &num_candidatos);
            case '0':
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != '0');

    liberarUFs(ufs, 35);
    liberarPessoas(pessoas, 400);
    liberarEleicoes(eleicoes, 100);
    liberarCandidatos(candidatos, 200);
    liberarVotos(votos, 1000);
    liberarComparecimentos(comparecimentos, 1000);

    return 0;
}

void carregarArquivos() {
    FILE *arquivos[] = {
        fopen("uf.data", "rb+"),
        fopen("pessoas.data", "rb+"),
        fopen("eleicao.data", "rb+"),
        fopen("candidatos.data", "rb+"),
        fopen("votos.data", "rb+"),
        fopen("comparecimentos.data", "rb+")
    };
    for (int i = 0; i < 6; i++) {
        if (arquivos[i] == NULL) {
            const char *nomes[] = {
                "uf.data", "pessoas.data", "eleicao.data",
                "candidatos.data", "votos.data", "comparecimentos.data"
            };
            arquivos[i] = fopen(nomes[i], "wb+");
            if (arquivos[i] == NULL) {
                printf("Erro ao criar arquivo %s\n", nomes[i]);
            } else {
                fclose(arquivos[i]);
            }
        } else {
            fclose(arquivos[i]);
        }
    }
}
