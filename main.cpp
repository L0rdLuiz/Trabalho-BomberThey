#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace chrono;

struct player {
    int vivo = 1;
    bool vidaExtra;
    int pontuacao;
    int movUtilizado;
    int bombaGasta;
    int maxVidaExtra = 2;
    bool intangivel = false;
    bool antiExplosao = false;
};

struct inimigo {
    bool iniVivo;
    int difInimigo;
    int ix;
    int iy;
};

struct bomba {
    int distBomba = 1;
    bool bombaRelogio = false;
    int bx;
    int by;
    int bombaAtual;
    int maxDistBomba = 3;
};

void geraMapa(int m[13][26]) {
    for(int l = 0; l < 13; l++) {
        for (int c = 0; c < 26; c++){
            if (m[l][c] == 0 && (rand() % 5 == 0))
            {
                m[l][c] = 2; // Parede quebrável
            }
        }
    }
    m[1][1] = 0;
    m[1][2] = 0;
    m[2][1] = 0;
    m[5][24] = 0;
    m[4][24] = 0;
}

bool colisaoBool(int p, player& p1){
    if (p1.intangivel && p == 2) {
        return true;
    }
    // Verifica se o valor de 'p' não é igual a nenhum dos valores listados
    if (p != 0 && p != 5 && p != 6 && p != 7 && p != 8 && p != 9 && p != 10 && p != 11 && p != 12) {
        return false; // Retorna false se 'p' não for igual a nenhum desses valores
    }
    else {
        return true; // Retorna true se 'p' for igual a qualquer um desses valores
    }
}

bool inimigoGeradorVerificador(int m[13][26], int x, int y){
    //Verificar se a posição é valida
    if(m[x][y] == 1 || m[x][y] == 2) return false;
    if((x == 1 && y == 1) || (x == 2 && y == 1) || (x == 1 && y == 2)) return false;
    if (x == 11 && y == 24) return false;
    return true;
}

void flick(int m[13][26], int bx, int by, bomba& b1) {
    bool pareCima = false, pareBaixo = false, pareEsq = false, pareDir = false;

    for (int dist = 1; dist <= b1.distBomba; dist++) {
        // Explosão para baixo
        if (!pareBaixo) {
            int l = bx + dist;
            int c = by;
            if (l < 13 && m[l][c] == 1) {
                pareBaixo = true;
            } else if (l < 13 && m[l][c] == 0) {
                m[l][c] = 5;
            }
        }

        // Explosão para cima
        if (!pareCima) {
            int l = bx - dist;
            int c = by;
            if (l >= 0 && m[l][c] == 1) {
                pareCima = true;
            } else if (l >= 0 && m[l][c] == 0) {
                m[l][c] = 5;
            }
        }

        // Explosão para a direita
        if (!pareDir) {
            int l = bx;
            int c = by + dist;
            if (c < 26 && m[l][c] == 1) {
                pareDir = true;
            } else if (c < 26 && m[l][c] == 0) {
                m[l][c] = 5;
            }
        }

        // Explosão para a esquerda
        if (!pareEsq) {
            int l = bx;
            int c = by - dist;
            if (c >= 0 && m[l][c] == 1) {
                pareEsq = true;
            } else if (c >= 0 && m[l][c] == 0) {
                m[l][c] = 5;
            }
        }
    }
}

void movInimigo (int m[13][26], int &ix, int &iy,int x,int y,int dificuldade = 3) {
    int movDir;
    int chanceMovAleatorio;

    // Definindo a chance de movimento aleatório com base na dificuldade
    switch (dificuldade) {
        case 1: // Fácil: 100% movimento aleatório
            chanceMovAleatorio = 100;
            break;
        case 2: // Médio: 50% movimento aleatório
            chanceMovAleatorio = 50;
            break;
        case 3: // Difícil: 25% movimento aleatório
            chanceMovAleatorio = 25;
            break;
        default:
            chanceMovAleatorio = 100; // Padrão para fácil
            break;
    }

    // Decidindo a direção de movimento
    if (rand() % 100 < chanceMovAleatorio) {
        movDir = rand() % 4 + 1; // Movimento aleatório
    } else {
        // Movimento em direção ao jogador
        if (abs(x - ix) > abs(y - iy)) {
            if (x > ix) {
                movDir = 2; // Mover para baixo
            } else {
                movDir = 1; // Mover para cima
            }
        } else {
            if (y > iy) {
                movDir = 3; // Mover para a direita
            } else {
                movDir = 4; // Mover para a esquerda
            }
        }
    }

    // Executando o movimento
    switch (movDir) {
        case 1: // Para cima
            ix--;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] != 6 && m[ix][iy] != 7 && m[ix][iy] != 8 && m[ix][iy] != 9 && m[ix][iy] != 10 && m[ix][iy] != 11) {
                ix++;
            }
            break;
        case 2: // Para baixo
            ix++;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] != 6 && m[ix][iy] != 7 && m[ix][iy] != 8 && m[ix][iy] != 9 && m[ix][iy] != 10 && m[ix][iy] != 11) {
                ix--;
            }
            break;
        case 3: // Para a direita
            iy++;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] != 6 && m[ix][iy] != 7 && m[ix][iy] != 8 && m[ix][iy] != 9 && m[ix][iy] != 10 && m[ix][iy] != 11) {
                iy--;
            }
            break;
        case 4: // Para a esquerda
            iy--;
            if (m[ix][iy] != 0 && m[ix][iy] != 5 && m[ix][iy] != 6 && m[ix][iy] != 7 && m[ix][iy] != 8 && m[ix][iy] != 9 && m[ix][iy] != 10 && m[ix][iy] != 11) {
                iy++;
            }
            break;
    }
}

