#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

char board[3][3] = {

    {' ',' ',' '},
    {' ',' ',' '},
    {' ',' ',' '}
};
char currentPlayer = 'X';
int gameOver = 0;

void drawGrid(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    SDL_RenderDrawLine(renderer,200,0,200,600);
    SDL_RenderDrawLine(renderer,400,0,400,600);

    SDL_RenderDrawLine(renderer,0,200,600,200);
    SDL_RenderDrawLine(renderer,0,400,600,400);
}

void drawXO(SDL_Renderer *renderer){

    const int pad = 50;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){

            int x = j * 200;
            int y = i * 200;

            if(board[i][j] == 'X'){
                SDL_SetRenderDrawColor(renderer,255,0,0,255);

                SDL_RenderDrawLine(renderer,x+pad,y+pad,x+200-pad,y+200-pad);
                SDL_RenderDrawLine(renderer,x+200-pad,y+pad,x+pad,y+200-pad);
            }else if(board[i][j] == 'O'){

                SDL_SetRenderDrawColor(renderer,0,0,255,255);

                const int cx = x + 100;
                const int cy = y + 100;
                const int r = 100 - pad;

                for(int w=0;w<360;w++){
                    float t1 = (w + 0) * M_PI / 180;
                    float t2 = (w + 1) * M_PI / 180;

                    SDL_RenderDrawLine(renderer,cx + (int)(r*cos(t1)), cy + (int)(r*sin(t1)), cx + (int)(r*cos(t2)), cy + (int)(r*sin(t2)));
                
                }
            }
        }
    }
}

char verificaCastigator(char board[3][3]){

    for(int i=0;i<3;i++)
        if(board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];

    for(int i=0;i<3;i++)
        if(board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];

    if(board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];

    if(board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return ' ';
}

void deseneazaTabla(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    SDL_RenderDrawLine(renderer,200,0,200,600);
    SDL_RenderDrawLine(renderer,400,0,400,600);

    SDL_RenderDrawLine(renderer,0,200,600,200);
    SDL_RenderDrawLine(renderer,0,400,600,400);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){

            int x = j * 200;
            int y = i * 200;

            if(board[i][j] == 'X'){

                SDL_SetRenderDrawColor(renderer,255,0,0,255);
                SDL_RenderDrawLine(renderer,x+20,y+20,x+180,y+180);
                SDL_RenderDrawLine(renderer,x+180,y+20,x+20,y+180);
            }else if(board[i][j] == 'O'){
                SDL_SetRenderDrawColor(renderer,0,0,255,255);
                for(int r = 0;r<360;r++){

                    double angle = r * M_PI / 180;

                    int px = (int)(x + 100 + 80 * cos(angle));
                    int py = (int)(y + 100 + 80 * sin(angle));

                    SDL_RenderDrawPoint(renderer,px,py);
                }
            }
        }
    }
}

int is_Draw(char tabla[3][3]){

    if(verificaCastigator(tabla) != ' '){
        return 0;
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(tabla[i][j] == ' '){
                return 0;
            }
        }
    }

    return 1;
}

int main(int argc, char **argv){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Eroare la initializarea SDL : %s\n", SDL_GetError());
        return 1;
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)
            board[i][j] = ' ';
    }

    SDL_Window* window = SDL_CreateWindow(
        "Tic Tac Toe",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600, 600,
        SDL_WINDOW_SHOWN
    );

    if(!window){
        printf("Eroare la crearea ferestrei : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Eroare la crerea renderer-ului : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event e;

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                running = 0;
            }else if(e.type == SDL_MOUSEBUTTONDOWN && !gameOver){

            int mx = e.button.x;
            int my = e.button.y;
            int col = mx / 200;
            int row = my / 200;

            /*if(board[row][col] == ' '){

                board[row][col] = currentPlayer;
                
                if(currentPlayer == 'X')
                    currentPlayer = 'O';
                else
                    currentPlayer = 'X';
            }*/

            if(row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' '){

                board[row][col] = currentPlayer;

                char castigator = verificaCastigator(board);

                if(castigator != ' '){
                    printf("Jucatorul %c a castigat! \n", castigator);
                    gameOver = 1;
                }else if(is_Draw(board)){
                    printf("Remiza! \n");
                }else{
                    if(currentPlayer == 'X')
                        currentPlayer = 'O';
                    else
                        currentPlayer = 'X'; 
                }

                for(int i=0;i<3;i++){
                    printf("| %c | %c | %c |\n", board[i][0],board[i][1], board[i][2]);
                    printf("|-----------|\n");
                }
                printf("\n\n");
                //printf("|-----------|\n");
            }

            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        //drawGrid(renderer);

        //drawXO(renderer);

        deseneazaTabla(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}