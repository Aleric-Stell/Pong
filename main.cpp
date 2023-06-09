#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};


int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

        void Draw(){
            DrawCircle(x, y, radius, Yellow);
        }

        void Update(){
            x += speed_x;
            y += speed_y;

            if (y + radius >= GetScreenHeight() || y - radius <= 0){
                speed_y *= -1;
            }

            if (x + radius >= GetScreenWidth()){

                cpu_score++;
                speed_x = 7;
                speed_y = 7;
                ResetBall();
            }

            if (x - radius <= 0){
                
                player_score++;
                speed_x = 7;
                speed_y = 7;
                ResetBall();
            }
        }


        void ResetBall(){

            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;

            int speed_choices[2] = {-1,1};
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_y *= speed_choices[GetRandomValue(0,1)];

        }
};

class Paddle {

    protected:
        void LimitMovement(){
            
            // Top Boundary
            if (y <= 0){
                
                y = 0;
            }
                
            // Bottom Boundary
            if (y + height >= GetScreenHeight()){
                
                y = GetScreenHeight() - height;
            }
        }
    public:
        float x, y, width, height;
        int speed;

        void Draw(){
            DrawRectangleRounded(Rectangle{x,y, width, height}, 0.8, 0, WHITE);
        }

        // Player Controls and Bounds Checking
        void Update(){
            if (IsKeyDown(KEY_UP)){
                y = y - speed;
            }

            if (IsKeyDown(KEY_DOWN)){
                y = y + speed;
            }

            LimitMovement();

        }

};

class CpuPaddle: public Paddle {

    public:
        void Update(int ball_y){
            
            // Ball is above center of cpu paddle
            if (y + height/2 > ball_y){
                y = y - speed;
            }

            // Ball is below center of cpu paddle
            if ( y + height/2 < ball_y){
                y = y + speed;
            }

            LimitMovement();
        }
};

int main () {

    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800; 

    // Game Window Called "Pong"
    InitWindow(screen_width, screen_height, "Pong");

    // FPS Target
    SetTargetFPS(60);

    // initialize ball object
    Ball ball;
    ball.radius = 15;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    Paddle player;
    player.height = 120;
    player.width = 25;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 10;

    CpuPaddle cpu;
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 15;

    const char* winnerText = nullptr;

    // Game Loop, does not end until Stop icon is pressed or KEY_ESCAPE is pressed
    while(WindowShouldClose() == false) { 

        // Checking for collisions
        // Player Paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            
            ball.speed_x *= -1;

            // Ball is coming left
            if (ball.speed_x < 0)
            {
                ball.speed_x *= 1.2f;
                ball.speed_y = ((ball.y - player.y) / (player.height/2)) * ball.speed_x;
            }
        }
        // CPU Paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            
            ball.speed_x *= -1;

            // Ball is coming right
            if (ball.speed_x > 0)
            {
                ball.speed_x *= 1.2f;
                ball.speed_y = ((ball.y - cpu.y) / (cpu.height/2)) * ball.speed_x;

            }
        }

        if (player_score == 7){
            winnerText = "You Win!";
        }
        if (cpu_score == 7){
            winnerText = "Better luck next time...";
        }



        // Drawing the objects
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Winner
        while(winnerText){
                
            while( WindowShouldClose() == 0){
                
                if (IsKeyDown(KEY_SPACE) == true){
                    ball.x = GetScreenWidth() / 2;
                    ball.y = GetScreenHeight() / 2;
                    ball.speed_x = 7;
                    ball.speed_y = 7;
                    cpu_score = 0;
                    player_score = 0;
                    winnerText = nullptr;
                    break;
                }
                const char* playAgainText = "Press SPACEBAR to play again or ESC to quit";
                int playAgainTextWidth = MeasureText(playAgainText, 25);
                int winnerTextWidth = MeasureText(winnerText, 60);

                
                ClearBackground(Dark_Green);
                DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
                DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
                DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
                DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
                DrawText(TextFormat("%i", player_score), 3 * screen_width/4 - 20, 20, 80, WHITE);

                player.Draw();
                cpu.Draw();
                
                DrawText(winnerText, GetScreenWidth() / 2 - (winnerTextWidth / 2), GetScreenHeight() / 2 - 45, 60, Yellow);
                DrawText(playAgainText, GetScreenWidth() / 2 - (playAgainTextWidth / 2), GetScreenHeight() / 2 + 45, 25, Yellow);

                EndDrawing();

                if(WindowShouldClose() == 1){
                    return 0;
                }
            }
        }

        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width/4 - 20, 20, 80, WHITE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}