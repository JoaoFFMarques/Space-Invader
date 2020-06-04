/*
Space Invader
Jogo Trabalho Daniela
Jota & Felipe
16/05/20
*/

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include <string.h>

enum sentido{CIMA = 72, ESQUERDA = 75, DIREITA = 77, BAIXO = 80};
enum quantidades{VIDA = 1, MUNICOES = 6, RANKING = 3};
enum dimencoes{ ALTURA = 15, LARGURA =18, TELA = 45, DIMENSAO= 92, INI_X = 23 , FIM_X = 66};

typedef struct variaveis{
    int x;
    int y;
    int aux;
    int efetuado;
    char **mapa;
}variavel;

void menu();
void sobre();
void mostrapontos();

void fimdejogo(int fimjogo);
void pontuacao(int maiorpontuacao);

int gameplay(int *maiorpontuacao);
char** constroe_mapa_jogo();
char** constroe_mapa_inimigo();
void construir_base(int y, int x, char **mapa);
void construir_nave(int y, int x, char **mapa);
void preparar_inimigo(int y, int x, char **mapa_inimigo, char **mapa_colisao);
void construir_inimigo(int y, int x, char **mapa_jogo, char **mapa_inimigo);
void apagar_inimigo(int y, int x, char **mapa);
void apagar_nave(int y, int x, char **mapa);
void imprimir_mapa(int *aux_jogo, int *aux_disparo, char **mapa, int *pontos, int vida, int *morte);
void controle_inimigo(int *x, int *y, char **mapa_jogo, char **mapa_inimigo, char **mapa_colisao, int *descer, int direcao);
void controle(char comando, int *y, int *x, int *aux_disparo, int *disparo);
void disparo_(variavel* disparo, char **mapa, char **colisao);
void bomba_(int *efetuado, int *Y, int X, char **mapa, int *morte);

void textcolor(int iColor);
void posicao_tela(int X, int Y);
void apagar_tela();
void pausar();
void logo();


int main(){

    system("mode con:cols=110 lines=60"); //dimensionamento da tela
    setlocale(LC_ALL, "");

    srand(time(NULL));

    for(;;){

        int maiorpontuacao=0;

        menu();
        fimdejogo(gameplay(&maiorpontuacao));
        pontuacao(maiorpontuacao);
    }

    return 0;
}

void menu(){

    char opcao;
    int menuY=1;

    for(int i=0;i<1;){

        textcolor(15);
        logo();

        posicao_tela(39, 26);
        printf("ESCOLHA A OPÇÃO");
        posicao_tela(30, 27);
        printf("use as setas direcionais e pressione ESPAÇO");

        posicao_tela(38, 29);
        if(menuY==1)
            textcolor(14);
        else
            textcolor(15);

        printf(" INICIAR PARTIDA");

        posicao_tela(38, 30);
        if(menuY==2)
            textcolor(14);
        else
            textcolor(15);


        printf(" SOBRE O JOGO");

        posicao_tela(38, 31);
        if(menuY==3)
            textcolor(14);
        else
            textcolor(15);

        printf(" HI-SCORE");

        posicao_tela(38, 32);
        if(menuY==4)
            textcolor(14);
        else
            textcolor(15);

        printf(" SAIR");

        opcao=getch();

        switch(opcao){
            case BAIXO:
                if(menuY<=3)
                    menuY+=1;
                else
                    menuY=1;
            break;

            case CIMA:
                if(menuY>=2)
                    menuY-=1;
                else
                    menuY=4;
            break;

            case ' ':
                if(menuY==1)
                    i++;
                else if(menuY==2)
                    sobre();
                else if(menuY==3)
                    mostrapontos();
                else if(menuY==4)
                    exit(1);
            break;
        }
    }

}

