#include <stdio.h>
#include "uf.h"
#include "../eleicao/eleicao.h"
#include <stdlib.h>
#include<time.h>
#include <string.h>

void ler(char sentenca[], int tamanho) {
    fflush(stdin);
    fgets(sentenca, tamanho, stdin);
    sentenca[strcspn(sentenca, "\n")] = '\0';
}

void carregarArquivos() {

    FILE *fuf = fopen("uf.data", "rb+");
    FILE *fpessoa = fopen("pessoas.data", "rb+");
    FILE *feleicao = fopen("eleicao.data", "rb+");
    FILE *fcandidato_eleicao = fopen("candidatos.data", "rb+");
    FILE *fvoto = fopen("votos.data", "rb+");
    FILE *fcomparecimento = fopen("comparecimentos.data", "rb+");

    if (fuf == NULL) {
        FILE *fuf = fopen("uf.data", "wb+");
        if (fuf == NULL) {
            printf("erro ao criar arquivo da uf\n");
            return;
        }
    }

    if (fpessoa == NULL) {
        FILE *fpessoa = fopen("pessoas.data", "wb+");
        if (fpessoa == NULL) {
            printf("erro ao criar arquivo das pessoa\n");
            return;
        }
    }

    if (feleicao == NULL) {
        FILE *feleicao = fopen("eleicao.data", "wb+");
        if (feleicao == NULL) {
            printf("erro ao criar arquivo da eleicao\n");
            return;
        }
    }

    if (fcandidato_eleicao == NULL) {
        FILE *fcandidato_eleicao = fopen("candidatos.data", "wb+");
        if (fcandidato_eleicao == NULL) {
            printf("erro ao criar arquivo dos candidatos\n");
            return;
        }
    }

    if (fvoto == NULL) {
        FILE *fvoto = fopen("votos.data", "wb+");
        if (fvoto == NULL) {
            printf("erro ao criar arquivo da voto\n");
            return;
        }
    }

    if (fcomparecimento == NULL) {
        FILE *fcomparecimento = fopen("comparecimentos.data", "wb+");
        if (fcomparecimento == NULL) {
            printf("erro ao criar arquivo do comparecimento\n");
            return;
        }
    }
}

void menuUF(UF *ufs[], int *num_ufs, int *num_eleicoes) {
    int opcao_uf;
    do {
        printf("----OPCOES PARA UNIDADES FEDERATIVAS----\n");
        printf("1. Inserir UF\n");
        printf("2. Alterar UF\n");
        printf("3. Excluir UF\n");
        printf("4. Mostrar dados de todas as UFs\n");
        printf("5. Mostrar dados de uma UF\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        scanf("%d", &opcao_uf);
        switch (opcao_uf) {
            case 1:
                adicionarUF(ufs, num_ufs);
                break;
            case 2:
                alterarUF(ufs, *num_ufs);
                break;
            case 3:
                excluirUF(ufs, num_ufs, num_eleicoes);
                break;
            case 4:
                mostrarDadosDasUFs(ufs, *num_ufs);
                break;
            case 5:
                mostrarUF(ufs, *num_ufs);
                break;
            case 0:
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\n");
                printf("digite outra opcao\n");
                break;
        }
    } while (opcao_uf != 0);
}

int verificarCodigo(int codigo_uf) {
    int codigo_existe = 0;

    FILE *fuf = fopen("uf.data", "rb+");
    UF uf;
    while (fread(&uf, sizeof(UF), 1, fuf) == 1) {
        if (uf.codigo ==  codigo_uf)
            codigo_existe++;
    }
    fclose(fuf);
    return codigo_existe;
}

int carregarUFs(UF *ufs[], int total_ufs) {

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf == NULL) return 0;

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    for (int i = 0; i < total_ufs; i++) {
        ufs[i] = NULL;
    }
    fseek(fuf, 0, SEEK_SET);
    for (int i = 0; i < num_ufs; i++) {
        UF *uf = (UF *)malloc(sizeof(UF));
        fread(uf, sizeof(UF), 1, fuf);

        ufs[i] = uf;
    }

    fclose(fuf);
    return num_ufs;
}

void liberarUFs(UF *ufs[], int total_uf) {
    for (int i = 0; i < total_uf; i++) {
        if (ufs[i] != NULL) {
            free(ufs[i]);
            ufs[i] = NULL;
        }
    }
}

