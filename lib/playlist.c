#include <ctype.h>
#include "playlist.h"
#include <shlwapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Play* inicializar_playlist() // Inicializa a estrutura da playlist
{
    Play* pl = (Play*) malloc(sizeof(Play)); // Aloca memória para ela;
    if(pl != NULL) // Se estrutura for válida (memória foi alocada com sucesso)
    {
        // Inicializa campos
        pl->inicio_playlist = NULL;
        pl->qtd_musicas_atual = 0;
        pl->qtd_musicas_total = 0;
    }
    else // Se não
    {
        system("cls");
        printf("ERRO: Falha na criação da lista.\n");
    }
    return pl;
}

void final(int msg) // Como terminam as mensagens ao final de algumas das funções
{
    if(msg == 1)
        printf("\nDigite qualquer tecla para voltar ao menu.");
    else if(msg == 2)
        printf("\nDigite qualquer tecla para recarregar a página atual.");
    else
        printf("\nDigite qualquer tecla para continuar.");
    fflush(stdin);
    getchar();
    system("cls");
}

time_t converter_data(Elem* no) // Converte a data e horário de gravação da música para uma timestamp
{
    struct tm data = no->dados.data_adicao;
    data.tm_mon -= 1;
    data.tm_year -= 1900;
    time_t tmstmp = mktime(&data);
    return tmstmp;
}

void excluir_playlist(Play* pl) // Exclui os dados de uma playlist
{
    if(pl != NULL)
    {
        Elem* no;
        while(pl->inicio_playlist != NULL) // Remove elemento por elemento, até chegar ao final
        {
            no = pl->inicio_playlist;
            pl->inicio_playlist = pl->inicio_playlist->prox;
            free(no);
        }
        // "reseta" campos
        pl->inicio_playlist = NULL;
        pl->qtd_musicas_atual = 0;
        pl->qtd_musicas_total = 0;
    }
    else
    {
        system("cls");
        printf("ERRO: Lista inválida.\n");
    }    
}

int condicao_ordem_playlist(Elem* atual, Elem* no, int ordem) // Retorna qual deve ser a comparação a ser feita de acordo com a ordenação escolhida
{
    if(ordem == 1) // Alfabética (A -> Z) -  Artista
        return (atual != NULL && strcasecmp(atual->dados.nome_artista, no->dados.nome_artista) < 0);
    else if(ordem == 2) // Alfabética (Z -> A) - Artista
        return (atual != NULL && strcasecmp(atual->dados.nome_artista, no->dados.nome_artista) > 0);
    else if(ordem == 3) // ID (Crescente)
        return (atual != NULL && atual->dados.id_musica < no->dados.id_musica);
    else if(ordem == 4) // ID (Decrescente)
        return (atual != NULL && atual->dados.id_musica > no->dados.id_musica);
    else if(ordem == 5) // Data (Mais antiga)
        return (atual != NULL && converter_data(atual) < converter_data(no));
    else // ordem == 6 -> Data (Mais recente)
        return (atual != NULL && converter_data(atual) > converter_data(no));
}

int condicao_ordem_playlist_musica(Elem* atual, Elem* no, int ordem) // No caso da ordem ser alfabética (A -> Z ou Z -> A), retorna a comparação a ser feita em relação ao nome da música
{
    if(ordem == 1) // (A -> Z) - Música
        return (atual != NULL && strcasecmp(atual->dados.nome_musica, no->dados.nome_musica) < 0 && strcasecmp(atual->dados.nome_artista, no->dados.nome_artista) == 0);
    else // (Z -> A) - Música
        return (atual != NULL && strcasecmp(atual->dados.nome_musica, no->dados.nome_musica) > 0 && strcasecmp(atual->dados.nome_artista, no->dados.nome_artista) == 0);
}