void sobre(){

    apagar_tela();
    logo();
    posicao_tela(25, 27);
    printf("Em SPACE INVADERS o jogador deverá impedir que os");
    posicao_tela(25, 28);
    printf(" alienigenas invadam nosso planeta, contando com");
    posicao_tela(25, 29);
    printf("os pdoerosos tiros de seu TANQUE par destruir as naves inimigas ");
    posicao_tela(25, 30);
    printf("O jogador deverá gerenciar a munição de seu tanque ");
    posicao_tela(25, 31);
    printf("para não ser pego desprevinido E também desviar das bombas");
    posicao_tela(25, 32);
    printf(" alienigenas usando as construções para se esconder ");

    getch();
    apagar_tela();
}

void mostrapontos(){

    FILE *hiscore;
    int score[RANKING];
    char iniciais[RANKING][5];

    for(int i=0;i<=RANKING;i++){
        score[i]=0;
    }

    apagar_tela();
    logo();

    hiscore=fopen("score.bin", "r+b");
    if(hiscore==NULL){
        posicao_tela(30,32);
        printf("NÃO HÁ PONTUAÇÃO SALVA AINDA!!");
        fclose(hiscore);
    }
    else{
        for(int i=0;i<RANKING;i++){
            fread(&score[i], sizeof(int), 1, hiscore);
            fgets(iniciais[i],5,hiscore);
        }

        posicao_tela(30,30);
        printf("HISCORE:");

        for(int i=0;i<RANKING && score[i]!=0;i++){
            posicao_tela(30,(31+i));
            printf("%i %s", i+1, iniciais[i]);
            posicao_tela(40,(31+i));
            printf("%i PONTOS", score[i]);
        }
        fclose(hiscore);

    }
    getch();
    apagar_tela();

}

void fimdejogo(int fimjogo){

    if(fimjogo==0){
        apagar_tela();
        logo();
        posicao_tela(48,27);
        printf("VOCE VENCEU!!");
        posicao_tela(35,29);
        printf("A INVASÃO ALIENÍGENA FOI DERROTADA!");
        getch();
        getch();
    }
    else{
        apagar_tela();
        logo();
        posicao_tela(48,27);
        printf("VOCE PERDEU!!");
        posicao_tela(44,29);
        printf("A TERRA FOI INVADIDA!");
        getch();
        getch();
    }

    apagar_tela();
}

