#include <stdio.h>

#include "UF/uf.h"
#include "eleicao/eleicao.h"
#include "candidato/candidato_eleicao.h"
#include "pessoa/pessoa.h"
#include "voto/voto.h"

UF **ufs = NULL;
Pessoa **pessoas = NULL;
Eleicao **eleicoes = NULL;
Candidato **candidatos = NULL;
Voto **votos = NULL;
Comparecimento **comparecimentos = NULL;

void carregarArquivos();

int main() {

    carregarArquivos();

    int capacidade_ufs = 10;
    int capacidade_pessoas = 10;
    int capacidade_eleicoes = 10;
    int capacidade_candidatos = 10;
    int capacidade_votos = 10;
    int capacidade_comp = 10;

    int num_ufs = carregarUFs(&capacidade_ufs);
    int num_pessoas = carregarPessoas(&capacidade_pessoas);
    int num_eleicoes = carregarEleicoes(&capacidade_eleicoes);
    int num_candidatos = carregarCandidatos(&capacidade_candidatos);
    int num_votos = carregarVotos(&capacidade_votos);
    int num_comparecimentos = carregarComparecimentos(&capacidade_comp);

    char opcao;
    do {
        printf("==========MENU==========\n");
        printf("1 - Unidades Federativas\n");
        printf("2 - Pessoas\n");
        printf("3 - Eleicoes\n");
        printf("4 - Candidatos\n");
        printf("5 - Votos/comparecimentos\n");
        printf("0 - Sair\n");
        printf("========================\n");
        scanf("%c", &opcao);
        limparBuffer();
        switch (opcao) {
            case '1':
                opcaoUF opcao_uf;
                do {
                    printf("====OPCOES PARA UNIDADES FEDERATIVAS====\n");
                    printf("1. Inserir UF\n");
                    printf("2. Alterar UF\n");
                    printf("3. Excluir UF\n");
                    printf("4. Mostrar dados de todas as UFs\n");
                    printf("5. Mostrar dados de uma UF\n");
                    printf("0. Sair\n");
                    printf("=========================================\n");
                    scanf("%c", &opcao_uf);
                    limparBuffer();
                    switch (opcao_uf) {
                        case INSERIR_UF: adicionarUF(&num_ufs, &capacidade_ufs);
                            break;
                        case ALTERAR_UF: alterarUF(num_ufs);
                            break;
                        case EXCLUIR_UF: excluirUF(&num_ufs, &num_eleicoes);
                            break;
                        case MOSTRAR_DADOS: mostrarDadosDasUFs(num_ufs);
                            break;
                        case MOSTRAR_UF: mostrarUF(num_ufs);
                            break;
                        case SAIR_UF: printf("Saindo\n");
                            break;
                        default:
                            printf("Opcao invalida!\nDigite outra opcao\n");
                            break;
                    }
                } while (opcao_uf != '0');
                break;
            case '2':
                opcaoPessoa opcao_pessoa;
                do {
                    printf("===========OPCOES PARA PESSOAS=============\n");
                    printf("1. Inserir Pessoa\n");
                    printf("2. Alterar Pessoa\n");
                    printf("3. Excluir Pessoa\n");
                    printf("4. Mostrar dados de todas as pessoas\n");
                    printf("5. Mostrar uma pessoa pelo titulo de eleitor\n");
                    printf("0. Sair\n");
                    printf("============================================\n");
                    scanf("%c", &opcao_pessoa);
                    limparBuffer();
                    switch (opcao_pessoa) {
                        case INSERIR_PESSOA: inserirPessoa(&num_pessoas, &capacidade_pessoas);
                            break;
                        case ALTERAR_PESSOA: alterarPessoa(num_pessoas);
                            break;
                        case EXCLUIR_PESSOA: excluirPessoa(&num_pessoas, &num_candidatos);
                            break;
                        case MOSTRAR_PESSOAS: mostrarPessoas(num_pessoas);
                            break;
                        case MOSTRAR_POR_TITULO: mostrarPorTitulo(num_pessoas);
                            break;
                        case SAIR_PESSOA: printf("Saindo\n");
                            break;
                        default: printf("Opcao invalida!\nDigite outra opcao\n");
                            break;
                    }
                } while (opcao_pessoa != '0');
                break;
            case '3':
                opcaoEleicao opcao_eleicao;
                do {
                    printf("=========OPCOES PARA ELEICOES=========\n");
                    printf("1. Inserir Eleicao\n");
                    printf("2. Alterar Eleicao\n");
                    printf("3. Excluir Eleicao\n");
                    printf("4. Mostrar dados de todas as eleicoes\n");
                    printf("5. Mostrar dados de uma eleicao\n");
                    printf("0. Sair\n");
                    printf("=======================================\n");
                    scanf("%c", &opcao_eleicao);
                    limparBuffer();
                    switch (opcao_eleicao) {
                        case INSERIR_ELEICAO: inserirEleicao(&num_eleicoes, &capacidade_eleicoes, num_ufs);
                            break;
                        case ALTERAR_ELEICAO: alterarEleicao(num_eleicoes);
                            break;
                        case EXCLUIR_ELEICAO: excluirEleicao(&num_eleicoes);
                            break;
                        case MOSTRAR_DADOS_ELEICOES: mostrarDadosDasEleicoes(num_eleicoes);
                            break;
                        case MOSTRAR_ELEICAO: mostrarEleicao(num_eleicoes);
                            break;
                        case SAIR_ELEICAO: printf("Saindo\n");
                            break;
                        default: printf("Opcao invalida!\nDigite outra opcao\n");
                            break;
                    }
                } while (opcao_eleicao != '0');
                break;
            case '4':
                opcaoCandidato opcao_candidato;
                do {
                    printf("==============OPCOES PARA CANDIDATOS==============\n");
                    printf("1. Inserir candidato\n");
                    printf("2. Excluir candidato\n");
                    printf("3. Mostrar candidatos de uma eleicao por UF e ano\n");
                    printf("4. Mostrar candidatos das eleicoes por ano\n");
                    printf("0. Sair\n");
                    printf("==================================================\n");
                    scanf("%c", &opcao_candidato);
                    limparBuffer();
                    switch (opcao_candidato) {
                        case INSERIR_CANDIDATO: mostrarPessoas(num_pessoas);
                            inserirCandidato(&num_candidatos, &capacidade_candidatos, num_ufs, num_pessoas, num_eleicoes);
                            break;
                        case EXCLUIR_CANDIDATO: excluirCandidato(&num_candidatos, &num_votos, &num_comparecimentos);
                            break;
                        case MOSTRAR_CANDIDATOS_POR_ANO: mostrarCandidatosPorUFeAno(num_candidatos, num_ufs, num_eleicoes);
                            break;
                        case MOSTRAR_TODOS_OS_CANDIDATOS: mostrarTodosOsCandidatos(num_candidatos, num_eleicoes, num_ufs);
                            break;
                        case SAIR_CANDIDATO: printf("Saindo\n");
                            break;
                        default:
                            printf("Opcao invalida!\nDigite outra opcao\n");
                            break;
                    }
                } while (opcao_candidato != '0');
                break;
            case '5':
                opcaoVoto opcao_voto;
                do {
                    printf("===================OPCOES PARA VOTO===================\n");
                    printf("1. Inserir Voto\n");
                    printf("2. Mostrar todos os votos por candidato de uma eleicao\n");
                    printf("3. Mostrar todos os votos das eleicoes\n");
                    printf("4. Mostrar comparecimentos por UF e ano\n");
                    printf("5. Mostrar todos os comparecimentos\n");
                    printf("6. Contagem de Votos\n");
                    printf("0. Sair\n");
                    printf("======================================================\n");
                    scanf("%c", &opcao_voto);
                    limparBuffer();
                    switch (opcao_voto) {
                        case INSERIR_VOTO: mostrarPessoas(num_pessoas);
                            inserirVoto(&num_votos, &capacidade_votos, &num_comparecimentos, &capacidade_comp,
                                num_pessoas, num_eleicoes, num_candidatos);
                            break;
                        case MOSTRAR_VOTOS_CANDIDATOS: mostrarVotosPorCandidato(num_votos, num_ufs, num_candidatos, num_eleicoes);
                            break;
                        case MOSTRAR_TUDO_VOTOS: mostrarTodosOsVotos(num_votos, num_ufs, num_eleicoes);
                            break;
                        case MOSTRAR_COMPARECIMENTOS_ELEICAO: mostrarComparecimentosEleicao(num_comparecimentos, num_eleicoes);
                            break;
                        case MOSTRAR_COMPARECIMENTOS: mostrarTodosOsComparecimentos(num_comparecimentos, num_ufs, num_eleicoes);
                            break;
                        case CONTAGEM_VOTOS: contagemDeVotos(num_votos, num_candidatos, num_eleicoes);
                            break;
                        case SAIR_VOTOS_COMPARECIMENTOS: printf("Saindo\n");
                            break;
                        default: printf("Opcao invalida!\nDigite outra opcao\n");
                            break;
                    }
                } while (opcao_voto != '0');
                break;
            case '0':
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != '0');

    liberarUFs(capacidade_ufs);
    liberarPessoas(capacidade_pessoas);
    liberarEleicoes(capacidade_eleicoes);
    liberarCandidatos(capacidade_candidatos);
    liberarVotos(capacidade_votos);
    liberarComparecimentos(capacidade_comp);

    return 0;
}

void carregarArquivos() {
    FILE *arquivos[] = {
        fopen("uf.data", "rb+"),
        fopen("pessoas.data", "rb+"),
        fopen("eleicao.data", "rb+"),
        fopen("candidatos.data", "rb+"),
        fopen("votos.data", "rb+"),
        fopen("comparecimentos.data", "rb+")
    };
    for (int i = 0; i < 6; i++) {
        if (arquivos[i] == NULL) {
            const char *nomes[] = {
                "uf.data", "pessoas.data", "eleicao.data",
                "candidatos.data", "votos.data", "comparecimentos.data"
            };
            arquivos[i] = fopen(nomes[i], "wb+");
            if (arquivos[i] == NULL) {
                printf("Erro ao criar arquivo %s\n", nomes[i]);
            } else {
                fclose(arquivos[i]);
            }
        } else {
            fclose(arquivos[i]);
        }
    }
}