void inserir_musica_playlist(Play* pl, Elem* no, int ord_art, int ord_msc) // Insere as músicas dentro da estrutura, de acordo com a ordenação determinada
{
    if(pl->inicio_playlist == NULL) // Se playlist estiver vazia (inserção da primeira música)
    {
        no->prox = NULL;
        no->ant = NULL;
        pl->inicio_playlist = no;
    }
    else // Caso já houverem outras músicas na playlist
    {
        Elem *ante, *atual = pl->inicio_playlist;
        while(condicao_ordem_playlist(atual, no, ord_art)) // Percorre a lista até encontrar o ponto de inserção, conforme a ordem escolhida;
        {
            ante = atual;
            atual = atual->prox;
        }

        if(ord_msc != 0 && (atual != NULL && strcasecmp(atual->dados.nome_artista, no->dados.nome_artista) == 0)) // Caso a ordem seja alfabética (A -> Z ou Z -> A) e já houverem outras músicas do mesmo artista/mesma banda na playlist
        {
            while(condicao_ordem_playlist_musica(atual, no, ord_msc)) // Percorre a lista até encontrar o ponto exato de inserção, considerando agora os nomes das demais músicas desse artista/banda
            {
                ante = atual;
                atual = atual->prox;
            }
        }
        
        if(atual == pl->inicio_playlist) // Se o ponto de inserção for no inicio da lista (primeiro elemento);
        {
            no->ant = NULL;
            pl->inicio_playlist->ant = no;
            no->prox = pl->inicio_playlist;
            pl->inicio_playlist = no;
        }
        else // Caso ele não esteja no inicio
        {
            no->prox = ante->prox;
            no->ant = ante;
            ante->prox = no;
            if(atual != NULL) // Caso ele não esteja no final (não seja o último elemento)
                atual->ant = no;
        }
    }
    pl->qtd_musicas_atual++;
    pl->qtd_musicas_total++;
}

Elem* cria_no(Musica msc) // Inicializa um nó, e passa os dados de uma música para ele
{
    Elem* no = (Elem*) malloc(sizeof(Elem)); // Aloca memória
    if(no == NULL)
    {
        system("cls");
        printf("ERRO: Falha na criação do nó.\n");
    }
    else
        no->dados = msc; // Nó recebe dados da música
    return no;
}

void data_adicao_musica(Musica* msc) // Obtém a data na qual música foi adicionada/editada
{
    time_t tempo;
    tempo = time(NULL);
    struct tm data = *localtime(&tempo); // Gera struct com data e hora atual do sistema;

    msc->data_adicao = data;
    msc->data_adicao.tm_mon += 1;
    msc->data_adicao.tm_year += 1900;
}

void input_musica(Musica* msc) // Inserção manual dos principais dados de uma música
{
    printf("Nome: ");
    scanf(" %[^\n]", msc->nome_musica);
    printf("Artista/Banda: ");
    scanf(" %[^\n]", msc->nome_artista);
    printf("Ano de lançamento: ");
    scanf(" %[^\n]", msc->ano_lancamento);
    printf("País: ");
    scanf(" %[^\n]", msc->pais_musica);
}

void nova_musica(Play* pl, int ord_art, int ord_msc) // Cria uma música nova, e a insere na playlist
{
    // Obtém dados da música
    Musica msc;
    printf("Insira os dados da música:\n");
    input_musica(&msc);
    msc.id_musica = pl->qtd_musicas_total + 1;
    data_adicao_musica(&msc);

    // Passa dados para nó e o insere na playlst
    Elem* no = cria_no(msc);
    inserir_musica_playlist(pl, no, ord_art, ord_msc);

    printf("\nMúsica adicionada.");
    final(1);
}

int tamanho_string(const char* string, int qtd_max) // Calcula a quantidade real de caracteres de uma string
{
    int tam_string_real = 0, tam_string_bytes = strlen(string), diferenca, qtd_esp;
    while(*string != '\0') // Percorre string enquanto ponteiro não apontar para caractere nulo (final)
    {
        // Se não for um byte de continuação (10xxxxxx), é o início de um novo caractere
        if((unsigned char)(*string) >> 6 != 0b10) // Converte para caractere de 1 byte, e desloca seu valor para a direita, para descobrir se é um caractere de continuação (faz parte de um caractere multibyte) ou não
            tam_string_real++; // Se não for um caractere de continuação (se for o começo de um novo caractere), incrementa contador;
        string++;
    }

    diferenca = tam_string_bytes - tam_string_real; // Calcula a diferença entre os dois tamanhos
    qtd_esp = qtd_max - tam_string_bytes; // Quantidade de espaços será o tamanho máximo que a string pode ter, menos seu tamanho atual em bytes
    if(diferenca != 0) // Caso haja diferença entre os tamanhos de bytes e caracteres (a string tiver letras acentuadas ou outros caracteres especiais)
        qtd_esp += diferenca; // Diferença será incrementada à quantidade de espaços, de modo a não prejudicar a exibição/gravação da música na playlist

    return qtd_esp;
}

