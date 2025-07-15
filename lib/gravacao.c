#include "playlist.h"
#include "gravacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Ind* inicializar_indice() // Inicializa a estrutura que armazenará o indice
{
    Ind* in;
    in = (Ind*) malloc(sizeof(Ind)); // Aloca memória para ela
    return in;
}

int verificar_id_indice(Ind* in, int qtd, int id) // Verifica onde/se uma playlist está no índice dado seu ID
{
    int i = 0;
    while(i < qtd && in->arr_ind[i].id_playlist != id)
        i++;
    return i;
}

int verificar_nome_playlist(Ind* in, char temp[])
{
    /* Verifica se já não há outra playlist com o mesmo nome no índice (ou se o nome digitado não é o mesmo que o atual)
    Isso é importante por causa das funções que acessam os arquivos com as playlists gravadas, já que é preciso passar
    o nome deles (o nome das playlists) - e ter mais de um arquivo com determinado nome pode ter consequências indesejadas*/
    int j = 0;
    while(j < in->qtd_atual_playlists && strcmp(in->arr_ind[j].nome_playlist, temp) != 0)
        j++;
    return j;
}

void obter_nome_arquivo_playlist(char nome_arquivo[], char nome_playlist[]) // Obtém o endereço e o nome do arquivo de uma playlist
{
    char nome[25];
    strcat(strcpy(nome_arquivo, "..\\log\\playlists\\"), strcat(strcpy(nome, nome_playlist), ".txt"));
}

void editar_nome_playlist(Play* pl, Ind* in) // Permite que você edite o nome de uma playlist
{
    int j;
    char temp[25];

    printf("Digite o novo nome da playlist: ");
    scanf(" %[^\n]", temp); // Atribui nome para variável temporária

    j = verificar_nome_playlist(in, temp);
    if(j != in->qtd_atual_playlists) // Se já houver outra playlist com esse nome (ou novo nome for o mesmo que o atual)
    {
        if(strcmp(pl->nome_playlist, temp) == 0)
            printf("\nERRO: Novo nome não pode ser o mesmo que o atual!");
        else
            printf("\nERRO: Já existe uma playlist com esse nome!");
    }
    else // Se não
    {
        // Obtém localização do arquivo da playlist dado seu nome atual
        char nome_arquivo[60];
        obter_nome_arquivo_playlist(nome_arquivo, pl->nome_playlist);

        strcpy(pl->nome_playlist, temp); // Passa novo nome para a estrutura da playlist
        int i = verificar_id_indice(in, in->qtd_atual_playlists, pl->id_playlist);
        strcpy(in->arr_ind[i].nome_playlist, pl->nome_playlist); // Atualiza seu nome no índice

        remove(nome_arquivo); // Exclui arquivo com nome original;
        printf("\nNome alterado.");      
    }
    final(1);
}   

void carregar_indice(Ind* in) // Carrega o indice
{
    int i = 0;
    char linha[150];
    
    FILE *f_index;
    f_index = fopen("..\\log\\Index.txt", "r"); // Abre o arquivo Index.txt (que reune os dados das playlists atuais) no modo de leitura
    // Lê informações preliminares (quantas playlists existem atualmente, e quantas já foram criadas no total)
    fscanf(f_index, "Quantidade atual de playlists: %i;\n", &in->qtd_atual_playlists);
    fscanf(f_index, "Quantidade total de playlists criadas: %i;\n", &in->qtd_total_playlists);
    
    for(int j = 0; j < 2; j++)
        fgets(linha, 150, f_index); // Lê cabeçalho

    if(in->qtd_atual_playlists > 0) // Se houverem playlists no índice
    {
        while(fscanf(f_index, "%i| %[^|]| %i| %i| %d/%d/%d %d:%d:%d|\n",
            &in->arr_ind[i].id_playlist, in->arr_ind[i].nome_playlist, &in->arr_ind[i].qtd_musicas_atual, &in->arr_ind[i].qtd_musicas_total, &in->arr_ind[i].ultima_modificacao_playlist.tm_mday,
            &in->arr_ind[i].ultima_modificacao_playlist.tm_mon, &in->arr_ind[i].ultima_modificacao_playlist.tm_year, &in->arr_ind[i].ultima_modificacao_playlist.tm_hour,
            &in->arr_ind[i].ultima_modificacao_playlist.tm_min, &in->arr_ind[i].ultima_modificacao_playlist.tm_sec) != EOF)
            i++; // Lê os dados de cada uma, os atribuindo aos seus respectivos campos (e posições) no vetor
    }
    fclose(f_index); // Fecha arquivo
}

