struct voto {
    int ano;
    int codigo_uf;
    int numero_candidato;
    char data_hora[25];
};

typedef struct voto Voto;

int carregarVotos(Voto *votos[], int num_votos);
void liberarVotos(Voto *votos[], int num_votos);
void inserirVoto(Voto *votos[], int *num_votos);