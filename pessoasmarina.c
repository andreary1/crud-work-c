#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoas.h"

int carregarPessoas (Pessoa **pessoas, int num_pessoas) {

    FILE *fpessoas = fopen ("pessoas.data", "rb+");
    if (fpessoas == NULL) {
        printf ("Erro ao abrir arquivo.");
        return 0;
    }
    
    fseek (fpessoas, 0, SEEK_END);
    long int num_pessoas = ftell (fpessoas);
    num_pessoas /= sizeof(Pessoa);
    
    int i;
    for (i = 0; i < num_pessoas; i++) {
        pessoas[i] = NULL;
    }
    
    fseek (fpessoas, 0, SEEK_SET);
    for (i = 0; i < num_pessoas; i++) {
        Pessoa *pessoa = (Pessoa *) malloc (sizeof(Pessoa));
        fread (pessoa, sizeof(Pessoa), 1, fpessoas);
        pessoas[i] = pessoa;
    }
    
    fclose (fpessoas);
    return num_pessoas;
}

void liberarPessoas (Pessoa **pessoas, int num_pessoas) {
    
    int i;
    for (i = 0; i < num_pessoas; i++) {
        if (pessoas[i] != NULL) {
            free (pessoas[i]);
            pessoas[i] = NULL;
        }
    }
    
}

int verificarCpf (Pessoa **pessoas, int num_pessoas) {

    char cpf_digitado[12];
    printf ("Digite o CPF: ");
    getchar();
    fgets(cpf_digitado, sizeof(cpf_digitado), stdin);

    if (strlen(cpf_digitado) != 11) {
        printf("Quantidade de caracteres digitados é incompatível com CPF\n");
        return 0;
    }

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->cpf, cpf_digitado) == 0) {
            printf("CPF já cadastrado.\n");
            return 0;
        }
    }
    return 1;
}

int verificarTitulo (Pessoa **pessoas, int num_pessoas) {

    char titulo_digitado[20];
    printf ("Digite o Titulo: ");
    getchar();
    fgets(titulo_digitado, sizeof(titulo_digitado), stdin);

    if (strlen(titulo_digitado) != 12) {
        printf("Quantidade de caracteres digitados é incompatível com Título\n");
        return 0;
    }

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->titulo, titulo_digitado) == 0) {
            printf("Título já cadastrado.\n");
            return 0;
        }
    }
    return 1;
}

int verificarDataNascimento (int ano, int mes, int dia) {
    struct tm data = {0};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;

    time_t nascimento = mktime(&data);
    time_t agora = time(NULL);

    double segundos_agora = difftime(agora, nascimento);
    double dezoito_anos = 18 * 365.25 * 24 * 60 * 60;

    return segundos_agora >= dezoito_anos;
}

void inserirPessoa (Pessoa **pessoas, int *num_pessoas) {

    Pessoa *novaPessoa = (Pessoa *) malloc (sizeof(Pessoa));
    
    if (novaPessoa == NULL) {
        printf ("Erro ao alocar memória");
        return;
    }

    printf ("Digite o CPF: ");
    getchar();
    fgets(novaPessoa->cpf, sizeof(novaPessoa->cpf), stdin);

    if (!verificarCpf(pessoas, *num_pessoas)) {
        free(novaPessoa);
        return;
    }
    
    printf ("Digite o Titulo: ");
    getchar();
    fgets(novaPessoa->titulo, sizeof(novaPessoa->titulo), stdin);

    if (!verificarTitulo(pessoas, *num_pessoas)) {
        free(novaPessoa);
        return;
    }
    
    printf ("Digite o Nome: ");
    getchar();
    fgets (novaPessoa->nome, sizeof(novaPessoa->nome), stdin);
    
    printf ("Digite o Fone: ");
    getchar();
    fgets (novaPessoa->fone, sizeof(novaPessoa->fone), stdin);
    
    printf ("Digite o Endereço: ");
    getchar();
    fgets (novaPessoa->endereco, sizeof(novaPessoa->endereco), stdin);
    
    printf ("Digite a data de Nascimento (formato DD/MM/AAAA): ");
    scanf ("%d/%d/%d", &novaPessoa->dia_nascimento, &novaPessoa->mes_nascimento, &novaPessoa->ano_nascimento);
    if (!verificarDataNascimento(novaPessoa->dia_nascimento, novaPessoa->mes_nascimento, novaPessoa->ano_nascimento)) {
        printf("Data de nascimento inválida.\n");
        free(novaPessoa);
        return;
    }

    pessoas[*num_pessoas] = novaPessoa;
    pessoas[*num_pessoas]->ano_nascimento = novaPessoa->ano_nascimento;
    pessoas[*num_pessoas]->mes_nascimento = novaPessoa->mes_nascimento;
    pessoas[*num_pessoas]->dia_nascimento = novaPessoa->dia_nascimento;
    pessoas[*num_pessoas]->cpf = novaPessoa->cpf;
    pessoas[*num_pessoas]->titulo = novaPessoa->titulo;
    pessoas[*num_pessoas]->nome = novaPessoa->nome;
    pessoas[*num_pessoas]->fone = novaPessoa->fone;
    pessoas[*num_pessoas]->endereco = novaPessoa->endereco;
    (*num_pessoas)++;
    
    FILE *fpessoas = fopen("pessoas.data", "ab+");
    if (fpessoas == NULL) {
        printf("Erro ao abrir arquivo.");
        free(novaPessoa); 
        return;
    }
    
    fseek (fpessoas, 0, SEEK_END);
    fwrite(novaPessoa, sizeof(Pessoa), 1, fpessoas);
    fclose(fpessoas);

    printf("Pessoa cadastrada com sucesso!\n");
}