void adicionarUF(UF *ufs[], int *num_ufs) {

    if (*num_ufs >= 35) {
        printf("maximo de UFs atingido\n");
        return;
    }

    ufs[*num_ufs] = (UF *)malloc(sizeof(UF));
    if (ufs[*num_ufs] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        return;
    }

    srand(time(NULL));
    do {
        ufs[*num_ufs]->codigo = rand() % 100 + 1;
    } while (verificarCodigo(ufs[*num_ufs]->codigo));

    printf("codigo atribuido: %d\n", ufs[*num_ufs]->codigo);
    printf("Digite o nome da UF: ");
    ler(ufs[*num_ufs]->descricao, sizeof(ufs[*num_ufs]->descricao));

    printf("Digite a sigla da UF: ");
    ler(ufs[*num_ufs]->sigla, sizeof(ufs[*num_ufs]->sigla));

    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf != NULL) {
        fseek(fuf, 0, SEEK_END);
        fwrite(ufs[*num_ufs], sizeof(UF), 1, fuf);
        fclose(fuf);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }

    (*num_ufs)++;
    printf("UF adicionada!\n");
}

void alterarUF(UF *ufs[], int num_ufs) {

    int codigo_uf;
    int opcao_alterar_uf;

    printf("Digite o codigo da UF a ser alterada: ");
    scanf("%d", &codigo_uf);
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            do {
                FILE *fuf = fopen("uf.data", "rb+");
                if (fuf == NULL) {
                    printf("Erro ao abrir arquivo para alterar\n");
                    return;
                }
                printf("O que gostaria de alterar nessa UF?\n");
                printf("1. Nome (atual: %s)\n", ufs[i]->descricao);
                printf("2. Sigla (atual: %s)\n", ufs[i]->sigla);
                printf("0. Nada\n");
                scanf("%d", &opcao_alterar_uf);
                switch (opcao_alterar_uf) {
                    case 1:
                        printf("Novo nome da UF: ");
                        ler(ufs[i]->descricao, sizeof(ufs[i]->descricao));
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(ufs[i], sizeof(UF), 1, fuf);
                        printf("Nome da UF alterado!\n");
                        fclose(fuf);
                        break;
                    case 2:
                        printf("Nova sigla da UF: ");
                        ler(ufs[i]->sigla, sizeof(ufs[i]->sigla));
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(ufs[i], sizeof(UF), 1, fuf);
                        printf("Sigla da UF alterada!\n");
                        fclose(fuf);
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        fclose(fuf);
                        break;
                }
            } while (opcao_alterar_uf != 0);

            return;
        }
    }

    printf("Nao existe UF com esse codigo\n");
}

void mostrarDadosDasUFs(UF *ufs[], int num_ufs) {

    if (num_ufs == 0) {
        printf("Nao ha ufs cadastradas\n");
        return;
    }

    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i] == NULL) continue;
        printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", ufs[i]->codigo, ufs[i]->descricao, ufs[i]->sigla);
    }

}

void mostrarUF(UF *ufs[], int num_ufs) {

    if (num_ufs == 0) {
        printf("Nao ha UFs cadastradas\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF que deseja observar: ");
    scanf("%d", &codigo_uf);

    int encontrado = -1;
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    printf("codigo da UF: %d | nome da UF: %s | sigla da UF: %s\n", ufs[encontrado]->codigo, ufs[encontrado]->descricao,
        ufs[encontrado]->sigla);

}

void excluirUF(UF *ufs[], int *num_ufs, int *total_eleicoes) {

    int codigo_uf;
    printf("Digite o codigo da UF que deseja excluir: ");
    scanf("%d", &codigo_uf);

    int encontrado = -1;
    for (int i = 0; i < *num_ufs; i++) {
        if (ufs[i] != NULL && ufs[i]->codigo == codigo_uf) {
            free(ufs[i]);
            ufs[i] = NULL;
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe UF com esse codigo\n");
        return;
    }

    for (int i = encontrado; i < *num_ufs - 1; i++) {
        ufs[i] = ufs[i + 1];
    }
    ufs[*num_ufs - 1] = NULL;
    (*num_ufs)--;

    FILE *fuf = fopen("uf.data", "wb+");
    if (fuf == NULL) {
        printf("erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *num_ufs; i++) {
        if (ufs[i] != NULL) {
            fwrite(ufs[i], sizeof(UF), 1, fuf);
        }
    }

    fclose(fuf);
    printf("UF removida!\n");
    //excluirEleicoesPorUF(eleicoes, codigo_uf, total_eleicoes);
    //
}