void pontuacao(int maiorpontuacao){

    FILE *hiscore;

    char iniciaisatual[5];
    int score[RANKING];
    char iniciais[RANKING][5];

    for(int i=0;i<RANKING;i++){
        score[i]=0;
    }


    logo();

    posicao_tela(48,27);
    printf("JOGO TERMINOU");
    posicao_tela(40,29);
    printf("SUA MAIOR PONTUAÇÃO FOI = %i", maiorpontuacao);
    getch();

    hiscore=fopen("score.bin", "r+b");

    if(hiscore==NULL){
        fclose(hiscore);
        hiscore=fopen("score.bin", "w+b");
        posicao_tela(40,35);
        printf("INSIRA SUAS INICIAIS (ATÉ 4 CARACTERES)");
        posicao_tela(40,37);
        fgets(iniciaisatual, 5, stdin);
        getchar();
        iniciaisatual[strcspn( iniciaisatual, "\n" )] = '\0';

        fwrite(&maiorpontuacao, sizeof(int), 1, hiscore);
        fwrite(iniciaisatual, sizeof(strlen(iniciaisatual)), 1, hiscore);
        fclose(hiscore);

    }
    else{

        for (int i=0;i<RANKING;i++){
            fread(&score[i], sizeof(int), 1, hiscore);
            fgets(iniciais[i],5,hiscore);
        }

        posicao_tela(40,30);
        printf("HISCORE:");

        for(int i=0;i<RANKING && score[i]!=0;i++){
            posicao_tela(40,(31+i));
            printf("%i %s", i+1, iniciais[i]);
            posicao_tela(50,(31+i));
            printf("%i PONTOS", score[i]);
        }

        fclose(hiscore);
        rewind(hiscore);

        if (maiorpontuacao>score[0]){

            hiscore=fopen("score.bin", "r+b");

            posicao_tela(40,35);
            printf("INSIRA SUAS INICIAIS (ATÉ 4 CARACTERES)");
            posicao_tela(40,37);
            fgets(iniciaisatual, 5, stdin);
            getchar();
            iniciaisatual[strcspn( iniciaisatual, "\n" )] = '\0';

            fwrite(&maiorpontuacao, sizeof(int), 1, hiscore);
            fwrite(iniciaisatual, sizeof(strlen(iniciaisatual)), 1, hiscore);

            fwrite(&score[0], sizeof(int), 1, hiscore);
            fwrite(iniciais[0], sizeof(strlen(iniciais[0])), 1, hiscore);

            if(score[1]!=0){
                fwrite(&score[1], sizeof(int), 1, hiscore);
                fwrite(iniciais[1], sizeof(strlen(iniciais[1])), 1, hiscore);
            }

            fclose(hiscore);
        }

        else if (maiorpontuacao>score[1]){

            hiscore=fopen("score.bin", "r+b");

            posicao_tela(40,35);
            printf("INSIRA SUAS INICIAIS (ATÉ 4 CARACTERES)");
            posicao_tela(40,37);
            fgets(iniciaisatual, 5, stdin);
            getchar();
            iniciaisatual[strcspn( iniciaisatual, "\n" )] = '\0';

            fwrite(&score[0], sizeof(int), 1, hiscore);
            fwrite(iniciais[0], sizeof(strlen(iniciais[0])), 1, hiscore);

            fwrite(&maiorpontuacao, sizeof(int), 1, hiscore);
            fwrite(iniciaisatual, sizeof(strlen(iniciaisatual)), 1, hiscore);

            if(score[1]!=0){
                fwrite(&score[1], sizeof(int), 1, hiscore);
                fwrite(iniciais[1], sizeof(strlen(iniciais[1])), 1, hiscore);
            }

            fclose(hiscore);
        }

        else if (maiorpontuacao>score[2]){

            hiscore=fopen("score.bin", "r+b");

            posicao_tela(40,35);
            printf("INSIRA SUAS INICIAIS (ATÉ 4 CARACTERES)");
            posicao_tela(40,37);
            fgets(iniciaisatual, 5, stdin);
            getchar();
            iniciaisatual[strcspn( iniciaisatual, "\n" )] = '\0';

            fwrite(&score[0], sizeof(int), 1, hiscore);
            fwrite(iniciais[0], sizeof(strlen(iniciais[0])), 1, hiscore);

            fwrite(&score[1], sizeof(int), 1, hiscore);
            fwrite(iniciais[1], sizeof(strlen(iniciais[1])), 1, hiscore);

            fwrite(&maiorpontuacao, sizeof(int), 1, hiscore);
            fwrite(iniciaisatual, sizeof(strlen(iniciaisatual)), 1, hiscore);

            fclose(hiscore);
        }
    }

    getch();
    apagar_tela();

}

