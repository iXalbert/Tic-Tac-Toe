#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_audio.h>
#include <math.h>
#include <assert.h>

#define CELL_SIZE 200
#define ANIM_STEPS 10
#define ANIM_DELAY 20

SDL_AudioSpec moveSpec,winSpec;
Uint32 moveLen,winLen;
Uint8 *moveBuffer = NULL, *winBuffer = NULL;
SDL_AudioDeviceID audioDevice = 0;
int soundEnabled = 0;  

typedef struct{

    int scorX;
    int scorO;
}Scoruri;

char board[3][3];
char currentPlayer = 'X';
int gameOver = 0;
char winner = ' ';
char mesaj[32] = " ";
int scoreX = 0;
int scoreO = 0;
char scorText[100];


SDL_Rect restartButton = {200,700,200,60};

int win_x1=-1,win_y1=-1,win_x2=-1,win_y2=-1;

Scoruri scoruri = {0,0};

void drawX(SDL_Renderer *renderer, int row, int col, float scale){

    int size = CELL_SIZE * scale;

    int offsetX = col * CELL_SIZE + (CELL_SIZE - size) / 2;
    int offsetY = row * CELL_SIZE + (CELL_SIZE - size) / 2;

    SDL_SetRenderDrawColor(renderer,255,0,0,255);

    SDL_RenderDrawLine(renderer,offsetX,offsetY,offsetX + size,offsetY + size);
    SDL_RenderDrawLine(renderer,offsetX + size,offsetY,offsetX,offsetY + size);
}

void drawO(SDL_Renderer *renderer, int row, int col, float scale){

    int radius = CELL_SIZE / 2 * scale;

    int centerX = col * CELL_SIZE + CELL_SIZE / 2;
    int centerY = row * CELL_SIZE + CELL_SIZE / 2;

    SDL_SetRenderDrawColor(renderer,0,0,255,255);

    for(int w=0;w<2;w++){
        int r = radius - w;
        int dx = r - 1;
        int dy = 0;
        int err = dx - (r << 1);

        while(dx >= dy){

            SDL_RenderDrawPoint(renderer,centerX+dx,centerY+dy);
            SDL_RenderDrawPoint(renderer,centerX+dx,centerY+dy);
            SDL_RenderDrawPoint(renderer,centerX-dx,centerY+dy);
            SDL_RenderDrawPoint(renderer,centerX-dx,centerY+dy);
            SDL_RenderDrawPoint(renderer,centerX-dx,centerY-dy);
            SDL_RenderDrawPoint(renderer,centerX-dx,centerY-dy);
            SDL_RenderDrawPoint(renderer,centerX+dx,centerY-dy);
            SDL_RenderDrawPoint(renderer,centerX+dx,centerY-dy);

            if(err <= 0 ){
                dy++;
                err = err + dy * 2 + 1;
            }

            if(err > 0){
                dx--;
                err = err - dx * 2 + 1;
            }
        }
    }
}

void resetGame(){

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            board[i][j] = ' ';
        } 
   }

   currentPlayer = 'X';
   gameOver = 0;
   mesaj[0] = '\0';
   winner = ' ';
   win_x1=win_y1=win_x2=win_y2=-1;
}

void drawGrid(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    SDL_RenderDrawLine(renderer,200,0,200,600);
    SDL_RenderDrawLine(renderer,400,0,400,600);
    SDL_RenderDrawLine(renderer,600,0,600,600);

    SDL_RenderDrawLine(renderer,0,200,600,200);
    SDL_RenderDrawLine(renderer,0,400,600,400);
    SDL_RenderDrawLine(renderer,0,600,600,600);
}