void espacamento_string(char string[], int qtd_max) // Determina quantos espaços vazios devem ser impressos antes de uma string na hora de exibir os dados de uma música
{
    int qtd_esp = tamanho_string(string, qtd_max);
    for(int i = 0; i < qtd_esp; i++)
        printf(" ");
}

void linha_musica(Elem* no) // Exibe os dados de uma música (dado seu nó), devidamente formatados
{
    printf("%5i|", no->dados.id_musica);
    espacamento_string(no->dados.nome_artista, 30);
    printf("%s|", no->dados.nome_artista);
    espacamento_string(no->dados.nome_musica, 45);
    printf("%s|%10s|", no->dados.nome_musica, no->dados.ano_lancamento);
    espacamento_string(no->dados.pais_musica, 25);
    printf("%s|                %.2d/%.2d/%d %.2d:%.2d:%.2d|\n", no->dados.pais_musica, no->dados.data_adicao.tm_mday,
           no->dados.data_adicao.tm_mon, no->dados.data_adicao.tm_year, no->dados.data_adicao.tm_hour,
           no->dados.data_adicao.tm_min, no->dados.data_adicao.tm_sec);
}

void exibir_cabecalho(Play* pl) // Exibe o cabeçalho da playlist
{
    printf("NOME DA PLAYLIST: %s\n   ID|                 ARTISTA/BANDA|                                       MÚSICA|       ANO|              PAÍS/REGIÃO|     ADICIONADA/ULTIMA ALTERAÇÃO EM|\n", pl->nome_playlist);
    for(int i = 0; i < 156; i++)
        printf("-");
    printf("\n");
}

int exibir_playlist(Play* pl) // Exibe a playlist completa/resultados de busca, com direito a uma implementação de navegação por páginas
{
    int inicio_exib = 0, opcao = 0, exec = 1, op_sair, i, j;
    Elem* no;
    while(exec == 1) // Enquanto não selecionar opção de sair (encerrar execução da função retornando 0)
    { 
        no = pl->inicio_playlist;
        i = 0;
        j = 0;
        op_sair = 1;

        while(i < inicio_exib) // Percorre um determinado número de elementos a partir do início da playlist (correspondentes a "página" atual) antes de começar a exibir os demais
        {
            no = no->prox;
            i++;
        }
        
        system("cls");
        printf("Músicas encontradas/na playlist: %i\n\n", pl->qtd_musicas_atual);
        exibir_cabecalho(pl);
        while(no != NULL && j < 25) // Exibe um máximo de 25 músicas por "página" (até chegar a esse número, ou ao final da playlist);
        {
            linha_musica(no);
            no = no->prox;
            j++;
        }

        printf("\nOpções:\n"); // Mostra opções correspondentes a "página" atual e ao tamanho da playlist
        if(inicio_exib > 0) // Se não estiver na primeira página
        {
            printf("%i - Página anterior\n", op_sair);
            op_sair++;
        }
        if(no != NULL) // Se não estiver na última página (não tiver chegado ao último elemento)
        {
            printf("%i - Próxima página\n", op_sair);
            op_sair++;
        }
        printf("%i - Sair\n\n", op_sair);
        scanf("%i", &opcao);
        fflush(stdin);

        if(pl->qtd_musicas_atual <= 25) // Se playlist não tiver mais de 25 músicas (possui apenas uma única página)
        {
            if(opcao == 1)
                return 0;
            else
            {     
                printf("\nOpção inválida.");
                final(2);
            }
        }
        else // Se tiver mais de 25 músicas (permitindo assim navegar entre diferentes páginas)
        {
            if(opcao == 1)
            {
                if(inicio_exib == 0) // Se estiver na primeira página
                    inicio_exib += 25;
                else // Se não estiver na primeira página
                    inicio_exib -= 25;
            }
            else if(opcao == 2)
            {
                if(inicio_exib == 0 || no == NULL) // Se estiver na primeira página ou na última (ter chegado ao final da playlist)
                    return 0;
                else // Se não estiver nem na primeira e nem na última
                    inicio_exib += 25;
            }
            else if(opcao == 3)
            {
                if(inicio_exib != 0 && no != NULL) // Se não estiver nem na primera página e nem na última
                    return 0;
                else
                {
                    printf("\nOpção inválida.");
                    final(2);
                }
            }
            else
            {
                printf("\nOpção inválida.");
                final(2);
            }
        }
    }
}

