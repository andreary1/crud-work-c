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

int carregarComparecimentos(Comparecimento *comparecimentos[], int total_comparecimentos);
void liberarComparecimentos(Comparecimento *comparecimentos[], int total_comparecimentos);
void mostrarComparecimentosEleicao(Comparecimento *comparecimentos[], int total_comparecimentos);
void mostrarTodosOsComparecimentos(Comparecimento *comparecimentos[], int total_comparecimentos);

#endif //COMPARECIMENTO_H
