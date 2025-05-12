#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
//tabuleiro
#define BOARD_HEIGTH 8
#define BOARD_WIDTH 8
//pecas
#define PIECES_NAME {"PEAO","CAVALO","BISPO","TORRE","RAINHA","REI"};
#define PEAO 0
#define CAVALO 1
#define BISPO 2
#define TORRE 3
#define RAINHA 4
#define REI 5
//opcoes para menu
#define SAIR_LOOP 0
#define INSPECIONAR 1
#define MOVER 2
//descomente se estiver rodando no windowns
//#include <Windows.h>
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
  int turnoAtual;
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
    };
    for(int j = 0; j < BOARD_WIDTH; j++){
        tabuleiroCompleto.tabuleiro[i][j] = 0;
    };
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
  
  tabuleiroCompleto.turnoAtual = 0;
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

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
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
    printf("Comando     |   Digite\n");
    printf("Sair              0\n");
    printf("Examinar peça     1\n");
    printf("Mover peça        2\n");
    printf("===========================\n");
};

int verificarMovimento(peca* X, int xAtual, int yAtual, int xAlvo, int yAlvo) {
    //peca* X, -> objeto da peca
    //int xAtual, -> posicao atual
    //int yAtual, -> posicial atual
    //int xAlvo, -> posicao desejada
    //int yAlvo -> posicao desejada
    //pegue o código da peça(que determina qual o movimento dela)]
    //verificar se a coordenada inputada é valida com base no tipo da peça
    //esse bloco não se responsabiliza se o espaço está vago , apenas se ele está
    //lógicamente correto
    //estou desconsiderando a regra de colisão, que diz que uma peça não pode passar por cima da outra
    //meu objetivo é simular apenas o movimento
    if (xAtual < 0 || yAtual < 0 || xAlvo < 0 || yAlvo < 0 || xAlvo > BOARD_WIDTH - 1 || yAlvo > BOARD_HEIGTH - 1) {
        return 0; //inválido
    }

    switch (X->cod) {
        case PEAO:
            // se permanecer na mesma coluna
            // e se estiver se movendo apenas uma casa
            // estou desconsiderando a regra que no
            //começo eles aindam mais de uma casa

            return ((xAtual == xAlvo) && (abs(yAtual - yAlvo) == 1)) ? 1 : 0;
            break;

        case CAVALO:
            return ((abs(xAtual - xAlvo) == 1) && (abs(yAtual - yAlvo) == 2) || (abs(xAtual - xAlvo) == 2) && (abs(yAtual - yAlvo) == 1)) ? 1 : 0;
            break;

        case BISPO:
            // Movimento do Bispo: Diagonal
            return (abs(xAlvo - xAtual) == abs(yAlvo - yAtual)) ? 1 : 0;
            break;

        case TORRE:
            // Movimento da Torre: Horizontal ou Vertical
            return (xAtual == xAlvo || yAtual == yAlvo) ? 1 : 0;
            break;

        case RAINHA:
            // Movimento da Rainha: Diagonal ou Horizontal ou Vertical
            return (abs(xAlvo - xAtual) == abs(yAlvo - yAtual) || xAtual == xAlvo || yAtual == yAlvo) ? 1 : 0;
            break;

        case REI:
            // Movimento do Rei: Uma casa em qualquer direção
            return (abs(xAlvo - xAtual) <= 1 && abs(yAlvo - yAtual) <= 1) ? 1 : 0;
            break;
    }
    return 0;
}