void exibir_cabecalho_indice() // Exibe o cabeçalho do índice
{
    printf("   ID|                     NOME|    N° DE MÚSICAS (ATUAL)|         TOTAL DE MÚSICAS|       ÚLTIMA MODIFICAÇÃO|\n");
    for(int j = 0; j < 110; j++)
        printf("-");
}

void linha_indice(Ind* in, int i) // Exibe os dados de uma playlist no índice, dada sua posição no vetor ind
{
    printf("\n%5i|", in->arr_ind[i].id_playlist);
    espacamento_string(in->arr_ind[i].nome_playlist, 25);
    printf("%s|%25i|%25i|      %.2d/%.2d/%d %.2d:%.2d:%.2d|", in->arr_ind[i].nome_playlist, in->arr_ind[i].qtd_musicas_atual, in->arr_ind[i].qtd_musicas_total,
           in->arr_ind[i].ultima_modificacao_playlist.tm_mday, in->arr_ind[i].ultima_modificacao_playlist.tm_mon, in->arr_ind[i].ultima_modificacao_playlist.tm_year,
           in->arr_ind[i].ultima_modificacao_playlist.tm_hour, in->arr_ind[i].ultima_modificacao_playlist.tm_min, in->arr_ind[i].ultima_modificacao_playlist.tm_sec);
}

void carregar_playlist(Play* pl, Ind* in, int i) // Carrega os dados (músicas) de uma playlist (já existente) a partir de seu respectivo arquivo
{
    // Passa demais dados da playlist no índice para estrutura
    strcpy(pl->nome_playlist, in->arr_ind[i].nome_playlist);
    pl->qtd_musicas_total = (in->arr_ind[i].qtd_musicas_total - in->arr_ind[i].qtd_musicas_atual);
    pl->id_playlist = in->arr_ind[i].id_playlist;

    Elem* no;
    Musica msc;
    
    FILE* f_carregamento;
    char nome_arquivo[60];
    obter_nome_arquivo_playlist(nome_arquivo, pl->nome_playlist);
    f_carregamento = fopen(nome_arquivo, "r"); // Abre arquivo com playlist gravada no modo de leitura
    
    char linha[175];
    for(int i = 0; i < 3; i++)
        fgets(linha, 175, f_carregamento); // Lê o cabeçalho

    while(fscanf(f_carregamento, "%i| %[^|]| %[^|]| %[^|]| %[^|]| %d/%d/%d %d:%d:%d|\n", &msc.id_musica, msc.nome_artista, 
        msc.nome_musica, msc.ano_lancamento, msc.pais_musica, &msc.data_adicao.tm_mday, &msc.data_adicao.tm_mon, 
        &msc.data_adicao.tm_year, &msc.data_adicao.tm_hour, &msc.data_adicao.tm_min, &msc.data_adicao.tm_sec) != EOF)
    {
        no = cria_no(msc);
        inserir_musica_playlist(pl, no, 1, 1); // Lê cada música no arquivo, passando seus dados para os nós da estrutura
    }
    fclose(f_carregamento); // Fecha arquivo
}

int procurar_playlist(Play* pl, Ind* in, int qtd) // Selecionar qual playlist (já existente) você quer carregar
{
    int play_carregada;
    // Exibe a quantidade atual, o cabeçalho, e os dados de cada playlist   
    printf("Playlists existentes: %i\n\n", qtd);
    exibir_cabecalho_indice();
    for(int j = 0; j < qtd; j++)
        linha_indice(in, j);
    printf("\n\n");
    
    // Insere o ID, e verifica se ele corresponde a alguma playlist no índice
    int id = input_id(1);
    int i = verificar_id_indice(in, qtd, id);

    if(i == in->qtd_atual_playlists)
    {
        printf("\nERRO: ID inválido.");
        final(1);
        play_carregada = 0;
    }
    else
    {
        carregar_playlist(pl, in, i);
        printf("\nPlaylist %s carregada com sucesso.", pl->nome_playlist);
        final(3);
        play_carregada = 1;
    }

    return play_carregada;
}

