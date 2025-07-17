#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char board[3][3];
int difficulty;

void initBoard(){

    int k=1;
    for(int i=0;i<3;i++){

        for(int j=0;j<3;j++){

            board[i][j] = '0' + k++;
        }
    }
}

void printBoard(){

    printf("\n");
    for(int i=0;i<3;i++){

        printf(" ");

        for(int j=0;j<3;j++){

            printf("%c", board[i][j]);
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

        if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }    

    if(board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

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

int main(){

    srand(time(NULL));

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
                if(difficulty == 2 && rand() % 2 == 0){
                    makeAimove('O');
                }else{
                    bestMove();
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