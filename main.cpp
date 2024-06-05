#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <chrono>
#include <mmsystem.h> //musica
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;
using namespace std::chrono;


struct Snake {
    int x, y;
    bool vivo;
};

bool contemApenasLetras(const string& str)   /// verifica se o nome contém apenas letras
{
    for (char c : str)
    {
        if (!isalpha(c))
        {
            return false;
        }
    }
    return true;
}

void MovimentoCobra(vector <Snake> &Cobra){
    for(int i = Cobra.size();i>0;i--){
        Cobra[i]=Cobra[i-1];
        }
    }

void geraMaca (int m[15][17], bool& macaNoJogo) {
    int mx = rand() % 15;
    int my = rand() % 17;
    if (macaNoJogo == false) {
        if (m[mx][my] != 1) {

            m[mx][my] = 2;
            macaNoJogo = true;
        }
    }
}

//void IncrementoDaCobra (vector <Snake>&Cobra){
//    for(int i = 0;i<Cobra.size();;i--){
//        Cobra[i]
//    }
//
//}

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
    //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    int menu;
    string nome;
    int repetir = 0;
    int TamanhoCobra = 3;
    vector <Snake> Cobra;
    //Posicao inicial do personagem no console
    Cobra.push_back({5,5});
    Cobra.push_back({5,4});
    Cobra.push_back({5,3});
    ///VERIFICAÇÃO COBRA NA HORIZONTAL OU VERTICAL
    bool CobraHorizontal = false;
    bool CobraVertical   = false;
    ///ONDE A COBRA ESTA OLHANDO
    bool CabecaCima = false;
    bool CabecaBaixo = false;
    bool CabecaDireita = false;
    bool CabecaEsquerda = false;
    //Variavel para tecla pressionada
    char tecla;

    //Variavel para contagem de tempo em tela

    auto inicio = steady_clock::now();
    auto final = steady_clock::now();

    //Gerador de Maçã
    srand (time(NULL));
    bool macaNoJogo = false;

    //Cobra viva
    Snake CobraViva;

    do
    {
        PlaySound(TEXT("menu.wav"), NULL, SND_ASYNC); //musica menu
        system("cls");
         cout << "                           _____             _        " << endl;
        cout << "                          /  ___|           | |       " << endl;
        cout << "                          \\ `--. _ __   __ _| | _____ " << endl;
        cout << "                           `--. \\ '_ \\ / _` | |/ / _ \\" << endl;
        cout << "                          /\\__/ / | | | (_| |   <  __/" << endl;
        cout << "                          \\____/|_| |_|\\__,_|_|\\_\\___|" << endl;
        cout << "                                                      " << endl<<endl;

        cout <<"                          ______________________________ " << endl;
        cout << "                         |                              |" << endl;
        cout << "                         |         1 - COMECAR          |" << endl;
        cout << "                         |         2 - TUTORIAL         |" << endl;
        cout << "                         |         3 - RANKING          |" << endl;
        cout << "                         |         4 - SAIR             |" << endl;
        cout << "                         |______________________________|" << endl;


        cin >> menu;
        switch (menu)
        {
        case 1:{ //Jogo

            cout << "digite seu nome: " << endl; //nome do jogador para o rank
            cin >> nome;
            if (!contemApenasLetras(nome))
            {
                cout << "Erro: O nome deve conter apenas letras." << endl;
                cout << "digite seu nome:" << endl;
                cin >> nome;
            }

            system("cls");
            auto inicio = steady_clock::now();
            do
            {
                //Botar coisas para repetir aqui
                PlaySound(TEXT("trilha.wav"), NULL, SND_ASYNC); //musica
                bool jogo = true;
                CobraViva.vivo = true;
                bool macaNoJogo = false;
                auto inicio = steady_clock::now();
                milliseconds velocidade(750);
                auto inicioCobra = high_resolution_clock::now();
                CobraHorizontal = true;
                CabecaDireita= true;

                int m[15][17] =
                {
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                };




                while (jogo == true)
                {
                    ///Posiciona a escrita no inicio do console
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

                    ///Imprime o jogo: mapa e personagem.
                    for (int i = 0; i < 15; i++)
                    {
                        for (int j = 0; j < 17; j++)
                        {
                            bool CobraOn = false;
                            if(i==Cobra[0].x&&j==Cobra[0].y){
                                cout<<char (79);
                            }else{
                            for(auto const &Snake:Cobra)
                            {
                                if(i==Snake.x&&j==Snake.y){
                                cout << char(111); //personagem
                                CobraOn = true;
                                break;
                                }
                            }

                            if(!CobraOn){
                                switch (m[i][j])
                                {
                                case 0: cout << " "; break; //caminho
                                case 1: cout << char(219); break; //parede
                                case 2: cout<<char(162); break; //maçã
                                //default: cout << "-"; //erro
                                }

                                } //fim switch
                            }
                        }
                        cout << "\n";
                    } //fim for mapa

                    auto agoraCobra = high_resolution_clock::now();
                    auto passouCobra = duration_cast<milliseconds>(agoraCobra - inicioCobra);

                    if (passouCobra >= velocidade) {
                        if (CabecaCima == true) {
                            MovimentoCobra(Cobra);
                            Cobra[0].x--;
                        }
                        if (CabecaBaixo == true) {
                            MovimentoCobra(Cobra);
                            Cobra[0].x++;
                        }
                        if (CabecaDireita == true) {
                            MovimentoCobra(Cobra);
                            Cobra[0].y++;
                        }
                        if (CabecaEsquerda == true) {
                            MovimentoCobra(Cobra);
                            Cobra[0].y--;
                        }
                        inicioCobra = agoraCobra;
                    }

                    ///executa os movimentos
                    if (_kbhit())
                    {
                        tecla = getch();
                        switch (tecla)
                        {
                        case 72:
                        case 'w': ///cima
                            if(CobraHorizontal==true){
                            MovimentoCobra(Cobra);
                            Cobra[0].x--;
                            CobraVertical=true;
                            CobraHorizontal=false;
                            CabecaCima = true;
                            CabecaBaixo = false;
                            CabecaDireita = false;
                            CabecaEsquerda = false;
                            }
                            break;
                        case 80:
                        case 's': ///baixo
                            if(CobraHorizontal==true){
                            MovimentoCobra(Cobra);
                            Cobra[0].x++;
                            CobraVertical=true;
                            CobraHorizontal=false;
                            CabecaCima = false;
                            CabecaBaixo = true;
                            CabecaDireita = false;
                            CabecaEsquerda = false;
                            }
                            break;
                        case 75:
                        case 'a': ///esquerda
                            if(CobraVertical==true){
                            MovimentoCobra(Cobra);
                            Cobra[0].y--;
                            CobraVertical=false;
                            CobraHorizontal=true;
                            CabecaCima = false;
                            CabecaBaixo = false;
                            CabecaDireita = false;
                            CabecaEsquerda = true;
                            }
                            break;
                        case 77:
                        case 'd': ///direita
                            if(CobraVertical==true){
                            MovimentoCobra(Cobra);
                            Cobra[0].y++;
                            CobraVertical=false;
                            CobraHorizontal=true;
                            CabecaCima = false;
                            CabecaCima = false;
                            CabecaDireita = true;
                            CabecaEsquerda = false;
                            }
                            break;
                        case 'o': ///TECLA PARA TESTAR O VERIFCADOR DE MOVIMENTO/ POR QUE NAO TEM AINDA O CHRONO. DEPOIS DO CHRONO TIRAR ISSO
                            CobraVertical=true;
                            CabecaDireita = true;
                            break;
                        }

                    }
                    //tempo em tela
                    final = steady_clock::now();
                    auto tempo = final - inicio;
                    cout << "   TEMPO: " << duration_cast<seconds>(tempo).count();

                    geraMaca(m, macaNoJogo);

                    if (m[Cobra[0].x][Cobra[0].y] == 2) {
                        m[Cobra[0].x][Cobra[0].y] = 0;
                        macaNoJogo = false;
                    }

                    if (CobraViva.vivo == true && m[Cobra[0].x][Cobra[0].y] == 1) {
                        CobraViva.vivo = false;
                        Cobra.clear();
                        Cobra.push_back({5,5});
                        Cobra.push_back({5,4});
                        Cobra.push_back({5,3});
                        jogo = false;
                    }



                }; //fim do laco do jogo
                if (CobraViva.vivo == false) {
                    system ("cls");
                    cout<<"Voce perdeu o jogo"<<endl;
                    cout<<"Jogo feito por:"<<endl<<"Luiz Antonio Haenisch"<<endl<<"Carlos Henrique Okarenski Ramos Depieri"<<endl<<"Isabela Silverio Cardoso Pereira"<<endl;
                    cout<<"Professor: Alex Luciano"<<endl;
                    cout<<"Quer jogar novamente?"<<endl;
                    cout<<"Digite 1 para jogar de novo ou 0 para sair"<<endl;
                    cin>>repetir;
                    system ("cls");
                }
            }
            while (repetir == 1);
            break;}
        case 2: //Sobre o jogo
            system ("cls");


            cout<< "                                                                  " << endl;

            cout<< "                                                                  " << endl;

            cout << "OBJETIVO: Coma 100 macas sem colidir com a parede ou com a cobra." << endl;
            cout << "MOVIMENTO: Use W, A, S e D para mover a cobra que se move sempre para frente" << endl;
            cout << "MACAS: Mova-se em direcao as macas para come-las e crescer." << endl;
            cout << "EVITAR COLISOES: Evite colidir com a parede ou com o corpo da cobra." << endl;
            cout << "VITORIA: Ao comer 100 macas sem colisoes, voce vence o jogo!" << endl<< endl;
            cout << "BOA SORTE!" << endl << endl;
            system("pause");
            system ("cls");
            break;
        case 3: //Rank
            system ("cls");

            system("pause");
            system ("cls");
            break;
        case 4: // Obrigado por jogar

            break;
        default:
            cout << "Esta opcao nao e aceita digite outro numero"<<endl;
            system("pause");
            break;
        }
    }
    while (menu != 4);

    return 0;
} //fim main