int gameplay(int *maiorpontuacao){

    int direcao;
    int descer=0;
    int pontos=0;

    char comando;

    int morte=0;
    int vida=VIDA;
    int mov=4;

    variavel nave, disparo[1000], inimigo, jogo, colisao, bomba[1000];

    for(;vida>0;){

        jogo.mapa=constroe_mapa_jogo();
        inimigo.mapa=constroe_mapa_inimigo();
        colisao.mapa=constroe_mapa_inimigo();

        construir_base(37, INI_X+5, jogo.mapa);
        construir_base(37, INI_X+15, jogo.mapa);
        construir_base(37, INI_X+26, jogo.mapa);
        construir_base(37, INI_X+36, jogo.mapa);

        morte=0;

        nave.x=TELA;
        nave.y=TELA-3;
        nave.aux=0;
        bomba[0].aux=0;

        inimigo.y=3;
        inimigo.x=INI_X;

        direcao=DIREITA;

        for(int cont=1;morte<1;cont++){

            construir_nave(nave.y, nave.x, jogo.mapa);
            preparar_inimigo(inimigo.y, inimigo.x, inimigo.mapa, colisao.mapa);
            construir_inimigo(inimigo.y, inimigo.x, jogo.mapa, inimigo.mapa);

            imprimir_mapa(&jogo.aux, &disparo[0].aux, jogo.mapa, &pontos, vida, &morte);

            if(kbhit()){
                comando=getch();
                apagar_nave(nave.y, nave.x, jogo.mapa);
                controle(comando, &nave.y, &nave.x, &disparo[0].aux, &disparo[nave.aux].efetuado);
                if (disparo[nave.aux].efetuado==1){
                    disparo[nave.aux].x = nave.x;
                    if(cont%2==0)
                        disparo[nave.aux].y = nave.y-1;
                    else
                        disparo[nave.aux].y = nave.y;

                    nave.aux+=1;
                }
            }
            fflush(stdin);

            for(int i=0;i<nave.aux;i++){ //disparos da nave
                //disparo_(&disparo[i].efetuado, &disparo[i].y, disparo[i].x, jogo.mapa, colisao.mapa);
                disparo_(&disparo[i], jogo.mapa, colisao.mapa);
            }

            if(nave.aux==1000)
                nave.aux=0;

            if(inimigo.y<21){
                if(cont%7==0){
                    bomba[bomba[0].aux].efetuado=1;
                    bomba[bomba[0].aux].y=inimigo.y+ALTURA;
                    bomba[bomba[0].aux].x=INI_X+ rand()%FIM_X;
                    bomba[0].aux+=1;
                }
            }
            for(int i=0;i<bomba[0].aux;i++){ //Bombas dos alienigenas
                bomba_(&bomba[i].efetuado, &bomba[i].y, bomba[i].x, jogo.mapa, &morte);
            }

            if(bomba[0].aux==1000)
                bomba[0].aux=0;

            if (jogo.aux==0){
                vida=0;
                break;
            }

            if(cont%mov==0){
                if(pontos>=60)
                    mov=1;
                else if(pontos>=30)
                    mov=2;
                else
                    mov=3;

                for(int i=inimigo.y; i<TELA;i++){
                    if(jogo.mapa[i][FIM_X]=='@' && direcao==DIREITA){
                        direcao=ESQUERDA;
                        if(pontos>=60){
                            descer=3;
                        }
                        else if(pontos>=30){
                            descer=2;
                        }
                        else
                            descer=1;
                    }
                    else if(jogo.mapa[i][INI_X]=='@' && direcao==ESQUERDA){
                        direcao=DIREITA;
                        if(pontos>=60){
                            descer=3;
                        }
                        else if(pontos>=30){
                            descer=2;
                        }
                        else
                            descer=1;
                    }
                }
                controle_inimigo(&inimigo.x,&inimigo.y, jogo.mapa, inimigo.mapa, colisao.mapa, &descer, direcao);
            }

        }

        free(jogo.mapa);
        free(inimigo.mapa);
        free(colisao.mapa);

        if (morte==1){
            posicao_tela(TELA,(TELA/2));
            printf("VOCE MORREU");
            posicao_tela(TELA-10,((TELA/2)+2));
            printf("APERTE QUALQUER TECLA PARA RETORNAR");

            if(pontos>*maiorpontuacao)
                *maiorpontuacao=pontos;

            getch();
            vida-=1;

        }

    }

    if(pontos>*maiorpontuacao)
        *maiorpontuacao=pontos;

    return jogo.aux;
}

char** constroe_mapa_jogo(){

   char **mapa;
   mapa = (char**) malloc(DIMENSAO*sizeof(char*));        /*Alocação de uma dimensão da sala*/
   for(int i = 0; i < DIMENSAO; i++){
       mapa[i] = (char*) malloc(DIMENSAO*sizeof(char));  /*Alocação da segunda dimensão da sala*/
       for(int j = 0; j < DIMENSAO; j++){
           if(i == (TELA-1) )
                mapa[i][j] ='H';

            else
                mapa[i][j]=' ';
       }
   }
   return mapa;
}

char** constroe_mapa_inimigo(){

   char **mapa;
   mapa = (char**) malloc(DIMENSAO*sizeof(char*));        /*Alocação de uma dimensão da sala*/
   for(int i = 0; i < DIMENSAO; i++){
       mapa[i] = (char*) malloc(DIMENSAO*sizeof(char));  /*Alocação da segunda dimensão da sala*/
       for(int j = 0; j < DIMENSAO; j++){
            mapa[i][j] ='0';
       }
   }
   return mapa;
}

