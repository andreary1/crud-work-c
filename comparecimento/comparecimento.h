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
void mostrarComparecimentosEleicao(int total_comparecimentos);
void mostrarTodosOsComparecimentos(int total_comparecimentos);

#endif //COMPARECIMENTO_H
