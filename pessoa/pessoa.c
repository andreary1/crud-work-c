#include <stdio.h>
#include "pessoa.h"
#include "../UF/uf.h"
#include <stdlib.h>
#include <string.h>

extern Pessoa **pessoas;

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

int verificarCPF(char cpf[]) {

    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa == NULL) return 0;
    Pessoa pessoa;
    while (fread(&pessoa, sizeof(Pessoa), 1, fpessoa)) {
        if (strcmp(pessoa.cpf, cpf) == 0) {
            fclose(fpessoa);
            return 1;
        }
    }

    fclose(fpessoa);
    return 0;
}

int verificarTitulo(char titulo[]) {

    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa == NULL) return 0;
    Pessoa pessoa;
    while (fread(&pessoa, sizeof(Pessoa), 1, fpessoa)) {
        if (strcmp(pessoa.titulo, titulo) == 0) {
            fclose(fpessoa);
            return 1;
        }
    }

    fclose(fpessoa);
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
    printf("Digite o CPF da pessoa: ");
    ler(cpf, sizeof(cpf));

    if (verificarCPF(cpf)) {
        printf("Esse CPF ja foi cadastrado\n");
        return;
    }

    strcpy(pessoas[*num_pessoas]->cpf, cpf);

    printf("Digite o nome da pessoa: ");
    ler(pessoas[*num_pessoas]->nome, sizeof(pessoas[*num_pessoas]->nome));

    char titulo[30];
    printf("Digite o titulo da pessoa: ");
    ler(titulo, sizeof(titulo));

    if (verificarTitulo(titulo)) {
        printf("Esse titulo ja foi cadastrado\n");
        return;
    }

    printf("Digite o numero de telefone da pessoa:");
    lerNaoObrigatorio(pessoas[*num_pessoas]->fone, sizeof(pessoas[*num_pessoas]->fone));

    printf("Digite o endereco da pessoa: ");
    lerNaoObrigatorio(pessoas[*num_pessoas]->endereco, sizeof(pessoas[*num_pessoas]->endereco));

    printf("Digite a data de nascimento da pessoa: ");
    ler(pessoas[*num_pessoas]->data_nascimento, sizeof(pessoas[*num_pessoas]->data_nascimento));

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
    char cpf[30];

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
                        ler(pessoas[i]->titulo, sizeof(pessoas[i]->titulo));
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
                        ler(pessoas[i]->data_nascimento, sizeof(pessoas[i]->data_nascimento));
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

    for (int i = 0; i < num_pessoas; i++) {
        if (pessoas[i] == NULL) continue;
        printf("| Nome: %s | CPF: %s | Telefone: %s | Titulo: %s | Endereco: %s | Data de Nascimento: %s |\n",
            pessoas[i]->nome, pessoas[i]->cpf, pessoas[i]->fone, pessoas[i]->titulo, pessoas[i]->endereco,
            pessoas[i]->data_nascimento);
    }
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


    printf("| Nome: %s | CPF: %s | Telefone: %s | Endereco: %s | Data de Nascimento: %s |\n",
                pessoas[encontrado]->nome, pessoas[encontrado]->cpf, pessoas[encontrado]->fone, pessoas[encontrado]->endereco,
                pessoas[encontrado]->data_nascimento);

}

void excluirPessoa(int *num_pessoas) {

    char cpf[30];
    printf("Digite o CPF da pessoa que deseja excluir: ");
    ler(cpf, sizeof(cpf));

    int encontrado = -1;
    for (int i = 0; i < *num_pessoas; i++) {
        if (pessoas[i] != NULL && strcmp(cpf, pessoas[i]->cpf) == 0) {
            free(pessoas[i]);
            pessoas[i] = NULL;
            encontrado = i;
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
}