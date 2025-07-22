#pragma once

struct comparecimento {
    char cpf;
    char ano;
    char codigo_uf;
};

enum opcaoComparecimento {
    COMPARECIMENTO_UF_ANO = 1,
    COMPARECIMENTO_TOTAL = 2,
    SAIR_COMPARECIMENTO = 0
};

typedef struct comparecimento comparecimento;

typedef enum opcaoComparecimento opcaoComparecimento;

void menuComparecimento (comparecimento **comparecimentos, int *num_comparecimentos);
int carregarComparecimentos(comparecimento **comparecimentos, int num_comparecimentos);
void liberarComparecimentos(comparecimento **comparecimentos, int num_comparecimentos);
void mostrarComparecimentoUfAno(comparecimento **comparecimentos, int num_comparecimentos);
void mostrarComparecimentoTotal(comparecimento **comparecimentos, int num_comparecimentos);
void associarCodigoUf (comparecimento **comparecimentos, int *num_comparecimentos);
void associarAno (comparecimento **comparecimentos, int *num_comparecimentos);
void associarCpf (comparecimento **comparecimentos, int *num_comparecimentos);
