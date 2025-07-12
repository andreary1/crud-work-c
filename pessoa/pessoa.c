#include "pessoa.h"
#include "../pessoa.h"

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
        scanf("%d", &opcao_pessoa);
        switch (opcao_pessoa) {
            case INSERIR_PESSOA://inserirPessoa(pessoas, num_pessoas);
                break;
            case ALTERAR_PESSOA://alterarPessoa(pessoas, *num_pessoas);
                break;
            case EXCLUIR_PESSOA://excluirPessoa(pessoas, num_pessoas);
                break;
            case MOSTRAR_PESSOAS://mostrarPessoas(pessoas, *num_pessoas);
                break;
            case MOSTRAR_POR_TITULO://mostrarUF(ufs, *num_ufs);
                break;
            case 0: printf("Saindo\n");
                break;
            default: printf("Opcao invalida!\nDigite outra opcao\n");
                break;
        }
    } while (opcao_pessoa != 0);
};