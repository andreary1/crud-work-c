struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

typedef struct candidato Candidato;

void carregarCandidatoes(Candidato *candidatos);
void menuCandidato(Candidato *candidatos, int *total_candidatos);
void inserirCandidato();
void excluirCandidato();
void mostrarCandidato();