void verificarColisaoBomba(int l, int c, int x, int y, inimigo& i) {
    if ((l == i.ix && c == i.iy) || (i.ix == x && i.iy == y)) {
        i.iniVivo = false;
    }
}

int soltarEspeciais () {
    int especial = rand() % 5 + 1;
        switch (especial) {
            case 1:
                return (7);
            case 2:
                return (8);
            case 3:
                return (9);
            case 4:
                return (10);
            case 5:
                return (11);
        }
}

void pontosMorte(inimigo& iniEsp,player& p1) {
    if (iniEsp.iniVivo == false) {
        p1.pontuacao += 100;
    }
}

void pontosMorte(inimigo& i, player& p1, bool& deuPonto) {
    if (i.iniVivo == false && deuPonto == false) {
        p1.pontuacao += 50;
        deuPonto = true;
    }
}

void pontosBombaGasta(player& p1) {
    if (p1.bombaGasta > 8) {
        p1.pontuacao -= 10;
    }
    if (p1.bombaGasta > 10) {
        p1.pontuacao -= 15;
    }
    if (p1.bombaGasta > 15) {
        p1.pontuacao -= 25;
    }
}

void pontosPorMovimento(player& p1){
    if (p1.movUtilizado > 250) {
        p1.pontuacao -= 10;
    }
    if (p1.movUtilizado > 300) {
        p1.pontuacao -= 15;
    }
    if (p1.movUtilizado > 400) {
        p1.pontuacao -= 25;
    }
}

int randEspeciais (int dificuldade) {
    switch (dificuldade) {
        case 1:
            return(60);
        case 2:
            return(70);
        case 3:
            return(75);
    }
}

