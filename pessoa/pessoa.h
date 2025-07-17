struct pessoa {
    char cpf[30];
    char titulo[20];
    char nome[30];
    char fone[15];
    char endereco[20];
    char data_nascimento[20];
};

enum opcaoPessoa {
    INSERIR_PESSOA = '1',
    ALTERAR_PESSOA = '2',
    EXCLUIR_PESSOA = '3',
    MOSTRAR_PESSOAS = '4',
    MOSTRAR_POR_TITULO = '5',
    SAIR_PESSOA = '0',
};

typedef struct pessoa Pessoa;

typedef enum opcaoPessoa opcaoPessoa;

int verificarCPF(char cpf[]);
int verificarTitulo(char titulo[]);
int carregarPessoas(int *capacidade_pessoas);
void liberarPessoas(int num_pessoas);
void inserirPessoa(int *num_pessoas, int *capacidade_pessoas);
void excluirPessoa(int *num_pessoas);
void alterarPessoa(int num_pessoas);
void mostrarPessoas(int num_pessoas);
void mostrarPorTitulo(int num_pessoas);