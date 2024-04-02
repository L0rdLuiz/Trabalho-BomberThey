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

    int m[13][13]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,0,0,0,2,2,0,0,0,0,0,0,1,
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
                } else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        case 2: cout<<char(35); break; //parede quebravel
                        case 3: cout<<char(162); break;
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
                    for(int i=0;i<13;i++){
                        for(int j=0;j<13;j++){
                            if (m[i==x][j==y]) {
                                m[x][y] = 3;
                            }
                        }
                    };
                break;
            }
         }


    } //fim do laco do jogo

    return 0;
} //fim main
