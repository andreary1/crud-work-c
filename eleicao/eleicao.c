#include "../eleicao/eleicao.h"
#include <stdio.h>
#include <stdlib.h>
#include "../UF/uf.h"

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
    for (int i = 0; i < num_eleicoes; i++) {
        Eleicao *eleicao = (Eleicao *)malloc(sizeof(Eleicao));
        fread(eleicao, sizeof(Eleicao), 1, feleicao);

        eleicoes[i] = eleicao;
    }

    fclose(feleicao);
    return num_eleicoes;
}

void liberarEleicoes(Eleicao *eleicoes[], int total_eleicoes) {
    for (int i = 0; i < total_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            free(eleicoes[i]);
            eleicoes[i] = NULL;
        }
    }
};

void menuEleicao(Eleicao *eleicoes[], int *num_eleicoes) {
    int opcao_eleicao;
    do {
        printf("----------OPCOES DE ELEICOES---------\n");
        printf("1. Inserir Eleicao\n");
        printf("2. Alterar Eleicao\n");
        printf("3. Excluir Eleicao\n");
        printf("4. Mostrar dados de todas as eleicoes\n");
        printf("5. Mostrar dados de uma eleicao\n");
        printf("0. Sair\n");
        printf("-------------------------------------\n");
        scanf("%d", &opcao_eleicao);
        limparBuffer();
        switch (opcao_eleicao) {
            case 1:
                inserirEleicao(eleicoes, num_eleicoes);
                break;
            case 2:
                alterarEleicao(eleicoes, *num_eleicoes);
                break;
            case 3:
                excluirEleicao(eleicoes, num_eleicoes);
                break;
            case 4:
                mostrarDadosDasEleicoes(eleicoes, *num_eleicoes);
                break;
            case 5:
                mostrarEleicao(eleicoes, *num_eleicoes);
                break;
            case 0:
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_eleicao != 0);
}


void inserirEleicao(Eleicao *eleicoes[], int *num_eleicoes) {

    if (*num_eleicoes >= 100) {
        printf("maximo de Eleicoes atingido\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que ocorreu a eleicao: ");

    scanf("%d", &codigo_uf);
    limparBuffer();
    if (!verificarCodigo(codigo_uf)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    limparBuffer();

    if (verificarAnoeCodigo(codigo_uf, ano)) {
        printf("Ja existe uma eleicao com essa configuracao\n");
        return;
    }

    eleicoes[*num_eleicoes] = (Eleicao *)malloc(sizeof(Eleicao));
    if (eleicoes[*num_eleicoes] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
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

void mostrarDadosDasEleicoes(Eleicao *eleicoes[], int total_eleicoes) {
    if (total_eleicoes == 0) {
        printf("Nao ha eleicoes cadastradas\n");
        return;
    }

    for (int i = 0; i < total_eleicoes; i++) {
        printf("codigo da UF: %d | ano da eleicao: %d | descricao da eleicao: %s\n", eleicoes[i]->codigo_uf, eleicoes[i]->ano,
            eleicoes[i]->descricao);
    }
}

void mostrarEleicao(Eleicao *eleicoes[], int total_eleicoes) {

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

    printf("codigo da UF: %d | ano da eleicao: %d | descricao da eleicao: %s\n", eleicoes[encontrado]->codigo_uf,
        eleicoes[encontrado]->ano, eleicoes[encontrado]->descricao);

}

void alterarEleicao(Eleicao *eleicoes[], int num_eleicoes) {

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
                printf("O que gostaria de alterar nessa Eleicao?\n");
                printf("1. Codigo (atual: %d)\n", eleicoes[i]->codigo_uf);
                printf("2. Descricao (atual: %s)\n", eleicoes[i]->descricao);
                printf("3. Ano (atual: %d)\n", eleicoes[i]->ano);
                printf("0. Nada\n");
                scanf("%d", &opcao_alterar_uf);
                limparBuffer();
                switch (opcao_alterar_uf) {
                    case 1:
                        int novo_codigo_uf;
                        printf("Digite o codigo da UF a qual pertence essa eleicao ou 0 para sair: ");
                        do {
                            scanf("%d", &novo_codigo_uf);
                            limparBuffer();
                        } while (!verificarCodigo(novo_codigo_uf));
                        if (verificarAnoeCodigo(novo_codigo_uf, eleicoes[i]->ano)) {
                            printf("Nao e possivel alterar, pois ja existe uma eleicao com essa configuracao\n");
                            return;
                        }
                        eleicoes[i]->codigo_uf = novo_codigo_uf;
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
                        printf("Localizacao da eleicao alterada!\n");
                        break;
                    case 2:
                        printf("Digite a nova descricao da Eleicao: ");
                        lerNaoObrigatorio(eleicoes[i]->descricao, sizeof(eleicoes[i]->descricao));
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
                        printf("Descricao da eleicao alterada!\n");
                        fclose(feleicao);
                        break;
                    case 3:
                        int novo_ano;
                        printf("Digite o ano em que essa eleicao foi realizada: ");
                        scanf("%d", &novo_ano);
                        limparBuffer();
                        if (verificarAnoeCodigo(eleicoes[i]->codigo_uf, novo_ano)) {
                            printf("Nao e possivel alterar, pois ja existe uma eleicao com essa configuracao\n");
                            return;
                        }
                        eleicoes[i]->ano = novo_ano;
                        fseek(feleicao, i * sizeof(Eleicao), SEEK_SET);
                        fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
                        printf("Ano da Eleicao alterado!\n");
                        fclose(feleicao);
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

int verificarAnoeCodigo(int codigo_uf, int ano) {
    int ano_e_codigo_existe = 0;

    FILE *feleicao = fopen("eleicao.data", "rb+");
    Eleicao eleicao;
    while (fread(&eleicao, sizeof(Eleicao), 1, feleicao) == 1) {
        if (eleicao.codigo_uf == codigo_uf && eleicao.ano == ano)
            ano_e_codigo_existe++;
    }
    fclose(feleicao);
    return ano_e_codigo_existe;
}

void excluirEleicao(Eleicao *eleicoes[], int *total_eleicoes) {

    int codigo_uf;
    int ano;

    printf("Digite o codigo da eleicao que deseja excluir: ");
    scanf("%d", &codigo_uf);
    limparBuffer();
    printf("Digite o ano da eleicao que deseja excluir: ");
    scanf("%d", &ano);
    limparBuffer();

    int encontrado = -1;

    for (int i = 0; i < *total_eleicoes; i++) {
        if (eleicoes[i] != NULL && eleicoes[i]->codigo_uf == codigo_uf && eleicoes[i]->ano == ano) {
            free(eleicoes[i]);
            eleicoes[i] = NULL;
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe eleicao com essa configuracao\n");
        return;
    }

    for (int i = 0; i < *total_eleicoes - 1; i++) {
        eleicoes[i] = eleicoes[i + 1];
    }

    eleicoes[*total_eleicoes - 1] = NULL;
    (*total_eleicoes)--;


    FILE *feleicao = fopen("eleicao.data", "wb+");
    if (feleicao == NULL) {
        printf("erro ao abrir arquivo\n");
        return;
    }
    for (int i = 0; i < *total_eleicoes; i++) {
        if (eleicoes[i] != NULL) {
            fwrite(eleicoes[i], sizeof(Eleicao), 1, feleicao);
        }
    }

    fclose(feleicao);
    printf("Eleicao removida!\n");
}