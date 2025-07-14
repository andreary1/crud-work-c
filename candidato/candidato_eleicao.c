#include "candidato_eleicao.h"
#include "../UF/uf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../pessoa/pessoa.h"

int carregarCandidatos(Candidato *candidatos[], int total_candidatos) {

    FILE *fcandidato = fopen("candidatos.data", "rb+");
    if (fcandidato == NULL) return 0;

    fseek(fcandidato, 0, SEEK_END);
    long int num_candidatos = ftell(fcandidato);
    num_candidatos /= sizeof(Candidato);

    for (int i = 0; i < total_candidatos; i++) {
        candidatos[i] = NULL;
    }
    fseek(fcandidato, 0, SEEK_SET);
    for (int i = 0; i < num_candidatos; i++) {
        Candidato *candidato = (Candidato *)malloc(sizeof(Candidato));
        fread(candidato, sizeof(Candidato), 1, fcandidato);

        candidatos[i] = candidato;
    }

    fclose(fcandidato);
    return num_candidatos;
}

void liberarCandidatos(Candidato *candidatos[], int total_cand) {
    for (int i = 0; i < total_cand; i++) {
        if (candidatos[i] != NULL) {
            free(candidatos[i]);
            candidatos[i] = NULL;
        }
    }
}

void menuCandidatos(Candidato *candidatos[], UF *ufs[], int *num_candidatos, int *total_ufs) {
    int opcao_candidato;
    do {
        printf("--------------OPCOES PARA CANDIDATOS--------------\n");
        printf("1. Inserir candidato\n");
        printf("2. Excluir candidato\n");
        printf("3. Mostrar candidatos de uma eleicao por UF e ano\n");
        printf("4. Mostrar candidatos das eleicoes por ano\n");
        printf("0. Sair\n");
        printf("--------------------------------------------------\n");
        scanf("%d", &opcao_candidato);
        switch (opcao_candidato) {
            case 1:
                inserirCandidato(candidatos, num_candidatos);
                break;
            case 2:
                excluirCandidato(candidatos, num_candidatos);
                break;
            case 3:
                mostrarCandidatosPorUFeAno(candidatos, ufs, *num_candidatos, *total_ufs);
                break;
            case 4:
                mostrarTodosOsCandidatos(candidatos, ufs, *num_candidatos, *total_ufs);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_candidato != 0);
}

int verificarNumero(int numero) {

        FILE *fcandidato = fopen("candidatos.data", "rb+");
        Candidato candidato;
        while (fread(&candidato, sizeof(UF), 1, fcandidato) == 1) {
            if (candidato.numero == numero) {
                fclose(fcandidato);
                return 1;
            }
        }
        fclose(fcandidato);
        return 0;

}

void inserirCandidato(Candidato *candidatos[], int *total_cand) {
    if (*total_cand >= 200) {
        printf("maximo de Candidatos atingido\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF a qual o candidato pertence: ");

    scanf("%d", &codigo_uf);
    if (!verificarCodigo(codigo_uf)) {
        printf("nao existe uf com esse codigo\n");
        return;
    }

    int ano;
    printf("Digite o ano em que ele concorreu: ");
    scanf("%d", &ano);

    int numero_candidato;
    printf("Digite o numero desse candidato: ");
    scanf("%d", &numero_candidato);

    char cpf[30];
    printf("Digite o CPF do candidato: ");
    ler(cpf, sizeof(cpf));

    if (verificarCPF(cpf) == 0) {
        printf("Nao existe cpf com esse CPF\n");
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

void excluirCandidato(Candidato *candidatos[], int *total_cand) {

    int codigo_uf;
    printf("Digite o codigo da UF em que o candidato concorreu: ");
    scanf("%d", &codigo_uf);

    int ano;
    printf("Digite o ano em que ele concorreu: ");
    scanf("%d", &ano);

    int numero_candidato;
    printf("Digite o numero desse candidato: ");
    scanf("%d", &numero_candidato);

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

void mostrarCandidatosPorUFeAno(Candidato *candidatos[], UF *ufs[], int total_cand, int total_ufs) {
    if (total_cand == 0) {
        printf("Nao ha candidatos cadastrados\n");
        return;
    }

    int codigo_uf;
    printf("Digite o codigo da UF em que a eleicao concorreu: ");
    scanf("%d", &codigo_uf);

    int ano;
    printf("Digite o ano em que a eleicao ocorreu: ");
    scanf("%d", &ano);

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

void mostrarTodosOsCandidatos(Candidato *candidatos[], UF *ufs[], int total_cand, int total_ufs) {
    if (total_cand == 0) {
        printf("Nao ha candidatos cadastrados.\n");
        return;
    }

    // 1. Coletar anos distintos
    int anos[100];
    int total_anos = 0;
    for (int i = 0; i < total_cand; i++) {
        int ano = candidatos[i]->ano;
        int encontrado = 0;
        for (int j = 0; j < total_anos; j++) {
            if (anos[j] == ano) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            anos[total_anos++] = ano;
        }
    }

    // 2. Para cada ano, listar candidatos por UF
    for (int i = 0; i < total_anos; i++) {
        int ano_atual = anos[i];
        printf("\n====== Ano %d ======\n", ano_atual);

        // Ordenar por código UF (você pode trocar por nome se quiser)
        for (int u = 0; u < total_ufs; u++) {
            if (ufs[u] == NULL) continue;

            int uf_codigo = ufs[u]->codigo;
            int candidatos_na_uf = 0;

            // Verifica se existe algum candidato dessa UF nesse ano
            for (int c = 0; c < total_cand; c++) {
                if (candidatos[c] != NULL && candidatos[c]->codigo_uf == uf_codigo && candidatos[c]->ano == ano_atual) {
                    if (candidatos_na_uf == 0) {
                        printf("\nUF: %s (%d)\n", ufs[u]->descricao, uf_codigo);
                    }
                    printf("Número: %d | CPF: %s\n", candidatos[c]->numero, candidatos[c]->CPF);
                    candidatos_na_uf++;
                }
            }
        }
    }
}
