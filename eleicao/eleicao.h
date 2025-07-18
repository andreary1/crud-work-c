
struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

typedef struct eleicao Eleicao;

int verificarAnoeCodigo(int codigo_uf, int ano, int num_eleicoes);
int carregarEleicoes(int *capacidade_eleicoes);
void liberarEleicoes(int total_eleicoes);
void inserirEleicao(int *num_eleicoes, int *capacidade_eleicoes, int num_ufs);
void mostrarDadosDasEleicoes(int total_eleicoes);
void alterarEleicao(int total_eleicoes);
void excluirEleicao(int *total_eleicoes);
void mostrarEleicao(int total_eleicoes);