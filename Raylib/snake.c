/*******************************************************************************************
*
*   raylib - classic game: snake
*
*   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

Texture2D snakesheet;
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static Rectangle sourceFruit = (Rectangle){3, 191, 64, 64};
static Rectangle sourceFaceUp = (Rectangle){192, 0, 64, 64};
static Rectangle sourceFaceDown = (Rectangle){256, 64, 64, 64};
static Rectangle sourceFaceLeft = (Rectangle){192, 64, 64, 64};
static Rectangle sourceFaceRight = (Rectangle){257, 0, 63, 64};
static Rectangle sourceBodyHor = (Rectangle){64, 0, 64, 64};
static Rectangle sourceBodyVer = (Rectangle){128, 64, 64, 64};
static Rectangle sourceTurnDR = (Rectangle){0, 0, 64, 64};
static Rectangle sourceTurnUR = (Rectangle){0, 64, 64, 64};
static Rectangle sourceTurnDL = (Rectangle){128, 0, 64, 64};
static Rectangle sourceTurnUL = (Rectangle){128, 128, 64, 64};
static Rectangle sourceTailUp = (Rectangle){192, 128, 64, 64};
static Rectangle sourceTailDown = (Rectangle){256, 192, 64, 64};
static Rectangle sourceTailLeft = (Rectangle){193, 192, 63, 64};
static Rectangle sourceTailRight = (Rectangle){256, 128, 64, 64};



static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;
static int framesToMove = 15;

static Food fruit = { 0 };
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool allowMove = false;
static Vector2 offset = { 0 };
static int counterTail = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: snake");

    snakesheet = LoadTexture("snake-graphics.png");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    framesCounter = 0;
    gameOver = false;
    pause = false;

    counterTail = 3;
    allowMove = false;
    framesToMove = 15;

    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){10*SQUARE_SIZE + offset.x/2, 4*SQUARE_SIZE + offset.y/2 };
        snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        snake[i].speed = (Vector2){ SQUARE_SIZE, 0 };

        if (i == 0) snake[i].color = DARKBLUE;
        else snake[i].color = BLUE;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = (Vector2){ 0.0f, 0.0f };
    }

    fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    fruit.color = SKYBLUE;
    fruit.active = false;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            // Player control
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
                allowMove = false;
            }

            // Snake movement
            for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;

            if ((framesCounter%framesToMove) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {

                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;

                        if ((snake[0].position.x) >= (screenWidth - offset.x)) snake[0].position.x = offset.x/2.;
                        else if ((snake[0].position.y) >= (screenHeight - offset.y)) snake[0].position.y = offset.y/2;
                        else if (snake[0].position.x <= 0) snake[0].position.x = SQUARE_SIZE*(offset.x-1) + offset.x/2.;
                        else if (snake[0].position.y <= 0) snake[0].position.y = SQUARE_SIZE*(offset.y-3)+ offset.y/2.;

                    }
                    else snake[i].position = snakePosition[i-1];
                }
            }





            // Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)) gameOver = true;
            }

            // Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){ GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, (screenHeight/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.y/2 };


                for (int i = 0; i < counterTail; i++)
                {
                    while ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                    {
                        fruit.position = (Vector2){ GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, (screenHeight/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.y/2 };
                        i = 0;

                    }
                }
            }

            // Collision
            if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
                (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail += 1;
                if(framesToMove > 5) framesToMove--; //Make the snake fo faster when picking fruit.
                fruit.active = false;
            }

            framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;

        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameOver)
        {
            // Draw grid lines
            for (int i = 0; i < screenWidth/SQUARE_SIZE + 1; i++)
            {
                DrawLineV((Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2}, (Vector2){SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2}, LIGHTGRAY);
            }

            for (int i = 0; i < screenHeight/SQUARE_SIZE + 1; i++)
            {
                DrawLineV((Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, (Vector2){screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2}, LIGHTGRAY);
            }

            // Draw snake
		    
	    Rectangle snakePos = (Rectangle){snake[0].position.x*2, snake[0].position.y*2, SQUARE_SIZE, SQUARE_SIZE};
            if(snake[0].speed.x > 0)
		DrawTexturePro(snakesheet, sourceFaceRight, snakePos, (Vector2){snake[0].position.x, snake[0].position.y}, 0, WHITE);
            else if(snake[0].speed.x < 0)
	    	DrawTexturePro(snakesheet, sourceFaceLeft, snakePos, (Vector2){snake[0].position.x, snake[0].position.y}, 0, WHITE);
            else if(snake[0].speed.y > 0)
	        DrawTexturePro(snakesheet, sourceFaceDown, snakePos, (Vector2){snake[0].position.x, snake[0].position.y}, 0, WHITE);
	    else
	        DrawTexturePro(snakesheet, sourceFaceUp, snakePos, (Vector2){snake[0].position.x, snake[0].position.y}, 0, WHITE);
            for (int i = 1; i < counterTail; i++){
	        snakePos = (Rectangle){snake[i].position.x*2, snake[i].position.y*2, SQUARE_SIZE, SQUARE_SIZE};
	        if(i == counterTail-1){
		    //Draw tail according to the speed
		    if(snake[i-1].position.x == snake[i].position.x - SQUARE_SIZE)
		        DrawTexturePro(snakesheet, sourceTailLeft, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
		    else if(snake[i-1].position.x == snake[i].position.x + SQUARE_SIZE)
		        DrawTexturePro(snakesheet, sourceTailRight, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
	            else if(snake[i-1].position.y == snake[i].position.y + SQUARE_SIZE)
		        DrawTexturePro(snakesheet, sourceTailDown, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
		    else 
		        DrawTexturePro(snakesheet, sourceTailUp, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
		}
		else{		
		
		    if(snake[i-1].position.x == snake[i].position.x - SQUARE_SIZE || snake[i-1].position.x == snake[i].position.x + SQUARE_SIZE)
		        DrawTexturePro(snakesheet, sourceBodyHor, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
		    else if(snake[i-1].position.y == snake[i].position.y - SQUARE_SIZE || snake[i-1].position.y == snake[i].position.y + SQUARE_SIZE)
		        DrawTexturePro(snakesheet, sourceBodyVer, snakePos, (Vector2){snake[i].position.x, snake[i].position.y}, 0, WHITE);
	        }
	    }

            // Draw fruit to pick
	    Rectangle test = (Rectangle){fruit.position.x*2, fruit.position.y*2, SQUARE_SIZE, SQUARE_SIZE};
	    DrawTexturePro(snakesheet, sourceFruit, test, (Vector2){fruit.position.x, fruit.position.y}, 0, WHITE);
           
	    if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
