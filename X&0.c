#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

char board[3][3];
int difficulty;
int winningLine[3][2];
int hasWinner = 0;

void initBoard(){

    int k=1;
    for(int i=0;i<3;i++){

        for(int j=0;j<3;j++){

            board[i][j] = '0' + k++;
        }
    }
}

int isWinningCell(int i,int j){

    for(int k=0;k<3;k++){
        if(winningLine[k][0] == i && winningLine[k][1] == j){
            return 1;
        }
    }

    return 0;
}

void printBoard(){

    printf("\n");
    for(int i=0;i<3;i++){

        printf(" ");

        for(int j=0;j<3;j++){

            char c = board[i][j];

            if(hasWinner && isWinningCell(i,j)){
                printf("\033[1;32m%c\033[0m",c);
            }
            else if(c == 'X')
                printf("\033[1;31m%c\033[0m",c);
            else if(c == 'O')
                printf("\033[1;34m%c\033[0m",c);
            else 
                printf("%c",c);

            //printf("%c", board[i][j]);
            if(j<2) 
                printf(" | ");
        }

        if(i<2)
            printf("\n---|---|---\n");
    }

    printf("\n");
}

char checkWin(){

    for(int i=0;i<3;i++){

        if(board[i][0] == board[i][1] && board[i][1] == board[i][2]){
            
            winningLine[0][0] = i;
            winningLine[0][1] = 0;
            winningLine[1][0] = i;
            winningLine[1][1] = 1;
            winningLine[2][0] = i;
            winningLine[2][1] = 2;
            hasWinner = 1;
            return board[i][0];
        }

        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]){
            
            winningLine[0][0] = 0;
            winningLine[0][1] = i;
            winningLine[1][0] = 1;
            winningLine[1][1] = i;
            winningLine[2][0] = 2;
            winningLine[2][1] = i;
            hasWinner = 1;
            return board[0][i];
        }

    }    

    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]){
        
        winningLine[0][0] = 0; 
        winningLine[0][1] = 0;
        winningLine[1][0] = 1; 
        winningLine[1][1] = 1;
        winningLine[2][0] = 2; 
        winningLine[2][1] = 2;
        hasWinner = 1;
        return board[0][0];
    }
    
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0]){

        winningLine[0][0] = 0;
        winningLine[0][1] = 2;
        winningLine[1][0] = 1;
        winningLine[1][1] = 1;
        winningLine[2][0] = 2;
        winningLine[2][1] = 0;
        hasWinner = 1;
        return board[0][2];
    }


    return ' ';
}

int isDraw(){

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j] != 'X' && board[i][j] != 'O')
            return 0;
        }
    }

    return 1;
}

int makeMove(char player){

    int pos;

    printf("Jucatorul %c, alege posizita : ", player);
    scanf("%d", &pos);

    if(pos < 1 || pos > 9)
        return 0;

    int row = (pos-1) / 3;
    int col = (pos-1) % 3;

    if(board[row][col] == 'X' || board[row][col] == 'O')
        return 0;

    board[row][col] = player;

    return 1;
}

int makeYourmove(char player){

    int pos;

    printf("Jucatorul %c, alege pozitia (1-9) : ", player);
    scanf(" %d", &pos);

    if(pos < 1 || pos > 9) return 0;

    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    if(board[row][col] == 'X' || board[row][col] == 'O')
        return 0;

    board[row][col] = player;

    return 1;
}

void makeAimove(char player){

    int empty[9];

    int count = 0;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){

            if(board[i][j] != 'X' && board[i][j] != 'O'){
                empty[count++] = (i * 3 + j + 1);   
            }
        }
    }

    if(count == 0)
     return;

    int randomIndex = rand() % count;
    int pos = empty[randomIndex];

    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    board[row][col] = player;

    printf("Calculatorul a ales pozitia %d \n", pos);

}

int evaluate(){

    char winner = checkWin();
    
    if(winner == 'O') 
        return +10;
    if(winner == 'X')
        return -10;

    return 0;
}

int minmax(int depth, int isMaximing){

    int score = evaluate();

    if(score == 10 || score == -10){
        return score;
    }

    if(isDraw()){
        return 0;
    }

    if(isMaximing){

        int best = -1000;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(board[i][j] != 'X' && board[i][j] !='O'){

                    char backup = board[i][j]; 
                    board[i][j] = 'O';

                    int val = minmax(depth+1,0);
                    board[i][j] = backup;

                    if( val > best )
                    best = val;
                    
                }
            }
        }

        return best;

    }else{

        int best = 1000;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(board[i][j] != 'X' && board[i][j] != 'O'){

                    char backup = board[i][j];
                    board[i][j] = 'X';

                    int val = minmax(depth+1,1);
                    board[i][j] = backup;

                    if(val < best)
                        best = val;
                }

            }
        }
        
        return best;

    }
}

