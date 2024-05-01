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
    if (p != 0 && p!= 5 && p!=6) {
        return false;
    }
    else {
        return true;
    }
}

void flick (int m[13][13], int bx, int by, bool Explosao) {
    if (Explosao == false) {
        for (int l = 0; l < 13; l++) {
            for (int c = 0; c < 13; c++) {
                if ((l==bx+1 && c == by && m[l][c] == 0) || (l==bx-1 && c == by && m[l][c] == 0)) { //para baixo e para cima explosão
                    m[l][c] = 5;
                } else if ((l == bx && c == by-1 && m[l][c] == 0) || (l == bx && c == by+1 && m[l][c] == 0)) { //para direita e para esquerda explosão
                    m[l][c] = 5;
                }
            }
        }
    }else {
         for (int l = 0; l < 13; l++) {
            for (int c = 0; c < 13; c++) {
                if ((l==bx+1 && c == by && m[l][c] != 1) || (l==bx-1 && c == by && m[l][c] != 1)) { //para baixo e para cima explosão
                    m[l][c] = 6;
                } else if ((l == bx && c == by-1 && m[l][c] != 1) || (l == bx && c == by+1 && m[l][c] != 1)) { //para direita e para esquerda explosão
                    m[l][c] = 6;
                }
            }
        }
    }
}

