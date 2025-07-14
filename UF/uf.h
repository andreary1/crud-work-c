#ifndef UF_H
#define UF_H
struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

enum opcaoUF {
    INSERIR_UF = 1,
    ALTERAR_UF = 2,
    EXCLUIR_UF = 3,
    MOSTRAR_DADOS = 4,
    MOSTRAR_UF = 5,
    SAIR_UF = 0,
};

typedef enum opcaoUF opcaoUF;

typedef struct UF UF;

void limparBuffer();
void ler(char sentenca[], int tamanho);
int verificarCodigo(int codigo_uf);
int carregarUFs(UF *ufs[], int total_ufs);
void liberarUFs(UF *ufs[], int total_ufs);
void menuUF(UF *ufs[], int *num_ufs);
void adicionarUF(UF *ufs[], int *total_ufs);
void mostrarDadosDasUFs(UF *ufs[], int num_ufs);
void alterarUF(UF *ufs[], int num_ufs);
void excluirUF(UF *ufs[], int *total_ufs);
void mostrarUF(UF *ufs[], int num_ufs);
#endif