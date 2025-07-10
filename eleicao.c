#include "eleicao.h"
#include <stdio.h>
#include <stdlib.h>
#include "uf.h"

int carregarEleicoes(Eleicao *eleicoes[], int total_eleicoes) {
    FILE *feleicao = fopen("eleicao.data", "rb+");
    if (feleicao == NULL) return 0;

    fseek(feleicao, 0, SEEK_END);
    long int num_eleicoes = ftell(feleicao);
    num_eleicoes /= sizeof(Eleicao);

    for (int i = 0; i < total_eleicoes; i++) {
        eleicoes[i] = NULL;
    }
    fseek(feleicao, 0, SEEK_SET);
    for (int i = 0; i < total_eleicoes; i++) {
        Eleicao *eleicao = (Eleicao *)malloc(sizeof(Eleicao));
        fread(eleicao, sizeof(Eleicao), 1, feleicao);

        eleicoes[i] = eleicao;
    }

    fclose(feleicao);
    return num_eleicoes;
}

void menuEleicao(Eleicao *eleicoes[]) {
    int opcao_eleicao;
    printf("----OPCOES DE ELEICOES----\n");
    printf("1. Inserir Eleicao\n");
    printf("2. Alterar Eleicao\n");
    printf("3. Excluir Eleicao\n");
    printf("4. Mostrar dados de todas as eleicoes\n");
    printf("5. Mostrar dados de uma eleicao\n");
    printf("0. Sair\n");
    printf("-------------------------------------\n");
    scanf("%d", &opcao_eleicao);
    switch (opcao_eleicao) {
        case 1:
            inserirEleicao(eleicoes);
            carregarEleicoes(eleicoes, 100);
            break;
        case 2:
            //alterarEleicao(eleicoes, *total_eleicoes);
            break;
        case 3:
            //excluirEleicao(eleicoes, total_eleicoes);
            break;
        case 4:
            //carregarEleicoes(eleicoes);
            //mostrarDadosDasEleicoes(*total_eleicoes);
            break;
        case 5:
            //carregarEleicoes(eleicoes);
            //mostrarEleicao(*total_eleicoes);
            break;
        case 0:
            break;
    }
}

void liberarEleicoes(Eleicao *eleicoes[], int total_eleicoes) {
    for (int i = 0; i < total_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            free(eleicoes[i]);
            eleicoes[i] = NULL;
        }
    }
};