int nova_playlist_indice(Play* pl, Ind* in) // Cria uma nova playlist, obtendo seus principais dados (nome e ID)
{
    char temp[25];
    int exec = 0, j, play_criada;

    printf("Digite o nome da playlist: ");
    scanf(" %[^\n]", temp);

    j = verificar_nome_playlist(in, temp);

    if(j != in->qtd_atual_playlists)
    {
        printf("\nERRO: Já existe uma playlist com esse nome!");
        final(1);
        play_criada = 0;
    }
    else
    {
        // Registra nome e ID no índice
        int i = in->qtd_atual_playlists;
        pl->id_playlist = in->qtd_total_playlists + 1;
        in->arr_ind[i].id_playlist = pl->id_playlist;
        strcpy(pl->nome_playlist, temp);
        strcpy(in->arr_ind[i].nome_playlist, pl->nome_playlist);

        // Incrementa a quantidade de playlists no índice (atual e total)
        in->qtd_atual_playlists++;
        in->qtd_total_playlists++;
        play_criada = 1;
        printf("\nPlaylist %s criada com sucesso.", pl->nome_playlist);
        final(3);
    }
    return play_criada;
}

int excluir_playlist_indice(Play* pl, Ind* in) // Remove uma playlist do índice, assim como seu arquivo correspondente
{
    int play_excluida = 0, i = verificar_id_indice(in, in->qtd_atual_playlists, pl->id_playlist), op;

    printf("Tem certeza de que quer excluir essa playlist?\n\n1 - Sim\n(Digite qualquer outra tecla para cancelar)\n\n"); // Exibe opções
    scanf("%i", &op);
    fflush(stdin);

    if(op == 1) // Se confirmar que deseja mesmo excluir a playlist
    {   
        char nome_arquivo[60];
        obter_nome_arquivo_playlist(nome_arquivo, in->arr_ind[i].nome_playlist);

        for(int k = i; k < in->qtd_atual_playlists - 1; k++)
            in->arr_ind[k] = in->arr_ind[k + 1]; // Desloca elementos (playlists) à direita do índice da playlist removida

        remove(nome_arquivo); // Exclui o arquivo
        in->qtd_atual_playlists--; // Decrementa a quantidade atual de músicas
        printf("\nPlaylist excluída.");
        play_excluida = 1; // Sinaliza que playlist foi excluida
    }
    else // Se não
        printf("\nExclusão cancelada.");
    final(1);
    return play_excluida;
}

void espacamento_arquivo(FILE* f_gravacao, char nome[], int qtd_max) // Determina quantos espaços vazios devem ser impressos antes de uma string na hora de gravar os dados de uma música no arquivo de sua playlist
{
    int qtd_esp = tamanho_string(nome, qtd_max);
    for(int i = 0; i < qtd_esp; i++)
        fprintf(f_gravacao, " ");
}

Elem* ultima_modificacao_musica(Play* pl) // Encontra a última modificação feita na playlist
{
    Elem* no = pl->inicio_playlist;
    Play* temp = inicializar_playlist(); // Cria playlist temporária
    Elem* novo_no;
    while(no != NULL) // Copia dados da playlist original para temp;
    {
        novo_no = cria_no(no->dados);
        inserir_musica_playlist(temp, novo_no, 6, 0); // Adiciona as copias dos elementos em ordem de data (mais recente);
        no = no->prox;
    }
    novo_no = cria_no(temp->inicio_playlist->dados); // Copia primeiro elemento da playlist (elemento mais recentemente adicionado/modificado) para novo nó
    excluir_playlist(temp); // Exclui playlist temporária
    return novo_no;
}

void obter_dados_musica_gravacao(Ind* in, Play* pl) // Registra os demais dados da playlist antes de gravar
{
    int i = verificar_id_indice(in, in->qtd_atual_playlists, pl->id_playlist); // Encontra a posição da playlist no índice dado seu ID

    // Registra a quantidade de músicas na playlist (atual e total)
    in->arr_ind[i].qtd_musicas_atual = pl->qtd_musicas_atual;
    in->arr_ind[i].qtd_musicas_total = pl->qtd_musicas_total;

    if(pl->qtd_musicas_atual > 0) // Se houverem músicas na playlist
    {
        // Campo ultima_modificacao receberá a data e horário da música mais recentemente adicionada/modificada
        Elem* no = ultima_modificacao_musica(pl);
        in->arr_ind[i].ultima_modificacao_playlist = no->dados.data_adicao;
    }
    else // Se não houverem músicas (playlist vazia)
    {
        // Campo última_modificacao receberá a data e horário na qual arquivo será gravado
        time_t tempo;
        tempo = time(NULL);
        struct tm data_atual = *localtime(&tempo);
        in->arr_ind[i].ultima_modificacao_playlist = data_atual;
        in->arr_ind[i].ultima_modificacao_playlist.tm_mon += 1;
        in->arr_ind[i].ultima_modificacao_playlist.tm_year += 1900;
    }
}

