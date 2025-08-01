#ifndef COMPARECIMENTO_H
#define COMPARECIMENTO_H

#include "comparecimento.h"
#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"

struct comparecimento {
    char CPF[30];
    int ano;
    int codigo_uf;
};

typedef struct comparecimento Comparecimento;

int carregarComparecimentos(int *capacidade_comp);
void liberarComparecimentos(int capacidade_comp);
void mostrarComparecimentosEleicao(int total_comparecimentos, int num_eleicoes);
void mostrarTodosOsComparecimentos(int total_comparecimentos, int num_ufs, int num_eleicoes);

#endif //COMPARECIMENTO_H
