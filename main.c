#include<stdio.h>
#include"raylib.h"
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define SIZE 4
#define TOP_BAR_HEIGHT 110
#define BOARD_TOP (TOP_BAR_HEIGHT + 10)
#define TILE_SIZE 105
#define GAP 10
#define SCREEN_PADDING 20
#define SCREEN_WIDTH 490
#define SCREEN_LENGTH 560+40

static int board[SIZE][SIZE];
static int score=0;
static int highscore=0;
static bool gameover=false;
static bool gamewon=false;

int processrowleft(int row[SIZE],int*deltascore){
int newrow[SIZE]={0};
int pos=0;
int changed=0;
// takes all zeros to right
for(int i=0;i<SIZE;i++){
    if(row[i] !=0){
        newrow[pos++]=row[i];
    }
}

for(int i=0;i<SIZE-1;i++){
    if(newrow[i] !=0 && newrow[i] == newrow[i+1]){
        newrow[i]*=2;
        *deltascore+=newrow[i];
        newrow[i+1]=0;
        i++; //skips the next row
    }
}

int finalrow[SIZE]={0};
pos=0;
//takes all zeros to the right
for(int i=0;i<SIZE;i++){
    if(newrow[i] !=0){
        finalrow[pos++] = newrow[i];
    }
}

for(int i=0;i<SIZE;i++){
    if(row[i] != finalrow[i]){
        row[i]=finalrow[i];
        changed=1;
    }
}

return changed;

}

int initializingBoard(){

    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            board[i][j]=0;}
        }

            srand((unsigned int) time(NULL));
            for(int n=0;n<2;n++){
                int x,y;

                do{
                    x=rand()%SIZE;
                    y=rand()%SIZE;
                }while(board[x][y]  != 0);

                board[x][y]=(rand()%10<9) ? 2:4;
            }
        
        
return 0;
}

int addrandomtile(){
       int empty[SIZE*SIZE][2];
       int count=0;

       for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(board[i][j]==0){
                empty[count][0]=i;
                empty[count][1]=j;
                count++;
            }
        }
       }
       if(count==0){
        return 0;
       }

       int r=rand()%count;
       int x=empty[r][0];
       int y=empty[r][1];

       board[x][y]=(rand()%10<9) ? 2:4;
       return 0;

       }

int moveleft(){
    bool moved=false;
    
    for(int i=0;i<SIZE;i++){
        int row[SIZE];

        for(int j=0;j<SIZE;j++){
            row[j]=board[i][j];
        }
        int delta=0;

        if(processrowleft(row,&delta)){
            moved=true;
            score+=delta;

            for(int j=0;j<SIZE;j++){
                board[i][j]=row[j];
            }
        }
    }
    return moved;
}

int moveright(){
    bool moved=false;

    for(int i=0;i<SIZE;i++){
        int row[SIZE];

        for(int j=0;j<SIZE;j++){
            row[j]=board[i][SIZE-1-j];
        }

        int delta=0;

        if(processrowleft(row,&delta)){
            moved=true;
            score+=delta;

            for(int j=0;j<SIZE;j++){
                board[i][SIZE-1-j]=row[j];
            }
        }
    }
    return moved;
}

int moveup(){
    bool moved=false;

    for(int i=0;i<SIZE;i++){
        int col[SIZE];

        for(int j=0;j<SIZE;j++){
            col[j]=board[j][i];
        }

        int delta=0;
        if(processrowleft(col,&delta)){
            moved=true;
            score+=delta;

            for(int j=0;j<SIZE;j++){
                board[j][i]=col[j];
            }
        }
    }
    return moved;
}

int movedown(){
    bool moved=false;
    for(int i=0;i<SIZE;i++){
        int col[SIZE];

        for(int j=0;j<SIZE;j++){
            col[j]=board[SIZE-1-j][i];
        }
        int delta=0;

        if(processrowleft(col,&delta)){
            moved=true;
            score+=delta;

            for(int j=0;j<SIZE;j++){
                board[SIZE-1-j][i]=col[j];
            }
        }
    }
    return moved;
}

