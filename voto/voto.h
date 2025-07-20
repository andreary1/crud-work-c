#include "../UF/uf.h"
#include "../candidato/candidato_eleicao.h"
#include "../comparecimento/comparecimento.h"

struct voto {
    int ano;
    int codigo_uf;
    int numero_candidato;
    char data_hora[25];
};

enum opcao_voto {
    INSERIR_VOTO = '1',
    MOSTRAR_VOTOS_CANDIDATOS = '2',
    MOSTRAR_TUDO_VOTOS = '3',
    MOSTRAR_COMPARECIMENTOS_ELEICAO = '4',
    MOSTRAR_COMPARECIMENTOS = '5',
    CONTAGEM_VOTOS = '6',
    SAIR_VOTOS_COMPARECIMENTOS = '0',
};

typedef struct voto Voto;
typedef enum opcao_voto opcaoVoto;

int carregarVotos(int *capacidade_votos);
void liberarVotos(int capacidade_votos);
int verificarVoto(int ano, char cpf[]);
void inserirVoto(int *num_votos, int *capacidade_votos, int *num_comparecimentos, int *capacidade_comparecimentos,
    int num_pessoas, int num_eleicoes, int num_candidatos);
void mostrarVotosPorCandidato(int num_votos, int num_ufs, int num_candidatos, int num_eleicoes);
void mostrarTodosOsVotos(int num_votos, int num_ufs, int num_eleicoes);
void contagemDeVotos(int num_votos, int num_candidatos, int num_eleicoes);