void construir_base(int y, int x, char **mapa){

    for (int i=y+1;i<=y+2;i++){
        for(int j=x-1;j<=x+4;j++){
            mapa[i][j]='#';
        }
    }

    for (int i=x;i<=x+3;i++)
    mapa[y][i]='#';

    mapa[y-1][x+1]='#';
    mapa[y-1][x+2]='#';
}

void construir_nave(int y, int x, char **mapa){

    mapa[y][x-1]='<';
    mapa[y][x]='W';
    mapa[y][x+1]='>';
    mapa[y-1][x]='A';
}

void preparar_inimigo(int y, int x, char **mapa_inimigo, char **mapa_colisao){

    for(int i=0;i<ALTURA;i++){
        for(int j=0;j<LARGURA;j++){
            if (((i==0 && j==0) || (i==4 && j==0) || (i==8 && j==0) || (i==12 && j==0) ||
                (i==0 && j==2) || (i==4 && j==2) || (i==8 && j==2) || (i==12 && j==2) ||
                (i==0 && j==5) || (i==4 && j==5) || (i==8 && j==5) || (i==12 && j==5) ||
                (i==0 && j==7) || (i==4 && j==7) || (i==8 && j==7) || (i==12 && j==7) ||
                (i==0 && j==10) || (i==4 && j==10) || (i==8 && j==10) || (i==12 && j==10) ||
                (i==0 && j==12) || (i==4 && j==12) || (i==8 && j==12) || (i==12 && j==12) ||
                (i==0 && j==15) || (i==4 && j==15) || (i==8 && j==15) || (i==12 && j==15) ||
                (i==0 && j==17) || (i==4 && j==17) || (i==8 && j==17) || (i==12 && j==17) ||
                (i==2 && j==1) || (i==6 && j==1) || (i==10 && j==1) || (i==14 && j==1) ||
                (i==2 && j==6) || (i==6 && j==6) || (i==10 && j==6) || (i==14 && j==6) ||
                (i==2 && j==11) || (i==6 && j==11) || (i==10 && j==11) || (i==14 && j==11) ||
                (i==2 && j==16) || (i==6 && j==16) || (i==10 && j==16) || (i==14 && j==16) ||
                (i==3) || (i==7)|| (i==11) || (j==3) || (j==4) || (j==8) || (j==9) || (j==13) || (j==14)) ||
                mapa_colisao[y+i][x+j]=='1'){
                mapa_inimigo[y+i][x+j]='0';
            }
            else
                mapa_inimigo[y+i][x+j]='1';
        }
    }
}

void construir_inimigo(int y, int x, char **mapa_jogo, char **mapa_inimigo){

    for(int i=0;i<ALTURA;i++){
        for(int j=0;j<LARGURA;j++){
            if(mapa_inimigo[y+i][x+j]=='1')
                mapa_jogo[y+i][x+j]='@';
        }
    }
}

void apagar_inimigo(int y, int x, char **mapa){
    for(int i=0;i<ALTURA;i++){
        for(int j=0;j<LARGURA;j++){
            if(mapa[y+i][x+j]=='|' || mapa[y+i][x+j]=='H'){}
            else
                mapa[y+i][x+j]=' ';
        }
    }
}

void apagar_nave(int y, int x, char **mapa){

    mapa[y][x-1]=' ';
    mapa[y][x]=' ';
    mapa[y][x+1]=' ';
    mapa[y-1][x]=' ';

}

