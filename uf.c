#include <stdio.h>
#include "uf.h"
#include "eleicao.h"
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

void menuUF(UF *ufs[]) {
    int num_ufs;
    int opcao_uf;
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
            adicionarUF(ufs);
            break;
        case 2:
            num_ufs = carregarUFs(ufs, 50);
            alterarUF(ufs);
            break;
        case 3:
            num_ufs = carregarUFs(ufs, 50);
            excluirUF(ufs, &num_ufs);
            break;
        case 4:
            num_ufs = carregarUFs(ufs, 50);
            mostrarDadosDasUFs(ufs, num_ufs);
            break;
        case 5:
            num_ufs = carregarUFs(ufs, 50);
            mostrarUF(ufs, num_ufs);
            break;
        case 0:
            printf("Saindo\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
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

int carregarUFs(UF *ufs[], int total_uf) {
    FILE *fuf = fopen("uf.data", "rb+");
    if (fuf == NULL) return 0;

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    for (int i = 0; i < total_uf; i++) {
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

void adicionarUF(UF *ufs[]) {
    FILE *fuf = fopen("uf.data", "rb+");

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    if (num_ufs >= 50) {
        printf("maximo de UFs atingido\n");
        fclose(fuf);
        return;
    }

    ufs[num_ufs] = (UF *)malloc(sizeof(UF));
    if (ufs[num_ufs] == NULL) {
        printf("Erro ao alocar memória para nova UF.\n");
        fclose(fuf);
        return;
    }

    srand(time(NULL));
    do {
        ufs[num_ufs]->codigo = rand() % 100 + 1;
    } while (verificarCodigo(ufs[num_ufs]->codigo));

    printf("codigo atribuido: %d\n", ufs[num_ufs]->codigo);
    printf("Digite o nome da UF: ");
    ler(ufs[num_ufs]->descricao, sizeof(ufs[num_ufs]->descricao));

    printf("Digite a sigla da UF: ");
    ler(ufs[num_ufs]->sigla, sizeof(ufs[num_ufs]->sigla));

    fseek(fuf, 0, SEEK_END);
    fwrite(ufs[num_ufs], sizeof(UF), 1, fuf);
    fclose(fuf);
    printf("UF adicionada!\n");
}

void alterarUF(UF *ufs[]) {
    FILE *fuf = fopen("uf.data", "rb+");
    int codigo_uf;
    int opcao_alterar_uf;

    fseek(fuf, 0, SEEK_END);
    long int num_ufs = ftell(fuf);
    num_ufs /= sizeof(UF);

    printf("Digite o codigo da UF a ser alterada: ");
    scanf("%d", &codigo_uf);
    for (int i = 0; i < num_ufs; i++) {
        if (ufs[i]->codigo == codigo_uf) {
            do {
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
                        break;
                    case 2:
                        printf("Nova sigla da UF: ");
                        ler(ufs[i]->sigla, sizeof(ufs[i]->sigla));
                        fseek(fuf, i * sizeof(UF), SEEK_SET);
                        fwrite(ufs[i], sizeof(UF), 1, fuf);
                        printf("Sigla da UF alterada!\n");
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        break;
                }
            } while (opcao_alterar_uf != 0);

            fclose(fuf);
            return;
        }
    }
    printf("Nao existe UF com esse codigo\n");
    fclose(fuf);
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

void excluirUF(UF *ufs[], int *num_ufs) {

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

    printf("UF removida!\n");
    //excluirEleicoesPorUF(codigo_uf);
}