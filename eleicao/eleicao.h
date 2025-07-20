struct eleicao {
    int ano;
    int codigo_uf;
    char descricao[50];
};

enum opcaoEleicao {
    INSERIR_ELEICAO = '1',
    ALTERAR_ELEICAO = '2',
    EXCLUIR_ELEICAO = '3',
    MOSTRAR_DADOS_ELEICOES = '4',
    MOSTRAR_ELEICAO = '5',
    SAIR_ELEICAO = '0',
};

typedef struct eleicao Eleicao;
typedef enum opcaoEleicao opcaoEleicao;

int verificarAnoeCodigo(int codigo_uf, int ano, int num_eleicoes);
int carregarEleicoes(int *capacidade_eleicoes);
void liberarEleicoes(int total_eleicoes);
void inserirEleicao(int *num_eleicoes, int *capacidade_eleicoes, int num_ufs);
void mostrarDadosDasEleicoes(int total_eleicoes);
void alterarEleicao(int total_eleicoes);
void excluirEleicao(int *total_eleicoes);
void mostrarEleicao(int total_eleicoes);