struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

typedef struct eleicao Eleicao;

void menuEleicao(Eleicao *eleicoes, int *total_eleicoes, int *codigo_uf_atual);
int carregarEleicoes(UF *eleicoes);
void adicionarEleicoes(int *total_eleicoes, int *codigo_uf);
void mostrarDadosDasEleicoes(int total_eleicoes);
void alterarEleicao(UF *uf, int total_eleicoes);
void excluirEleicao(UF *uf, int *total_eleicoes, int *codigo_uf);
void mostrarEleicao(UF *uf, int total_eleicoes);