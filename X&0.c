#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char board[3][3];

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

    printf("Jucatorul %c, alege posizita (1-9): ", player);
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

    printf("Jucatorul %c, alege pozitia (1-9)", player);
    scanf(" %d", &pos);

    if(pos < 1 || pos > 9) return 0;

    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    if(board[row][col] == 'X' || board[row][col] == 'O')
        return 0;

    board[row][col] = player;

    return 1;
}

void makeAimove(char pleyer){

    int empty[9];

    int count = 0;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){

            if(board[i][j] != 'X' || board[i][j] != 'O'){
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

    board[row][col] = pleyer;

    printf("Calculatorul a ales pozitia %d \n", pos);

}

int main(){

    srand(time(NULL));

    char currentPlayer;

    char winner;

    char replay;

    int scoreU = 0;
    int scoreAi = 0;
    int scoreDraw = 0;

    do{

    initBoard();

    currentPlayer = 'X';

    winner = ' ';

    while(1){

        printBoard();

        int valideMove = 0;

        if(currentPlayer == 'X'){
            valideMove = makeYourmove('X');
        }else{
            makeAimove('O');
            valideMove = 1;
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
    printf(" Jucator U : %d", scoreU);
    printf(" Jucator Ai : %d", scoreAi);
    printf(" Remize : %d", scoreDraw);

    printf("Vrei sa continui joaca?");
    printf("D - Da / N - Nu");

    scanf(" %c", &replay);

    }while(replay == 'D' || replay == 'd');

    printf("Sper ca te-ai distrat \n");

    return 0;
}