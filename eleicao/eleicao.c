#include "../eleicao/eleicao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../candidato/candidato_eleicao.h"
#include "../UF/uf.h"

extern Eleicao **eleicoes;
extern Candidato **candidatos;

int carregarEleicoes(int *capacidade_eleicoes) {
    FILE *feleicao = fopen("eleicao.data", "rb+");
    if (feleicao == NULL) return 0;

    fseek(feleicao, 0, SEEK_END);
    long int num_eleicoes = ftell(feleicao);
    num_eleicoes /= sizeof(Eleicao);

    while (num_eleicoes > *capacidade_eleicoes) {
        *capacidade_eleicoes *= 2;
    }

    eleicoes = malloc(*capacidade_eleicoes * sizeof(Eleicao *));
    if (eleicoes == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 0;
    }

    fseek(feleicao, 0, SEEK_SET);
    for (int i = 0; i < num_eleicoes; i++) {
        Eleicao *eleicao = (Eleicao *)malloc(sizeof(Eleicao));
        if (eleicao == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(eleicao, sizeof(Eleicao), 1, feleicao);

        eleicoes[i] = eleicao;
    }

    for (int i = num_eleicoes; i < *capacidade_eleicoes; i++) {
        eleicoes[i] = NULL;
    }

    fclose(feleicao);
    return num_eleicoes;
}

void liberarEleicoes(int total_eleicoes) {
    for (int i = 0; i < total_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            free(eleicoes[i]);
            eleicoes[i] = NULL;
        }
    }
    free(eleicoes);
    eleicoes = NULL;
}