void inserirEleicao(Eleicao *eleicoes[]) {
    FILE *feleicao = fopen("eleicao.data", "rb+");

    fseek(feleicao, 0, SEEK_END);
    long int num_eleicoes = ftell(feleicao);
    num_eleicoes /= sizeof(Eleicao);

    if (num_eleicoes >= 100) {
        printf("maximo de Eleicoes atingido\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que ocorreu a eleicao: ");

    scanf("%d", &codigo_uf);
    if (!verificarCodigo(codigo_uf)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);

    if (verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Ja existe uma eleicao com essa configuracao\n");
        return;
    }

    eleicoes[num_eleicoes] = (Eleicao *)malloc(sizeof(Eleicao));
    if (eleicoes[num_eleicoes] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        fclose(feleicao);
        return;
    }

    eleicoes[num_eleicoes]->codigo_uf = codigo_uf;
    eleicoes[num_eleicoes]->ano = ano;

    printf("Digite a descricao da eleicao: ");
    ler(eleicoes[num_eleicoes]->descricao, sizeof(eleicoes[num_eleicoes]->descricao));

    fseek(feleicao, 0, SEEK_END);
    fwrite(eleicoes[num_eleicoes], sizeof(Eleicao), 1, feleicao);
    fclose(feleicao);
    printf("Eleicao adicionada!\n");
}

void mostrarDadosDasEleicoes(int total_eleicoes) {
    FILE *feleicoes = fopen("eleicao.data", "rb+");
    if (total_eleicoes == 0) {
        printf("Nao ha eleicoes cadastradas\n");
        return;
    }
    Eleicao eleicao;
    while (fread(&eleicao, sizeof(Eleicao), 1, feleicoes)) {
        printf("codigo da UF: %d | ano da eleicao: %d | descricao da eleicao: %s\n", eleicao.codigo_uf, eleicao.ano, eleicao.descricao);
    }
    fclose(feleicoes);
}

void mostrarEleicao(int total_eleicoes) {
    FILE *feleicao = fopen("eleicao.data", "rb+");
    if (total_eleicoes == 0) {
        printf("Nao ha eleicoes cadastradas\n");
        return;
    }

    Eleicao eleicao;

    int codigo_uf;
    int ano;

    int encontrado = 0;
    printf("Digite o codigo da UF: ");
    scanf("%d", &codigo_uf);
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    while (fread(&eleicao, sizeof(Eleicao), 1, feleicao)) {
        if (eleicao.codigo_uf == codigo_uf && eleicao.ano == ano) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Nao ha uma eleicao cadastrada com essa configuracao\n");
        return;
    }

    printf("codigo da UF: %d | ano da eleicao: %d | descricao da eleicao: %s\n", eleicao.codigo_uf, eleicao.ano, eleicao.descricao);

    fclose(feleicao);
}

void alterarEleicao(Eleicao *eleicao, int total_eleicoes) {
    FILE *feleicao = fopen("eleicao.data", "rb+");
    int codigo_uf;
    int ano;
    int opcao_alterar_uf;
    printf("Digite o codigo da eleicao a ser alterada: ");
    scanf("%d", &codigo_uf);
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    for (int i = 0; i < total_eleicoes; i++) {
        if (eleicao[i].codigo_uf == codigo_uf && eleicao[i].ano == ano) {
            do {
                printf("O que gostaria de alterar nessa Eleicao?\n");
                printf("1. Codigo (atual: %d)\n", eleicao[i].codigo_uf);
                printf("2. Descricao (atual: %s)\n", eleicao[i].descricao);
                printf("3. Ano (atual: %d)\n", eleicao[i].ano);
                printf("0. Nada\n");
                scanf("%d", &opcao_alterar_uf);
                switch (opcao_alterar_uf) {
                    case 1:
                        int novo_codigo_uf;
                        printf("Digite o codigo da UF a qual pertence essa eleicao ou 0 para sair: ");
                        do {
                            scanf("%d", &novo_codigo_uf);
                        } while (!verificarCodigo(novo_codigo_uf));
                        if (verificarAnoeCodigo(novo_codigo_uf, eleicao[i].ano)) {
                            printf("Nao e possivel alterar, pois ja existe uma eleicao com essa configuracao\n");
                            return;
                        }
                        eleicao[i].codigo_uf = novo_codigo_uf;
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(&eleicao[i], sizeof(Eleicao), 1, feleicao);
                        printf("Localizacao da eleicao alterada!\n");
                        break;
                    case 2:
                        printf("Digite a nova descricao da Eleicao: ");
                        fflush(stdin);
                        gets(eleicao[i].descricao);
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(&eleicao[i], sizeof(Eleicao), 1, feleicao);
                        printf("Descricao da eleicao alterada!\n");
                        break;
                    case 3:
                        int novo_ano;
                        printf("Digite o ano em que essa eleicao foi realizada: ");
                        scanf("%d", &novo_ano);
                        if (verificarAnoeCodigo(eleicao[i].codigo_uf, novo_ano)) {
                            printf("Nao e possivel alterar, pois ja existe uma eleicao com essa configuracao\n");
                            return;
                        }
                        eleicao[i].ano = novo_ano;
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(&eleicao[i], sizeof(Eleicao), 1, feleicao);
                        printf("Ano da Eleicao alterado!\n");
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        break;
                }
            } while (opcao_alterar_uf != 0);

            fclose(feleicao);
            return;
        }
    }
    printf("Nao existe eleicao com essa configuracao\n");
    fclose(feleicao);
}

int verificarAnoeCodigo(int codigo_uf, int ano) {
    int ano_e_codigo_existe = 0;

    FILE *feleicao = fopen("eleicao.data", "rb+");
    Eleicao *eleicao = (Eleicao *)malloc(sizeof(Eleicao));
    while (fread(eleicao, sizeof(Eleicao), 1, feleicao) == 1) {
        if (eleicao->codigo_uf == codigo_uf && eleicao->ano == ano)
            ano_e_codigo_existe++;
    }
    free(eleicao);
    fclose(feleicao);
    return ano_e_codigo_existe;
}

void excluirEleicao(Eleicao *eleicoes, int *total_eleicoes) {

    int codigo_uf;
    int ano;

    printf("Digite o codigo da eleicao que deseja excluir: ");
    scanf("%d", &codigo_uf);
    printf("Digite o ano da eleicao que deseja excluir: ");
    scanf("%d", &ano);

    int remover = 0;
    int novo_total_eleicoes = 0;

    for (int i = 0; i < *total_eleicoes; i++) {
        if (eleicoes[i].codigo_uf == codigo_uf && eleicoes[i].ano == ano) {
            remover++;
        }
        else {
            eleicoes[novo_total_eleicoes] = eleicoes[i];
            novo_total_eleicoes++;
        }
    }

    if (!remover) {
        printf("Nao existe eleicao com essa configuracao\n");
        return;
    }

    FILE *feleicao = fopen("eleicao.data", "wb+");
    fwrite(eleicoes, sizeof(Eleicao), novo_total_eleicoes, feleicao);
    fclose(feleicao);
    printf("Eleicao removida!\n");

    *total_eleicoes = novo_total_eleicoes;
}

void excluirEleicoesPorUF(int codigo_uf) {

    FILE *feleicao = fopen("eleicao.data", "rb+");

    Eleicao eleicoes[100];

    int novo_total_eleicoes = 0;
    int remover = 0;

    int total= 0;
    while (fread(&eleicoes[total], sizeof(Eleicao), 1 ,feleicao))
        total++;

    fclose(feleicao);

    for (int i = 0; i < total; i++) {
        if (eleicoes[i].codigo_uf == codigo_uf) {
            remover++;
        }
        else {
            eleicoes[novo_total_eleicoes] = eleicoes[i];
            novo_total_eleicoes++;
        }
    }

    if (!remover) {
        return;
    }

    feleicao = fopen("eleicao.data", "wb+");
    fwrite(eleicoes, sizeof(Eleicao), novo_total_eleicoes, feleicao);
    fclose(feleicao);
    printf("Todas as eleicoes associadas a UF foram removidas!\n");
}