void imprimir_mapa(int *aux_jogo, int *aux_disparo, char **mapa, int *pontos, int vida, int *morte){

    *aux_jogo=0;
    *aux_disparo=0;

    posicao_tela(0,0);

    for (int i=0;i<TELA;i++){
        printf("\n\t");
        for(int j=INI_X;j<=FIM_X;j++){
            if (mapa[i][j]=='H'){
                textcolor(15);
                printf(" %c", mapa[i][j]);
            }

            else if (mapa[i][j]=='@'){
                textcolor(10);
                printf(" %c", mapa[i][j]);
            }
            else if (mapa[i][j]=='*'){
                textcolor(12);
                printf(" %c", mapa[i][j]);
            }
             else if (mapa[i][j]=='#'){
                textcolor(8);
                printf(" %c", mapa[i][j]);
            }

            else if (mapa[i][j]!='H'){
                textcolor(14);
                printf(" %c", mapa[i][j]);
            }

            else
                printf(" %c", mapa[i][j]);

            if(mapa[i][j]=='@')
                *aux_jogo+=1;

            if(mapa[i][j]=='|')
                *aux_disparo+=1;

            if (i>38){
                if(mapa[i][j]=='@')
                    *morte=1;
            }

        }
    }


    *pontos=96-*aux_jogo;

    posicao_tela(TELA-10,(TELA+2));
    printf("PONTOS = %i         VIDA = %i      MUNIÇÃO = %i  ", *pontos, vida, MUNICOES-*aux_disparo);
    posicao_tela(TELA-15,(TELA+4));
    printf("MOVIMENTO: SETAS DIRECIONAIS (DIREITA E ESQUERDA)");
    posicao_tela(TELA-15,(TELA+6));
    printf("DISPARO:   <ESPAÇO>");
    posicao_tela(TELA+10,(TELA+6));
    printf("PAUSAR:   <P> ou <p>");

}

void controle_inimigo(int *x, int *y, char **mapa_jogo, char **mapa_inimigo, char **mapa_colisao, int *descer, int direcao){

   if(*descer!=0){
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_jogo[*y+i+*descer][*x+j]=='|')
                    mapa_inimigo[*y+i+*descer][*x+j]='0';
                else if(mapa_colisao[*y+i][*x+j]=='0')
                    mapa_inimigo[*y+i+*descer][*x+j]='1';
                else
                    mapa_inimigo[*y+i+*descer][*x+j]='0';
            }
        }
        apagar_inimigo(*y, *x, mapa_jogo);
        *y+=*descer;

        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_inimigo[*y+i-*descer][*x+j]=='1')
                    mapa_colisao[*y+i][*x+j]='0';
                else
                    mapa_colisao[*y+i][*x+j]='1';
            }
        }
        *descer=0;
    }

    if(direcao==DIREITA){
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_jogo[*y+i][*x+j+1]=='|')
                    mapa_colisao[*y+i][*x+j+1]='1';
                else if(mapa_inimigo[*y+i][*x+j]=='0')
                    mapa_colisao[*y+i][*x+j+1]='1';
                else
                    mapa_colisao[*y+i][*x+j+1]='0';
            }
        }
        apagar_inimigo(*y, *x, mapa_jogo);
        *x+=1;
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_colisao[*y+i][*x+j-1]=='1')
                    mapa_inimigo[*y+i][*x+j]='0';
                else
                    mapa_inimigo[*y+i][*x+j]='1';
            }
        }


    }

    if(direcao==ESQUERDA){
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_jogo[*y+i][*x+j-1]=='|')
                    mapa_colisao[*y+i][*x+j-1]='1';
                else if(mapa_inimigo[*y+i][*x+j]=='0')
                    mapa_colisao[*y+i][*x+j-1]='1';
                else
                    mapa_colisao[*y+i][*x+j-1]='0';
            }
        }
        apagar_inimigo(*y, *x, mapa_jogo);
        *x-=1;
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(mapa_colisao[*y+i][*x+j+1]=='1')
                    mapa_inimigo[*y+i][*x+j]='0';
                else
                    mapa_inimigo[*y+i][*x+j]='1';
            }
        }

    }

}

