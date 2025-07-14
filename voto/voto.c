#include "voto.h"
#include <stdlib.h>
#include <stdio.h>

#include "../eleicao/eleicao.h"
#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"

int carregarVotos(Voto *votos[], int total_votos) {
    FILE *fvoto = fopen("votos.data", "rb+");
    if (fvoto == NULL) return 0;

    fseek(fvoto, 0, SEEK_END);
    long int num_votos = ftell(fvoto);
    num_votos /= sizeof(Voto);

    for (int i = 0; i < total_votos; i++) {
        votos[i] = NULL;
    }
    fseek(fvoto, 0, SEEK_SET);
    for (int i = 0; i < num_votos; i++) {
        Voto *voto = (Voto *)malloc(sizeof(Voto));
        fread(voto, sizeof(Voto), 1, fvoto);

        votos[i] = voto;
    }

    fclose(fvoto);
    return num_votos;
}

void liberarVotos(Voto *votos[], int num_votos) {
    for (int i = 0; i < num_votos; i++) {
        if (votos[i] != NULL) {
            free(votos[i]);
            votos[i] = NULL;
        }
    }
};

void inserirVoto(Voto *votos[], int *num_votos) {
    if (*num_votos >= 1000) {
        printf("maximo de votos atingido\n");
        return;
    }

    votos[*num_votos] = (Voto *)malloc(sizeof(Voto));
    if (votos[*num_votos] == NULL) {
        printf("Erro ao alocar memória para novo Voto.\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que esse voto foi computado: ");
    scanf("%d", &codigo_uf);

    int ano;
    printf("Digite o ano em que esse voto foi computado: ");
    scanf("%d", &ano);

    if (!verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    int numero;
    printf("Digite o numero do candidato que foi votado: ");
    scanf("%d", &numero);

    if (!verificarNumero(numero)) {
        printf("Nao existe candidato cadastrado com esse numero\n");
        return;
    }

    char data_hora[25];
    printf("Digite a data e a hora em que esse voto foi computado: ");
    ler(data_hora, sizeof(data_hora));

    FILE *fvoto= fopen("votos.data", "rb+");
    if (fvoto != NULL) {
        fseek(fvoto, 0, SEEK_END);
        fwrite(votos[*num_votos], sizeof(Voto), 1, fvoto);
        fclose(fvoto);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }

    (*num_votos)++;
    printf("Voto adicionado!\n");
}