int input_id(int msc_ou_play) // Inserção manual do ID de uma música ou playlist
{
    int id;
    if(msc_ou_play == 0)
        printf("Digite o ID da música: ");
    else
        printf("Digite o ID da playlist: ");
    scanf("%i", &id);
    return id;
}

void exibir_unica_musica(Play* pl, Elem* no) // Exibe os dados de uma única música (no caso das funções que envolvem busca por ID)
{
    printf("Musica encontrada:\n\n");
    exibir_cabecalho(pl);
    linha_musica(no);
}

Elem* verificar_no_id(Play* pl, int id) // Procura pela música (nó) que contenha o ID selecionado
{
    Elem* no = pl->inicio_playlist;
    while (no != NULL && no->dados.id_musica != id)
        no = no->prox;
    return no;
}

void buscar_musica_id(Play* pl) // Busca uma música na playlist dado o seu ID (identificador único)
{
    int id = input_id(0);
    Elem* no = verificar_no_id(pl, id);

    if(no == NULL)
        printf("\nERRO: Música não encontrada.");
    else
    {
        system("cls");
        exibir_unica_musica(pl, no);
    }       
}

int condicao_busca_string(Elem* no, char string[], int msc_ou_art) // Verifica se o nome de determinada música/artista contém a string digitada
{
    if(msc_ou_art == 0) // Busca por nome de artista
        return(StrStrIA(no->dados.nome_artista, string) != NULL);
    else // Busca por nome de música
        return(StrStrIA(no->dados.nome_musica, string) != NULL);
    // StrStrIA é uma função específica do Windows; ela verifica se uma substring está contida em outra string (como strstr), mas sem levar em conta se os caracteres são maiúsculos ou minúsculos
}

void buscar_musica_string(Play* pl, int msc_ou_art) // Busca músicas na playlist com base no nome (ou parte do nome) do(s) artista/banda ou da música
{
    char string[45];
    int cont = 0;

    if(msc_ou_art == 0) // Se estiver pesquisando por nome de artista/banda
        printf("Digite o nome do(a) artista/banda: ");
    else // Se estiver pesquisando por nome de música
        printf("Digite o nome da música: ");
    scanf(" %[^\n]", string);

    Play* temp = inicializar_playlist(); // Cria playlist temporária para exibir resultados
    strcpy(temp->nome_playlist, pl->nome_playlist);

    Elem* no = pl->inicio_playlist;
    Elem* novo_no;
    while(no != NULL) // Percorre playlist original
    {
        if(condicao_busca_string(no, string, msc_ou_art)) // Se condição for verdadeira (string estiver no nome de música/artista daquele nó)
        {
            novo_no = cria_no(no->dados); // Copia dados para novo nó, preservando playlist original
            inserir_musica_playlist(temp, novo_no, 1, 1); // Insere novo nó na playlist temporária
            cont++;
        }
        no = no->prox;
    }

    if(cont != 0) // Se encontrou resultados
        exibir_playlist(temp);
        
    else
    {
        printf("\nERRO: Nenhum resultado encontrado.");
        final(1);
    }
    
    excluir_playlist(temp); // Exclui playlist temporária ao final
}

int opcoes_busca(Play* pl) // Selecionar o critério pelo qual você quer buscar músicas na playlist
{
    int opcao;
    
    printf("Escolha uma forma de busca digitando o número correspondente:\n");
    printf("1 - ID\n");
    printf("2 - Nome de artista/banda\n");
    printf("3 - Nome de música\n");
    printf("4 - Sair\n\n");
    scanf("%i", &opcao);

    system("cls");
    switch(opcao)
    {
    case 1:
        buscar_musica_id(pl);
        final(1);
    break;
    case 2:
        buscar_musica_string(pl, 0);
    break;
    case 3:
        buscar_musica_string(pl, 1);
    break;
    case 4:
        return 0;
    break;
    default:
        printf("Opção inválida.");
        final(1);
    break;
    }
}