void drawXO(SDL_Renderer *renderer){

    int cellSize = 600 / 3;

    for(int row = 0 ; row < 3 ; row++){
        for(int col = 0 ; col < 3 ; col++){
            int x = col * cellSize;
            int y = row * cellSize;

            if(board[row][col] == 'X'){

                drawX(renderer,row,col,1.0f);
                //SDL_SetRenderDrawColor(renderer,255,0,0,255);
                //SDL_RenderDrawLine(renderer,x+20,y+20,x+cellSize-20,y+cellSize-20);
                //SDL_RenderDrawLine(renderer,x+cellSize-20,y+20,x+20,y+cellSize-20);
            }else if(board[row][col] == 'O'){


                drawO(renderer,row,col,1.0f);
                //SDL_RenderDrawLine(renderer,0,0,255,255);
                //SDL_Rect o = {x+20,y+20,cellSize-40,cellSize-40};
                //SDL_RenderDrawRect(renderer,&o);
                //SDL_SetRenderDrawColor(renderer,0,0,255,255);
                //for(int r=0;r<360;r++){
                //    double angle = r * M_PI / 180;
                //    int px = (int)(x + 100 + 80 * cos(angle));
                //    int py = (int)(y + 100 + 80 * sin(angle));
                //    SDL_RenderDrawPoint(renderer,px,py);
                //}
            }
        }
    }

    /*
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
        */
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
                int cx = x + 200 / 2;
                int cy = y + 200 / 2;
                int r = 200 / 2 - 20;

                for(int angle = 0; angle < 360; angle++){
                    double rad = angle * M_PI / 180;
                    int px = cx + (int)(r * cos(rad));
                    int py = cy + (int)(r * sin(rad));
                    SDL_RenderDrawPoint(renderer, px, py);
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

char checkWinner(char board[3][3], int *x1, int *x2, int *y1, int *y2){

    for(int i=0;i<3;i++){

        if(board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]){
            *x1 = 0;
            *x2 = 2;
            *y1 = i;
            *y2 = i;

            return board[i][0];
        }

        if(board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]){
            *x1 = i;
            *x2 = i;
            *y1 = 0;
            *y2 = 2;

            return board[0][i];
        }
    }

    if(board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]){

        *x1 = 0;
        *y1 = 0;
        *x2 = 2;
        *y2 = 2;

        return board[0][0];
    }

    if(board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]){

        *x1 = 2;
        *y1 = 0;
        *x2 = 0;
        *y2 = 2;

        return board[0][2];
    }

    return ' ';

}

void markWinnerLine(SDL_Renderer *renderer, int x1, int x2, int y1, int y2){

    if(x1 == -1 && x2 == -1 && y1 == -1 && y2 == -1){
        return;
    }

    int cellSize = 600 / 3;

    int startX = x1 * cellSize + cellSize / 2;
    int startY = y1 * cellSize + cellSize / 2;

    int endX = x2 * cellSize + cellSize / 2;
    int endY = y2 * cellSize + cellSize / 2;

    SDL_SetRenderDrawColor(renderer,0,255,0,255);

    for(int i = -3; i < 3; i++){

        SDL_RenderDrawLine(renderer,startX+i,startY,endX+i,endY);
        SDL_RenderDrawLine(renderer,startX,startY+i,endX,endY+i);
    }
}

void ruleazaTeste(){

    char t1[3][3] = {

        {'X','X','X'},
        {'O',' ','O'},
        {' ',' ',' '}
    };

    assert(verificaCastigator(t1) == 'X');

    char t2[3][3] = {

        {'O','X','X'},
        {'O','X',' '},
        {'O',' ',' '}
    };

    assert(verificaCastigator(t2) == 'O');

    char t3[3][3] = {

        {'X','O','X'},
        {'X','O','O'},
        {'O','X','X'}
    };

    assert(is_Draw(t3) == 1);
    assert(verificaCastigator(t3) == ' ');

    char t4[3][3] = {

        {'X',' ','O'},
        {'X','O',' '},
        {'O',' ','X'}
    };

    assert(verificaCastigator(t4) == 'O');

    printf("Toate testele au trecut cu succes \n");
}

