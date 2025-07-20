#include <stdio.h>
#include "pessoa.h"
#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"
#include "../voto/voto.h"
#include "../comparecimento/comparecimento.h"
#include <stdlib.h>
#include <string.h>

extern Pessoa **pessoas;
extern Candidato **candidatos;
extern Voto **votos;
extern Comparecimento **comparecimentos;

int carregarPessoas(int *capacidade_pessoas) {
    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa == NULL) return 0;

    fseek(fpessoa, 0, SEEK_END);
    long int num_pessoas = ftell(fpessoa);
    num_pessoas /= sizeof(Pessoa);

    while (num_pessoas >= *capacidade_pessoas) {
        *capacidade_pessoas *= 2;
    }

    pessoas = malloc(*capacidade_pessoas * sizeof(Pessoa *));
    if (pessoas == NULL) {
        printf("Erro na alocacao de memoria\n");
        return 0;
    }


    fseek(fpessoa, 0, SEEK_SET);
    for (int i = 0; i < num_pessoas; i++) {
        Pessoa *pessoa = (Pessoa *)malloc(sizeof(Pessoa));
        if (pessoa == NULL) {
            printf("Erro na alocacao de memoria\n");
            return 0;
        }
        fread(pessoa, sizeof(Pessoa), 1, fpessoa);

        pessoas[i] = pessoa;
    }

    for (int i = num_pessoas; i < *capacidade_pessoas; i++) {
        pessoas[i] = NULL;
    }

    fclose(fpessoa);
    return num_pessoas;
}

void liberarPessoas(int total_pessoas) {
    for (int i = 0; i < total_pessoas; i++) {
        if (pessoas[i] != NULL) {
            free(pessoas[i]);
            pessoas[i] = NULL;
        }
    }
    free(pessoas);
    pessoas = NULL;
}

int verificarCPF(char cpf[], int num_pessoas) {

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->cpf, cpf) == 0) {
            return 1;
        }
    }

    return 0;
}

int verificarTitulo(char titulo[], int num_pessoas) {

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->titulo, titulo) == 0) {
            return 1;
        }
    }

    return 0;
}

void inserirPessoa(int *num_pessoas, int *capacidade_pessoas) {

    if (*num_pessoas >= *capacidade_pessoas) {
        *capacidade_pessoas *= 2;
        pessoas = realloc(pessoas, *capacidade_pessoas * sizeof(Pessoa *));
        if (pessoas == NULL) {
            printf("Erro na alocacao de memoria\n");
            return;
        }
        for (int i = *num_pessoas; i < *capacidade_pessoas; i++) {
            pessoas[i] = NULL;
        }
    }


    pessoas[*num_pessoas] = (Pessoa *)malloc(sizeof(Pessoa));
    if (pessoas[*num_pessoas] == NULL) {
        printf("Erro ao alocar memoria para nova Pessoa\n");
        return;
    }

    char cpf[30];
    do {
        printf("Digite o CPF da pessoa: ");
        ler(cpf, sizeof(cpf));
    } while (strlen(cpf) != 11 || strspn(cpf, "0123456789") != strlen(cpf));


    if (verificarCPF(cpf, *num_pessoas)) {
        printf("Esse CPF ja foi cadastrado\n");
        return;
    }

    strcpy(pessoas[*num_pessoas]->cpf, cpf);

    printf("Digite o nome da pessoa: ");
    ler(pessoas[*num_pessoas]->nome, sizeof(pessoas[*num_pessoas]->nome));

    char titulo[30];
    do {
        printf("Digite o titulo da pessoa: ");
        ler(titulo, sizeof(titulo));
    } while (strlen(titulo) != 12 || strspn(titulo, "0123456789") != strlen(titulo));


    if (verificarTitulo(titulo, *num_pessoas)) {
        printf("Esse titulo ja foi cadastrado\n");
        return;
    }

    strcpy(pessoas[*num_pessoas]->titulo, titulo);

    printf("Digite o numero de telefone da pessoa:");
    lerNaoObrigatorio(pessoas[*num_pessoas]->fone, sizeof(pessoas[*num_pessoas]->fone));

    printf("Digite o endereco da pessoa: ");
    lerNaoObrigatorio(pessoas[*num_pessoas]->endereco, sizeof(pessoas[*num_pessoas]->endereco));

    char data_nasc[11];
    do {
        printf("Digite a data de nascimento da pessoa (dd/mm/aaaa): ");
        ler(data_nasc, sizeof(data_nasc));
    } while (strlen(data_nasc) != 10 || data_nasc[2] != '/' || data_nasc[5] != '/');

    strcpy(pessoas[*num_pessoas]->data_nascimento, data_nasc);

    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa != NULL) {
        fseek(fpessoa, 0, SEEK_END);
        fwrite(pessoas[*num_pessoas], sizeof(Pessoa), 1, fpessoa);
        fclose(fpessoa);
    }
    else {
        printf("Erro ao abrir arquivo para escrita\n");
        return;
    }

    (*num_pessoas)++;
    printf("Pessoa adicionada!\n");
}

