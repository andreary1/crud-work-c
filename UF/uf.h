#pragma once
struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

typedef struct UF UF;

void ler(char sentenca[], int tamanho);
void carregarArquivos();
int verificarCodigo(int codigo_uf);
void menuUF(UF *ufs[]);
int carregarUFs(UF *ufs[], int total_ufs);
void liberarUFs(UF *ufs[], int total_ufs);
void adicionarUF(UF *ufs[], int *total_ufs);
void mostrarDadosDasUFs(UF *ufs[], int num_ufs);
void alterarUF(UF *ufs[], int num_ufs);
void excluirUF(UF *ufs[], int *total_ufs);
void mostrarUF(UF *ufs[], int num_ufs);