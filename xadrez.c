#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define BOARD_HEIGTH 8
#define BOARD_WIDTH 8
#define PIECES_NAME {"PEAO","CAVALO","BISPO","TORRE","RAINHA","REI"}
#define PEAO 0
#define CAVALO 1
#define BISPO 2
#define TORRE 3
#define RAINHA 4
#define REI 5
#define SAIR_LOOP 0
#define INSPECIONAR 1
#include <Windows.h>
// Desafio de Xadrez - MateCheck
// Este código inicial serve como base para o desenvolvimento do sistema de movimentação das peças de xadrez.
// O objetivo é utilizar estruturas de repetição e funções para determinar os limites de movimentação dentro do jogo.

typedef struct Peca
{
  /* data */
  /*cod = igual a posição da array PIECE_NAME*/
  /*cor = 0 para preto, e 1 para branco*/
  /*capturada = 0 para não, 1 para sim*/
  int cod, cor, capturada;
}peca;

typedef struct Xadrez{
  peca pecasBrancas[17];
  peca pecasPretas[17];
  int* tabuleiro[BOARD_WIDTH];
}xadrez;

peca criarPeca(int cod,int cor){
  peca pecaJogaveu;
  pecaJogaveu.cod = cod <= 5?cod:5;
  pecaJogaveu.cor = cor == 0 || cor == 1?cor:0;
  pecaJogaveu.capturada = 0;
  return pecaJogaveu;
};

void inserirPecas(peca lado[17],int cor){
  /*inserindo peão*/
  /*o primeiro elemento sempre é vázio para evitar confundir com um espaço vázio na hora de cruzar o dado*/
  for (int i = 1;i < 9;i++){
    lado[i] = criarPeca(PEAO,cor);
  };
  /*inserindo cavalo*/
  lado[9] = criarPeca(CAVALO,cor);
  lado[10] = criarPeca(CAVALO,cor);

  /*inserindo bispo*/
  lado[11] = criarPeca(BISPO,cor);
  lado[12] = criarPeca(BISPO,cor);

  /*inserindo TORRE*/
  lado[13] = criarPeca(TORRE,cor);
  lado[14] = criarPeca(TORRE,cor);

  /*inserindo rainha*/
  lado[15] = criarPeca(RAINHA,cor);
  /*inserindo rei*/
  lado[16] = criarPeca(REI,cor);

  /*return lado;*/
};

xadrez criarTabuleiro(){
  xadrez tabuleiroCompleto;
  inserirPecas(tabuleiroCompleto.pecasBrancas,1);
  inserirPecas(tabuleiroCompleto.pecasPretas,0);
  
  
  //uma peça branco é representada por número positivos em relacao a sua posicao a array do lado correspondente
  //uma peça preta é representada por números negativos em relacao a sua posicao a array do lado correspondente
  for(int i = 0;i < BOARD_HEIGTH;i++){
    tabuleiroCompleto.tabuleiro[i] = (int*) malloc(BOARD_WIDTH * sizeof(int));
    if (tabuleiroCompleto.tabuleiro[i] == NULL) {
            // Tratar erro de alocação de memória, se necessário
            perror("Erro ao alocar memória para a linha do tabuleiro");
            exit(EXIT_FAILURE);
        }
        for(int j = 0; j < BOARD_WIDTH; j++){
            tabuleiroCompleto.tabuleiro[i][j] = 0;
        }
  };
  /*
  0, 1, 2,  3 ,      4, 5, 6,   7
  T, C, B, REI, RAINHA, B, C ,  T
  P, P, P, P  , P     , P, P ,  P
  
  */
  //botar as torres brancas no tabuleiro
  tabuleiroCompleto.tabuleiro[0][0] = 13;
  tabuleiroCompleto.tabuleiro[7][0] = 14;
  //botas os cavalos brancos
  tabuleiroCompleto.tabuleiro[1][0] = 9;
  tabuleiroCompleto.tabuleiro[6][0] = 10;
  //botas os bispos brancos
  tabuleiroCompleto.tabuleiro[2][0] = 11;
  tabuleiroCompleto.tabuleiro[5][0] = 12;
  //rei e a rainha
  tabuleiroCompleto.tabuleiro[3][0] = 16;
  tabuleiroCompleto.tabuleiro[4][0] = 15;

  //peaos brancos
  for(int i = 0;i < 8;i++){
    tabuleiroCompleto.tabuleiro[i][1] = 1;
  };

  //botar as torres pretas no tabuleiro
  tabuleiroCompleto.tabuleiro[0][7] = -13;
  tabuleiroCompleto.tabuleiro[7][7] = -14;
  //botas os cavalos pretos
  tabuleiroCompleto.tabuleiro[1][7] = -9;
  tabuleiroCompleto.tabuleiro[6][7] = -10;
  //botas os bispos pretos
  tabuleiroCompleto.tabuleiro[2][7] = -11;
  tabuleiroCompleto.tabuleiro[5][7] = -12;
  //rei e a rainha preta
  tabuleiroCompleto.tabuleiro[3][7] = -16;
  tabuleiroCompleto.tabuleiro[4][7] = -15;

  //peaos pretos
  for(int i = 0;i < 8;i++){
    tabuleiroCompleto.tabuleiro[i][6] = -1;
  };
  

  return tabuleiroCompleto;
};

