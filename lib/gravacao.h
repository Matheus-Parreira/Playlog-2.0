#define MAX 10
#include <time.h>
#include <stdio.h>

// Estrutura que representa os dados de uma playlist individual dentro do índice
typedef struct Elemento_Indice
{
    int id_playlist;
    char nome_playlist[25];
    int qtd_musicas_atual;
    int qtd_musicas_total;
    struct tm ultima_modificacao_playlist;
}Elem_Ind;

// Estrutura que representa o índice (que armazenará as playlists existentes)
typedef struct Indice
{
    int qtd_atual_playlists;
    int qtd_total_playlists;
    Elem_Ind arr_ind[MAX];
}Ind;

Ind* inicializar_indice();
int verificar_id_indice(Ind* in, int qtd, int id);
int verificar_nome_playlist(Ind* in, char temp[]);
void obter_nome_arquivo_playlist(char nome_arquivo[], char nome_playlist[]);
void editar_nome_playlist(Play* pl, Ind* in);
void carregar_indice(Ind* in);
void exibir_cabecalho_indice();
void linha_indice(Ind* in, int i);
void carregar_playlist(Play* pl, Ind* in, int i);
int procurar_playlist(Play* pl, Ind* in, int qtd);
int nova_playlist_indice(Play* pl, Ind* in);
int excluir_playlist_indice(Play* pl, Ind* in);
void espacamento_arquivo(FILE* f_gravacao, char nome[], int qtd);
Elem* ultima_modificacao_musica(Play* pl);
void obter_dados_musica_gravacao(Ind* in, Play* pl);
void gravar_cabecalho_playlist_arquivo(Play* pl, FILE* f_gravacao);
void gravar_linha_musica(FILE* f_gravacao, Elem* no);
void gravar_playlist(Play* pl, Ind* in);
void gravar_cabecalho_indice(FILE* f_index);
void gravar_linha_indice(FILE* f_index, Ind* in, int i);
void gravar_indice(Ind* in);