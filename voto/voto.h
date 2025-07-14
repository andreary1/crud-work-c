#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"

struct voto {
    int ano;
    int codigo_uf;
    int numero_candidato;
    char data_hora[25];
};

typedef struct voto Voto;

void menuVotos(Voto *votos[], int *num_votos, UF *ufs[], int *num_ufs, Candidato *candidatos[], int *num_candidatos);
int carregarVotos(Voto *votos[], int num_votos);
void liberarVotos(Voto *votos[], int num_votos);
void inserirVoto(Voto *votos[], int *num_votos);
void mostrarVotosPorCandidato(Voto *votos[], int num_votos, UF *ufs[], int num_ufs, Candidato *candidatos[], int num_candidatos);