void remocao_musica(Play* pl, Elem* no) // Remove uma única música da playlist
{
    if(no->ant == NULL)
        pl->inicio_playlist = no->prox;
    else
        no->ant->prox = no->prox;

    if(no->prox != NULL)
        no->prox->ant = no->ant;

    free(no);
    pl->qtd_musicas_atual--;
}

void excluir_musica(Play* pl) // Seleciona uma música para remover da playlist dado seu ID
{
    int id = input_id(0);
    Elem* no = verificar_no_id(pl, id);
    if(no != NULL)
    {
        system("cls");
        int op = 0;
        exibir_unica_musica(pl, no);
        printf("\nTem certeza de que quer excluir essa música?\n\n1 - Sim\n(Digite qualquer outra tecla para cancelar)\n\n");
        scanf("%i", &op);
        fflush(stdin);

        if(op == 1)
        {
            remocao_musica(pl, no);
            printf("\nMúsica excluída.");
        }
        else
            printf("\nExclusão cancelada.");
        final(1);
    }
    else
    {
        printf("\nERRO: Música não encontrada.");
        final(1);
    }
}

void editar_musica(Play* pl) // Seleciona uma música da playlist para editar seus dados com base em seu ID
{
    int id = input_id(0), edit = 0;
    Elem* no = verificar_no_id(pl, id);

    if(no == NULL)
        printf("\nERRO: Música não encontrada.");      
    else
    {
        system("cls");
        exibir_unica_musica(pl, no);
        printf("\nAtualize os dados da música:\n");
        input_musica(&no->dados); // Altera diretamente os dados da música na playlist
        data_adicao_musica(&no->dados);
        printf("\nMúsica editada.");
    }
    final(1);
}

void codigos_ordenacao(int opcao, int* ord_art, int* ord_msc) // Determina os códigos que serão passados para inserir_musica_playlist com base na opcao de ordenacao selecionada
{
    if(opcao == 2) // Alfabética (Z -> A)
    {
        *ord_art = 2;
        *ord_msc = 2;
    }
    else
    {
        *ord_msc = 0;
        if(opcao == 3) // ID (Crescente)
            *ord_art = 3;
        else if(opcao == 4) // ID (Decrescente)
            *ord_art = 4;
        else if(opcao == 5) // Data de adição/modificação (Mais antigo)
            *ord_art = 5;
        else // 6 - Data de adição/modificação (Mais recente)
            *ord_art = 6;
    }
}

int opcoes_ordenacao(Play* pl) // Escolher a ordenação com a qual você quer que as músicas sejam exibidas
{
    int opcao, ord_art, ord_msc;
    printf("Escolha uma ordem de exibição digitando o número correspondente:\n");
    printf("1 - Alfabética (A -> Z)\n");
    printf("2 - Alfabética (Z -> A)\n");
    printf("3 - ID (Crescente)\n");
    printf("4 - ID (Decrescente)\n");
    printf("5 - Data de adição/modificação (Mais antigo)\n");
    printf("6 - Data de adição/modificação (Mais recente)\n");
    printf("7 - Sair\n\n");
    scanf("%i", &opcao);

    system("cls");
    if(opcao <= 7)
    {
        if(opcao == 1)
            exibir_playlist(pl); // Simplesmente exibe playlist original
        else if(opcao == 7)
            return 0;
        else
        {
            codigos_ordenacao(opcao, &ord_art, &ord_msc);
            Play* temp;
            temp = inicializar_playlist(); // Cria playlist temporária para armazenar elementos reordenados
            strcpy(temp->nome_playlist, pl->nome_playlist);

            Elem* no = pl->inicio_playlist;
            Elem* novo_no;
                
            while(no != NULL) // Insere cópias de todas as músicas seguindo a ordenação especificada
            {
                novo_no = cria_no(no->dados);
                inserir_musica_playlist(temp, novo_no, ord_art, ord_msc);
                no = no->prox;
            }
            exibir_playlist(temp);
            excluir_playlist(temp);
        }
    }
    else
    {
        printf("Opção inválida.");
        final(1);
    }
}