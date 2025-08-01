#ifndef CANDIDATO_ELEICAO_H
#define CANDIDATO_ELEICAO_H

#include "../UF/uf.h"

struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

enum opcaoCandidato {
    INSERIR_CANDIDATO = '1',
    EXCLUIR_CANDIDATO = '2',
    MOSTRAR_CANDIDATOS_POR_ANO = '3',
    MOSTRAR_TODOS_OS_CANDIDATOS = '4',
    SAIR_CANDIDATO = '0'
};

typedef struct candidato Candidato;
typedef enum opcaoCandidato opcaoCandidato;

int carregarCandidatos(int *capacidade_candidatos);
void liberarCandidatos(int capacidade_candidatos);
int verificarCandidato(int numero, int ano, int codigo_uf, int num_candidatos);
int verificarCandidatura(char cpf[], int ano, int num_candidatos);
void inserirCandidato(int *total_cand, int *capacidade_cand, int num_ufs, int num_pessoas, int num_eleicoes);
void excluirCandidato(int *num_candidatos, int *num_votos, int *num_comparecimentos);
void exclusaoVotosEComparecimentos(int *num_votos, int *num_comparecimentos, Candidato c);
void mostrarCandidatosPorUFeAno(int total_cand, int total_ufs, int num_eleicoes);
void mostrarTodosOsCandidatos(int total_cand, int num_eleicoes, int total_ufs);

#endif