void drawRestartButton(SDL_Renderer *renderer, TTF_Font *font){

    SDL_SetRenderDrawColor(renderer,0,128,255,255);
    SDL_RenderFillRect(renderer,&restartButton);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    SDL_Color textColor = {255,255,255,255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Restart", textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_Rect textRect = {restartButton.x + 18, restartButton.y + 0, surface->w, surface->h};
    SDL_RenderCopy(renderer,texture,NULL,&textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void afiseazaMesaj(SDL_Renderer* renderer, TTF_Font* font){

    if(strlen(mesaj) == 0){
        return;
    }

    SDL_Color culoare = {255,255,255,255};

    SDL_Surface* surface = TTF_RenderText_Solid(font,mesaj,culoare);
    if(!surface){
        printf("Eroare la crearea text surface: %s \n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    //SDL_FreeSurface(surface);
    if(!texture){
        printf("Eroare la creare textura text : %s \n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int centeredX = (600 - surface->w) / 2;    
    SDL_Rect rect = {centeredX, 630, surface->w, surface->h};
    
    SDL_RenderCopy(renderer,texture,NULL,&rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void afiseazaScore(SDL_Renderer *renderer, TTF_Font *font, int scoreX, int scoreO){

        SDL_Color culoareText = {255,255,255,255};

        char textX[32];

        sprintf(textX,"Score X : %d ",scoreX);
        SDL_Surface* textSurfaceX = TTF_RenderText_Solid(font,textX,culoareText);
        SDL_Texture* textTextureX = SDL_CreateTextureFromSurface(renderer,textSurfaceX);
        SDL_Rect destX = {620,100,textSurfaceX->w,textSurfaceX->h};
        SDL_RenderCopy(renderer,textTextureX,NULL,&destX);
        SDL_FreeSurface(textSurfaceX);
        SDL_DestroyTexture(textTextureX);

        char textO[32];

        sprintf(textO,"Score 0 : %d ",scoreO);
        SDL_Surface* textSurfaceO = TTF_RenderText_Solid(font,textO,culoareText);
        SDL_Texture* textTextureO = SDL_CreateTextureFromSurface(renderer,textSurfaceO);
        SDL_Rect destO = {620,150,textSurfaceO->w,textSurfaceO->h};
        SDL_RenderCopy(renderer,textTextureO,NULL,&destO);
        SDL_FreeSurface(textSurfaceO);
        SDL_DestroyTexture(textTextureO);

}
/*
void aninmateSymbol(SDL_Renderer* renderer, SDL_Textue* texture, SDL_Rect destRect){

    for(int i=0;i<=255;i=i+15){

        SDL_SetTextureAlphaMod(texture,i);
        SDL_RenderClear(renderer);

        drawBoard(renderer);
        SDL_RenderCopy(renderer,texture,NULL,&destRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_SetTextureAlphaMod(texture,255);
}*/

void animateSymbol(SDL_Renderer* renderer, int row, int col, char symbol) {
    
    TTF_Font *font = TTF_OpenFont("arial.ttf", 48);
    if (!font) {
        
        for (int step = 1; step <= ANIM_STEPS; step++) {
            float scale = step / (float)ANIM_STEPS;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            drawGrid(renderer);
            deseneazaTabla(renderer);
            
            if (symbol == 'X')
                drawX(renderer, row, col, scale);
            else if (symbol == 'O')
                drawO(renderer, row, col, scale);

            SDL_RenderPresent(renderer);
            SDL_Delay(ANIM_DELAY);
        }
        return;
    }
    

    char temp = board[row][col];
    board[row][col] = ' '; 
    
    for (int step = 1; step <= ANIM_STEPS; step++) {
        float scale = step / (float)ANIM_STEPS;
        
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        
        drawGrid(renderer);
        deseneazaTabla(renderer);  
        afiseazaScore(renderer, font, scoreX, scoreO);  
        afiseazaMesaj(renderer, font);  
        drawRestartButton(renderer, font); 
        
        
        if (symbol == 'X')
            drawX(renderer, row, col, scale);
        else if (symbol == 'O')
            drawO(renderer, row, col, scale);

        SDL_RenderPresent(renderer);
        SDL_Delay(ANIM_DELAY);
    }
    
    
    board[row][col] = temp;
    
    
    TTF_CloseFont(font);
}

void incarcareScoruri(){

    FILE* f = fopen("scor.txt","r");
    if(f){

        fscanf(f," %d %d", &scoruri.scorX, &scoruri.scorO);
        fclose(f);
    }
}

void salveazaScor(){

    FILE *f = fopen("scor.txt","w");
    if(f){

        fprintf(f," %d %d\n", scoruri.scorX, scoruri.scorO);
        fclose(f);
    }
}

void playSound(SDL_AudioSpec spec, Uint8 *buffer, Uint32 len){
   
    if (buffer == NULL) {
        return;  
    }

    
    SDL_CloseAudio();
    
    if(SDL_OpenAudio(&spec, NULL) < 0){
        return;
    }

    SDL_PauseAudio(0);
    SDL_QueueAudio(1, buffer, len);
    
    
    SDL_Delay(50); 
}

int main(int argc, char **argv){

    ruleazaTeste();
    incarcareScoruri();

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        printf("Eroare la initializarea SDL : %s\n", SDL_GetError());
        return 1;
    }

    
    if(SDL_LoadWAV("move.wav",&moveSpec, &moveBuffer,&moveLen) != NULL){
        soundEnabled = 1; 
    } else {
        fprintf(stderr,"Info: move.wav not found - game will run without move sounds\n");
        moveBuffer = NULL;
    }

    if(SDL_LoadWAV("win.wav", &winSpec, &winBuffer,&winLen) != NULL){
        soundEnabled = 1;  
    } else {
        fprintf(stderr,"Info: win.wav not found - game will run without win sounds\n");
        winBuffer = NULL;
    }

    if(TTF_Init() == -1){
        printf("Eroare la initializarea SDL_ttf: %s \n",TTF_GetError());
        SDL_Quit();
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
        950, 800,
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
 
    TTF_Font *font = TTF_OpenFont("arial.ttf", 48);
    if(!font){
        printf("Eraore la incarcare font : %s \n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    resetGame();
    int running = 1;
    SDL_Event e;

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                running = 0;
            }else if(e.type == SDL_MOUSEBUTTONDOWN){// && !gameOver){

            int mx = e.button.x;
            int my = e.button.y;
            //int col = mx / 200;
            //int row = my / 200;

            /*if(board[row][col] == ' '){

                board[row][col] = currentPlayer;
                
                if(currentPlayer == 'X')
                    currentPlayer = 'O';
                else
                    currentPlayer = 'X';
            }*/

            if(e.button.x >= restartButton.x && e.button.x <= restartButton.x + restartButton.w && 
                e.button.y >= restartButton.y && e.button.y <= restartButton.y + restartButton.h){

                    resetGame();
                    continue;
                }
            /*
            if(mx >= 200 && mx <= 400 && my >= 610 && my <= 660){

                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        board[i][j] = ' ';
                    }
                }

                gameOver = 0;
                currentPlayer = 'X';
                continue;
            }
            */

            if(!gameOver && my < 600){

                int col = mx / 200;
                int row = my / 200;
            
                if(row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' '){

                board[row][col] = currentPlayer;

                playSound(moveSpec,moveBuffer,moveLen);
                animateSymbol(renderer,row,col,currentPlayer);

                //char castigator = checkWinner(board,&x1,&x2,&y1,&y2);

                char castigator = verificaCastigator(board);

                if(castigator != ' '){
                    playSound(winSpec,winBuffer,winLen);
                    winner = castigator;
                    checkWinner(board,&win_x1,&win_x2,&win_y1,&win_y2);
                    snprintf(mesaj,sizeof(mesaj),"  Jucatorul %c a castigat! \n", winner);
                    gameOver = 1;
                    if(castigator == 'X'){
                        
                        scoreX++;
                        salveazaScor();
                    }else{

                        scoreO++;
                        salveazaScor();
                    }
                    FILE *f = fopen("score.txt","a");
                    if(f!= NULL){
                        fprintf(f,"Scor : X = %d, O = %d \n", scoreX,scoreO);
                        fclose(f);
                    }
                }else if(is_Draw(board)){
                    playSound(winSpec,winBuffer,winLen);
                    strcpy(mesaj,"Remiza! \n");
                    castigator = 'D';
                    gameOver = 1;
                }else{
                    if(currentPlayer == 'X')
                        currentPlayer = 'O';
                    else
                        currentPlayer = 'X'; 
                    
                    snprintf(mesaj,sizeof(mesaj),"Randul jucatorului %c", currentPlayer);
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
        } 
        
        // Render everything
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);

        //drawXO(renderer);

        deseneazaTabla(renderer);
        afiseazaScore(renderer,font,scoreX,scoreO);
        /*
        SDL_SetRenderDrawColor(renderer,0,200,0,255);
        SDL_RenderFillRect(renderer,&restartButton);

        SDL_SetRenderDrawColor(renderer,0,100,0,255);
        SDL_RenderDrawRect(renderer,&restartButton);

        TTF_Font* restartFont = TTF_OpenFont("arial.ttf", 24);
        if(restartFont){

            SDL_Color text_color = {255,255,255};
            SDL_Surface* textSurface = TTF_RenderText_Solid(restartFont,"Restart game", text_color);

            if(textSurface){

                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer,textSurface); 
                SDL_Rect textRect = {restartButton.x + 25, restartButton.y + 10, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer,textTexture,NULL,&textRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);

            }

            TTF_CloseFont(restartFont);
        }

        if(gameOver){
            markWinnerLine(renderer,win_x1,win_x2,win_y1,win_y2);
        }

        if(gameOver){

            char mesaj[50];

            if(currentPlayer != ' '){
                sprintf(mesaj, "Jucatorul %c a castigat!\n", currentPlayer);
            }else{
                sprintf(mesaj,"Remiza!\n");
            }

            afiseazaMesaj(renderer,font,mesaj,25,255);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        */

        afiseazaMesaj(renderer,font);
        drawRestartButton(renderer,font);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();
     
    if (moveBuffer != NULL) {
        SDL_FreeWAV(moveBuffer);
    }
    if (winBuffer != NULL) {
        SDL_FreeWAV(winBuffer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
