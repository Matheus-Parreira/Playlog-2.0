#include <time.h>

// Estrutura que representa uma música de forma individual
typedef struct musica
{
    int id_musica;
    char nome_musica[45];
    char nome_artista[30];
    char ano_lancamento[10];
    char pais_musica[25];
    struct tm data_adicao;

}Musica;

// Estrutura que representa uma música como um nó da lista (com ponteiros para o elemento anterior e o próximo)
typedef struct elemento
{
    Musica dados;
    struct elemento* prox;
    struct elemento* ant;
}Elem;

// Estrutura que representa a playlist, com suas informações (id, nome e quantidade de músicas) e ponteiro para início
typedef struct playlist
{
    struct elemento* inicio_playlist;
    int qtd_musicas_atual;
    int qtd_musicas_total;
    char nome_playlist[25];
    int id_playlist;
}Play;

// Cabeçalhos das funções do arquivo playlist.c
Play* inicializar_playlist();
void final(int msg);
time_t converter_data(Elem* no);
void excluir_playlist(Play* pl);
int condicao_ordem_playlist(Elem* atual, Elem* no, int ordem);
int condicao_ordem_playlist_musica(Elem* atual, Elem* no, int ordem);
void inserir_musica_playlist(Play* pl, Elem* no, int ord_art, int ord_msc);
Elem* cria_no(Musica msc);
void data_adicao_musica(Musica* msc);
void input_musica(Musica* msc);
void nova_musica(Play* pl, int ord_art, int ord_msc);
int tamanho_string(const char* string, int qtd_max);
void espacamento_string(char string[], int qtd_max);
void linha_musica(Elem* no);
void exibir_cabecalho(Play* pl);
int exibir_playlist(Play* pl);
int input_id(int msc_ou_play);
void exibir_unica_musica(Play* pl, Elem* no);
Elem* verificar_no_id(Play* pl, int id);
void buscar_musica_id(Play* pl);
int condicao_busca_string(Elem* no, char pesquisa[], int msc_ou_art);
void buscar_musica_string(Play* pl, int msc_ou_art);
int opcoes_busca(Play* pl);
void remocao_musica(Play* pl, Elem* no);
void excluir_musica(Play* pl);
void editar_musica(Play* pl);
void codigos_ordenacao(int opcao, int* ord_art, int* ord_msc);
int opcoes_ordenacao(Play* pl);