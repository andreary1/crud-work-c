struct UF {
    int codigo;
    char descricao[30];
    char sigla[3];
};

typedef struct UF UF;

void adicionarUF(UF *uf, int *total_ufs);
void mostrarDados(UF *uf, int total_ufs);
void alterarUF(UF *uf, int total_ufs);
void excluirUF(UF *uf);
void mostrarUF(UF *uf);