void movInimigo (int m[13][13], int &ix, int &iy) {
    int movDir = rand()%4+1;
        switch (movDir) {
        //para cima
        case 1:
            ix--;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] !=6) {
                ix++;
            }
        break;
        //para baixo
        case 2:
            ix++;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] !=6) {
                ix--;
            }
        break;
        //para direita
        case 3:
            iy++;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] !=6) {
                iy--;
            }
        break;
        //para esquerda
        case 4:
            iy--;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] !=6) {
                iy++;
            }
        break;
        }
}

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.movInimigo(m, ix1, iy1);
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

    int repetir = 1;
    do{
        system("cls");
        using namespace chrono;
        srand (time(NULL));
        milliseconds intervalo(500);
        auto inicio = high_resolution_clock::now();
        bool bomba = false;
        milliseconds flick1Bomba1(1000);
        milliseconds flick1Bomba2(2000);
        milliseconds flickExplosao(2500);
        milliseconds intervaloBomba(3000);
        auto inicioBomba = high_resolution_clock::now();
        bool vivo = true; //verificador se o player morreu ou nao
        bool jogo = true; // loop do jogo para o menu depois
        bool inimigo1 = true; //Booleano do inimigo
        bool inimigo2 = true;
        bool inimigo3 = true;
        bool Explosao = false;

        int m[13][13]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,
                        1,0,0,0,2,2,2,2,2,0,0,0,1,
                        1,0,1,0,1,2,1,0,1,0,1,0,1,
                        1,0,0,0,2,2,2,2,2,0,0,0,1,
                        1,0,1,2,1,0,1,2,1,0,1,2,1,
                        1,2,2,2,2,2,2,2,2,2,2,2,1,
                        1,2,1,0,1,2,1,0,1,2,1,0,1,
                        1,2,2,2,2,2,2,2,2,2,2,2,1,
                        1,2,1,0,1,2,1,0,1,2,1,0,1,
                        1,0,0,0,2,2,2,2,2,0,0,0,1,
                        1,0,1,0,1,0,1,0,1,0,1,0,1,
                        1,0,0,0,2,2,2,2,2,0,0,0,1,
                        1,1,1,1,1,1,1,1,1,1,1,1,1};
        //Posicao inicial do personagem no console
        int x=1, y=1;
        //Posicao inicial inimigo1
        int ix1=11, iy1=11;
        //Posicao inicial inimigo2
        int ix2=11, iy2=1;
        //Posicao inicial inimigo3
        int ix3=1, iy3=11;
        //Variavel para tecla precionada
        int bx, by;
        //onde está a bomba
        char tecla;

        while(jogo == true){
            ///Posiciona a escrita no iicio do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            ///Imprime o jogo: mapa e personagem.
            for(int i=0;i<13;i++){
                for(int j=0;j<13;j++){
                    if(i==x && j==y){
                        cout<<char(36); //personagem
                        if (vivo == false) {
                            jogo = false;
                        }
                    } else if((i==ix1 && j==iy1 && inimigo1 == true) || (i==ix2 && j==iy2 && inimigo2 == true) || (i==ix3 && j==iy3 && inimigo3 == true)) {
                        cout<<char(169); //inimigo
                        //Randomificador de quantos movimentos ele irá fazer
                        auto atual = high_resolution_clock::now();
                        auto passou = duration_cast<milliseconds>(atual - inicio);
                        if (passou >= intervalo) {
                            //Inimigo 1
                            movInimigo(m, ix1, iy1);
                            if (ix1 == x && iy1 == y && inimigo1 == true) {
                                    vivo = false;
                            }
                            //Inimigo 2
                            movInimigo(m, ix2, iy2);
                            if (ix2 == x && iy2 == y && inimigo2 == true) {
                                    vivo = false;
                            }
                            //Inimigo 3
                            movInimigo(m, ix3, iy3);
                            if (ix3 == x && iy3 == y && inimigo3 == true) {
                                    vivo = false;
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
                            case 6: cout<<char(157); break; // explosao
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

            if (inimigo1 == false && inimigo2 == false && inimigo3 == false) {
                jogo = false;
                break;
            }

            // Verifica se a bomba explodiu após 3 segundos
            auto agora = high_resolution_clock::now();
            auto passouBomba = duration_cast<milliseconds>(agora - inicioBomba);
            if (bomba && passouBomba >= flick1Bomba1) {
                m[bx][by] = 3;
                flick(m,bx,by, Explosao);
            }
            if (bomba && passouBomba >= flick1Bomba2) {
                m[bx][by] = 4;
            }
            if (bomba && passouBomba >= flickExplosao) {
                m[bx][by] = 6;
                Explosao = true;
                flick(m,bx,by,Explosao);
                Explosao = false;
            }
            if (bomba && passouBomba >= intervaloBomba) {
                // Explode a bomba
                m[bx][by] = 3;
                for (int l = 0; l < 13; l++) {
                    for (int c = 0; c < 13; c++) {
                        if ((l==bx+1 && c == by && m[l][c] != 1) || (l==bx-1 && c == by && m[l][c] != 1)) { //para baixo e para cima explosão
                            m[l][c] = 0;
                            m[bx][by] = 0;
                            if ((l == x && c == y) || (bx == x && by == y)) {
                                vivo = false;
                            }
                            else if ((l == ix1 && c == iy1) || (ix1 == x && iy1 == y)) {
                                inimigo1 = false;
                            }
                            else if ((l == ix2 && c == iy2) || (ix2 == x && iy2 == y)) {
                                inimigo2 = false;
                            }
                            else if ((l == ix3 && c == iy3) || (ix3 == x && iy3 == y)) {
                                inimigo3 = false;
                            }
                            bomba = false;
                        } else if ((l == bx && c == by-1 && m[l][c] != 1) || (l == bx && c == by+1 && m[l][c] != 1)) { //para direita e para esquerda explosão
                            m[l][c] = 0;
                            m[bx][by] = 0;
                            if (l == x && c == y) {
                                vivo = false;
                            }
                            else if (l == ix1 && c == iy1) {
                                inimigo1 = false;
                            }
                            else if (l == ix2 && c == iy2) {
                                inimigo2 = false;
                            }
                            else if (l == ix3 && c == iy3) {
                                inimigo3 = false;
                            }
                        }
                    }
                }
            }

        } //fim do laco do jogo


        system("cls");

        if (inimigo1 == false && inimigo2 == false && inimigo3 == false) {
            cout<<"Você matou todos os inimigos e ganhou o jogo, parabéns!"<<endl;
            cout<<"Jogo feito por:"<<endl<<"Luiz Antonio Haenisch"<<endl<<"Daniel Machado"<<endl<<"Vitoria Araujo"<<endl;
            cout<<"Professor: Alex Luciano"<<endl;
            cout<<"Quer jogar novamente?"<<endl;
            cout<<"Digite 1 para jogar de novo ou 0 para sair"<<endl;
            cin>>repetir;
        }
        if (vivo == false) {
            cout<<"Você perdeu o jogo"<<endl;
            cout<<"Jogo feito por:"<<endl<<"Luiz Antonio Haenisch"<<endl<<"Daniel Machado"<<endl<<"Vitoria Araujo"<<endl;
            cout<<"Professor: Alex Luciano"<<endl;
            cout<<"Quer jogar novamente?"<<endl;
            cout<<"Digite 1 para jogar de novo ou 0 para sair"<<endl;
            cin>>repetir;
        }
    }while (repetir == 1);

    return 0;
} //fim main
