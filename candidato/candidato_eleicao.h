#include "../UF/uf.h"

struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

enum opcaoCandidato {
    INSERIR_CANDIDATO = 1,
    EXCLUIR_CANDIDATO = 2,
    MOSTRAR_CANDIDATOS_POR_ANO = 3,
    MOSTRAR_TODOS_OS_CANDIDATOS = 4,
    SAIR_CANDIDATO = 0
};

typedef struct candidato Candidato;

int carregarCandidatos(Candidato *candidatos[], int total_cand);
void liberarCandidatos(Candidato *candidatos[], int total_cand);
void menuCandidatos(Candidato *candidatos[], UF *ufs[], int *num_cand, int *total_ufs);
int verificarNumero(int numero);
void inserirCandidato(Candidato *candidatos[], int *total_cand);
void excluirCandidato(Candidato *candidatos[], int *total_cand);
void mostrarCandidatosPorUFeAno(Candidato *candidatos[], UF *ufs[], int total_cand, int total_ufs);
void mostrarTodosOsCandidatos(Candidato *candidatos[], UF *ufs[], int total_cand, int total_ufs);