int canmove(){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(board[i][j]==0){
                return 1;}

                if(i<SIZE-1 && board[i][j]==board[i+1][j]){
                    return 1;
                }
            if(j<SIZE-1 && board[i][j]==board[i][j+1]){
                return 1;
            }    
    }
}
return 0;

}

Color tileColor(int value){
switch(value){
case 0:return (Color){200,200,200,255};
case 2:return (Color){238,228,218,255};
case 4:return (Color){237,224,200,255};
case 8:return (Color){242,177,121,255};
case 16:return (Color){245,149,99,255};
case 32:return (Color){246,124,95,255};
case 64:return (Color){246,94,59,255};
case 128:return (Color){237,207,114,255};
case 256:return (Color){237,204,97,255};
case 512:return (Color){237,200,80,255};
case 1024:return (Color){237,197,63,255};
case 2048:return (Color){237,194,46,255};
default:return (Color){60,58,50,255};
}
}

int drawTile(int r, int c, int value, Font font) {
    // compute tiles screen position
    //gives the pixel coordinates
    int x = SCREEN_PADDING + c * (TILE_SIZE + GAP); 
    int y = BOARD_TOP + r * (TILE_SIZE + GAP);

    Color bg = tileColor(value);

    DrawRectangleRounded(
        (Rectangle){x, y, TILE_SIZE, TILE_SIZE},
        0.12f, 6, bg
    );

    if (value != 0) {
        //converts number to string like 16
        const char *s = TextFormat("%d", value);

        int fontSize;
        if (value < 100) fontSize = 42;
        else if (value < 1000) fontSize = 34;
        else fontSize = 28;

        int textW = MeasureText(s, fontSize);
        Color textColor = (value <= 4)
            ? (Color){119,110,101,255}
            : WHITE;

        DrawText(
            //alligns the number in centre
            s,
            x + TILE_SIZE/2 - textW/2,
            y + TILE_SIZE/2 - fontSize/2,
            fontSize,
            textColor
        );
    }

    return 0;
}

int drawBoard(Font font) {
    DrawTextEx(
        font,
        "2048",
        (Vector2){ SCREEN_PADDING, SCREEN_PADDING },
        58,
        2,
        (Color){119,110,101,255}
    );

    // Label: HIGHEST SCORE
DrawTextEx(
    font,
    "HIGH SCORE",
    (Vector2){
        SCREEN_WIDTH - SCREEN_PADDING - 110 - 130 + 4,
        SCREEN_PADDING - 15
    },
    16,
    1,
    (Color){119,110,101,255}
);


    // HIGH SCORE BOX (placed left of score box)
DrawRectangleRounded(
    (Rectangle){
        SCREEN_WIDTH - SCREEN_PADDING - 110 - 130,   // 130 pixels left
        SCREEN_PADDING,
        110, 60
    },
    0.15f, 8,
    (Color){187,173,160,255}
);

// Draw high score number
DrawTextEx(
    font,
    TextFormat("%d", highscore),
    (Vector2){
        SCREEN_WIDTH - SCREEN_PADDING - 110 - 130 + 28,
        SCREEN_PADDING + 12
    },
    32,
    2,
    WHITE
);

// Label: SCORE
DrawTextEx(
    font,
    "SCORE",
    (Vector2){
        SCREEN_WIDTH - SCREEN_PADDING - 110 + 22,
        SCREEN_PADDING - 15
    },
    20,
    1,
    (Color){119,110,101,255}
);

    DrawRectangleRounded(
        (Rectangle){
            SCREEN_WIDTH - SCREEN_PADDING - 110,
            SCREEN_PADDING,
            110, 60
        },
        0.15f, 8,
        (Color){187,173,160,255}
    );

    DrawTextEx(
        font,
        TextFormat("%d", score),
        (Vector2){
            SCREEN_WIDTH - SCREEN_PADDING - 110 + 28,
            SCREEN_PADDING + 12
        },
        32,
        2,
        WHITE
    );

    
    int boardHeight = SIZE * TILE_SIZE + (SIZE - 1) * GAP;
    
    //draws board background frame
    DrawRectangleRounded(
        (Rectangle){
            SCREEN_PADDING / 2,
            BOARD_TOP - 10,
            SCREEN_WIDTH - SCREEN_PADDING,
            boardHeight + 20 
        },
        0.08f, 12,
        (Color){205,193,180,255}
    );

    // === Tiles ===
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            int x = SCREEN_PADDING + j * (TILE_SIZE + GAP);
            int y = BOARD_TOP + i * (TILE_SIZE + GAP);

            DrawRectangleRounded(
                (Rectangle){ x, y, TILE_SIZE, TILE_SIZE },
                0.15f, 10,
                (Color){187,173,160,255}
            );

            if (board[i][j] != 0) {
                drawTile(i, j, board[i][j], font);
            }
        }
    }

    // === Game Over Overlay ===
