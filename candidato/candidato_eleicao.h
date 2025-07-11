struct candidato {
    int ano;
    int codigo_uf;
    char CPF[30];
    int numero;
};

typedef struct candidato Candidato;

int carregarCandidatos(Candidato *candidatos[], int total_cand);
void liberarCandidatos(Candidato *candidatos[], int total_cand);
void menuCandidatos(Candidato *candidatos[]);
void inserirCandidato(Candidato *candidatos[], int *total_cand);
void excluirCandidato();
void mostrarCandidatosPorUFeAno(Candidato *candidatos[], int total_cand);