struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

typedef struct UF UF;

int verificarCodigo(int codigo_uf);
void menuUF(UF *ufs, int *total_ufs);
int carregarUFs(UF *ufs);
void adicionarUF(int *total_ufs);
void mostrarDadosDasUFs(int total_ufs);
void alterarUF(UF *uf, int total_ufs);
void excluirUF(UF *uf, int *total_ufs);
void mostrarUF(int total_ufs);