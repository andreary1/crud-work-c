#include "voto.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../eleicao/eleicao.h"
#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"
#include "../comparecimento/comparecimento.h"
#include "../pessoa/pessoa.h"

void menuVotos(Voto *votos[], int *num_votos, UF *ufs[], int *num_ufs, Candidato *candidatos[], int *num_candidatos,
               Comparecimento *comparecimentos[], int *num_comparecimentos) {
    char opcao_voto;
    do {
        printf("-------------------OPCOES PARA VOTO-------------------\n");
        printf("1. Inserir Voto\n");
        printf("2. Mostrar todos os votos por candidato de uma eleicao\n");
        printf("3. Mostrar todos os votos das eleicoes\n");
        printf("0. Sair\n");
        printf("------------------------------------------------------\n");
        scanf("%c", &opcao_voto);
        limparBuffer();
        switch (opcao_voto) {
            case '1':
                inserirVoto(votos, num_votos, comparecimentos, num_comparecimentos);
                break;
            case '2':
                mostrarVotosPorCandidato(votos, *num_votos, ufs, *num_ufs, candidatos, *num_candidatos);
                break;
            case '3':
                //mostrarTodosOsVotos(votos, *num_votos, ufs, *num_ufs, candidatos, *num_candidatos)
                break;
            case '0':
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_voto != '0');
}

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

void inserirVoto(Voto *votos[], int *num_votos, Comparecimento *comparecimentos[], int *num_comparecimentos) {
    if (*num_votos >= 1000 || *num_comparecimentos >= 1000) {
        printf("maximo de votos e comparecimentos atingido\n");
        return;
    }

    votos[*num_votos] = (Voto *)malloc(sizeof(Voto));
    comparecimentos[*num_comparecimentos] = (Comparecimento *)malloc(sizeof(Comparecimento));

    if (votos[*num_votos] == NULL) {
        printf("Erro ao alocar memória para novo voto.\n");
        return;
    }
    if (comparecimentos[*num_comparecimentos] == NULL) {
        printf("Erro ao alocar memória para novo comparecimento.\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que esse voto foi computado: ");
    scanf("%d", &codigo_uf);
    limparBuffer();

    int ano;
    printf("Digite o ano em que esse voto foi computado: ");
    scanf("%d", &ano);
    limparBuffer();

    if (!verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Nao existe eleicao cadastrada com essa configuracao\n");
        return;
    }

    int numero;
    printf("Digite o numero do candidato que foi votado: ");
    scanf("%d", &numero);
    limparBuffer();

    if (!verificarNumero(numero)) {
        printf("Nao existe candidato cadastrado com esse numero\n");
        return;
    }

    char cpf[30];
    printf("Digite o CPF do eleitor: ");
    ler(cpf, sizeof(cpf));

    if (!verificarCPF(cpf)) {
        printf("Esse CPF nao foi cadastrado\n");
        return;
    }

    char data_hora[25];
    printf("Digite a data e a hora em que esse voto foi computado: ");
    ler(data_hora, sizeof(data_hora));

    votos[*num_votos]->codigo_uf = codigo_uf;
    votos[*num_votos]->ano = ano;
    votos[*num_votos]->numero_candidato = numero;
    strcpy(votos[*num_votos]->data_hora, data_hora);

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
        return;
    }

    (*num_votos)++;
    (*num_comparecimentos)++;

    printf("Voto e informacoes do comparecimento adicionados!\n");
}

void mostrarVotosPorCandidato(Voto *votos[], int num_votos, UF *ufs[], int num_ufs, Candidato *candidatos[], int num_candidatos) {

    int codigo_uf;
    printf("Digite o codigo da UF dessa eleicao: ");
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

//void mostrarTodosOsVotos(Voto *votos[], int num_votos, UF *ufs[], int num_ufs, Candidato *candidatos[], int num_candidatos) {
//
//}