void alterarPessoa(int num_pessoas) {

    int opcao_alterar_pessoa;
    char cpf[20];

    printf("Digite o CPF da pessoa a ser alterada: ");
    ler(cpf, sizeof(cpf));
    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->cpf, cpf) == 0) {
            do {
                FILE *fpessoa = fopen("pessoas.data", "rb+");
                if (fpessoa == NULL) {
                    printf("erro ao abrir arquivo para alterar\n");
                    return;
                }
                printf("O que voce gostaria de alterar nessa pessoa?\n");
                printf("1. nome (atual: %s)\n", pessoas[i]->nome);
                printf("2. telefone (atual: %s)\n", pessoas[i]->fone);
                printf("3. titulo (atual: %s)\n", pessoas[i]->titulo);
                printf("4. endereco (atual: %s)\n", pessoas[i]->endereco);
                printf("5. data de nascimento (atual: %s)\n", pessoas[i]->data_nascimento);
                printf("0. Sair\n");
                scanf("%d", &opcao_alterar_pessoa);
                limparBuffer();
                switch (opcao_alterar_pessoa) {
                    case 1:
                        printf("Novo nome da pessoa: ");
                        ler(pessoas[i]->nome, sizeof(pessoas[i]->nome));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Nome da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 2:
                        printf("Novo telefone da pessoa: ");
                        lerNaoObrigatorio(pessoas[i]->fone, sizeof(pessoas[i]->fone));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Telefone da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 3:
                        printf("Novo titulo da pessoa: ");
                        char titulo[20];
                        do {
                            ler(titulo, sizeof(titulo));
                        } while (strlen(titulo) != 12 || strspn(titulo, "0123456789") != strlen(titulo));
                        if (verificarTitulo(titulo, num_pessoas)) {
                            printf("Esse titulo ja foi cadastrado\n");
                            break;
                        }
                        strcpy(pessoas[i]->titulo, titulo);
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Titulo da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 4:
                        printf("Novo endereco da pessoa: ");
                        lerNaoObrigatorio(pessoas[i]->endereco, sizeof(pessoas[i]->endereco));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Endereco da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 5:
                        printf("Nova data de nascimento da pessoa (dd/mm/aaaa): ");
                        char data_nasc[11];
                        do {
                            ler(data_nasc, sizeof(data_nasc));
                        } while (strlen(data_nasc) != 10 || data_nasc[2] != '/' || data_nasc[5] != '/');
                        strcpy(pessoas[i]->data_nascimento, data_nasc);
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Data de nascimento da pessoa alterada!\n");
                        fclose(fpessoa);
                        break;
                    case 0:
                        fclose(fpessoa);
                        break;
                    default:
                        printf("opcao invalida!\n");
                        fclose(fpessoa);
                        break;
                }
            } while (opcao_alterar_pessoa != 0);

            return;
        }
    }

    printf("Nao existe pessoa cadastrada com esse CPF\n");
}

