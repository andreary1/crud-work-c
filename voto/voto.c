#include "voto.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../eleicao/eleicao.h"
#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"
#include "../comparecimento/comparecimento.h"
#include "../pessoa/pessoa.h"

extern Voto **votos;
extern Comparecimento **comparecimentos;
extern UF **ufs;
extern Candidato **candidatos;
extern Eleicao **eleicoes;
extern Pessoa **pessoas;

int carregarVotos(int *capacidade_votos) {
    FILE *fvoto = fopen("votos.data", "rb+");
    if (fvoto == NULL) return 0;

    fseek(fvoto, 0, SEEK_END);
    long int num_votos = ftell(fvoto);
    num_votos /= sizeof(Voto);

    while (num_votos >= *capacidade_votos) {
        *capacidade_votos *= 2;
    }

    votos = realloc(votos, *capacidade_votos * sizeof(Voto *));
    if (votos == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 0;
    }

    fseek(fvoto, 0, SEEK_SET);
    for (int i = 0; i < num_votos; i++) {
        Voto *voto = (Voto *)malloc(sizeof(Voto));
        if (voto == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(voto, sizeof(Voto), 1, fvoto);

        votos[i] = voto;
    }

    for (int i = num_votos; i < *capacidade_votos; i++) {
        votos[i] = NULL;
    }

    fclose(fvoto);
    return num_votos;
}

void liberarVotos(int capacidade_votos) {
    for (int i = 0; i < capacidade_votos; i++) {
        if (votos[i] != NULL) {
            free(votos[i]);
            votos[i] = NULL;
        }
    }
    free(votos);
    votos = NULL;
}

int verificarVoto(int ano, char cpf[]) {
    FILE *fvoto = fopen("votos.data", "rb+");
    FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");

    Voto voto;
    Comparecimento comp;
    while (fread(&voto, sizeof(Voto), 1, fvoto) && fread(&comp, sizeof(Comparecimento), 1, fcomparecimento)) {
        if (voto.ano == ano && strcmp(comp.CPF, cpf) == 0) {
            return 1;
        }
    }

    fclose(fvoto);
    fclose(fcomparecimento);
    return 0;
}

void inserirVoto(int *num_votos, int *capacidade_votos, int *num_comparecimentos, int *capacidade_comp,
    int num_pessoas, int num_eleicoes, int num_candidatos) {
    if (*num_votos >= *capacidade_votos || *num_comparecimentos >= *capacidade_comp) {
        *capacidade_comp *= 2;
        *capacidade_votos *= 2;
        votos = realloc(votos, *capacidade_votos * sizeof(Voto *));
        comparecimentos = realloc(comparecimentos, *capacidade_comp * sizeof(Comparecimento *));
        if (votos == NULL || comparecimentos == NULL) {
            printf("Erro na realocacao de memoria\n");
            return;
        }
        for (int i = *num_comparecimentos; i < *capacidade_comp; i++) {
            comparecimentos[i] = NULL;
        }
        for (int i = *num_votos; i < *capacidade_votos; i++) {
            votos[i] = NULL;
        }
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que esse voto foi computado: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano em que esse voto foi computado: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano, num_eleicoes)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    int numero;
    printf("Digite o numero do candidato que foi votado: ");
    scanf("%d", &numero);
    limparBuffer();

    if (!verificarCandidato(numero, ano, codigo_uf, num_candidatos)) {
        printf("Nao existe candidato cadastrado com essa configuracao\n");
        return;
    }

    char cpf[30];
    printf("Digite o CPF do eleitor: ");
    ler(cpf, sizeof(cpf));

    if (!verificarCPF(cpf, num_pessoas)) {
        printf("Esse CPF nao foi cadastrado\n");
        return;
    }

    if (verificarVoto(ano, cpf)) {
        printf("Essa pessoa ja votou nesse ano\n");
        return;
    }

    if (!verificarIdade(cpf, ano, num_pessoas)) {
        printf("Essa pessoa nao tem idade para votar esse ano\n");
        return;
    }

    time_t horavoto;
    time(&horavoto);
    struct tm *informacoestempo = localtime(&horavoto);

    char data_formatada[20];
    strftime(data_formatada, sizeof(data_formatada), "%d/%m %H:%M:%S", informacoestempo);

    votos[*num_votos] = (Voto *)malloc(sizeof(Voto));
    comparecimentos[*num_comparecimentos] = (Comparecimento *)malloc(sizeof(Comparecimento));

    if (votos[*num_votos] == NULL) {
        printf("Erro ao alocar memória para novo voto.\n");
        return;
    }
    if (comparecimentos[*num_comparecimentos] == NULL) {
        printf("Erro ao alocar memoria para novo comparecimento.\n");
        return;
    }

    votos[*num_votos]->codigo_uf = codigo_uf;
    votos[*num_votos]->ano = ano;
    votos[*num_votos]->numero_candidato = numero;
    strcpy(votos[*num_votos]->data_hora, data_formatada);

    comparecimentos[*num_comparecimentos]->codigo_uf = codigo_uf;
    comparecimentos[*num_comparecimentos]->ano = ano;
    strcpy(comparecimentos[*num_comparecimentos]->CPF, cpf);

    FILE *fvoto = fopen("votos.data", "rb+");
    if (fvoto != NULL) {
        fseek(fvoto, 0, SEEK_END);
        fwrite(votos[*num_votos], sizeof(Voto), 1, fvoto);
        fclose(fvoto);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        free(votos[*num_votos]);
        free(comparecimentos[*num_comparecimentos]);
        votos[*num_votos] = NULL;
        comparecimentos[*num_comparecimentos] = NULL;
        return;
    }

    FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");
    if (fcomparecimento != NULL) {
        fseek(fcomparecimento, 0, SEEK_END);
        fwrite(comparecimentos[*num_comparecimentos], sizeof(Comparecimento), 1, fcomparecimento);
        fclose(fcomparecimento);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        free(votos[*num_votos]);
        free(comparecimentos[*num_comparecimentos]);
        votos[*num_votos] = NULL;
        comparecimentos[*num_comparecimentos] = NULL;
        return;
    }

    (*num_votos)++;
    (*num_comparecimentos)++;

    printf("Voto e informacoes do comparecimento adicionados!\n");
}

void mostrarVotosPorCandidato(int num_votos, int num_ufs, int num_candidatos, int num_eleicoes) {

    int codigo_uf;
    printf("Digite o codigo da UF dessa eleicao: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano dessa eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano, num_eleicoes)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] != NULL && ufs[i]->codigo == codigo_uf) {
            printf("--------Eleicao %s %d--------\n", ufs[i]->descricao, ano);
            break;
        }
    }

    for (int i = 0; i < num_candidatos; i++) {
        int num = 1;
        if (candidatos[i]->codigo_uf != codigo_uf || candidatos[i]->ano != ano)
            continue;
        printf("Votos para o candidato numero %d:\n", candidatos[i]->numero);
        for (int j = 0; j < num_votos; j++) {
            if (votos[j]->numero_candidato == candidatos[i]->numero &&
                votos[j]->codigo_uf == codigo_uf &&
                votos[j]->ano == ano) {
                printf("%d. data e hora: %s\n", num, votos[j]->data_hora);
                num++;
            }
        }
    }

}