void alterarPessoa (Pessoa **pessoas, int num_pessoas) {
    if (num_pessoas == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }

    char titulo[20];
    printf("Digite o título da pessoa que deseja alterar: ");
    getchar();
    fgets(titulo, sizeof(titulo), stdin);

    // VERIFICAR TITULO EXISTE

    int alterar_pessoa;
    int i;

    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i]->titulo, titulo) == 0) {
            FILE *fpessoa = fopen("pessoa.data", "rb+");
                if (fpessoa == NULL) {
                    printf("Erro ao abrir arquivo para alterar\n");
                    return;
                }
                printf("O que deseja alterar nessa pessoa?\n");
                printf("1. Nome (atual: %s)\n", pessoa[i]->nome);
                printf("2. CPF (atual: %s)\n", pessoa[i]->cpf);
                printf("3. Fone (atual: %s)\n", pessoa[i]->fone);
                printf("4. Endereço (atual: %s)\n", pessoa[i]->endereco);
                printf("5. Data de Nascimento (atual: %d/%d/%d)\n", 
                    pessoa[i]->dia_nascimento, pessoa[i]->mes_nascimento, pessoa[i]->ano_nascimento);
                printf ("6. Titulo (atual: %s)\n", pessoa[i]->titulo);
                printf("0. Nada\n");
                scanf("%d", &alterar_pessoa);
                switch (alterar_pessoa) {
                    case 1:
                        printf("Novo nome da pessoa: ");
                        ler(pessoa[i]->nome, sizeof(pessoa[i]->nome));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Nome da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 2:
                        printf("Novo CPF da pessoa: ");
                        ler(pessoa[i]->cpf, sizeof(pessoa[i]->cpf));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("CPF da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 3:
                        printf("Novo Fone da pessoa: ");
                        ler(pessoa[i]->fone, sizeof(pessoa[i]->fone));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Fone da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 4:
                        printf("Novo Endereço da pessoa: ");
                        ler(pessoa[i]->endereco, sizeof(pessoa[i]->endereco));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Endereço da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 5:
                        printf("Nova Data de Nascimento da pessoa (formato DD/MM/AAAA): ");
                        scanf("%d/%d/%d", &pessoa[i]->dia_nascimento, &pessoa[i]->mes_nascimento, &pessoa[i]->ano_nascimento);
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Data de Nascimento da pessoa alterada!\n");
                        fclose(fpessoa);
                        break;
                    case 6:
                        printf("Novo Titulo da pessoa: ");
                        ler(pessoa[i]->titulo, sizeof(pessoa[i]->titulo));
                        fseek(fpessoa, i * sizeof(Pessoa), SEEK_SET);
                        fwrite(pessoa[i], sizeof(Pessoa), 1, fpessoa);
                        printf("Titulo da pessoa alterado!\n");
                        fclose(fpessoa);
                        break;
                    case 0:
                        break;
                    default:
                        printf("opcao invalida!\n");
                        fclose(fpessoa);
                        break;
                }
            } while (alterar_pessoa != 0);
        }
}