void inserirEleicao(int *num_eleicoes, int *capacidade_eleicoes, int num_ufs) {

    if (*num_eleicoes >= *capacidade_eleicoes) {
        *capacidade_eleicoes *= 2;
        eleicoes = realloc(eleicoes, *capacidade_eleicoes * sizeof(Eleicao *));
        if (eleicoes == NULL) {
            printf("Erro na alocacao de memoria\n");
            return;
        }
        for (int i = *num_eleicoes; i < *capacidade_eleicoes; i++) {
            eleicoes[i] = NULL;
        }
    }

    eleicoes[*num_eleicoes] = (Eleicao *)malloc(sizeof(Eleicao));
    if (eleicoes[*num_eleicoes] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que ocorreu a eleicao: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    if (!verificarCodigo(codigo_uf, num_ufs)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    if (verificarAnoeCodigo(codigo_uf, ano, *num_eleicoes)) {
        printf("Ja existe uma eleicao com essa configuracao\n");
        return;
    }

    eleicoes[*num_eleicoes]->codigo_uf = codigo_uf;
    eleicoes[*num_eleicoes]->ano = ano;

    printf("Digite a descricao da eleicao: ");
    lerNaoObrigatorio(eleicoes[*num_eleicoes]->descricao, sizeof(eleicoes[*num_eleicoes]->descricao));

    FILE *feleicao = fopen("eleicao.data", "rb+");
    if (feleicao != NULL) {
        fseek(feleicao, 0, SEEK_END);
        fwrite(eleicoes[*num_eleicoes], sizeof(Eleicao), 1, feleicao);
        fclose(feleicao);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }

    (*num_eleicoes)++;
    printf("Eleicao adicionada!\n");
}

void mostrarDadosDasEleicoes(int total_eleicoes) {
    if (total_eleicoes == 0) {
        printf("Nao ha eleicoes cadastradas\n");
        return;
    }

    printf("======================================================\n");
    printf("| %-7s | %-7s | %-30s |\n",
           "Codigo", "Ano", "Descricao");
    printf("======================================================\n");


    for (int i = 0; i < total_eleicoes; i++) {
        printf("| %-7d | %-7d | %-30s |\n", eleicoes[i]->codigo_uf, eleicoes[i]->ano,
            eleicoes[i]->descricao);
    }
    printf("======================================================\n");
}

void mostrarEleicao(int total_eleicoes) {

    if (total_eleicoes == 0) {
        printf("Nao ha eleicoes cadastradas\n");
        return;
    }

    int codigo_uf;
    int ano;

    printf("Digite o codigo da UF: ");
    scanf("%d", &codigo_uf);
    limparBuffer();
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    int encontrado = -1;
     for (int i = 0; i < total_eleicoes; i++) {
         if (eleicoes[i]->codigo_uf == codigo_uf && eleicoes[i]->ano == ano) {
             encontrado = i;
             break;
         }
     }

    if (encontrado == -1) {
        printf("Nao ha uma eleicao cadastrada com essa configuracao\n");
        return;
    }

    printf("======================================================\n");
    printf("| %-7s | %-7s | %-30s |\n",
           "Codigo", "Ano", "Descricao");
    printf("======================================================\n");

    printf("| %-7d | %-7d | %-30s |\n", eleicoes[encontrado]->codigo_uf, eleicoes[encontrado]->ano,
        eleicoes[encontrado]->descricao);
    printf("======================================================\n");

}

void alterarEleicao(int num_eleicoes) {

    int codigo_uf;
    int ano;
    int opcao_alterar_uf;

    printf("Digite o codigo da eleicao a ser alterada: ");
    scanf("%d", &codigo_uf);
    limparBuffer();
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    limparBuffer();
    for (int i = 0; i < num_eleicoes; i++) {
        if (eleicoes[i]->codigo_uf == codigo_uf && eleicoes[i]->ano == ano) {
            do {
                FILE *feleicao = fopen("eleicao.data", "rb+");
                if (feleicao == NULL) {
                    printf("Erro ao abrir arquivo para alterar\n");
                    return;
                }
                printf("Gostaria de alterar a descricao da eleicao?\n");
                printf("1. Sim (atual: %s)\n", eleicoes[i]->descricao);
                printf("0. Nao\n");
                scanf("%d", &opcao_alterar_uf);
                limparBuffer();
                switch (opcao_alterar_uf) {
                    case 1:
                        char nova_descricao[50];
                        printf("Nova descricao da eleicao: ");
                        lerNaoObrigatorio(nova_descricao, sizeof(nova_descricao));
                        strcpy(eleicoes[i]->descricao, nova_descricao);
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
                        printf("Localizacao da eleicao alterada!\n");
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        printf("Digite outra opcao\n");
                        break;
                }
            } while (opcao_alterar_uf != 0);

            return;
        }
    }
    printf("Nao existe eleicao com essa configuracao\n");
}

int verificarAnoeCodigo(int codigo_uf, int ano, int num_eleicoes) {

    for (int i = 0; i < num_eleicoes; i++) {
        if (eleicoes[i]->codigo_uf == codigo_uf && eleicoes[i]->ano == ano)
            return 1;
    }
    return 0;
}

void excluirEleicao(int *num_eleicoes, int *num_candidatos, int *num_votos, int *num_comparecimentos) {

    int codigo_uf;
    int ano;

    printf("Digite o codigo da eleicao que deseja excluir: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    printf("Digite o ano da eleicao que deseja excluir: ");
    scanf("%d", &ano);
    limparBuffer();

    int encontrado = -1;

    for (int i = 0; i < *num_eleicoes; i++) {
        if (eleicoes[i] != NULL && eleicoes[i]->codigo_uf == codigo_uf && eleicoes[i]->ano == ano) {
            Eleicao e = *eleicoes[i];
            free(eleicoes[i]);
            eleicoes[i] = NULL;
            encontrado = i;
            exclusaoCandidatoPelaEleicao(num_candidatos, num_votos, num_comparecimentos, e);
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe eleicao com essa configuracao\n");
        return;
    }

    for (int i = encontrado; i < *num_eleicoes - 1; i++) {
        eleicoes[i] = eleicoes[i + 1];
    }

    eleicoes[*num_eleicoes - 1] = NULL;
    (*num_eleicoes)--;


    FILE *feleicao = fopen("eleicao.data", "wb+");
    if (feleicao == NULL) {
        printf("erro ao abrir arquivo\n");
        return;
    }
    for (int i = 0; i < *num_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
        }
    }

    fclose(feleicao);
    printf("Eleicao removida!\n");
}

void exclusaoCandidatoPelaEleicao(int *num_candidatos, int *num_votos, int *num_comparecimentos, Eleicao e) {

        int encontrado = -1;
        for (int i = 0; i < *num_candidatos; i++) {
            if (candidatos[i] != NULL && e.codigo_uf == candidatos[i]->codigo_uf && e.ano == candidatos[i]->ano) {

                Candidato c = *candidatos[i];
                free(candidatos[i]);
                candidatos[i] = NULL;

                encontrado = i;

                for (int j = encontrado; j < *num_candidatos - 1; j++) {
                    candidatos[j] = candidatos[j + 1];
                }

                candidatos[*num_candidatos - 1] = NULL;

                (*num_candidatos)--;
                i--;
                exclusaoVotosEComparecimentos(num_votos, num_comparecimentos, c);
            }
        }

        if (encontrado == -1) {
            return;
        }

        FILE *fcandidato = fopen("candidatos.data", "wb+");

        for (int i = 0; i < *num_votos; i++) {
            if (candidatos[i] != NULL) {
                fwrite(candidatos[i], sizeof(Candidato), 1, fcandidato);
            }
        }

        fclose(fcandidato);
}