void bestMove(){

    int bestVal = -1000;
    int bestMove = -1;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j] != 'X' && board[i][j] != 'O'){

                char backup = board[i][j];
                board[i][j] = 'O';

                int moveVal = minmax(0,0);
                board[i][j] = backup;

                if(moveVal > bestVal){
                    bestVal = moveVal;
                    bestMove = i * 3 + j + 1;
                }
            }
        }
    }

    int row = (bestMove - 1) / 3;
    int col = (bestMove - 1) % 3;

    board[row][col] = 'O';
    printf("Ai a ales pozitia %d \n", bestMove);
}

void test_checkWin(){

    char t1[3][3] = {
        {'X','X','X'},
        {'4','5','6'},
        {'7','8','9'}
    };

    memcpy(board,t1,sizeof(board));
    assert(checkWin() == 'X');

    char t2[3][3] = {
        {'O','2','3'},
        {'O','5','6'},
        {'O','8','9'}
    };

    memcpy(board,t2,sizeof(board));
    assert(checkWin() == 'O');

    char t3[3][3] = {
        {'O','2','3'},
        {'4','O','6'},
        {'7','8','O'}
    };

    memcpy(board,t3,sizeof(board));
    assert(checkWin() == 'O');

    char t4[3][3] = {
        {'1','2','X'},
        {'4','X','6'},
        {'X','8','9'}
    };

    memcpy(board,t4,sizeof(board));
    assert(checkWin() == 'X');

    char t5[3][3] = {
        {'O','X','X'},
        {'X','X','O'},
        {'O','O','X'}
    };

    memcpy(board,t5,sizeof(board));
    assert(checkWin() == ' ');
    
    printf("\033[1;32mToate testele pentru checkWin() au trecut cu succes. \033[0m\n");
}

void test_isDraw(){

    char t1[3][3] = {
        {'X','2','3'},
        {'4','O','6'},
        {'X','8','9'}
    };

    memcpy(board,t1,sizeof(board));
    assert(isDraw() == 0);

    char t2[3][3] = {
        {'O','X','X'},
        {'O','X','O'},
        {'X','O','X'}
    };

    memcpy(board,t2,sizeof(board));
    assert(isDraw() == 1);

    printf("\033[1;32mToate testele pentru checkWin au trecut cu succes. \033[0m \n");
}

int main(){

    srand(time(NULL));

    test_checkWin();

    test_isDraw();

    int modJoc;

    char currentPlayer;

    char winner;

    char replay;

    int scoreU = 0;
    int scoreAi = 0;
    int scoreDraw = 0;

    printf("Alege modul de joc : \n");
    printf(" 1 - Pentru a juca contra AI-ului \n");
    printf(" 2 - Pentru a juca contra unui prieten \n");
    scanf(" %d", &modJoc);


    if(modJoc == 1){
    printf("Alege dificultate Ai-ului\n");
    printf("1 - Usor(aleator)\n");
    printf("2 - Mediu(aleatoriu sau inteligent)\n");
    printf("3 - Greu (inteligent)");
    scanf(" %d", &difficulty);
    }

    do{

    initBoard();

    currentPlayer = 'X';

    winner = ' ';

    hasWinner = 0;

    while(1){

        printBoard();

        int valideMove = 0;

        if(modJoc == 2){
            valideMove = makeYourmove(currentPlayer);
        }else{
            if(currentPlayer == 'X'){
                valideMove = makeYourmove('X');
        }
            else if(currentPlayer == 'O'){
                if(difficulty == 1){
                    makeAimove('O');
                }
                if(difficulty == 2){
                    if(rand() % 2 == 0){
                        makeAimove('O');
                    }
                    else{
                        bestMove();
                    }
                }
                if(difficulty == 3){
                    bestMove();
                }

                valideMove = 1;
        }
        }

        if(!valideMove){

            printf("Mutare invalida. Incearca din nou\n");
            continue;
        }
        /*
        if(!makeMove(currentPlayer)){

            printf("Mutare invalida. Incearca din nou \n");
            continue;
        }
        */


        winner = checkWin();

        if(winner == 'X' || winner == 'O'){

            printBoard();
            printf("Jucatorul %c a castigat! \n", winner);
            if(winner == 'X')
            scoreU++;
            else
            scoreAi++;
            break;
        }

        if(isDraw()){
            printBoard();
            printf("Remimza\n");
            scoreDraw++;
            break;
        }

        if(currentPlayer == 'X')
            currentPlayer = 'O';
        else
            currentPlayer = 'X';
    }

    printf("\n Score: \n");
    printf(" Jucator U : %d\n", scoreU);
    printf(" Jucator Ai : %d\n", scoreAi);
    printf(" Remize : %d\n", scoreDraw);

    printf("Vrei sa continui joaca?\n");
    printf("D - Da / N - Nu");

    scanf(" %c", &replay);

    }while(replay == 'D' || replay == 'd');

    printf("Sper ca te-ai distrat \n");

    return 0;
}