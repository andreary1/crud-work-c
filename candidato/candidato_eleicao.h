#include "../UF/uf.h"

struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

typedef struct candidato Candidato;

int verificarCPF(Candidato *candidatos[],char cpf[], int total_cand);
int carregarCandidatos(Candidato *candidatos[], int total_cand);
void liberarCandidatos(Candidato *candidatos[], int total_cand);
void menuCandidatos(Candidato *candidatos[], UF *ufs[]);
void inserirCandidato(Candidato *candidatos[], int *total_cand);
void excluirCandidato();
void mostrarCandidatosPorUFeAno(Candidato *candidatos[], int total_cand);