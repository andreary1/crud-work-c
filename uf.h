struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

typedef struct UF UF;

int carregarUFs(UF *ufs);
void adicionarUF(UF *uf, int *total_ufs, int *codigo_uf);
void mostrarDados(int total_ufs);
void alterarUF(UF *uf, int total_ufs);
void excluirUF(UF *uf, int *total_ufs, int *codigo_uf);
void mostrarUF(UF *ufs, int total_ufs);