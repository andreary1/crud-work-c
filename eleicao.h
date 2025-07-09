struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

typedef struct eleicao Eleicao;

void menuEleicao(Eleicao *eleicoes, int *total_eleicoes);
int carregarEleicoes(Eleicao *eleicoes);
void inserirEleicao(int *total_eleicoes);
void mostrarDadosDasEleicoes(int total_eleicoes);
void alterarEleicao(Eleicao *eleicao, int total_eleicoes);
void excluirEleicao(Eleicao *eleicao, int *total_eleicoes);
void mostrarEleicao(int total_eleicoes);