// Função para atualizar e mostrar o ranking
void atualizarMostrarRanking(const string& nome_atual, int pontuacao_atual) {
    vector<int> pontuacao_do_ranking(10, 0);
    vector<string> nome_no_ranking(10, "");

    ifstream ranking_in("pontuacao_e_nome.txt");
    if (!ranking_in) {
        cerr << "Arquivo de ranking não encontrado. Criando um novo." << endl;
        ofstream ranking_out("pontuacao_e_nome.txt");
        for (int i = 0; i < 10; i++) {
            ranking_out << 0 << " " << "empty" << endl;
        }
        ranking_out.close();
        ranking_in.open("pontuacao_e_nome.txt");
        if (!ranking_in) {
            cerr << "Erro ao criar o arquivo de ranking." << endl;
            return;
        }
    }

    for (int i = 0; i < 10; i++) {
        ranking_in >> pontuacao_do_ranking[i] >> nome_no_ranking[i];
    }
    ranking_in.close();

    pontuacao_do_ranking.push_back(pontuacao_atual);
    nome_no_ranking.push_back(nome_atual);

    vector<pair<int, string>> ranking;
    for (int i = 0; i < 11; i++) {
        ranking.push_back(make_pair(pontuacao_do_ranking[i], nome_no_ranking[i]));
    }

    sort(ranking.rbegin(), ranking.rend());
    ranking.resize(10);

    for (int i = 0; i < 10; i++) {
        pontuacao_do_ranking[i] = ranking[i].first;
        nome_no_ranking[i] = ranking[i].second;
    }

    ofstream ranking_out("pontuacao_e_nome.txt");
    if (!ranking_out) {
        cerr << "Erro ao abrir o arquivo de ranking para escrita." << endl;
        return;
    }

    for (int i = 0; i < 10; i++) {
        ranking_out << pontuacao_do_ranking[i] << " " << nome_no_ranking[i] << endl;
    }
    ranking_out.close();

    cout << "Ranking atualizado:" << endl;
    for (int i = 0; i < 10; i++) {
        cout << i + 1 << ". " << nome_no_ranking[i] << " - " << pontuacao_do_ranking[i] << endl;
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

    srand (time(NULL));
    int menu;
    int dificuldade = 1;
    int repetir = 1;
    int fase = 1;
    int bombaGasta;
    int movUtilizado;
    int pontuacao;

    do{
        PlaySound(TEXT("inicio.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        system("cls");
        cout << R"(
         ______     ______     __    __     ______     ______     ______     __    __     ______     __   __
        /\  == \   /\  __ \   /\ "-./  \   /\  == \   /\  ___\   /\  == \   /\ "-./  \   /\  __ \   /\ "-.\ \
        \ \  __<   \ \ \/\ \  \ \ \-./\ \  \ \  __<   \ \  __\   \ \  __<   \ \ \-./\ \  \ \  __ \  \ \ \-.  \
         \ \_____\  \ \_____\  \ \_\ \ \_\  \ \_____\  \ \_____\  \ \_\ \_\  \ \_\ \ \_\  \ \_\ \_\  \ \_\\"\_\
          \/_____/   \/_____/   \/_/  \/_/   \/_____/   \/_____/   \/_/ /_/   \/_/  \/_/   \/_/\/_/   \/_/ \/_/

)" << endl;
        cout<<"-----------------"<<endl;
        cout<<"Jogar (1)"<<endl;
        cout<<"Dificuldade (2)"<<endl;
        cout<<"Sobre o Jogo (3)"<<endl;
        cout<<"Rank (4)"<<endl;
        cout<<"Sair (5)"<<endl;
        cout<<"-----------------"<<endl;
        cin>>menu;
        switch(menu) {
            case 1: //Jogo
                system("cls");
                do{
                    system("cls");
                    PlaySound (0,0,0);
                    PlaySound (TEXT("musica jogo.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                    srand (time(NULL));
                    milliseconds intervalo(400);
                    auto inicio = high_resolution_clock::now();
                    bool bombaColocada = false;
                    bool ativaBomba = false;
                    bomba b1;
                    player p1;
                    inimigo i1;
                    inimigo i2;
                    inimigo i3;
                    inimigo i4;
                    inimigo i5;
                    inimigo iniEsp;
                    iniEsp.iniVivo = true;
                    iniEsp.ix = 5;
                    iniEsp.iy = 24;
                    if (fase == 1) {
                        p1.bombaGasta = 0;
                        p1.movUtilizado = 0;
                        p1.pontuacao = 0;
                    } else {
                        p1.bombaGasta = bombaGasta;
                        p1.movUtilizado = movUtilizado;
                        p1.pontuacao = pontuacao;
                    }
                    auto inicioTempo = system_clock::now();
                    p1.vidaExtra = 0;
                    milliseconds flick1Bomba1(1000);
                    milliseconds flick1Bomba2(2000);
                    milliseconds intervaloBomba(3000);
                    milliseconds flickExplosao(3500);
                    auto inicioBomba = high_resolution_clock::now();
                    bool jogo = true; // loop do jogo para o menu depois
                    bool ini1 = false;
                    bool ini2 = false;
                    bool ini3 = false;
                    bool ini4 = false;
                    bool ini5 = false;
                    bool deuPonto1 = false;
                    bool deuPonto2 = false;
                    bool deuPonto3 = false;
                    bool deuPonto4 = false;
                    bool deuPonto5 = false;

                    int m[13][26]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                    1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                    1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,
                                    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,1,
                                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
                    //Posicao inicial do personagem no consolewhile(menu != 5);
                    int x=1, y=1;
                    //Gera mapa
                    geraMapa(m);
                    //Gera posição do inimigo
                    i1.iniVivo = true;
                    i2.iniVivo = true;
                    i3.iniVivo = true;
                    i4.iniVivo = true;
                    i5.iniVivo = true;
                    // Inicialização
                    ini1 = ini2 = ini3 = ini4 = ini5 = false;
                    // Posicoes do inimigo
                    while (!ini1 || !ini2 || !ini3 || !ini4 || !ini5) {
                        if (!ini1) {
                            i1.ix = rand() % 12 + 1;
                            i1.iy = rand() % 25 + 1;
                            ini1 = inimigoGeradorVerificador(m, i1.ix, i1.iy);
                        }
                        if (!ini2) {
                            i2.ix = rand() % 12 + 1;
                            i2.iy = rand() % 25 + 1;
                            ini2 = inimigoGeradorVerificador(m, i2.ix, i2.iy);
                        }
                        if (!ini3) {
                            i3.ix = rand() % 12 + 1;
                            i3.iy = rand() % 25 + 1;
                            ini3 = inimigoGeradorVerificador(m, i3.ix, i3.iy);
                        }
                        if (!ini4) {
                            i4.ix = rand() % 12 + 1;
                            i4.iy = rand() % 25 + 1;
                            ini4 = inimigoGeradorVerificador(m, i4.ix, i4.iy);
                        }
                        if (!ini5) {
                            i5.ix = rand() % 12 + 1;
                            i5.iy = rand() % 25 + 1;
                            ini5 = inimigoGeradorVerificador(m, i5.ix, i5.iy);
                        }
                    }
                    //onde está a bomba
                    char tecla;

                    //Pontuação de inimigo morto

                    while(jogo == true){
                        ///Posiciona a escrita no iicio do console
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                        ///Imprime o jogo: mapa e personagem.
                        for(int i=0;i<13;i++){
                            for(int j=0;j<26;j++){
                                if(i==x && j==y){
                                    cout<<char(36); //personagem
                                    if (p1.vivo == 0) {
                                        jogo = false;
                                    }
                                } else if((i==i1.ix && j==i1.iy && i1.iniVivo == true) || (i==i2.ix && j==i2.iy && i2.iniVivo == true) || (i==i3.ix && j==i3.iy && i3.iniVivo == true) || (i==i4.ix && j==i4.iy && i4.iniVivo == true) || (i==i5.ix && j==i5.iy && i5.iniVivo == true)) {
                                    cout<<char(169); //inimigo
                                    //Randomificador de quantos movimentos ele irá fazer
                                    auto atual = high_resolution_clock::now();
                                    auto passou = duration_cast<milliseconds>(atual - inicio);
                                    if (passou >= intervalo) {
                                        //Inimigo 1
                                        movInimigo(m, i1.ix, i1.iy,x,y,dificuldade);
                                        if (i1.ix == x && i1.iy == y && i1.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        //Inimigo 2
                                        movInimigo(m, i2.ix, i2.iy,x,y, dificuldade);
                                        if (i2.ix == x && i2.iy == y && i2.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        //Inimigo 3
                                        movInimigo(m, i3.ix, i3.iy,x,y, dificuldade);
                                        if (i3.ix == x && i3.iy == y && i3.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        //Inimigo 4
                                        movInimigo(m, i4.ix, i4.iy,x,y, dificuldade);
                                        if (i4.ix == x && i4.iy == y && i4.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        //Inimigo 5
                                        movInimigo(m, i5.ix, i5.iy,x,y, dificuldade);
                                        if (i5.ix == x && i5.iy == y && i5.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        inicio = atual;
                                    }
                                }else if((i==iniEsp.ix && j==iniEsp.iy && iniEsp.iniVivo == true) && fase == 3){
                                    cout<<char(190); //inimigo Especial
                                    //Randomificador de quantos movimentos ele irá fazer
                                    auto atualEsp = high_resolution_clock::now();
                                    auto passouEsp = duration_cast<milliseconds>(atualEsp - inicio);
                                    if (passouEsp >= intervalo) {
                                        movInimigo(m, iniEsp.ix, iniEsp.iy,x,y);
                                        if (iniEsp.ix == x && iniEsp.iy == y && iniEsp.iniVivo == true) {
                                                p1.vivo = p1.vivo - 1;
                                        }
                                        inicio = atualEsp;
                                    }
                                }
                                else {
                                    switch (m[i][j]){
                                        case 0: cout<<" "; break; //caminho
                                        case 1: cout<<char(219); break; //parede
                                        case 2: cout<<char(35); break; //parede quebravel
                                        case 3: cout<<char(162); break; // bomba
                                        case 4: cout<<char(79); break; // bomba Flick
                                        case 5: cout<<char(88); break; // area de explosão
                                        case 6: cout<<char(157); break; // explosao
                                        // Poderes
                                        case 7: cout<<char(43); break; // aumentar range da bomba
                                        case 8: cout<<char(253); break; // vida extra
                                        case 9: cout<<char(73); break; // passar pelas paredes quebraveis
                                        case 10: cout<<char(153); break; // sobreviver a bomba
                                        case 11: cout<<char(207); break; // bomba relogio
                                        case 12: cout<<char(186); break; // portal
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
                                    p1.movUtilizado += 1;
                                    if (colisaoBool(m[x][y], p1)== false) {
                                        x++;
                                        p1.movUtilizado -= 1;
                                    };
                                break;
                                case 80: case 's': ///baixo
                                    x++;
                                    p1.movUtilizado += 1;
                                    if (colisaoBool(m[x][y], p1)== false) {
                                        x--;
                                        p1.movUtilizado -= 1;
                                    };
                                break;
                                case 75:case 'a': ///esquerda
                                    y--;
                                    p1.movUtilizado += 1;
                                    if (colisaoBool(m[x][y], p1)== false) {
                                        y++;
                                        p1.movUtilizado -= 1;
                                    };
                                break;
                                case 77: case 'd': ///direita
                                    y++;
                                    p1.movUtilizado += 1;
                                    if (colisaoBool(m[x][y], p1)== false) {
                                        y--;
                                        p1.movUtilizado -= 1;
                                    };
                                break;
                                case 'e': ///bomba
                                    if (!bombaColocada) {
                                        m[x][y] = 3; // Coloca a bomba no mapa
                                        b1.bx = x;
                                        b1.by = y;
                                        b1.bombaAtual = 1;
                                        if (b1.bombaRelogio == false) {
                                            inicioBomba = high_resolution_clock::now();
                                            bombaColocada = true;
                                        }
                                        p1.bombaGasta += 1;
                                        b1.distBomba;
                                    }
                                break;
                                case 'q': ///bomba relogio
                                    if (b1.bombaRelogio == true) {
                                        inicioBomba = high_resolution_clock::now();
                                        ativaBomba = true;
                                        bombaColocada = true;
                                    }
                                    break;
                                case 79: case 'm': ///parar a musica
                                        PlaySound (0,0,0);
                                    break;
                                case 82: case 'p': ///recomeça a musica
                                    PlaySound(TEXT("musica jogo.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                                    break;
                            }
                        }

                        if ((b1.bombaRelogio == false) || (b1.bombaRelogio == true && ativaBomba == true)) {
                            auto agora = high_resolution_clock::now();
                            auto passouBomba = duration_cast<milliseconds>(agora - inicioBomba);
                            if (bombaColocada && passouBomba >= flick1Bomba1) {
                                m[b1.bx][b1.by] = 3;
                                flick(m,b1.bx,b1.by, b1);
                            }
                            if (bombaColocada && passouBomba >= flick1Bomba2) {
                                m[b1.bx][b1.by] = 4;
                            }
                            if (bombaColocada && passouBomba >= intervaloBomba) { // Explosão
                                m[b1.bx][b1.by] = 3;
                                bool pareExCima = false, pareExBaixo = false, pareExDir = false, pareExEsq = false;
                                for(int l = 0; l < 13; l++) {
                                    for (int c = 0; c < 26; c++){
                                        for (int dist = 1; dist <= b1.distBomba; dist++) {
                                            // Para Baixo
                                            if (pareExBaixo == false) {
                                                if (l == b1.bx+dist && c == b1.by && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                    int randEspecial = rand() % 100 + 1;
                                                    if (randEspecial >= randEspeciais(dificuldade) && m[l][c] == 2) {
                                                        m[l][c] = soltarEspeciais();
                                                    }
                                                    if(p1.antiExplosao == false){
                                                        if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                            p1.vivo = p1.vivo - 1;
                                                            x = 1;
                                                            y = 1;
                                                            if (p1.vivo == 0) {
                                                                jogo = false;
                                                            }
                                                        }
                                                    }
                                                    verificarColisaoBomba(l, c, x, y, i1);
                                                    verificarColisaoBomba(l, c, x, y, i2);
                                                    verificarColisaoBomba(l, c, x, y, i3);
                                                    verificarColisaoBomba(l, c, x, y, i4);
                                                    verificarColisaoBomba(l, c, x, y, i5);
                                                    verificarColisaoBomba(l, c, x, y, iniEsp);
                                                    if (m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if (m[l][c] == 2) {
                                                            p1.pontuacao += 5;
                                                        }
                                                        m[l][c] = 6;
                                                    }
                                                    m[b1.bx][b1.by] = 6;
                                                    b1.bombaAtual -= 1;
                                                } else if(l==b1.bx+dist && c == b1.by && m[l][c] == 1) {
                                                    pareExBaixo = true;
                                                }
                                            }
                                            // Para Cima
                                            if (pareExCima == false) {
                                                if (l == b1.bx-dist && c == b1.by && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                    int randEspecial = rand() % 100 + 1;
                                                    if (randEspecial >= randEspeciais(dificuldade) && m[l][c] == 2) {
                                                        m[l][c] = soltarEspeciais();
                                                    }
                                                    if(p1.antiExplosao == false){
                                                        if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                            p1.vivo = p1.vivo - 1;
                                                            x = 1;
                                                            y = 1;
                                                            if (p1.vivo == 0) {
                                                                jogo = false;
                                                            }
                                                        }
                                                    }
                                                    verificarColisaoBomba(l, c, x, y, i1);
                                                    verificarColisaoBomba(l, c, x, y, i2);
                                                    verificarColisaoBomba(l, c, x, y, i3);
                                                    verificarColisaoBomba(l, c, x, y, i4);
                                                    verificarColisaoBomba(l, c, x, y, i5);
                                                    verificarColisaoBomba(l, c, x, y, iniEsp);
                                                    if (m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if (m[l][c] == 2) {
                                                            p1.pontuacao += 5;
                                                        }
                                                        m[l][c] = 6;
                                                    }
                                                    m[b1.bx][b1.by] = 6;
                                                    b1.bombaAtual -= 1;
                                                } else if(l==b1.bx-dist && c == b1.by && m[l][c] == 1) {
                                                    pareExCima = true;
                                                }
                                            }
                                            // Para Direita
                                            if (pareExDir == false) {
                                                if (l==b1.bx && c == b1.by+dist && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                    int randEspecial = rand() % 100 + 1;
                                                    if (randEspecial >= randEspeciais(dificuldade) && m[l][c] == 2) {
                                                        m[l][c] = soltarEspeciais();
                                                    }
                                                    if(p1.antiExplosao == false){
                                                        if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                            p1.vivo = p1.vivo - 1;
                                                            x = 1;
                                                            y = 1;
                                                            if (p1.vivo == 0) {
                                                                jogo = false;
                                                            }
                                                        }
                                                    }
                                                    verificarColisaoBomba(l, c, x, y, i1);
                                                    verificarColisaoBomba(l, c, x, y, i2);
                                                    verificarColisaoBomba(l, c, x, y, i3);
                                                    verificarColisaoBomba(l, c, x, y, i4);
                                                    verificarColisaoBomba(l, c, x, y, i5);
                                                    verificarColisaoBomba(l, c, x, y, iniEsp);
                                                    if (m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if (m[l][c] == 2) {
                                                            p1.pontuacao += 5;
                                                        }
                                                        m[l][c] = 6;
                                                    }
                                                    m[b1.bx][b1.by] = 6;
                                                    b1.bombaAtual -= 1;
                                                } else if(l==b1.bx && c == b1.by+dist && m[l][c] == 1) {
                                                    pareExDir = true;
                                                }
                                            }
                                            // Para Esquerda
                                            if (pareExEsq == false) {
                                                if (l==b1.bx && c == b1.by-dist && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                    int randEspecial = rand() % 100 + 1;
                                                    if (randEspecial >= randEspeciais(dificuldade) && m[l][c] == 2) {
                                                        m[l][c] = soltarEspeciais();
                                                    }
                                                    if(p1.antiExplosao == false){
                                                        if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                            p1.vivo = p1.vivo - 1;
                                                            x = 1;
                                                            y = 1;
                                                            if (p1.vivo == 0) {
                                                                jogo = false;
                                                            }
                                                        }
                                                    }
                                                    verificarColisaoBomba(l, c, x, y, i1);
                                                    verificarColisaoBomba(l, c, x, y, i2);
                                                    verificarColisaoBomba(l, c, x, y, i3);
                                                    verificarColisaoBomba(l, c, x, y, i4);
                                                    verificarColisaoBomba(l, c, x, y, i5);
                                                    verificarColisaoBomba(l, c, x, y, iniEsp);
                                                    if (m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if (m[l][c] == 2) {
                                                            p1.pontuacao += 5;
                                                        }
                                                        m[l][c] = 6;
                                                    }
                                                    m[b1.bx][b1.by] = 6;
                                                    b1.bombaAtual -= 1;
                                                } else if(l==b1.bx && c == b1.by-dist && m[l][c] == 1) {
                                                    pareExEsq = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                // Fogo
                                if (bombaColocada && passouBomba >= flickExplosao) {
                                bool pareFogoCima = false, pareFogoBaixo = false, pareFogoDir = false, pareFogoEsq = false;
                                    for(int l = 0; l < 13; l++) {
                                        for (int c = 0; c < 26; c++){
                                            for (int distFogo = 1; distFogo <= b1.distBomba; distFogo++) {
                                                // Para Baixo Fogo
                                                if (pareFogoBaixo == false) {
                                                    if (l==b1.bx+distFogo && c == b1.by && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if(p1.antiExplosao == false){
                                                            if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                                p1.vivo = p1.vivo - 1;
                                                                x = 1;
                                                                y = 1;
                                                                if (p1.vivo == 0) {
                                                                    jogo = false;
                                                                }
                                                            }
                                                        }
                                                        verificarColisaoBomba(l, c, x, y, i1);
                                                        verificarColisaoBomba(l, c, x, y, i2);
                                                        verificarColisaoBomba(l, c, x, y, i3);
                                                        verificarColisaoBomba(l, c, x, y, i4);
                                                        verificarColisaoBomba(l, c, x, y, i5);
                                                        verificarColisaoBomba(l, c, x, y, iniEsp);
                                                        bombaColocada = false;
                                                        m[l][c] = 0;
                                                        m[b1.bx][b1.by] = 0;
                                                    } else if(l==b1.bx+distFogo && c == b1.by && m[l][c] == 1) {
                                                        pareFogoBaixo = true;
                                                    }
                                                }
                                                // Para Cima Fogo
                                                if (pareFogoCima == false) {
                                                    if (l==b1.bx-distFogo && c == b1.by && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if(p1.antiExplosao == false){
                                                            if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                                p1.vivo = p1.vivo - 1;
                                                                x = 1;
                                                                y = 1;
                                                                if (p1.vivo == 0) {
                                                                    jogo = false;
                                                                }
                                                            }
                                                        }
                                                        verificarColisaoBomba(l, c, x, y, i1);
                                                        verificarColisaoBomba(l, c, x, y, i2);
                                                        verificarColisaoBomba(l, c, x, y, i3);
                                                        verificarColisaoBomba(l, c, x, y, i4);
                                                        verificarColisaoBomba(l, c, x, y, i5);
                                                        verificarColisaoBomba(l, c, x, y, iniEsp);
                                                        bombaColocada = false;
                                                        m[l][c] = 0;
                                                        m[b1.bx][b1.by] = 0;
                                                    } else if(l==b1.bx-distFogo && c == b1.by && m[l][c] == 1) {
                                                        pareFogoCima = true;
                                                    }
                                                }
                                                // Para Direita Fogo
                                                if (pareFogoDir == false) {
                                                    if (l==b1.bx && c == b1.by+distFogo && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if(p1.antiExplosao == false){
                                                            if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                                p1.vivo = p1.vivo - 1;
                                                                x = 1;
                                                                y = 1;
                                                                if (p1.vivo == 0) {
                                                                    jogo = false;
                                                                }
                                                            }
                                                        }
                                                        verificarColisaoBomba(l, c, x, y, i1);
                                                        verificarColisaoBomba(l, c, x, y, i2);
                                                        verificarColisaoBomba(l, c, x, y, i3);
                                                        verificarColisaoBomba(l, c, x, y, i4);
                                                        verificarColisaoBomba(l, c, x, y, i5);
                                                        verificarColisaoBomba(l, c, x, y, iniEsp);
                                                        bombaColocada = false;
                                                        m[l][c] = 0;
                                                        m[b1.bx][b1.by] = 0;
                                                    } else if(l==b1.bx && c == b1.by+distFogo && m[l][c] == 1) {
                                                        pareFogoDir = true;
                                                    }
                                                }
                                                // Para Esquerda Fogo
                                                if (pareFogoEsq == false) {
                                                    if (l==b1.bx && c == b1.by-distFogo && m[l][c] != 1 && m[l][c] != 7 && m[l][c] != 8 && m[l][c] != 9 && m[l][c] != 10 && m[l][c] != 11 && m[l][c] != 12) {
                                                        if(p1.antiExplosao == false){
                                                            if ((l == x && c == y) || (b1.bx == x && b1.by == y)) {
                                                                p1.vivo = p1.vivo - 1;
                                                                x = 1;
                                                                y = 1;
                                                                if (p1.vivo == 0) {
                                                                    jogo = false;
                                                                }
                                                            }
                                                        }
                                                        verificarColisaoBomba(l, c, x, y, i1);
                                                        verificarColisaoBomba(l, c, x, y, i2);
                                                        verificarColisaoBomba(l, c, x, y, i3);
                                                        verificarColisaoBomba(l, c, x, y, i4);
                                                        verificarColisaoBomba(l, c, x, y, i5);
                                                        verificarColisaoBomba(l, c, x, y, iniEsp);
                                                        bombaColocada = false;
                                                        m[l][c] = 0;
                                                        m[b1.bx][b1.by] = 0;
                                                    } else if(l==b1.bx && c == b1.by-distFogo && m[l][c] == 1) {
                                                        pareFogoEsq = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        //Power up de range de bomba
                        if (m[x][y] == 7 && b1.distBomba != b1.maxDistBomba) {
                            b1.distBomba = b1.distBomba + 1;
                            m[x][y] = 0;
                        } else if (m[x][y] == 7 && b1.distBomba == b1.maxDistBomba) {
                            b1.distBomba = 3;
                            m[x][y] = 0;
                            p1.pontuacao = p1.pontuacao + 50;
                        }
                        //Power Up de Vida Extra
                        if (m[x][y] == 8 && p1.vivo != p1.maxVidaExtra) {
                            p1.vivo = p1.vivo + 1;
                            m[x][y] = 0;
                        } else if (m[x][y] == 8 && p1.vivo == p1.maxVidaExtra) {
                            p1.vivo = 2;
                            m[x][y] = 0;
                            p1.pontuacao = p1.pontuacao + 50;
                        }
                        //Power Up passar pela parede quebravel
                        if (m[x][y] == 9 && p1.intangivel == false) {
                            p1.intangivel = true;
                            m[x][y] = 0;
                        } else if (m[x][y] == 9 && p1.intangivel == true) {
                            m[x][y] = 0;
                            p1.pontuacao = p1.pontuacao + 50;
                        }
                        //Power Up sobrevive para bomba
                        if (m[x][y] == 10 && p1.antiExplosao == false) {
                            p1.antiExplosao = true;
                            m[x][y] = 0;
                        } else if (m[x][y] == 10 && p1.antiExplosao == true) {
                            m[x][y] = 0;
                            p1.pontuacao = p1.pontuacao + 50;
                        }
                        //Power Up Bomba Relogio
                        if (m[x][y] == 11 && b1.bombaRelogio == false) {
                            b1.bombaRelogio = true;
                            m[x][y] = 0;
                        } else if (m[x][y] == 11 && b1.bombaRelogio == true) {
                            m[x][y] = 0;
                            p1.pontuacao = p1.pontuacao + 50;
                        }

                        //Portal
                        if (m[x][y] == 12 && fase != 3) {
                            x = 1;
                            y = 1;
                            fase += 1;
                            pontosBombaGasta(p1);
                            pontosPorMovimento(p1);
                            bombaGasta = p1.bombaGasta;
                            movUtilizado = p1.movUtilizado;
                            pontuacao = p1.pontuacao;
                            break;
                        }

                        //Sumir Portal
                        if (fase == 3) {
                            m[11][24] = 0;
                        }

                        //Sistema de contagem de pontuação
                        pontosMorte(i1,p1, deuPonto1);
                        pontosMorte(i2,p1, deuPonto2);
                        pontosMorte(i3,p1, deuPonto3);
                        pontosMorte(i4,p1, deuPonto4);
                        pontosMorte(i5,p1, deuPonto5);
                        pontosMorte (iniEsp,p1);

                        //Ganhar
                        if(fase == 3 && iniEsp.iniVivo == false) {
                            jogo = false;
                            break;
                        }

                    auto tempoAtualTimer = system_clock::now();
                    auto tempoDecorrido = duration_cast<seconds>(tempoAtualTimer - inicioTempo).count();

                    cout<<"-------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"Bomba: "<<p1.bombaGasta<<" Movimento: "<<p1.movUtilizado<<" Pontuacao: "<<p1.pontuacao<<" Tempo: "<<tempoDecorrido;
                    //teste
                    cout<<endl<<"Distancia da bomba: "<<b1.distBomba<<" Vida: "<<p1.vivo<<" Intagibilidade: "<< p1.intangivel<<" Contra-Explosao: "<< p1.antiExplosao<< " Bomba-Relogio: "<< b1.bombaRelogio;
                    cout<<endl<<"Voce esta na fase : "<<fase;
                    cout<<endl<<"-------------------------------------------------------------------------------------------------------------------";
                    } //fim do laco do jogo


                    system("cls");
                    if (fase == 3 && iniEsp.iniVivo == false) {
                        cout << "Voce Ganhou o jogo e matou o Boss!" << endl;
                        cout << "Você fez os seguintes pontos: " << endl;
                        cout << "Bomba Gasta: " << p1.bombaGasta << " Movimento Gasto: " << p1.movUtilizado << " Pontuacao Feita: " << p1.pontuacao << endl;
                        cout << "Jogo feito por:" << endl << "Luiz Antonio Haenisch" << endl << "Daniel Machado" << endl << "Vitoria Araujo" << endl;
                        cout << "Professor: Alex Luciano" << endl;
                        cout << "Quer jogar novamente?" << endl;
                        cout << "Digite 1 para jogar de novo ou 0 para sair" << endl;
                        cin >> repetir;
                        if (repetir == 1) {
                            fase = 1;
                        }
                        else {
                            string nome_atual;
                            cout << "Nick do jogador: ";
                            cin >> nome_atual;

                            // Atualiza e mostra o ranking
                            atualizarMostrarRanking(nome_atual, p1.pontuacao);
                        }
                    }

                    if (p1.vivo == 0) {
                        cout<<"Voce perdeu o jogo"<<endl;
                        cout<<"Jogo feito por:"<<endl<<"Luiz Antonio Haenisch"<<endl<<"Daniel Machado"<<endl<<"Vitoria Araujo"<<endl;
                        cout<<"Professor: Alex Luciano"<<endl;
                        cout<<"Quer jogar novamente?"<<endl;
                        cout<<"Digite 1 para jogar de novo ou 0 para sair"<<endl;
                        cin>>repetir;
                        if (repetir == 1) {
                            fase =1;
                        }
                    }
                }while (repetir == 1);
                break;
            case 2:
                system("cls");
                PlaySound(0,0,0);
                PlaySound(TEXT("dificuldade.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                cout<<"Selecione a Dificuldade que voce quer:\n";
                cout<<endl;
                cout<<"Facil (1): (Os Inimigos se movem totalmente aleatorio com 5 em cada fase e um especial no final)"<<endl;
                cout<<"Medio (2): (Os Inimigos se movem com 50% chance de irem ate voce com 5 em cada fase e um final especial)"<<endl;
                cout<<"Dificil (3): (Todos os Inimigos sao especiais com 5 em cada fase)"<<endl;
                cin>>dificuldade;
                break;
            case 3:
                system("cls");
                cout<<"Sobre o Jogo:"<<endl<<endl;
                cout<<"Como jogar:"<<endl;
                cout<<"Bomberman e um classico jogo de estrategia e acao, onde os jogadores assumem o papel de um"<<endl;
                cout<<"personagem que deve abrir caminho atraves de labirintos cheios de inimigos e obstaculos. O objetivo"<<endl;
                cout<<"principal e destruir todos os inimigos e alcancar a saida do labirinto sem ser derrotado."<<endl <<endl;
                cout<<"Como Jogar:"<<endl<<endl;
                cout<<"Movimento: O jogador controla Bomberman usando as teclas A (esquerda), S (baixo), W (cima) e D "<<endl;
                cout<<"(direita) do teclado, movendo-o pelo labirinto."<<endl;
                cout<<"Colocar Bombas: Bomberman pode colocar bombas para destruir obstaculos e inimigos. As "<<endl;
                cout<<"bombas sao colocadas pressionando a tecla E do teclado e explodem apos um certo periodo de "<<endl;
                cout<<"tempo."<<endl;
                cout<<"Evitar Explosoes: As bombas explodem em uma determinada area, entao e importante para o "<<endl;
                cout<<"jogador se manter fora do alcance das explosoes para nao ser atingido."<<endl;
                cout<<"Destruir Obstaculos: As bombas podem ser usadas para destruir paredes e outros obstaculos no "<<endl;
                cout<<"labirinto, abrindo caminho para o jogador avancar."<<endl;
                cout<<"Derrotar Inimigos: Os inimigos podem ser derrotados ao serem pegos nas explosoes das bombas. E "<<endl;
                cout<<"importante planejar estrategicamente as explosoes para pegar o maior numero possivel de inimigos "<<endl;
                cout<<"de uma so vez."<<endl;
                cout<<"Cuidado com os Inimigos: Os inimigos se movem pelo labirinto e tentam atacar Bomberman. Eles "<<endl;
                cout<<"podem derrota-lo se o tocarem, entao e importante evitar contato direto com eles."<<endl;
                cout<<"Colete Itens: Durante o jogo, diversos itens podem aparecer no labirinto, como power-ups para "<<endl;
                cout<<"aumentar a capacidade das bombas ou a velocidade de movimento de Bomberman. Colete esses itens"<<endl;
                cout<<"para melhorar suas chances de sobrevivencia."<<endl;
                cout<<endl<<endl;
                cout<<"Pontuacao:"<<endl<<endl;
                cout<<"- Cada inimigo que você derrotar lhe dara 50 pontos."<<endl;
                cout<<"- Derrotar o Boss lhe dara 100 pontos."<<endl;
                cout<<"- Caixas aleatorias espalhadas pelo cenario lhe darao 5 pontos cada."<<endl;
                cout<<"Mas cuidado! Ha penalidades por usar muitas bombas ou fazer muitos movimentos: "<<endl;
                cout<<"- Se voce usar mais de 8 bombas em uma fase, perdera 10 pontos por fase que passar."<<endl;
                cout<<"- Se usar mais de 10 bombas, perdera 15 pontos por fase."<<endl;
                cout<<"- Se usar mais de 15 bombas, perdera 25 pontos por fase."<<endl;
                cout<<"Movimentos:"<<endl;
                cout<<"- Se fizer mais de 250 movimentos em uma fase, perdera 10 pontos por fase."<<endl;
                cout<<"- Se fizer mais de 300 movimentos, perdera 15 pontos por fase."<<endl;
                cout<<"- Se fizer mais de 400 movimentos, perdera 25 pontos por fase."<<endl;
                cout<<endl<<endl<<endl;
                cout<<"Especiais:"<<endl<<endl;
                cout<<"No mapa soltam alguns especiais destruindo caixas:"<<endl;
                cout<<"+ : Aumenta a area da bomba."<<endl;
                cout<<"I : Deixara você intangivel e podera passar por paredes quebraveis."<<endl;
                cout<<"Õ : Um escudo que protege das explosoes."<<endl;
                cout<<"æ : Controla a bomba e faz ela explodir no momento que quer."<<endl;
                cout<<"² : Vida extra."<<endl;
                system("pause");
                break;
            case 4:
                system("cls");
                cout << "Rank:" << endl;
                atualizarMostrarRanking("", -1); // -1 como valor inválido para não adicionar novo jogador
                system("pause");
                break;
            case 5:
                system("cls");
                cout<<"Obrigado por jogar. :D";
                break;
        }
    }while(menu != 5);

    return 0;
} //fim main
