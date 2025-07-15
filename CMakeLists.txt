cmake_minimum_required(VERSION 3.31)
project(trabalho_ajalmar C)

set(CMAKE_C_STANDARD 99)

add_executable(trabalho_ajalmar
        main.c
        UF/uf.c pessoa/pessoa.c
        eleicao/eleicao.c
        candidato/candidato_eleicao.c
        voto/voto.c
        comparecimento/comparecimento.c
)
