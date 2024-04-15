#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

bool colisaoBool(int p){
    if (p != 0) {
        return false;
    }
    else {
        return true;
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

    using namespace chrono;
    srand (time(NULL));
    milliseconds intervalo(500);
    auto inicio = high_resolution_clock::now();
    bool bomba = false;
    milliseconds flick1Bomba1(1000);
    milliseconds flick1Bomba2(2000);
    milliseconds intervaloBomba(3000);
    auto inicioBomba = high_resolution_clock::now();

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
    int bx, by;
    //onde está a bomba
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
                    //Randomificador de quantos movimentos ele irá fazer
                    auto atual = high_resolution_clock::now();
                    auto passou = duration_cast<milliseconds>(atual - inicio);
                    if (passou >= intervalo) {
                        int movDir = rand()%4+1;
                        switch (movDir) {
                        //para cima
                        case 1:
                            ix--;
                            if (colisaoBool(m[ix][iy])== false) {
                                ix++;
                            };
                        break;
                        //para baixo
                        case 2:
                            ix++;
                            if (colisaoBool(m[ix][iy])== false) {
                                ix--;
                            };
                        break;
                        //para direita
                        case 3:
                            iy++;
                            if (colisaoBool(m[ix][iy])== false) {
                                iy--;
                            };
                        break;
                        //para esquerda
                        case 4:
                            iy--;
                            if (colisaoBool(m[ix][iy])== false) {
                                iy++;
                            };
                        break;
                        }
                        inicio = atual;
                    }
                }else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        case 2: cout<<char(35); break; //parede quebravel
                        case 3: cout<<char(162); break; // bomba
                        case 4: cout<<char(79); break; // bomba Flick
                        case 5: cout<<char(88); break; // area de explosão
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
                case 'e': ///bomba
                    if (!bomba) {
                        m[x][y] = 3; // Coloca a bomba no mapa
                        bomba = true;
                        bx=x;
                        by=y;
                        inicioBomba = high_resolution_clock::now(); // Armazena o tempo atual
                    }
                    break;
            }
        }

        // Verifica se a bomba explodiu após 3 segundos
        auto agora = high_resolution_clock::now();
        auto passouBomba = duration_cast<milliseconds>(agora - inicioBomba);
        if (bomba && passouBomba >= flick1Bomba1) {
            m[bx][by] = 3;
            for (int l = 0; l < 13; l++) {
                for (int c = 0; c < 13; c++) {
                    if ((l==bx+1 && c == by && m[l][c] == 0) || (l==bx-1 && c == by && m[l][c] == 0)) { //para baixo e para cima explosão
                        m[l][c] = 5;
                    } else if ((l == bx && c == by-1 && m[l][c] == 0) || (l == bx && c == by+1 && m[l][c] == 0)) { //para direita e para esquerda explosão
                        m[l][c] = 5;
                    }
                }
            }
        }
        if (bomba && passouBomba >= flick1Bomba2) {
            m[bx][by] = 4;
        }
        if (bomba && passouBomba >= intervaloBomba) {
            // Explode a bomba
            m[bx][by] = 3;
            for (int l = 0; l < 13; l++) {
                for (int c = 0; c < 13; c++) {
                    if ((l==bx+1 && c == by && m[l][c] != 1) || (l==bx-1 && c == by && m[l][c] != 1)) { //para baixo e para cima explosão
                        m[l][c] = 0;
                        m[bx][by] = 0;
                        bomba = false;
                    } else if ((l == bx && c == by-1 && m[l][c] != 1) || (l == bx && c == by+1 && m[l][c] != 1)) { //para direita e para esquerda explosão
                        m[l][c] = 0;
                        m[bx][by] = 0;
                        bomba = false;
                    }
                }
            }
        }

    } //fim do laco do jogo
    return 0;
} //fim main
