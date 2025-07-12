
struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

typedef struct eleicao Eleicao;

int verificarAnoeCodigo(int codigo_uf, int ano);
int carregarEleicoes(Eleicao *eleicoes[], int total_eleicoes);
void liberarEleicoes(Eleicao *eleicoes[], int total_eleicoes);
void menuEleicao(Eleicao *eleicoes[], int *num_eleicoes);
void inserirEleicao(Eleicao *eleicoes[], int *num_eleicoes);
void mostrarDadosDasEleicoes(Eleicao *eleicoes[], int total_eleicoes);
void alterarEleicao(Eleicao *eleicoes[], int total_eleicoes);
void excluirEleicao(Eleicao *eleicoes[], int *total_eleicoes);
void mostrarEleicao(Eleicao *eleicoes[], int total_eleicoes);