if (gameover) {

    // Dark transparent layer
    DrawRectangle(
        SCREEN_PADDING,
        BOARD_TOP,
        SCREEN_WIDTH - SCREEN_PADDING * 2,
        (SIZE * TILE_SIZE) + (SIZE - 1) * GAP,
        Fade(BLACK, 0.45f)
    );

    const char *msg = "     Game Over!\nPress R to restart";
    int fontSize = 27;
    int w = MeasureText(msg, fontSize);

    DrawText(
        msg,
        SCREEN_WIDTH/2 - w/2,
        BOARD_TOP + ((SIZE * TILE_SIZE + (SIZE - 1) * GAP) / 2) - fontSize/2,
        fontSize,
        WHITE
    );
}

// === Game Won Overlay ===
if (gamewon) {

    // Dark transparent layer (lighter than game over)
    DrawRectangle(
        SCREEN_PADDING,
        BOARD_TOP,
        SCREEN_WIDTH - SCREEN_PADDING * 2,
        (SIZE * TILE_SIZE) + (SIZE - 1) * GAP,
        Fade(BLACK, 0.30f)
    );

    const char *msg = "Shake your draids!";
    int fontSize = 32;
    int w = MeasureText(msg, fontSize);

    DrawText(
        msg,
        SCREEN_WIDTH/2 - w/2,
        BOARD_TOP + ((SIZE * TILE_SIZE + (SIZE - 1) * GAP) / 2) - fontSize/2,
        fontSize,
        WHITE
    );
}


    // Optional overlays (unchanged)
    return 0;
}

void loadhighscore(){
    FILE*f=fopen("highscore.txt","r");
    if(f){
        fscanf(f,"%d",&highscore);
        fclose(f);  
    }else{
        highscore=0;
    }
}

void savehighscore(){
    FILE*f=fopen("highscore.txt","w");
    if(f){
        fprintf(f,"%d",highscore);
        fclose(f);
    }
}

int main(){
InitWindow(SCREEN_WIDTH,SCREEN_LENGTH,"2048 - Raylib(c)");
SetTargetFPS(60);

Font font=GetFontDefault();

initializingBoard();
loadhighscore();
while(!WindowShouldClose()){
    bool moved=false;

    if(!gameover){

        if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            moved=moveleft();
        }

        else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
            moved=moveright();
        }

        else if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
            moved=movedown();
        }

        else if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
            moved=moveup();
        }

        if(moved){
            if(score>highscore){
                highscore=score;
                savehighscore();
            }
            addrandomtile();

            for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                    if(board[i][j]==2048){
                        gamewon=true;
                    }
                }
            }
            if(!canmove()){
                gameover=true;
            }
        }
    }

    if(IsKeyPressed(KEY_R)){
    initializingBoard();
    gameover=false;
    gamewon=false;
    score=0;
}

        BeginDrawing();
        ClearBackground((Color){250, 248, 239, 255});

        drawBoard(font);
        EndDrawing();

}

CloseWindow();

    return 0;
}