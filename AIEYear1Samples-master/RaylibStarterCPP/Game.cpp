#include "Game.h"

using namespace std;

Game::Game()
{
    cout << "New game!" << endl;

    currentTime = 0;
    lastTime = 0;
    timer = 0;
    fps = 1;
    frames;

    deltaTime = 0.005f;


    screenWidth = 1280;
    screenHeight = 720;

    screenMidX = screenWidth / 2;
    screenMidY = screenHeight / 2;

    _paddle = new Paddle(Vector2{ (float)screenMidX, (float)screenMidY });
    _ball = new Ball(Vector2{ (float)screenMidX, (float)screenMidY });

    //testing...
    _ballList[0] = _ball;
    _ballList[1] = new Ball(Vector2{ (float)screenMidX + 10, (float)screenMidY });
    _ballList[2] = new Ball(Vector2{ (float)screenMidX + 20, (float)screenMidY });
    _ballList[3] = new Ball(Vector2{ (float)screenMidX + 30, (float)screenMidY });
}

Game::~Game()
{
    delete _paddle;
    delete _ball;
    cout << "Deleted Game" << endl;
}

void Game::Init()
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------
}

void Game::Update()
{
    //Frame timer updates
    lastTime = currentTime;
    currentTime = stopWatch.ElapsedTime();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    timer += deltaTime;
    if (timer >= 1)
    {
        fps = frames;
        frames = 0;
        timer -= 1;
    }
    frames++;
    
    /*
    cout << endl;
    cout << "lastTime = " << lastTime << endl;
    cout << "currentTime = " << currentTime << endl;
    cout << "deltaTime = " << deltaTime << endl;
    cout << "timer = " << timer << endl;
    cout << "frames = " << frames << endl;
    cout << endl;
    */
    //Update paddle logic
    _paddle->Update(deltaTime);
    _ball->Update(deltaTime);

    if (IsKeyDown(KEY_A))
    {
        _paddle->AddPos(Vector2{ -20, 0 });
    }

    if (IsKeyDown(KEY_D))
    {
        _paddle->AddPos(Vector2{ 20, 0 });
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    string s = to_string(fps);
    char const* pchar = s.c_str();
    DrawText(pchar, screenMidX, screenMidY, 10, GREEN);

    //cout << "FPS = " << fps << endl;

    _paddle->Render();
    _ball->Render();

    for (int i = 0; i < MAX_BALLS; i++)
    {
        _ballList[i]->Render();
    }

    EndDrawing();
}

int Game::DeInit()
{
    CloseWindow();        // Close window and OpenGL context
    return 0;
}