/**
    Estrutura inicial para um jogo
    versão: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
#include <iostream>
#include <windows.h>
#include <conio.h>

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

    int m[7][7]={   1,1,1,1,1,1,1,
                    1,0,0,0,0,0,1,
                    1,0,0,0,1,1,1,
                    1,0,0,0,1,0,1,
                    1,1,0,0,1,0,1,
                    1,0,0,0,0,0,1,
                    1,1,1,1,1,1,1};

    int mov = 0;


    //Posicao inicial do personagem no console
    int x=5, y=5;
    //Variavel para tecla precionada
    char tecla;

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(i==x && j==y){
                    cout<<char(36); //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede
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
            }
         }


    } //fim do laco do jogo

    return 0;
} //fim main
