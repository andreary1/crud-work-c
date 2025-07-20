#ifndef UF_H
#define UF_H

struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

enum opcaoUF {
    INSERIR_UF = '1',
    ALTERAR_UF = '2',
    EXCLUIR_UF = '3',
    MOSTRAR_DADOS = '4',
    MOSTRAR_UF = '5',
    SAIR_UF = '0',
};

typedef enum opcaoUF opcaoUF;

typedef struct UF UF;

void limparBuffer();
void ler(char sentenca[], int tamanho);
void lerNaoObrigatorio(char sentenca[], int tamanho);
int verificarCodigo(int codigo_uf, int num_ufs);
int verificarSigla(char sig[], int num_ufs);
int carregarUFs(int *total_ufs);
void liberarUFs(int total_ufs);
void adicionarUF(int *num_ufs, int *capacidade_ufs);
void mostrarDadosDasUFs(int num_ufs);
void alterarUF(int num_ufs);
void excluirUF(int *num_ufs, int *num_eleicoes, int *num_candidatos, int *num_votos,int *num_comparecimentos);
void exclusaoEleicoesPorUF(int *num_eleicoes, int *num_candidatos, int *num_votos, int *num_comparecimentos, int codigo);
void mostrarUF(int num_ufs);
#endif