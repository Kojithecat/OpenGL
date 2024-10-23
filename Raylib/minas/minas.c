#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
#define CELL_SIZE 50


enum nivel {
NONE,
EASY,
MEDIUM,
HARD,
};

typedef struct{
int x;
int y;
} Coord;

typedef struct{
    int isMine;
    int isRevealed;
    int isFlagged;
    int adjacentMines;
} Cell;

Cell **grid;

const int screenWidth = 1600;
const int screenHeight = 900;
enum nivel gameMode = NONE;
int padding = 64;
int numMines;
int gridSize;
int winCondition;

void drawMenuScreen(){
    
    char* titleText = "MineSweeper";
    char* optionsText = "Select the level:";
    char* easyText = "Easy";
    char* mediumText = "Medium";
    char* hardText = "Hard";
    //char* mode = sprintf("%d", gameMode);
    //strcat(optionsText, mode)
    DrawText(titleText, screenWidth/2 - MeasureText(titleText,100)/2, screenHeight/4, 100, BLACK);
    DrawText(optionsText, screenWidth/2 - MeasureText(optionsText,40)/2, 2*screenHeight/4, 40, GRAY);

    int widthLevelRects = screenWidth/3 - padding;  	
    int heightLevelRects = screenHeight/5;


    Rectangle easyLevel = {padding/2, 3*screenHeight/5, widthLevelRects, heightLevelRects};
    Rectangle mediumLevel = {screenWidth/3 +padding/2, 3*screenHeight/5, widthLevelRects, heightLevelRects};
    Rectangle hardLevel = {2*screenWidth/3 + padding/2, 3*screenHeight/5, widthLevelRects, heightLevelRects};

    DrawRectangleRounded(easyLevel, 0.5, 4 , GREEN);
    DrawRectangleRounded(mediumLevel, 0.5, 4 , ORANGE);
    DrawRectangleRounded(hardLevel, 0.5, 4 , RED);

    DrawText(easyText, easyLevel.x + easyLevel.width/2 - MeasureText(easyText, 40)/2, easyLevel.y + easyLevel.height/2 - 20, 40, BLACK );
    DrawText(mediumText, mediumLevel.x + mediumLevel.width/2 - MeasureText(mediumText, 40)/2, mediumLevel.y + mediumLevel.height/2-20, 40, BLACK );
    DrawText(hardText, hardLevel.x + hardLevel.width/2 - MeasureText(hardText, 40)/2, hardLevel.y + hardLevel.height/2-20, 40, BLACK );

	
    //Logic of main screen
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    if(CheckCollisionPointRec(GetMousePosition(), easyLevel)){
        gameMode = EASY;
  	numMines = 10;
	gridSize = 9;
	winCondition = 0;
    }
    else if(CheckCollisionPointRec(GetMousePosition(), mediumLevel)){
    	gameMode = MEDIUM;
	numMines = 20;
	gridSize = 12;
	winCondition = 0;
    }
    else if(CheckCollisionPointRec(GetMousePosition(), hardLevel)){
    	gameMode = HARD;
	numMines = 40;
	gridSize = 16;
	winCondition = 0;
    }

}

void allocateGrid(){
    grid = (Cell **)malloc(gridSize * sizeof(Cell *));
    for(int i = 0; i<gridSize; i++)
	grid[i] = (Cell *)malloc(gridSize* sizeof(Cell));
}

void freeGrid(){
    for(int i = 0; i < gridSize; i++){
    	free(grid[i]);
    }
    free(grid);

}

void initGrid(){
    allocateGrid();
    //Initialize cells as empty
    for(int x = 0; x < gridSize; x++){
        for(int y = 0; y < gridSize; y++){
            grid[x][y].isMine = 0;
    	    grid[x][y].isFlagged = 0;
	    grid[x][y].isRevealed = 0;
	    grid[x][y].adjacentMines = 0;	    
	}
    }
    //Fill the bomb cells
    srand(42);
    int remainingMines = numMines;
    while(remainingMines > 0){
        int x = rand() % gridSize;
	int y = rand() % gridSize;
	if(!grid[x][y].isMine){
	    grid[x][y].isMine = 1;
	    //One more mine in the grid	
	    remainingMines--;
	    //Check adjacency of mines edge cases (top bottom left right)
            
	    int top = y == 0;
	    int bottom = y == gridSize-1;
	    int left = x == 0;
	    int right = x == gridSize-1;

	    if(!right && !bottom)grid[x+1][y+1].adjacentMines++;
            if(!right)           grid[x+1][y].adjacentMines++;  
	    if(!right && !top)   grid[x+1][y-1].adjacentMines++;
	    if(!bottom)          grid[x][y+1].adjacentMines++;
            if(!top)             grid[x][y-1].adjacentMines++;
	    if(!left && !bottom) grid[x-1][y+1].adjacentMines++;
	    if(!left)            grid[x-1][y].adjacentMines++;
	    if(!left && !top)    grid[x-1][y-1].adjacentMines++;
	}
    }
}

void updateGrid(){

}

void drawGrid(){
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            Rectangle cellRect = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            
            // If the cell is revealed
            if (grid[x][y].isRevealed) {
                if (grid[x][y].isMine) {
                    DrawRectangleRec(cellRect, RED);  // Draw the mine
                } else {
                    DrawRectangleRec(cellRect, LIGHTGRAY);  // Draw the empty cell
                    if (grid[x][y].adjacentMines > 0) {
                        DrawText(TextFormat("%d", grid[x][y].adjacentMines), x * CELL_SIZE + 10, y * CELL_SIZE + 10, 20, BLACK);
                    }
                }
            } else {
                DrawRectangleRec(cellRect, DARKGRAY);  // Draw unrevealed cells
                if (grid[x][y].isFlagged) {
                    DrawText("F", x * CELL_SIZE + 10, y * CELL_SIZE + 10, 20, BLUE);  // Draw flag
                }
            }

            // Draw the grid lines
            DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }

} 

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        printf("Hii1");
	fflush(stdout);
	while(gameMode == NONE){
	    BeginDrawing();
    	    ClearBackground(RAYWHITE);
            drawMenuScreen();
	    EndDrawing();
	}		
	printf("Hii"); 
	fflush(stdout);
        initGrid();
	printf("grid Done");
	fflush(stdout);
	SetWindowSize(CELL_SIZE * gridSize, CELL_SIZE * gridSize);

	while(!winCondition)
	{
	    //printf("grid post");
     	    //fflush(stdout);
	    updateGrid();
	    BeginDrawing();
    	    ClearBackground(RAYWHITE);
            drawGrid();
	    EndDrawing();
	}
	freeGrid();
        
	BeginDrawing();
    	ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
