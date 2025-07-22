#include "comparecimento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uf.h"

int carregarComparecimentos (comparecimento **comparecimentos, int num_comparecimentos) {

    FILE *fcomparecimentos = fopen ("comparecimentos.data","rb+");
    if (fcomparecimentos == NULL) {
        printf ("Erro ao abrir arquivo.");
        return 0;
    }
    
    fseek (fcomparecimentos, 0, SEEK_END);
    long int num_comparecimentos = ftell (fcomparecimentos);
    num_comparecimentos /= sizeof(comparecimento);
    
    int i;
    for (i = 0; i < num_comparecimentos; i++){
        comparecimentos[i] = NULL;
    }
    
    fseek (fcomparecimentos, 0, SEEK_SET);
    for (i = 0; i < num_comparecimentos; i++) {
        comparecimento *Comparecimentos = (comparecimento *) malloc (sizeof(comparecimento));
        fread (Comparecimentos, sizeof(comparecimento), 1, fcomparecimentos);
        comparecimentos[i] = Comparecimentos;
    }
    
    fclose (fcomparecimentos);
    return num_comparecimentos;
    
}


void liberarComparecimentos (comparecimento **comparecimentos, int num_comparecimentos) {
    
    int i;
    for (i = 0; i < num_comparecimentos; i++) {
        if (comparecimentos[i] != NULL) {
            free (comparecimentos[i]);
            comparecimentos[i] = NULL;
        }
    }
    
}

void menuComparecimentos (comparecimento **comparecimentos, int *num_comparecimentos) {
    
    int opcao_comparecimento;
    do {
        printf ("------------- MENU COMPARECIMENTOS --------------\n");
        printf ("1. Mostrar comparecimento por UF e ano\n");
        printf ("2. Mostrar total de comparecimentos\n");
        printf ("0. Sair\n");
        printf ("--------------------------------------------------");
        scanf ("%d", &opcao_comparecimento);
        switch (opcao_comparecimento) {
            case 1:
                void mostrarComparecimentoUfAno (comparecimentos, num_comparecimentos);
                break;
            case 2:
                void mostrarComparecimentoTotal (comparecimentos, num_comparecimentos);
                break;
            case 0:
                printf ("Saindo de comparecimentos\n");
                break;
            default:
                printf ("Opcao invalida! escolha outra opcao!\n");
                break;
        }       
    } while (opcao_comparecimento != 0);
}

void mostrarComparecimentoUfAno (comparecimento **comparecimentos, int num_comparecimentos) {
    
    int uf;
    int ano;
    
    printf ("Digite o codigo da UF para ver os comparecimentos: ");
    scanf ("%d", &uf);
    printf ("Digite o ano para ver os comparecimentos: ");
    scanf ("%d", &ano);
    
    int encontrado = -1;
    int i;
    for (i = 0; i < num_comparecimentos; i++) {
        if (comparecimentos[i]->codigo_uf == uf && comparecimentos[i]->ano == ano) {
            encontrado = i;
            break;
        }
    }
    
    if (encontrado == -1) {
        printf ("Codigo da Uf ou ano digitados não foram registrados no sistema.\n");
        return;
    }

    printf ("--------------- COMPARECIMENTOS UF %d em %d -----------------\n", uf, ano);
    for (i = 0; i < num_comparecimentos; i++) {
        if (comparecimentos[encontrado]->codigo_uf == uf && comparecimentos[encontrado]->ano == ano)
            printf ("%d. CPF: %c\n", (i+1), comparecimentos[encontrado]->cpf);
    }
    printf ("-------------------------------------------------------------\n");
}

void mostrarComparecimentosTotal (comparecimento **comparecimentos, int num_comparecimentos, int num_ufs) {

    if (num_comparecimentos == 0) {
        printf ("Nenhum comparecimento registrado.\n");
        return;
    }

    int i, j;
    printf ("             COMPARECIMENTO TOTAL               \n");
    for (i = 0; i < num_ufs; i++) {
        for (j = 0; j < num_comparecimentos; j++) {
            printf ("-------------------- UF %d -------------------\n", comparecimentos[j]->codigo_uf);
            printf ("%d. \t%c\n", j, comparecimentos[j]->cpf);
        }
    }
}
