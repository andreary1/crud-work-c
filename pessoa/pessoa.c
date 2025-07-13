#include <stdio.h>
#include "pessoa.h"
#include "../UF/uf.h"
#include <stdlib.h>
#include <string.h>

void menuPessoas(Pessoa *pessoas[], int *num_pessoas) {
    opcaoPessoa opcao_pessoa;
    do {
        printf("------------OPCOES PARA PESSOAS-------------\n");
        printf("1. Inserir Pessoa\n");
        printf("2. Alterar Pessoa\n");
        printf("3. Excluir Pessoa\n");
        printf("4. Mostrar dados de todas as pessoas\n");
        printf("5. Mostrar uma pessoa pelo titulo de eleitor\n");
        printf("0. Sair\n");
        printf("--------------------------------------------\n");
        scanf("%c", &opcao_pessoa);
        limparBuffer();
        switch (opcao_pessoa) {
            case INSERIR_PESSOA: inserirPessoa(pessoas, num_pessoas);
                break;
            case ALTERAR_PESSOA: alterarPessoa(pessoas, *num_pessoas);
                break;
            case EXCLUIR_PESSOA: excluirPessoa(pessoas, num_pessoas);
                break;
            case MOSTRAR_PESSOAS: mostrarPessoas(pessoas, *num_pessoas);
                break;
            case MOSTRAR_POR_TITULO:mostrarPorTitulo(pessoas, *num_pessoas);
                break;
            case SAIR_PESSOA: printf("Saindo\n");
                break;
            default: printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_pessoa != SAIR_PESSOA);
}

int carregarPessoas(Pessoa *pessoas[], int total_pessoas) {
    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa == NULL) return 0;

    fseek(fpessoa, 0, SEEK_END);
    long int num_pessoas = ftell(fpessoa);
    num_pessoas /= sizeof(Pessoa);

    for (int i = 0; i < total_pessoas; i++) {
        pessoas[i] = NULL;
    }
    fseek(fpessoa, 0, SEEK_SET);
    for (int i = 0; i < num_pessoas; i++) {
        Pessoa *pessoa = (Pessoa *)malloc(sizeof(Pessoa));
        fread(pessoa, sizeof(Pessoa), 1, fpessoa);

        pessoas[i] = pessoa;
    }

    fclose(fpessoa);
    return num_pessoas;
}

void liberarPessoas(Pessoa *pessoas[], int total_pessoas) {
    for (int i = 0; i < total_pessoas; i++) {
        if (pessoas[i] != NULL) {
            free(pessoas[i]);
            pessoas[i] = NULL;
        }
    }
}

int verificarCPF(char cpf[]) {

    FILE *fpessoa = fopen("pessoas.data", "rb+");
    if (fpessoa == NULL) return 0;
    Pessoa pessoa;
    while (fread(&pessoa, sizeof(Pessoa), 1, fpessoa)) {
        if (strcmp(pessoa.cpf, cpf) == 0) {
            return 1;
        }
    }

    return 0;
}

void inserirPessoa(Pessoa *pessoas[], int *num_pessoas) {
    if (*num_pessoas >= 200) {
        printf("Maximo de pessoas atingido\n");
        return;
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


    printf("Digite o titulo da pessoa: ");
    ler(pessoas[*num_pessoas]->titulo, sizeof(pessoas[*num_pessoas]->titulo));

    printf("Digite o numero de telefone da pessoa:");
    ler(pessoas[*num_pessoas]->fone, sizeof(pessoas[*num_pessoas]->fone));

    printf("Digite o endereco da pessoa: ");
    ler(pessoas[*num_pessoas]->endereco, sizeof(pessoas[*num_pessoas]->endereco));

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

void alterarPessoa(Pessoa *pessoas[], int num_pessoas) {

    int opcao_alterar_pessoa;
    char cpf[30];

    printf("Digite o CPF da pessoa a ser alterada: ");
    ler(cpf, sizeof(cpf));
    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->cpf, cpf) == 0) {
            do {
                FILE *fpessoa = fopen("pessoas.data", "rb+");
                if (fpessoa != NULL) {
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
                        ler(pessoas[i]->fone, sizeof(pessoas[i]->fone));
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
                        ler(pessoas[i]->endereco, sizeof(pessoas[i]->endereco));
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

void mostrarPessoas(Pessoa *pessoas[], int num_pessoas) {

    if (num_pessoas == 0) {
        printf("Nao ha ufs cadastradas\n");
        return;
    }

    for (int i = 0; i < num_pessoas; i++) {
        if (pessoas[i] == NULL) continue;
        printf("Nome: %s | CPF: %s | Telefone: %s | Titulo: %s | Endereco: %s | Data de Nascimento: %s\n",
            pessoas[i]->nome, pessoas[i]->cpf, pessoas[i]->fone, pessoas[i]->titulo, pessoas[i]->endereco,
            pessoas[i]->data_nascimento);
    }
}

void mostrarPorTitulo(Pessoa *pessoas[], int num_pessoas) {
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


    printf("Nome: %s | CPF: %s | Telefone: %s | Endereco: %s | Data de Nascimento: %s\n",
                pessoas[encontrado]->nome, pessoas[encontrado]->cpf, pessoas[encontrado]->fone, pessoas[encontrado]->endereco,
                pessoas[encontrado]->data_nascimento);

}

void excluirPessoa(Pessoa *pessoas[], int *num_pessoas) {

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