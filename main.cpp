#include <iostream>
#include <raylib.h>

using namespace std;

int main () 
{
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800; 

    // Game Window Called "Pong"
    InitWindow(screen_width, screen_height, "Pong");

    // FPS Target
    SetTargetFPS(144);

    // Game Loop, does not end until Stop icon is pressed or KEY_ESCAPE is pressed
    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Drawing 
        DrawCircle(screen_width/2, screen_height/2, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}