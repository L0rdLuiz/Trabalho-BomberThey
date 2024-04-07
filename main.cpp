/**
    Estrutura inicial para um jogo
    versão: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

bool colisaoBool(int p){
    if (p != 0) {
        return false;
    }
    else {
        return true;
    }
}

int movInimigo(int ix, int iy) {
    //Randomificador de quantos movimentos ele irá fazer
    int movMax = rand()%3+1;

    for (int movI = 0; movI<=movMax; movI++) {
        int movDir = rand()%4+1;
        switch (movDir) {
            //para cima
            case 1:
                ix--;
                return(ix);
            break;
            //para baixo
            case 2:
                ix++;
                return(ix);
            break;
            //para direita
            case 3:
                iy++;
                return(iy);
            break;
            //para esquerda
            case 4:
                iy--;
                return(iy);
            break;
        }
    }
}

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    srand (time(NULL));

    int m[13][13]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,0,0,0,0,0,0,0,0,0,0,0,1,
                    1,2,1,0,1,2,1,0,1,2,1,0,1,
                    1,2,2,2,2,2,2,2,2,2,2,2,1,
                    1,0,1,2,1,0,1,2,1,0,1,2,1,
                    1,2,2,2,2,2,2,2,2,2,2,2,1,
                    1,2,1,0,1,2,1,0,1,2,1,0,1,
                    1,2,2,2,2,2,2,2,2,2,2,2,1,
                    1,2,1,0,1,2,1,0,1,2,1,0,1,
                    1,2,2,2,2,2,2,2,2,2,2,2,1,
                    1,0,1,2,1,0,1,0,1,2,1,0,1,
                    1,0,0,0,0,0,0,0,0,0,0,0,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1};

    int mov = 0;


    //Posicao inicial do personagem no console
    int x=1, y=1;
    //Posicao inicial inimigo
    int ix=11, iy=11;
    //Variavel para tecla precionada
    char tecla;

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<13;i++){
            for(int j=0;j<13;j++){
                if(i==x && j==y){
                    cout<<char(36); //personagem
                } else if(i==ix && j==iy) {
                    cout<<char(169); //inimigo
                    movInimigo(ix, iy);
                }else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        case 2: cout<<char(35); break; //parede quebravel
                        case 3: cout<<char(162); break; // bomba
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        } //fim for mapa

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    x--;
                    if (colisaoBool(m[x][y])== false) {
                        x++;
                    };
                break;
                case 80: case 's': ///baixo
                    x++;
                    if (colisaoBool(m[x][y])== false) {
                        x--;
                    };
                break;
                case 75:case 'a': ///esquerda
                    y--;
                    if (colisaoBool(m[x][y])== false) {
                        y++;
                    };
                break;
                case 77: case 'd': ///direita
                    y++;
                    if (colisaoBool(m[x][y])== false) {
                        y--;
                    };
                break;
                case 90: case 'e': ///bomba
                    //na posição x e y ele solta a bomba
                    m[x][y] = 3;
                    for(int l=0;l<13;l++){
                        for(int c=0;c<13;c++){
                           if (m[l][c]==3 && m[l==x][c==y]) {
                                m[x][y] = 0;
                           }
                        }
                    }


                break;
            }
         }


    } //fim do laco do jogo

    return 0;
} //fim main