void mostrarTodosOsVotos(int num_votos, int num_ufs, int num_eleicoes) {

    if (num_votos == 0) {
        printf("Nenhum voto registrado.\n");
        return;
    }

    printf ("=================VOTOS REGISTRADOS ATE O MOMENTO=================\n");
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] == NULL) continue;
        printf("---- eleicoes %s (%s) ----\n", ufs[i]->descricao, ufs[i]->sigla);
        for (int j = 0; j < num_eleicoes; j++) {
            if (eleicoes[j] == NULL) continue;
            if (eleicoes[j]->codigo_uf == ufs[i]->codigo) {
                int num = 1;
                printf("Ano %d:\n", eleicoes[j]->ano);
                for (int k = 0; k < num_votos; k++) {
                    if (votos[k] == NULL) continue;
                    if (eleicoes[j]->ano == votos[k]->ano && votos[k]->codigo_uf == eleicoes[j]->codigo_uf) {
                        printf("%d. data e hora: %s | numero votado: %d\n", num, votos[k]->data_hora, votos[k]->numero_candidato);
                        num++;
                    }
                }
            }
        }
    }
    printf ("=================================================================\n");
}

void contagemDeVotos(int num_votos, int num_candidatos, int num_eleicoes) {

    int codigo_uf;
    printf("Digite o codigo da UF em que ocorreu essa eleicao: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano em que ocorreu essa eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano, num_eleicoes)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    int num_cand_eleicao_atual = 0;
    for (int i = 0; i < num_candidatos; i++) {
        if (candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
            num_cand_eleicao_atual++;
        }
    }

    if (num_cand_eleicao_atual == 0) {
        printf("Nao ha candidatos cadastrados nessa eleicao\n");
        return;
    }

    Candidato cand_eleicao_atual[num_cand_eleicao_atual];
    int votos_para_cada[num_cand_eleicao_atual];

    int indice = 0;
    for (int i = 0; i < num_candidatos; i++) {
        if (candidatos[i]->codigo_uf == codigo_uf && candidatos[i]->ano == ano) {
            cand_eleicao_atual[indice] = *candidatos[i];
            votos_para_cada[indice] = 0;
            indice++;
        }
    }

    for (int i = 0; i < num_cand_eleicao_atual; i++) {
        for (int j = 0; j < num_votos; j++) {
            if (votos[j] != NULL && votos[j]->ano == cand_eleicao_atual[i].ano && votos[j]->codigo_uf == cand_eleicao_atual[i].codigo_uf
                && votos[j]->numero_candidato == cand_eleicao_atual[i].numero) {
                votos_para_cada[i]++;
            }
        }
    }

    for (int i = 0; i < num_cand_eleicao_atual; i++) {
        for (int j = i + 1; j < num_cand_eleicao_atual; j++) {
            if (votos_para_cada[i] < votos_para_cada[j]) {
                const int aux = votos_para_cada[i];
                votos_para_cada[i] = votos_para_cada[j];
                votos_para_cada[j] = aux;

                const Candidato c = cand_eleicao_atual[i];
                cand_eleicao_atual[i] = cand_eleicao_atual[j];
                cand_eleicao_atual[j] = c;
            }
        }
    }

    printf("Contagem de votos para eleicao %d - UF %d:\n", ano, codigo_uf);
    for (int i = 0; i < num_cand_eleicao_atual; i++) {
        printf("Candidato %d - %d votos\n", cand_eleicao_atual[i].numero, votos_para_cada[i]);
    }

    if (num_cand_eleicao_atual > 1 &&
        votos_para_cada[0] == votos_para_cada[1]) {
        printf("Empate na eleicao entre candidatos com %d votos\n", votos_para_cada[0]);
    } else {
        printf("O candidato numero %d ganhou a eleicao com %d votos!\n",
        cand_eleicao_atual[0].numero, votos_para_cada[0]);
    }

}

int verificarIdade(char cpf[], int ano_eleicao, int num_pessoas) {

    char data_nasc[10];

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->cpf, cpf) == 0) {
            strcpy(data_nasc, pessoas[i]->data_nascimento);
        }
    }

    if (strlen(data_nasc) < 10)
        return 0;

    int ano_nasc = atoi(&data_nasc[6]);

    if (ano_eleicao - ano_nasc >= 16) {
        return 1;
    }

    return 0;
}