int main(){
    //iniciar tabuleiro
    //descomente se estiver rodando no windowns
    //SetConsoleOutputCP(CP_UTF8);
    int xAtual = 0;
    int yAtual = 0;
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
                        printf("Digite a coordenada x e y no formato x_y\n=> ");
                        if (fgets(line, sizeof(line), stdin)) {
                            if (1 == sscanf(line, "%d", &iz)) {
                                strcpy(str,line);
                                token = strtok(str, "_");
                               
                                // Loop para obter todos os tokens
                                while (token != NULL) {
                                    remove_spaces(token);
                                    // Converter o token para um inteiro e armazenar na array
                                    intArray[ic] = atoi(token);
                                    ic++;
                                    // Obter o próximo token
                                    token = strtok(NULL, "_");
                                };
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
                                };
                                
                            };
                        };
                    break;
                    
                    case MOVER:
                    ic = 0; // Reinicializar ic
                    printf("Digite a coordenada de qual peca ");
                    printf(Jogo.turnoAtual % 2 == 0?"branca ":"preta ");
                    printf("você deseja mover\n");
                    printf("Digite a coordenada x e y no formato x_y\n=> ");
                    if (fgets(line, sizeof(line), stdin)){
                        if (1 == sscanf(line, "%d", &iz)){
                            strncpy(str, line, sizeof(str) - 1); // Usar strncpy
                            str[sizeof(str) - 1] = '\0'; 
                            token = strtok(str, "_");
                               
                            // Loop para obter todos os tokens
                            while (token != NULL) {
                                remove_spaces(token);
                                intArray[ic] = atoi(token);
                                ic++;
                                token = strtok(NULL, "_");
                            }
                            peca pecaCod;
                            printf("x : %d / y : %d\n", intArray[0], intArray[1]);
                            // Verificação de limites reforçada
                            if (intArray[0] >= 0 && intArray[0] < BOARD_WIDTH && intArray[1] >= 0 && intArray[1] < BOARD_HEIGTH){
                                if (Jogo.tabuleiro[intArray[0]][intArray[1]] != 0){
                                    if ((Jogo.tabuleiro[intArray[0]][intArray[1]]<0?0:1) == (Jogo.turnoAtual % 2 ==0?1:0)){
                                        // Verificação de limites para pecasBrancas e pecasPretas
                                        if (abs(Jogo.tabuleiro[intArray[0]][intArray[1]]) >= 1 && abs(Jogo.tabuleiro[intArray[0]][intArray[1]]) <= 16) {
                                            if (Jogo.tabuleiro[intArray[0]][intArray[1]] < 0) {
                                                pecaCod = Jogo.pecasPretas[abs(Jogo.tabuleiro[intArray[0]][intArray[1]])];
                                            } else {
                                                pecaCod = Jogo.pecasBrancas[Jogo.tabuleiro[intArray[0]][intArray[1]]];
                                            }
                                        } else {
                                            printf("Erro: Índice da peça fora dos limites.\n");
                                            continue; // Ou tratar o erro de outra forma
                                        }
                                    } else {
                                        printf(Jogo.tabuleiro[intArray[0]][intArray[1]]<0?"Selecione pecas pretas":"Selecione pecas brancas");
                                    }
                                    if(pecaCod.cor == (Jogo.turnoAtual % 2 ==0?1:0)){
                                        xAtual = intArray[0];
                                        yAtual = intArray[1];
                                        printf("Digite para onde você quer mover sua peça\n");
                                        printf("Digite a coordenada x e y no formato x_y\n=> ");
                                        // ... (Código para obter a nova coordenada)
                                        // exibir uma explicação do movimento, e mostrar opções válidas para se mover
                                        ic = 0;
                                        if (fgets(line, sizeof(line), stdin)){
                                            if (1 == sscanf(line, "%d", &iz)){
                                                strncpy(str, line, sizeof(str) - 1); // Usar strncpy
                                                str[sizeof(str) - 1] = '\0'; 
                                                    token = strtok(str, "_");
                               
                                             // Loop para obter todos os tokens
                                            while (token != NULL) {
                                                remove_spaces(token);
                                                intArray[ic] = atoi(token);
                                                ic++;
                                                token = strtok(NULL, "_");
                                            };
                                            if ( verificarMovimento(&pecaCod, xAtual, yAtual, intArray[0], intArray[1]) == 1){
                                                printf("\n ==> Movendo ");
                                                printf(pecas[pecaCod.cod]);
                                                printf(" '");
                                                printf(Jogo.tabuleiro[xAtual][yAtual]<0?"PRETA":"BRANCA");
                                                printf("' para : (%d , %d) \n",intArray[0], intArray[1]);
                                                //PROVAVELMENTE A LÓGICA DE CAPTURA VIRIA AQUI, DEPOIS IMPLEMENTAREI
                                                //eu posso fazer um loop para contar as peças de cada lado, depois editar a propriedade 'capturada'
                                                Jogo.tabuleiro[intArray[0]][intArray[1]] = Jogo.tabuleiro[xAtual][yAtual];
                                                Jogo.tabuleiro[xAtual][yAtual] = 0;
                                                
                                            
                                            }else{
                                                printf("\033[1;31mMovimento inválido\033[0m\n");
                                            };};
                                        };
                                        };
                                } else {
                                    printf("Movimento inválido\n");
                                }
                            } else {
                                printf("Voce selecionou um index que invalido\n");
                            }
                        }        
                    }
                    break;
                };
            };
        };
    }while(sair != 0);
          
    //looping para escolher a posição da peça
    for(int z = 0;z < BOARD_HEIGTH;z++){
        free(Jogo.tabuleiro[z]);
    };
    return 0;
}
