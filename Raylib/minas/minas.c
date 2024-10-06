#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
enum nivel {
EASY,
MEDIUM,
HARD,
};

enum casilla {
UNO,
DOS,
TRES,
CUATRO,
CINCO,
SEIS,
SIETE,
OCHO,
VACIO,
BOMBA,
};

const int screenWidth = 1600;
const int screenHeight = 900;
int gameStarted = 0;
int padding = 64;


void drawMenuScreen(){
    
    BeginDrawing();
	
    ClearBackground(RAYWHITE);
    char* titleText = "MineSweeper";
    char* optionsText = "Select the level:";
    char* easyText = "Easy";
    char* mediumText = "Medium";
    char* hardText = "Hard";

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

    EndDrawing();

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
        while(!gameStarted)
		drawMenuScreen();	 
	    
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