void mostrarPessoas(int num_pessoas) {

    if (num_pessoas == 0) {
        printf("Nao ha pessoas cadastradas\n");
        return;
    }
    printf("======================================================================================================================\n");
    printf("| %-30s | %-11s | %-15s | %-12s | %-20s | %-10s |\n",
           "Nome", "CPF", "Titulo", "Telefone", "Endereco", "Nascimento");
    printf("======================================================================================================================\n");

    for (int i = 0; i < num_pessoas; i++) {
        if (pessoas[i] == NULL) continue;
        printf("| %-30s | %-11s | %-15s | %-12s | %-20s | %-10s |\n",
               pessoas[i]->nome,
               pessoas[i]->cpf,
               pessoas[i]->titulo,
               pessoas[i]->fone,
               pessoas[i]->endereco,
               pessoas[i]->data_nascimento);
    }

    printf("======================================================================================================================\n");
}

void mostrarPorTitulo(int num_pessoas) {
    if (num_pessoas == 0) {
        printf("Nao ha pessoas cadastradas\n");
        return;
    }

    char titulo[30];
    printf("Digite o titulo da pessoa que deseja observar: ");
    ler(titulo, sizeof(titulo));

    int encontrado = -1;
    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->titulo, titulo) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nao existe pessoa cadastrada com esse titulo\n");
        return;
    }

    printf("======================================================================================================================\n");
    printf("| %-30s | %-11s | %-15s | %-12s | %-20s | %-10s |\n",
           "Nome", "CPF", "Titulo", "Telefone", "Endereco", "Nascimento");
    printf("======================================================================================================================\n");
    printf("| %-30s | %-11s | %-15s | %-12s | %-20s | %-10s |\n",
       pessoas[encontrado]->nome,
       pessoas[encontrado]->cpf,
       pessoas[encontrado]->titulo,
       pessoas[encontrado]->fone,
       pessoas[encontrado]->endereco,
       pessoas[encontrado]->data_nascimento);
    printf("======================================================================================================================\n");

}

void excluirPessoa(int *num_pessoas, int *num_candidatos, int *num_votos, int *num_comparecimentos) {

    char cpf[30];
    printf("Digite o CPF da pessoa que deseja excluir: ");
    ler(cpf, sizeof(cpf));

    int encontrado = -1;
    for (int i = 0; i < *num_pessoas; i++) {
        if (pessoas[i] != NULL && strcmp(cpf, pessoas[i]->cpf) == 0) {
            free(pessoas[i]);
            pessoas[i] = NULL;
            encontrado = i;
            exclusaoCandidatoPeloCPF(num_candidatos, num_votos, num_comparecimentos, cpf);
            break;
        }
    }

    if (encontrado == -1) {
        printf("Esse CPF nao esta cadastrado\n");
        return;
    }

    for (int i = encontrado; i < *num_pessoas - 1; i++) {
        pessoas[i] = pessoas[i + 1];
    }
    pessoas[*num_pessoas - 1] = NULL;
    (*num_pessoas)--;

    FILE *fpessoa = fopen("pessoas.data", "wb+");
    if (fpessoa == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < *num_pessoas; i++) {
        if (pessoas[i] != NULL) {
            fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoa);
        }
    }

    fclose(fpessoa);
    printf("Pessoa removida!\n");
    exclusaoVotosEComparecimentosPeloCPF(num_votos, num_comparecimentos, cpf);
}

void exclusaoCandidatoPeloCPF(int *num_candidatos, int *num_votos, int *num_comparecimentos, char cpf[]) {

    int encontrado = -1;
    Candidato c;
    for (int i = 0; i < *num_candidatos; i++) {
        if (candidatos[i] != NULL && strcmp(candidatos[i]->CPF, cpf) == 0) {
            c = *candidatos[i];
            free(candidatos[i]);
            candidatos[i] = NULL;
            encontrado = i;
            exclusaoVotosEComparecimentos(num_votos, num_comparecimentos, c);
            break;
        }
    }

    if (encontrado == -1) {
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
}

void exclusaoVotosEComparecimentosPeloCPF(int *num_votos, int *num_comparecimentos, char cpf[]) {

    int encontrado = -1;
    for (int i = 0; i < *num_comparecimentos; i++) {
        if (comparecimentos[i] != NULL && strcmp(comparecimentos[i]->CPF, cpf) == 0) {

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