void controle(char comando, int *y, int *x, int *aux_disparo, int *disparo){

    switch(comando){

        case -32: //VALOR QUE TODA SETA RETORNA E DEVE SER DESPREZADO
        break;

        case ESQUERDA:   //esquerda
            if(*x-1==INI_X){}
            else
                *x-=1;
        break;

        case DIREITA:    //direita
            if(*x+2==FIM_X+1){}
            else
                *x+=1;
        break;

        case ' ':
            if(*aux_disparo>=MUNICOES){}
            else{
                *disparo=1;
            }
        break;

        case 'P':
            pausar();
        break;

        case 'p':
            pausar();
        break;

        default:
        break;

    }

}

void disparo_(variavel* disparo, char **mapa, char **colisao){

    if(disparo->efetuado==1){
        if(mapa[disparo->y-1][disparo->x]=='#'){
            disparo->efetuado=0;
            mapa[disparo->y-1][disparo->x]=' ';
        }

        else if(mapa[disparo->y-1][disparo->x]=='@'){
            disparo->efetuado=0;
            colisao[disparo->y-1][disparo->x]='1';
            mapa[disparo->y-1][disparo->x]=' ';
        }

        else if(mapa[disparo->y][disparo->x]=='@'){
            disparo->efetuado=0;
            colisao[disparo->y][disparo->x]='1';
            mapa[disparo->y][disparo->x]=' ';
        }

        else if (disparo->y==1)
            disparo->efetuado=0;

        if (disparo->efetuado==1){
            mapa[disparo->y][disparo->x]=' ';
            disparo->y-=1;
            mapa[disparo->y][disparo->x]='|';
        }

        else if(disparo->efetuado==0)
            mapa[disparo->y][disparo->x]=' ';

    }


}

void bomba_(int *efetuado, int *Y, int X, char **mapa, int *morte){

    if(*efetuado==1){
        if(mapa[*Y+1][X]=='#'){
            *efetuado=0;
            mapa[*Y+1][X]=' ';
        }

        else if(mapa[*Y+1][X]=='A' ||
                mapa[*Y+1][X]=='<' ||
                mapa[*Y+1][X]=='>' ){
            *morte+=1;
            *efetuado=0;
        }

        else if (*Y==(TELA-2))
            *efetuado=0;

        if (*efetuado==1){
            mapa[*Y][X]=' ';
            *Y+=1;
            mapa[*Y][X]='*';
        }

        else if(*efetuado==0)
            mapa[*Y][X]=' ';

       }

}

void textcolor (int iColor){ //comandos com windows.h
    HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(hl, &bufferInfo);
    bufferInfo.wAttributes &= 0x00F0;
    SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= iColor);
}

void posicao_tela(int X, int Y){ //comandos com windows.h

    COORD coord = {X,Y};
    HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition ( h, coord );
}

void apagar_tela(){  //comandos com windows.h
  DWORD n;                         /* Numero de caracteres escritos */
  DWORD size;                      /* Numero Visiveis de caracteres */
  COORD coord = {0};               /* posição do canto esquerdo da tela */
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  /* Get a handle to the console */
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );

  GetConsoleScreenBufferInfo ( h, &csbi );

  /* encontrar o numero de caracteres subrescritos */
  size = csbi.dwSize.X * csbi.dwSize.Y;

  /* sobreescrever o buffer da tela com espaço em branco */
  FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
  GetConsoleScreenBufferInfo ( h, &csbi );
  FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );
  /* Recolocar o cursor no topo esquerdo da tela */
  SetConsoleCursorPosition ( h, coord );
}

void pausar(){
    posicao_tela(TELA+2,(TELA/2));
    printf("JOGO PAUSADO");
    posicao_tela(TELA-8,((TELA/2)+2));
    printf("APERTE QUALQUER TECLA PARA RETORNAR");
    getch();
}

void logo(){

    char logo[14][67];
    FILE *arq_logo;

    arq_logo=fopen("logo.lg", "r+b");

    if(arq_logo==NULL){
        posicao_tela(30,32);
        printf("NÃO HÁ ARQUIVO DE LOGO");
        fclose(arq_logo);
    }
    else{
        for(int i=0;i<13;i++){
            fgets(logo[i],67,arq_logo);
            posicao_tela(20,(10+i));
            printf("%s", logo[i]);
        }
    }
    fclose(arq_logo);

}
