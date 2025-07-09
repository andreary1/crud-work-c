struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

typedef struct candidato Candidato;

void menuCandidato();
void inserirCandidato();
void excluirCandidato();
void mostrarCandidato();