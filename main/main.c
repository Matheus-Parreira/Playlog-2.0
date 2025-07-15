#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "..\lib\playlist.h"
#include "..\lib\gravacao.h"

int menu_playlist(Play* pl, Ind* in)
{
    int exec = 0, opcao, play_excluida, op_sair;
    while(exec == 0)
    {
        system("cls");
        printf("Playlist atual: %s\n\n", pl->nome_playlist);
        printf("Opções:\n");
        printf("1 - Adicionar nova música\n");
        op_sair = 1; 
        if(pl->qtd_musicas_atual != 0)
        {
            printf("2 - Buscar música\n");
            printf("3 - Editar música\n");
            printf("4 - Excluir música\n");
            printf("5 - Exibir playlist\n");
            op_sair = 5;
        }    
        printf("%i - Alterar nome da playlist\n", op_sair + 1);
        printf("%i - Excluir playlist\n", op_sair + 2);
        printf("%i - Salvar playlist e retornar ao menu inicial\n", op_sair + 3);
        printf("\nSelecione uma opção: ");

        scanf("%i", &opcao);
        system("cls");
        if(op_sair == 1) // Se não houverem músicas no momento (playlist estiver vazia)
        {
            switch(opcao)
            {
                case 1:
                    nova_musica(pl, 1, 1);
                break;
                case 2:
                    editar_nome_playlist(pl, in);
                break;
                case 3:
                    play_excluida = excluir_playlist_indice(pl, in);
                    if(play_excluida != 0)
                        return 0;
                break;
                case 4:
                    gravar_playlist(pl, in);
                    return 0;
                break;
                default:
                    printf("Opção Inválida.");
                    final(1);
                break;
            }
        }
        else // Se já houverem músicas
        {
            switch(opcao)
            {
                case 1:
                    nova_musica(pl, 1, 1);
                break;
                case 2:
                    opcoes_busca(pl);
                break;
                case 3:
                    editar_musica(pl);
                break;
                case 4:
                    excluir_musica(pl);
                break;
                case 5:
                    opcoes_ordenacao(pl);
                break;
                case 6:
                    editar_nome_playlist(pl, in);
                break;
                case 7:
                    play_excluida = excluir_playlist_indice(pl, in);
                    if(play_excluida != 0)
                        return 0;
                break;
                case 8:
                    gravar_playlist(pl, in);
                    return 0;
                break;
                default:
                    printf("Opção Inválida.");
                    final(1);
                break;
            }
        }
    }
}

int main()
{
    // Funções específicas do Windows para permitir a exibição e a inserção correta de caracteres especiais (letras acentuadas e outros) no terminal
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int opcao = 0, exec = 0, play_carregada = 0, play_criada = 0, op_sair;
    Play *pl = inicializar_playlist();
    Ind *in = inicializar_indice();
    carregar_indice(in);

    while(exec == 0)
    {
        system("cls");
        printf("Bem-vindo ao PlayLog!\nVersão 2.0\nCriado por Matheus Henrique Parreira (@Matheus-Parreira no GitHub)\n\nOpções:\n");
        printf("1 - Criar nova playlist\n");
        
        op_sair = 2;
        if(in->qtd_atual_playlists != 0) 
        {
            printf("2 - Carregar playlist\n");
            op_sair = 3;
        }
        printf("%i - Sair\n", op_sair);
        printf("\nSelecione uma opção: ");

        scanf("%i", &opcao);
        system("cls");

        if(op_sair == 2) // Se não houver nenhuma playlist no momento
        {
            switch(opcao)
            {
                case 1:
                    if(in->qtd_atual_playlists == MAX)
                    {
                        printf("ERRO: Não é possível criar mais playlists (quantidade máxima atingida).");
                        final(1);
                    }
                    else
                    {
                        play_criada = nova_playlist_indice(pl, in);
                        if(play_criada == 1)
                            menu_playlist(pl, in);
                    }
                break;
                case 2:
                    gravar_indice(in);
                    printf("Saindo...");
                    return 0;
                break;
                default:
                    printf("Opção Inválida.");
                    final(1);
                break;
            }
        }
        else // Se já houver
        {
            switch(opcao)
            {
                case 1:
                    if(in->qtd_atual_playlists == MAX)
                    {
                        printf("ERRO: Não é possível criar mais playlists (quantidade máxima atingida).");
                        final(1);
                    }
                    else
                    {
                        play_criada = nova_playlist_indice(pl, in);
                        if(play_criada == 1)
                            menu_playlist(pl, in);
                    }
                break;
                case 2:
                    play_carregada = procurar_playlist(pl, in, in->qtd_atual_playlists);
                    if(play_carregada == 1)
                        menu_playlist(pl, in);
                break;
                case 3:
                    gravar_indice(in);
                    printf("Saindo...");
                    return 0;
                break;
                default:
                    printf("Opção Inválida.");
                    final(1);
                break;
            }
        }
    }
}