void gravar_cabecalho_playlist_arquivo(Play* pl, FILE* f_gravacao) // Grava o cabeçalho da playlist
{
    fprintf(f_gravacao, "NOME DA PLAYLIST: %s\n   ID|                 ARTISTA/BANDA|                                       MÚSICA|       ANO|              PAÍS/REGIÃO|     ADICIONADA/ULTIMA ALTERAÇÃO EM|\n", pl->nome_playlist);
    for(int i = 0; i < 156; i++)
        fprintf(f_gravacao, "-");
    fprintf(f_gravacao, "\n");
}

void gravar_linha_musica(FILE* f_gravacao, Elem* no) // Grava os dados de uma música
{
    fprintf(f_gravacao, "%5i|", no->dados.id_musica);
    espacamento_arquivo(f_gravacao, no->dados.nome_artista, 30);
    fprintf(f_gravacao, "%s|", no->dados.nome_artista);
    espacamento_arquivo(f_gravacao, no->dados.nome_musica, 45);
    fprintf(f_gravacao, "%s|%10s|", no->dados.nome_musica, no->dados.ano_lancamento);
    espacamento_arquivo(f_gravacao, no->dados.pais_musica, 25);
    fprintf(f_gravacao, "%s|                %.2d/%.2d/%d %.2d:%.2d:%.2d|\n", no->dados.pais_musica, 
        no->dados.data_adicao.tm_mday, no->dados.data_adicao.tm_mon, no->dados.data_adicao.tm_year, 
        no->dados.data_adicao.tm_hour, no->dados.data_adicao.tm_min, no->dados.data_adicao.tm_sec);
}

void gravar_playlist(Play* pl, Ind* in) // Grava uma playlist em seu respectivo arquivo
{
    obter_dados_musica_gravacao(in, pl);

    char nome_arquivo[60];
    obter_nome_arquivo_playlist(nome_arquivo, pl->nome_playlist);

    FILE* f_gravacao = fopen(nome_arquivo, "w"); // Abre arquivo da playlist no modo de gravação
    // Grava a playlist
    gravar_cabecalho_playlist_arquivo(pl, f_gravacao);
    Elem* no = pl->inicio_playlist;
    while(no != NULL)
    {
        gravar_linha_musica(f_gravacao, no);
        no = no->prox;
    }
    
    fclose(f_gravacao); // Fecha o arquivo
    excluir_playlist(pl);
}

void gravar_cabecalho_indice(FILE* f_index) // Grava o cabeçalho do índice
{
    fprintf(f_index, "   ID|                     NOME|    N° DE MÚSICAS (ATUAL)|         TOTAL DE MÚSICAS|       ÚLTIMA MODIFICAÇÃO|\n");
    for(int j = 0; j < 110; j++)
        fprintf(f_index, "-");
}

void gravar_linha_indice(FILE* f_index, Ind* in, int i) // Grava os dados de uma playlist
{
    fprintf(f_index, "\n%5i|", in->arr_ind[i].id_playlist);
    espacamento_arquivo(f_index, in->arr_ind[i].nome_playlist, 25);
    fprintf(f_index, "%s|%25i|%25i|      %.2d/%.2d/%d %.2d:%.2d:%.2d|", in->arr_ind[i].nome_playlist, in->arr_ind[i].qtd_musicas_atual, in->arr_ind[i].qtd_musicas_total, 
        in->arr_ind[i].ultima_modificacao_playlist.tm_mday, in->arr_ind[i].ultima_modificacao_playlist.tm_mon, in->arr_ind[i].ultima_modificacao_playlist.tm_year, 
        in->arr_ind[i].ultima_modificacao_playlist.tm_hour, in->arr_ind[i].ultima_modificacao_playlist.tm_min, in->arr_ind[i].ultima_modificacao_playlist.tm_sec);
}

void gravar_indice(Ind* in) // Grava o índice no seu respectivo arquivo
{
    FILE* f_index;
    f_index = fopen("..\\log\\Index.txt", "w"); // Abre o arquivo Index.txt
    // Grava dados preliminares
    fprintf(f_index, "Quantidade atual de playlists: %i;\n", in->qtd_atual_playlists);
    fprintf(f_index, "Quantidade total de playlists criadas: %i;\n", in->qtd_total_playlists);
    
    // Grava o índice
    gravar_cabecalho_indice(f_index);
    for(int i = 0; i < in->qtd_atual_playlists; i++)
        gravar_linha_indice(f_index, in, i);
    fclose(f_index); // Fecha o arquivo
}