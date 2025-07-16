#include "candidato_eleicao.h"
#include "../UF/uf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../eleicao/eleicao.h"
#include "../pessoa/pessoa.h"

extern Candidato **candidatos;
extern UF **ufs;

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

void excluirCandidato(int *total_cand) {

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
    for (int i = 0; i < *total_cand; i++) {
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

    for (int i = encontrado; i < *total_cand - 1; i++) {
        candidatos[i] = candidatos[i + 1];
    }
    candidatos[*total_cand - 1] = NULL;
    (*total_cand)--;

    FILE *fcandidato = fopen("candidatos.data", "wb+");
    if (fcandidato == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *total_cand; i++) {
        if (candidatos[i] != NULL) {
            fwrite(candidatos[i], sizeof(Candidato), 1, fcandidato);
        }
    }

    fclose(fcandidato);
    printf("Candidato removido!\n");
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

void mostrarTodosOsCandidatos(int total_cand, int total_ufs) {
    if (total_cand == 0) {
        printf("Nao ha candidatos cadastrados.\n");
        return;
    }

    // Ordenar candidatos por codigo_uf
    for (int i = 0; i < total_cand - 1; i++) {
        for (int j = 0; j < total_cand - i - 1; j++) {
            if (candidatos[j] == NULL || candidatos[j + 1] == NULL) continue;
            if (candidatos[j]->codigo_uf > candidatos[j + 1]->codigo_uf) {
                Candidato *temp = candidatos[j];
                candidatos[j] = candidatos[j + 1];
                candidatos[j + 1] = temp;
            }
        }
    }

    int codigo_uf_atual = -1;
    int anos_exibidos[100];
    int total_anos = 0;

    for (int i = 0; i < total_cand; i++) {
        if (candidatos[i] == NULL) continue;

        int codigo_uf = candidatos[i]->codigo_uf;
        int ano = candidatos[i]->ano;

        // Se mudou a UF, reinicia anos_exibidos e mostra cabeçalho da UF
        if (codigo_uf != codigo_uf_atual) {
            codigo_uf_atual = codigo_uf;
            total_anos = 0;

            // Cabeçalho da UF
            for (int j = 0; j < total_ufs; j++) {
                if (ufs[j] != NULL && ufs[j]->codigo == codigo_uf) {
                    printf("\n----- %s (%s) -----\n", ufs[j]->descricao, ufs[j]->sigla);
                    break;
                }
            }
        }

        // Verificar se ano já foi exibido para essa UF
        int ano_ja_exibido = 0;
        for (int i = 0; i < total_anos; i++) {
            if (anos_exibidos[i] == ano) {
                ano_ja_exibido = 1;
                break;
            }
        }

        if (!ano_ja_exibido) {
            // Mostrar candidatos desse ano
            printf("---- Ano %d ----\n", ano);
            for (int i = 0; i < total_cand; i++) {
                if (candidatos[i] == NULL) continue;
                if (candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
                    printf("Numero: %d | CPF: %s\n", candidatos[i]->numero, candidatos[i]->CPF);
                }
            }
            anos_exibidos[total_anos++] = ano;
        }
    }
}