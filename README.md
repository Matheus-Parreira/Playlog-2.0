**Sobre o Projeto**

Essa é a evolução do [**Playlog**](https://github.com/Matheus-Parreira/Playlog), meu projeto pessoal de programa em C para gerenciar playlists de músicas.

Assim como o original, o objetivo principal dessa versão é colocar em prática o que aprendi em programação ao longo desse semestre do meu curso de Engenharia da Computação que se passou — com o foco dessa vez sendo em **Estruturas de Dados**.

**O que tem de novo?**

Ao invés de um simples *array*, as músicas agora são armazenadas por meio de uma **lista dinâmica duplamente encadeada**, permitindo assim uma maior flexibilidade e um melhor uso de memória durante a execução.

Além disso, o Playlog 2.0 trás uma **experiência muito mais completa**, com várias funcionalidades novas, — incluindo algumas que cheguei a mencionar no README da versão original — como:

* Poder criar e gerenciar **múltiplas playlists**;  
* Nova forma de exibição das músicas (com direito a **navegação por páginas**);  
* Diferentes **opções de ordenação** (alfabética, por ID e por data);  
* Diferentes **opções de busca** (por nome de música/artista e por ID);  
* Poder **editar ou excluir músicas**;  
* **Novos menus**;  
* Dentre outras…

O programa também **expande e melhora** as funções que estavam presentes na primeira versão — em especial aquelas relacionadas à **leitura e à gravação de arquivos**.

**Como rodar?**

Antes de usar o Playlog 2.0, gere o executável compilando os arquivos; se tiver o **GCC** instalado, basta acessar a pasta do repositório em seu terminal e digitar o seguinte comando:

```
# Compilando a partir da pasta main
gcc main.c ..\lib\playlist.c ..\lib\playlist.h ..\lib\gravacao.c -o <nome do executável>
```

Então, é só abri-lo toda vez que quiser usar o programa.

**E é isso.**

Foi um tanto quanto trabalhoso, mas no fim das contas estou bastante feliz com o resultado.  
Espero que goste do Playlog 2.0 também\!  
Sinta-se à vontade para fazer sugestões, relatar problemas, ou modificar o código da forma que achar melhor seguindo a [**licença**](LICENSE).

**Até a próxima\!**