void excluirPessoa (Pessoa **pessoas, int *num_pessoas) {
    if (*num_pessoas == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }

    char titulo[20];
    printf("Digite o título da pessoa que deseja excluir: ");
    getchar();
    fgets(titulo, sizeof(titulo), stdin);

    int encontrado = -1;
    for (int i = 0; i < *num_pessoas; i++) {
        if (strcmp(pessoas[i]->titulo, titulo) == 0) {
            free (pessoas[i]);
            pessoas[i] = NULL;
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Pessoa não encontrada \n");
        return;
    }

    for (int i = encontrado; i < *num_pessoas - 1; i++) {
        pessoas[i] = pessoas[i + 1];
    }
    pessoas[*num_pessoas - 1] = NULL;
    (*num_pessoas)--;

    FILE *fpessoas = fopen("pessoas.data", "wb+");
    if (fpessoas == NULL) {
        printf("Erro ao abrir arquivo para exclusão \n");
        return;
    }
    
    for (int i = 0; i < *num_pessoas; i++) {
        if (pessoas[i] != NULL) {
            fwrite(pessoas[i], sizeof(Pessoa), 1, fpessoas);
        }
    }

    fclose(fpessoas);
    printf("Pessoa excluída!\n");
}

void menu_pessoas (Pessoa **pessoas, int  *num_pessoas) {
       
    int opcao_pessoa;
    do {
        printf ("------------ OPCOES PARA PESSOAS ------------\n");
        printf ("1. Inserir pessoa\n");
        printf ("2. Alterar pessoa\n");
        printf ("3. Excluir pessoa\n");
        printf ("4. Mostrar dados de todas as pessoas\n");
        printf ("5. Mostrar dados de uma pessoa por título\n");
        printf ("0. Sair\n");
        printf ("---------------------------------------------\n");
        scanf ("%d", &opcao_pessoa);
            switch (opcao_pessoa) {
                case 1:
                    inserir_pessoa (pessoas, num_pessoas);
                    break;
                case 2:
                    alterar_pessoa (pessoas, num_pessoas);
                    break;
                case 3:
                    excluir_pessoa (pessoas, num_pessoas);
                    break;
                case 4:
                    mostrar_pessoas (pessoas, num_pessoas);
                    break;
                case 5:
                    mostrar_pessoa_portitulo (pessoas, num_pessoas);
                    break;
                case 0:
                    printf ("Saindo do programa\n");
                    break;
                default:
                    printf ("Opção inválida. Digite outra opcao\n");
                    break;
                }
        } while (opcao_pessoa != 0);
}

void mostarPessoas (Pessoa **pessoas, int num_pessoas) {

     if (num_pessoas == 0) {
        printf ("Nenhuma pessoa cadastrada.\n");
        return;
    }
    
    int i;
    printf ("--------------------------- PESSOAS CADASTRADAS NO SISTEMA ---------------------------\n");
    for (i = 0; i < num_pessoas; i++){
       printf ("%d. \tNome: %s    \t CPF: %s    \t Titulo: %s \n",
                (i+1), pessoas[i]->nome, pessoas[i]->cpf, pessoas[i]->titulo);
    printf ("--------------------------------------------------------------------------------------\n");
    }
}

void mostrarPessoaPorTitulo (Pessoa **pessoas, int num_pessoas) {

     if (num_pessoas == 0){
        printf ("Nao ha pessoas cadastradas.\n");
        return;
    }
    
    char titulo;
    printf ("Digite o titulo da pessoa a qual voce deseja consultar: ");
    scanf ("%c", &titulo);
    
    if (strlen(titulo) != 12) {
        printf ("Quantidade de caracteres digitados eh incompativel com titulo\n");
        return;
    }
    
    int i;
    int encontrado = -1;
    for (i = 0; i > num_pessoas; i++) {
        if (pessoas[i]->titulo == titulo) {
            encontrado = i;
            return;
        }   
    }

    if (encontrado == -1) {
        printf ("Pessoa nao encontrada.\n");
    }

    printf ("-------- PESSOA DE TÍTULO %s -------------\n", pessoas[encontrado]->titulo);
    printf ("Nome: %s\n", pessoas[encontrado]->nome);
    printf ("CPF: %s\n", pessoas[encontrado]->cpf);
    printf ("Fone: %s\n", pessoas[encontrado]->fone);
    printf ("Endereço: %s\n", pessoas[encontrado]->endereco);
    printf ("Data de Nascimento: %d/%d/%d\n", pessoas[encontrado]->dia_nascimento, pessoas[encontrado]->mes_nascimento, pessoas[encontrado]->ano_nascimento);
    printf ("-------------------------------------------\n");
}
