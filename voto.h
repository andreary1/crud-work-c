#pragma once

struct voto {
    ano;
    codigo_UF;
    numero_candidato;
    data_hora[10];
};

enum opcao_voto {
    INSERIR_VOTO = 1;
    MOSTRAR_VOTOS_CANDIDATOS = 2;
    MOSTRAR_TUDO_VOTOS = 3;
    SAIR_VOTOS = 4;
}

typedef enum opcao_voto opcao_voto;

typedef struct voto voto;

void menuVoto (voto *votos[], int *num_votos); //FEITO 
int carregarVotos (voto *votos[], int num_votos); // FEITO
void liberarVotos (voto *votos[], int num_votos); // FEITO
void inserirVoto (voto *votos[], int *num_votos);
void mostrarVotosCandidato (voto *votos[], int *num_votos); 
void mostrarTudoVotos (voto *votos[], int *num_votos);
int verificarCandidatoVoto (voto *votos[], int num_votos); // FEITO // verifica se o candidato existe
int verificarDataHora (voto *votos[], int num_votos);  // FEITO // verifica se já não tiveram votos nesse mesmo momento (devido PK)
// void verificarTitulo (voto *votos[], int *num_votos);  // FEITO // VER SE PRECISA MESMO