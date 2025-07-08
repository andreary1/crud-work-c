struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

typedef struct UF UF;

void menuUF(UF *ufs, int *total_ufs, int *codigo_uf_atual);
int carregarUFs(UF *ufs);
void adicionarUF(int *total_ufs, int *codigo_uf);
void mostrarDadosDasUFs(int total_ufs);
void alterarUF(UF *uf, int total_ufs);
void excluirUF(UF *uf, int *total_ufs, int *codigo_uf);
void mostrarUF(UF *uf, int total_ufs);