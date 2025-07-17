#include "candidato_eleicao.h"
#include "../UF/uf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../eleicao/eleicao.h"
#include "../pessoa/pessoa.h"
#include "../comparecimento/comparecimento.h"
#include "../voto/voto.h"

extern Candidato **candidatos;
extern UF **ufs;
extern Eleicao **eleicoes;
extern Voto **votos;
extern Comparecimento **comparecimentos;

int carregarCandidatos(int *capacidade_candidatos) {

    FILE *fcandidato = fopen("candidatos.data", "rb+");
    if (fcandidato == NULL) return 0;

    fseek(fcandidato, 0, SEEK_END);
    long int num_candidatos = ftell(fcandidato);
    num_candidatos /= sizeof(Candidato);

    while (num_candidatos > *capacidade_candidatos) {
        *capacidade_candidatos *= 2;
    }

    candidatos = (Candidato **)malloc(*capacidade_candidatos * sizeof(Candidato *));
    if (candidatos == NULL) {
        printf("erro na alocacao de memoria\n");
        return 0;
    }

    fseek(fcandidato, 0, SEEK_SET);
    for (int i = 0; i < num_candidatos; i++) {
        Candidato *candidato = (Candidato *)malloc(sizeof(Candidato));
        if (candidato == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(candidato, sizeof(Candidato), 1, fcandidato);

        candidatos[i] = candidato;
    }

    for (int i = num_candidatos; i < *capacidade_candidatos; i++) {
        candidatos[i] = NULL;
    }

    fclose(fcandidato);
    return num_candidatos;
}

void liberarCandidatos(int capacidade_cand) {
    for (int i = 0; i < capacidade_cand; i++) {
        if (candidatos[i] != NULL) {
            free(candidatos[i]);
            candidatos[i] = NULL;
        }
    }
    free(candidatos);
    candidatos = NULL;
}

int verificarCandidato(int numero, int ano, int codigo_uf) {

        FILE *fcandidato = fopen("candidatos.data", "rb+");
        Candidato candidato;
        while (fread(&candidato, sizeof(Candidato), 1, fcandidato) == 1) {
            if (candidato.numero == numero && candidato.codigo_uf == codigo_uf && candidato.ano == ano ) {
                fclose(fcandidato);
                return 1;
            }
        }
        fclose(fcandidato);
        return 0;

}

void inserirCandidato(int *total_cand, int *capacidade_cand) {
    if (*total_cand >= *capacidade_cand) {
        *capacidade_cand *= 2;
        candidatos = realloc(candidatos, *capacidade_cand * sizeof(Candidato *));
        if (candidatos == NULL) {
            printf("Erro na alocacao de memoria\n");
            return;
        }
        for (int i = *total_cand; i < *capacidade_cand; i++) {
            candidatos[i] = NULL;
        }
    }

    int codigo_uf;
    printf("Digite o codigo da UF a qual o candidato pertence: ");

    scanf("%d", &codigo_uf);
    limparBuffer();
    if (!verificarCodigo(codigo_uf)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano em que ele concorreu: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Essa eleicao nao foi cadastrada\n");
        return;
    }

    int numero_candidato;
    printf("Digite o numero desse candidato: ");
    scanf("%d", &numero_candidato);
    limparBuffer();

    if (verificarCandidato(numero_candidato, ano, codigo_uf)) {
        printf("Esse candidato ja foi cadastrado\n");
        return;
    }

    char cpf[30];
    printf("Digite o CPF do candidato: ");
    ler(cpf, sizeof(cpf));

    if (verificarCPF(cpf) == 0) {
        printf("Nao existe pessoa cadastrada com esse CPF\n");
        return;
    }

    candidatos[*total_cand] = (Candidato *)malloc(sizeof(Candidato));
    if (candidatos[*total_cand] == NULL) {
        printf("Erro ao alocar memória para novo candidato.\n");
        return;
    }

    candidatos[*total_cand]->codigo_uf = codigo_uf;
    candidatos[*total_cand]->ano = ano;
    candidatos[*total_cand]->numero = numero_candidato;
    strcpy(candidatos[*total_cand]->CPF, cpf);

    FILE *fcandidato = fopen("candidatos.data", "rb+");
    if (fcandidato != NULL) {
        fseek(fcandidato, 0, SEEK_END);
        fwrite(candidatos[*total_cand], sizeof(Candidato), 1, fcandidato);
        fclose(fcandidato);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }
    (*total_cand)++;

    printf("Candidato adicionado!\n");
}

void excluirCandidato(int *num_candidatos, int *num_votos, int *num_comparecimentos) {

    int codigo_uf;
    printf("Digite o codigo da UF em que o candidato concorreu: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano em que ele concorreu: ");
    scanf("%d", &ano);
    limparBuffer();

    int numero_candidato;
    printf("Digite o numero desse candidato: ");
    scanf("%d", &numero_candidato);
    limparBuffer();

    int encontrado = -1;
    for (int i = 0; i < *num_candidatos; i++) {
        if (candidatos[i] != NULL && codigo_uf == candidatos[i]->codigo_uf && ano == candidatos[i]->ano &&
            numero_candidato == candidatos[i]->numero) {
            free(candidatos[i]);
            candidatos[i] = NULL;
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe candidato cadastrado com essa configuracao\n");
        return;
    }

    for (int i = encontrado; i < *num_candidatos - 1; i++) {
        candidatos[i] = candidatos[i + 1];
    }
    candidatos[*num_candidatos - 1] = NULL;
    (*num_candidatos)--;

    FILE *fcandidato = fopen("candidatos.data", "wb+");
    if (fcandidato == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *num_candidatos; i++) {
        if (candidatos[i] != NULL) {
            fwrite(candidatos[i], sizeof(Candidato), 1, fcandidato);
        }
    }

    fclose(fcandidato);
    printf("Candidato removido!\n");
    exclusaoVotosEComparecimentos(num_votos, num_comparecimentos, codigo_uf, ano, numero_candidato);
}

void exclusaoVotosEComparecimentos(int *num_votos, int *num_comparecimentos, int codigo, int ano, int numero) {

    int encontrado = -1;
    for (int i = 0; i < *num_votos; i++) {
        if (votos[i] != NULL && codigo == votos[i]->codigo_uf && ano == votos[i]->ano &&
            numero == votos[i]->numero_candidato) {

            free(comparecimentos[i]);
            free(votos[i]);

            votos[i] = NULL;
            comparecimentos[i] = NULL;

            encontrado = i;

            for (int j = encontrado; j < *num_votos - 1; j++) {
                votos[j] = votos[j + 1];
                comparecimentos[j] = comparecimentos[j + 1];
            }

            comparecimentos[*num_comparecimentos - 1] = NULL;
            votos[*num_votos - 1] = NULL;

            (*num_comparecimentos)--;
            (*num_votos)--;
        }
    }

    FILE *fvoto = fopen("votos.data", "wb+");
    FILE *fcomparecimento = fopen("comparecimentos.data", "wb+");

    if (fvoto == NULL || fcomparecimento == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    if (encontrado == -1) {
        return;
    }

    for (int i = 0; i < *num_votos; i++) {
        if (votos[i] != NULL && comparecimentos[i] != NULL) {
            fwrite(votos[i], sizeof(Voto), 1, fvoto);
            fwrite(comparecimentos[i], sizeof(Comparecimento), 1, fcomparecimento);
        }
    }

    fclose(fvoto);
    fclose(fcomparecimento);

    printf("Os votos e comparecimentos relacionados a esse candidato tambem foram excluidos\n");
}

void mostrarCandidatosPorUFeAno(int total_cand, int total_ufs) {
    if (total_cand == 0) {
        printf("Nao ha candidatos cadastrados\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que a eleicao concorreu: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano em que a eleicao ocorreu: ");
    scanf("%d", &ano);
    limparBuffer();

    int encontrado = -1;
    for (int i = 0; i < total_cand; i++) {
        if (candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
            encontrado++;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe candidato cadastrado com essa configuracao\n");
        return;
    }

    for (int i = 0; i < total_ufs; i++) {
        if (ufs[i] != NULL && ufs[i]->codigo == codigo_uf) {
            printf("Eleicao %s %d\n", ufs[i]->descricao, ano);
            break;
        }
    }

    for (int i = 0; i < total_cand; i++) {
        if (candidatos[i] != NULL && candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
            printf("numero: %d | CPF: %s\n", candidatos[i]->numero, candidatos[i]->CPF);
        }
    }
}

void mostrarTodosOsCandidatos(int num_candidatos, int num_eleicoes, int num_ufs) {
    if (num_candidatos == 0) {
        printf("Nao ha candidatos cadastrados.\n");
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
                for (int k = 0; k < num_candidatos; k++) {
                    if (candidatos[k] == NULL) continue;
                    if (eleicoes[j]->ano == candidatos[k]->ano && candidatos[k]->codigo_uf == eleicoes[j]->codigo_uf) {
                        printf("%d. Numero: %d\n", num, candidatos[k]->numero);
                        num++;
                    }
                }
            }
        }
    }
}