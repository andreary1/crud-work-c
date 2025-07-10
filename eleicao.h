#include "uf.h"

struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

typedef struct eleicao Eleicao;

int verificarAnoeCodigo(int codigo_uf, int ano);
void menuEleicao(Eleicao *eleicoes[]);
int carregarEleicoes(Eleicao *eleicoes[], int total_eleicoes);
void liberarEleicoes(Eleicao *eleicoes[], int total_eleicoes);
void inserirEleicao(Eleicao *eleicoes[]);
void mostrarDadosDasEleicoes(int total_eleicoes);
void alterarEleicao(Eleicao *eleicoes[]);
void excluirEleicao(Eleicao *eleicao, int *total_eleicoes);
void excluirEleicoesPorUF(int codigo_uf);
void mostrarEleicao(int total_eleicoes);