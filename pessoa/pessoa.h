struct pessoa {
    char cpf[30];
    char titulo[20];
    char nome[30];
    char fone[15];
    char endereco[20];
};

enum opcaoPessoa {
    INSERIR_PESSOA = 1,
    ALTERAR_PESSOA = 2,
    EXCLUIR_PESSOA = 3,
    MOSTRAR_PESSOAS = 4,
    MOSTRAR_POR_TITULO = 5,
    SAIR_PESSOA = 0,
};

typedef struct pessoa Pessoa;

typedef enum opcaoPessoa opcaoPessoa;

void menuPessoas(Pessoa *pessoas[], int *num_pessoas);
int carregarPessoas(Pessoa *pessoas[], int num_pessoas);
void liberarPessoas(Pessoa *pessoas[], int num_pessoas);
void inserirPessoa(Pessoa *pessoas[], int *num_pessoas);
void excluirPessoa(Pessoa *pessoas[], int *num_pessoas);
void alterarPessoa(Pessoa *pessoas[], int num_pessoas);
void mostrarPessoas(Pessoa *pessoas[], int num_pessoas);
void MostrarPessoaPorTitulo(Pessoa *pessoas[], int num_pessoas);