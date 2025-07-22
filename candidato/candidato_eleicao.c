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

int verificarCandidato(int numero, int ano, int codigo_uf, int num_candidatos) {

        for (int i = 0; i < num_candidatos; i++) {
            if (candidatos[i]->numero == numero && candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano ) {
                return 1;
            }
        }

        return 0;
}

void inserirCandidato(int *num_candidatos, int *capacidade_cand, int num_ufs, int num_pessoas, int num_eleicoes) {
    if (*num_candidatos >= *capacidade_cand) {
        *capacidade_cand *= 2;
        candidatos = realloc(candidatos, *capacidade_cand * sizeof(Candidato *));
        if (candidatos == NULL) {
            printf("Erro na realocacao de memoria\n");
            return;
        }
        for (int i = *num_candidatos; i < *capacidade_cand; i++) {
            candidatos[i] = NULL;
        }
    }

    int codigo_uf;
    printf("Digite o codigo da UF a qual o candidato pertence: ");

    scanf("%d", &codigo_uf);
    limparBuffer();
    if (!verificarCodigo(codigo_uf, num_ufs)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano em que ele concorreu: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano, num_eleicoes)) {
        printf("Essa eleicao nao foi cadastrada\n");
        return;
    }

    int numero_candidato;
    do {
        printf("Digite o numero desse candidato: ");
        scanf("%d", &numero_candidato);
        limparBuffer();
    } while (numero_candidato < 10 || numero_candidato > 100);

    if (verificarCandidato(numero_candidato, ano, codigo_uf, *num_candidatos)) {
        printf("Ja existe candidato cadastrado com essa configuracao\n");
        return;
    }

    char cpf[30];
    printf("Digite o CPF do candidato: ");
    ler(cpf, sizeof(cpf));

    if (!verificarCPF(cpf, num_pessoas)) {
        printf("Nao existe pessoa cadastrada com esse CPF\n");
        return;
    }

    if (verificarCandidatura(cpf, ano, *num_candidatos)) {
        printf("Essa pessoa ja se candidatou esse ano\n");
        return;
    }

    if (!verificarIdade(cpf, ano, *num_candidatos)) {
        printf("Essa pessoa nao tem idade para se candidatar\n");
        return;
    }

    candidatos[*num_candidatos] = (Candidato *)malloc(sizeof(Candidato));
    if (candidatos[*num_candidatos] == NULL) {
        printf("Erro ao alocar memória para novo candidato.\n");
        return;
    }

    candidatos[*num_candidatos]->codigo_uf = codigo_uf;
    candidatos[*num_candidatos]->ano = ano;
    candidatos[*num_candidatos]->numero = numero_candidato;
    strcpy(candidatos[*num_candidatos]->CPF, cpf);

    FILE *fcandidato = fopen("candidatos.data", "rb+");
    if (fcandidato != NULL) {
        fseek(fcandidato, 0, SEEK_END);
        fwrite(candidatos[*num_candidatos], sizeof(Candidato), 1, fcandidato);
        fclose(fcandidato);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        free(candidatos[*num_candidatos]);
        candidatos[*num_candidatos] = NULL;
        return;
    }

    (*num_candidatos)++;
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
    Candidato c;
    for (int i = 0; i < *num_candidatos; i++) {
        if (candidatos[i] != NULL && codigo_uf == candidatos[i]->codigo_uf && ano == candidatos[i]->ano &&
            numero_candidato == candidatos[i]->numero) {
            c = *candidatos[i];
            free(candidatos[i]);
            candidatos[i] = NULL;
            encontrado = i;
            exclusaoVotosEComparecimentos(num_votos, num_comparecimentos, c);
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
}

void exclusaoVotosEComparecimentos(int *num_votos, int *num_comparecimentos, Candidato c) {

    int encontrado = -1;
    for (int i = 0; i < *num_votos; i++) {
        if (votos[i] != NULL && c.codigo_uf == votos[i]->codigo_uf && c.ano == votos[i]->ano &&
            c.numero == votos[i]->numero_candidato) {

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
            i--;
        }
    }

    if (encontrado == -1) {
        return;
    }

    FILE *fvoto = fopen("votos.data", "wb+");
    FILE *fcomparecimento = fopen("comparecimentos.data", "wb+");

    if (fvoto == NULL || fcomparecimento == NULL) {
        printf("Erro ao abrir arquivo\n");
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
}

void mostrarCandidatosPorUFeAno(int total_cand, int total_ufs, int num_eleicoes) {
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

    if (!verificarAnoeCodigo(codigo_uf, ano, num_eleicoes)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    for (int i = 0; i < total_ufs; i++) {
        if (ufs[i] != NULL && ufs[i]->codigo == codigo_uf) {
            printf("---- Candidatos eleicao %s (%d) ----\n", ufs[i]->descricao, ano);
            break;
        }
    }

    int num = 1;
    for (int i = 0; i < total_cand; i++) {
        if (candidatos[i] != NULL && candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
            printf("| %d. numero: %d | CPF: %s |\n", num, candidatos[i]->numero, candidatos[i]->CPF);
            num++;
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
        printf("----Candidatos Eleicoes %s (%s)----\n", ufs[i]->descricao, ufs[i]->sigla);
        for (int j = 0; j < num_eleicoes; j++) {
            if (eleicoes[j] == NULL) continue;
            if (eleicoes[j]->codigo_uf == ufs[i]->codigo) {
                int num = 1;
                printf("Ano %d:\n", eleicoes[j]->ano);
                for (int k = 0; k < num_candidatos; k++) {
                    if (candidatos[k] == NULL) continue;
                    if (eleicoes[j]->ano == candidatos[k]->ano && candidatos[k]->codigo_uf == eleicoes[j]->codigo_uf) {
                        printf("| %d. Numero: %d | CPF : %s |\n", num, candidatos[k]->numero, candidatos[k]->CPF);
                        num++;
                    }
                }
            }
        }
    }
}

int verificarCandidatura(char cpf[], int ano, int num_candidatos) {
    for (int i = 0; i < num_candidatos; i++) {
        if (candidatos[i] != NULL && candidatos[i]->ano == ano && strcmp(candidatos[i]->CPF, cpf) == 0) {
            return 1;
        }
    }
    return 0;
}