void logo(){
    printf(".-----------------------------------------------------------------------------------------------.\n");
  printf("|.::      .::           .:            .:::::         .:::::::         .::::::::     .::::::: .::|\n");
  printf("| .::   .::            .: ::          .::   .::      .::    .::       .::                  .::  |\n");
  printf("|  .:: .::            .:  .::         .::    .::     .::    .::       .::                 .::   |\n");
  printf("|    .::             .::   .::        .::    .::     .: .::           .::::::           .::     |\n");
  printf("|  .:: .::          .:::::: .::       .::    .::     .::  .::         .::              .::      |\n");
  printf("| .::   .::        .::       .::      .::   .::      .::    .::       .::            .::        |\n");
  printf("|.::      .::     .::         .::     .:::::         .::      .::     .::::::::     .:::::::::::|\n");
  printf("'-----------------------------------------------------------------------------------------------'\n");
  printf("By Antonio Melo\n");

};

//
void codEmEmoji(peca* X){
    switch (X->cod)
    {  
      case PEAO:
        /*printf(se branco?"|caracter unicode da peça branca : caractere unicode da peca preta);*/
        printf(X->cor==1?"|\U00002659|":"|\U0000265F|");
      break;
      
      case CAVALO:
        printf(X->cor==1?"|\U00002658|":"|\U0000265E|");
      break;
      
      case BISPO:
        printf(X->cor==1?"|\U00002657|":"|\U0000265D|");
      break;
      
      case TORRE:
        printf(X->cor==1?"|\U00002656|":"|\U0000265C|");
      break;
      
      case RAINHA:
        printf(X->cor==1?"|\U00002655|":"|\U0000265B|");
      break;
      
      case REI:
        printf(X->cor==1?"|\U00002654|":"|\U0000265A|");
      break;
    };
};

void exibirTabuleiro(xadrez Tabuleiro){
    int pecaPos, cor;
    peca pecaCod;
    printf("===========================\n");
    printf("|y||0||1||2||3||4||5||6||7| \n");
    for (int x = 0;x < BOARD_WIDTH;x++){
        printf("|%d|",x);
        for (int y = 0;y < BOARD_HEIGTH;y++){
            if (Tabuleiro.tabuleiro[x][y] != 0){
                pecaPos = Tabuleiro.tabuleiro[x][y];
                cor = pecaPos<0?0:1;
                if (cor==0){
                    pecaCod = Tabuleiro.pecasPretas[abs(pecaPos)];
                }else{
                    pecaCod = Tabuleiro.pecasBrancas[pecaPos];
                };
                codEmEmoji(&pecaCod);
            }else{
                printf("|_|");
            };
        };
        printf(" \n");
    };
    printf("|X|\n");
    
};

void exibirOpcoes(){
    printf("===========================\n");
    printf("Comando     Digite\n");
    printf("Sair            0\n");
    printf("Examinar peça   1\n");
    printf("===========================\n");
};

int main() {
    //iniciar tabuleiro
    SetConsoleOutputCP(CP_UTF8);
    
    int opcoes = 0;
    int sair = 1;
    int opt = 0;
    char pecas[6][8] = PIECES_NAME;
    logo();
    xadrez Jogo = criarTabuleiro();
    char line[200];
    int iz;
    
    do{
        printf("\n\n\n");
        exibirTabuleiro(Jogo);
        exibirOpcoes();
        
        if (fgets(line, sizeof(line), stdin)) {
            if (1 == sscanf(line, "%d", &iz)) {
                opt = atoi(line);
                switch (opt){
                    case SAIR_LOOP:
                        sair = 0;
                        if(opt == 0){
                            sair = 0;  
                        };
                    break;
                    case INSPECIONAR:
                        char str[4];
                        char *token;
                        int intArray[2];
                        int ic = 0;
                        printf("Digite cordenada x_cordenada y\n Exemplo 6_9 ou 0_7 para inspecionar\n a peça na casa correspondente \n=>");
                        if (fgets(line, sizeof(line), stdin)) {
                            if (1 == sscanf(line, "%d", &iz)) {
                                strcpy(str,line);
                                token = strtok(str, "_");

                                // Loop para obter todos os tokens
                                while (token != NULL) {
                                    // Converter o token para um inteiro e armazenar na array
                                    intArray[ic] = atoi(token);
                                    ic++;
                                    // Obter o próximo token
                                    token = strtok(NULL, "_");
                                }
                                //Jogo.tabuleiro[intArray[0]][[intArray[1]]];
                                peca pecaCod;
                                printf("x : %d / y : %d\n",intArray[0],intArray[1]);
                                if (intArray[0] < BOARD_WIDTH  && intArray[1] < BOARD_HEIGTH){
                                    if (Jogo.tabuleiro[intArray[0]][intArray[1]] != 0){
                                    if ((Jogo.tabuleiro[intArray[0]][intArray[1]]<0?0:1) == 0){
                                        pecaCod = Jogo.pecasPretas[abs(Jogo.tabuleiro[intArray[0]][intArray[1]])];
                                    }else{
                                        pecaCod = Jogo.pecasBrancas[Jogo.tabuleiro[intArray[0]][intArray[1]]];
                                    };
                                    printf("Codigo da Peca %d \n",pecaCod.cod);
                                    printf("%s ",pecas[pecaCod.cod]);
                                    printf(pecaCod.cor == 1?"BRANCO\n":"PRETO\n");
                                }else{
                                    printf("Espaco vazio, nenhuma peca aqui\n");
                                };
                                
                                }else{
                                    printf("Voce selecionou um index que invalido\n");
                                }
                                
                            }
                        }
                    break;
                };
            };
        };
    printf("codition check\n");
    }while(sair != 0);
    printf("code finalized\n");
    

    //looping para escolher a posição da peça
    for(int z = 0;z < BOARD_HEIGTH;z++){
        free(Jogo.tabuleiro[z]);
    };
    return 0;
}
