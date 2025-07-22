#include <stdio.h>
#include <stdlib.h>
#include "voto.h"
#include <string.h>
#include <time.h>

int carregarVotos (voto **votos, int *num_votos) {

    FILE *fvotos = fopen ("votos.data", "rb+");
    if (fvotos == NULL){
        printf ("Erro ao abrir arquivo.");
        return 0;
    }
    
    fseek (fvotos, 0, SEEK_END);
    long int num_votos = ftell (fvotos);
    num_votos /= sizeof(voto);
    
    int i;
    for (i = 0; i < num_votos; i++) {
        votos[i] = NULL;
    }
    
    fseek (fvotos, 0, SEEK_SET);
    for (i = 0; i < num_votos; i++){
        voto *voto = (voto *) malloc (sizeof(voto));
        fread (&voto, sizeof(voto), 1, fvoto);
        votos[i] = voto;
    }
    
    fclose (fvotos);
    return num_votos;

}

void liberarVotos (voto **votos, int *num_votos) {

    for (i = 0; i <= *num_votos; i++){
        if (votos[i] != NULL){
            free (votos[i]);
            votos[i] = NULL;
        }
    }
    free (votos);
    votos = NULL;
}

int verificarCandidatoVoto (voto **votos, int *num_votos) {

    int codigo_existe = 0;

    FILE *fvoto = fopen("votos.data", "rb+");
    voto voto;
    while (fread(&voto, sizeof(voto), 1, fvoto) == 1) {
        if (strcmp (voto.numero_candidato, candidato.numero) == 0)
            codigo_existe++;
    }
    fclose(fvoto);
    return codigo_existe;
}

int verificarDataHora (voto **votos, int *num_votos) {

    int codigo_existe = 0;

    FILE *fvoto = fopen("votos.data", "rb+");
    voto voto;
    while (fread(&voto, sizeof(voto), 1, fvoto) == 1) {
        if (voto.data_hora == data_hora)             //REVER ISSO!!!
            codigo_existe++;
    }
    fclose(fvoto);
    return codigo_existe;
}

void menuVoto (voto *votos[], int *num_votos) {
    int opcao_voto;
    do {
        printf("--------------OPCOES PARA VOTOS--------------\n");
        printf("1. Votar\n");
        printf("2. Mostrar votos por candidato\n");
        printf("3. Mostrar todos os votos\n");
        printf("4. Sair\n");
        printf("---------------------------------------------\n");
        scanf("%d", &opcao_voto);
        switch (opcao_voto) {
            case 1:
                inserirVoto(votos, num_votos);
                break;
            case 2:
                mostrarVotosCandidato(votos, num_votos);
                break;
            case 3:
                mostrarTudoVotos(votos, num_votos);
                break;
            case 4:
                printf("Saindo do menu de votos...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao_voto != 4);
}

void inserirVoto (voto **votos, int *num_votos) {
    
    voto *novoVoto = (voto *) malloc (sizeof(voto));
    
    if (novoVoto == NULL){
        printf ("Erro ao alocar memória");
        return votos;
    }

    int max_votos = 20;
        
    if ((num_votos - 1) >= max_votos) {
        max_votos = max_votos * 2;
        votos = realloc(votos, max_votos * sizeof(voto *));
        if (votos == NULL) {
            printf("Erro ao realocar memória");
            free(novoVoto);
            return;
        }
    }

    int codigo_uf;

    printf("Digite o codigo da UF: ");
    scanf("%d", &codigo_uf);
    if (!verificarCodigo(codigo_uf)) {
        printf("Codigo de UF não cadastrado.\n");
        free(novoVoto);
        return;
    }

    int ano;

    printf("Digite o ano da eleicao: ");
    scanf("%d", &ano);
    if(!verificarAno(ano)) {
        printf("Ano não cadastrado.\n");
        free(novoVoto);
        return;
    }

    time_t horavoto; // time_t armazena o numero de segundos (Unix epoch)
    struct tm *informacoestempo;  // a struct tm armazena as informacoes mais especificas sobre cada "parte" do tempo (ex: ano, mes, hora, minuto, horário de verão)
    time (&horavoto);  // a função time passa para horavoto o tempo atual
    informacoestempo = localtime (&horavoto);  // local time tranforma para o horario local
    if (!verificarDataHora(horavoto)) {
        printf("Já existe um voto registrado nessa mesma data e hora.\n");
        free(novoVoto);
        return;
    }

    novoVoto->codigo_uf = codigo_uf;                 //REVER ISSO!!!
    novoVoto->ano = ano;                             //REVER ISSO!!!
    novoVoto->data_hora = horavoto;                  //REVER ISSO!!!

    int numero_candidato;

    printf ("Digite o numero do candidato o qual deseja votar: ");
    scanf ("%d", &numero_candidato);
    if (!verificarCandidato(numero_candidato)) {
        printf ("Candidato não cadastrado.\n");
        free (novoVoto);
        return;
    }
    ler (votos[*num_votos]->numero_candidato, sizeof(votos[*num_votos]->numero_candidato));

    FILE *fvotos = fopen("votos.data", "rb+");
    if (fvotos == NULL) {
        printf("Erro ao abrir o arquivo de votos.\n");
        free(novoVoto);
        return;
    }
    fseek(fvotos, 0, SEEK_END);
    fwrite (votos[*num_votos], sizeof(voto), 1, fvotos);
    fclose(fvotos);

    (*num_votos)++;
    free (novoVoto);

    printf("Voto registrado com sucesso!\n");
}

void mostrarVotosCandidato (voto **voto, int *num_votos) {

    if (*num_votos == 0) {
        printf("Nenhum voto registrado.\n");
        return;
    }
    int numero_candidato;
    printf ("Digite um numero de candidato para ver os votos:");
    scanf ("%d", &numero_candidato);

    if (!verificarCandidato(numero_candidato)) {          // REVER ISSO!!!
        printf("Candidato não cadastrado.\n");
        return;
    }

    printf ("--------VOTOS REGISTRADOS DO CANDIDATO DE CODIGO %d --------\n", numero_candidato);
    for (i = 0; i < *num_votos; i++) {
        if (votos[i]->numero_candidato == numero_candidato) {
            printf("%d. \tAno: %d \tUF: %d \tCandidato: %d \tData/Hora: %s\n", 
                i + 1, votos[i]->ano, votos[i]->codigo_UF, votos[i]->numero_candidato, asctime(&votos[i]->data_hora));  // asctime coverte a data e hora para uma forma legível
        }
    }
    printf ("------------------------------------------------------------\n");
}

void mostrarTudoVotos (voto **votos, int *num_votos) {

    if (*num_votos == 0) {
        printf ("Nenhum voto registrado.\n)");
        return;
    }

    printf ("-------------- VOTOS REGISTRADOS ATÉ O MOMENTO --------------\n");
    for (int i = 0; i < *num_votos; i++) {
        printf("%d. \tAno: %d \tUF: %d \tCandidato: %d \tData/Hora: %s\n", 
            i + 1, votos[i]->ano, votos[i]->codigo_UF, votos[i]->numero_candidato, asctime(&votos[i]->data_hora));
    }
    printf ("------------------------------------------------------------\n");
}
