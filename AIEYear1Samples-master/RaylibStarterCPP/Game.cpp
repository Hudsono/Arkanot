#include "Game.h"

using namespace std;

//Declare static variables to keep the linker happy
bool Game::_debugMode;
Rectangle Game::_boundary;
bool Game::_paused;

Game::Game()
{
    Game::_debugMode = false;
    Game::_paused = false;

    cout << "New game!" << endl;
}

Game::~Game()
{
    delete _paddle;
    _paddle = nullptr;

    cout << "Deleted Game" << endl;
}

void Game::Init()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);


    currentTime = 0;
    lastTime = 0;
    timer = 0;
    fps = 1;
    frames = 0;

    deltaTime = 0.005f;

    screenMidX = screenWidth / 2;
    screenMidY = screenHeight / 2;

    _paddle = new Paddle(Vector2{ (float)screenMidX, (float)GetScreenHeight() - 100 });

    //set the play area boundary --paddle and balls cannot go outside of this rectangle
    _borderPadding = { (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 50 };
    _boundary = Rectangle{ _borderPadding.x, _borderPadding.y, (float)GetScreenWidth() - (_borderPadding.x * 2), (float)GetScreenHeight() - (_borderPadding.y * 2) };
    //_boundRight = _boundary.x + _boundary.width;
    //_boundBottom = _boundary.y + _boundary.height;

    //GameObject::_boundArea = &_boundary;
    //GameObject::_debug = &_debugMode;

    GameObject::_boundaryPtr = &Game::_boundary;


    _debugMode = true;

    LoadLevel(0);
}

void Game::Update()
{
    ////////////////////////////////////////////
    //Update logic NOT SUBJECT to game pausing//
    ////////////////////////////////////////////

    //Toggle debug view
    if (IsKeyPressed(KEY_B))
    {
        _debugMode = !_debugMode;
    }

    //Pause the game
    if (IsKeyDown(KEY_P))
    {
        _paused = !_paused;
        cout << "GAME PAUSE TOGGLE" << endl;
    }

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

    ////////////////////////////////////////
    //Update logic SUBJECT to game pausing//
    ////////////////////////////////////////

    if (!_paused)
    {
        //Move the paddle
        //Left
        if (IsKeyDown(KEY_A))
            _paddle->MoveLeft();

        //Right
        if (IsKeyDown(KEY_D))
            _paddle->MoveRight();

        //Fire balls/lasers/etc.
        if (IsKeyPressed(KEY_SPACE))
            _paddle->Fire();

        //Debug: toggle Catch
        if (IsKeyPressed(KEY_R))
        {
            if (_paddle->_paddleState == Paddle::PaddleState::Catch) {
                _paddle->_paddleState = Paddle::PaddleState::Normal;
                cout << "Catch Off" << endl;
            }
            else
            {
                _paddle->_paddleState = Paddle::PaddleState::Catch;
                cout << "Catch ON" << endl;
            }
        }

        //Debug: disruption powerup
        if (IsKeyPressed(KEY_Q))
        {
            //Have to save the vector size beforehand otherwise the size changes every time a new ball is added; infinite loop
            int savedSize = Ball::_ballList.size();

            for (int i = 0; i < savedSize; i++)
            {
                Ball::_ballList[i]->Disrupt();
            }
        }

        //Debug: breaking things on purpose (trying to)
        if (IsKeyPressed(KEY_X))
        {
            //Have to save the vector size beforehand otherwise the size changes every time a new ball is added; infinite loop
            int savedSize = Brick::_brickList.size();

            for (int i = 0; i < savedSize; i++)
            {
                new Brick({ Brick::_brickList[i]->_pos }, 'r');
            }
        }

        //Debug: spawn ball above paddle
        if (IsKeyPressed(KEY_E))
        {
            new Ball(Vector2{ _paddle->_pos.x + _paddle->Size().x / 2, _paddle->_pos.y - _paddle->Size().y }, Vector2{ 0.7, -0.7 });
        }


        //Update paddle logic
        _paddle->Update(deltaTime);

        //Update all balls' logic
        //for (std::vector<Ball*>::iterator itBall = Ball::_ballList.begin(); itBall != Ball::_ballList.end(); itBall++)
        //{
        //    (*itBall)->Update(deltaTime);
        //}
        for (int i = 0; i < Ball::_ballList.size(); i++)
        {
            Ball::_ballList[i]->Update(deltaTime);
        }

        //Calculate entity physics for the whole game.
        DoPhysics();
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    string ttt = "FPS = ";

    string ss = to_string(GetFPS());

    ss.insert(0, ttt);

    char const* fpsChar = ss.c_str();
    DrawText(fpsChar, 20, 20, 20, RED);

    string s = to_string(fps);
    char const* pchar = s.c_str();
    DrawText(pchar, screenMidX, screenMidY, 10, GREEN);

    //cout << "FPS = " << fps << endl;

    //Draw the paddle
    _paddle->Render();

    //Draw all balls
    for (int i = 0; i < Ball::_ballList.size(); i++)
    {
        Ball::_ballList[i]->Render();
    }

    //Draw all bricks
    for (int i = 0; i < Brick::_brickList.size(); i++)
    {
        Brick::_brickList[i]->Render();
    }

    if (_debugMode)
    {
        DrawRectangleLines(_boundary.x, _boundary.y, _boundary.width, _boundary.height, BLUE);
    }

    EndDrawing();
}

int Game::DeInit()
{
    CloseWindow();        // Close window and OpenGL context
    return 0;
}

//Check all ball objects against each other for reactive physics.
//Check all ball objects against the paddle, bricks, powerups and enemies as well.
//Game border collision is done on the ball object's update routine.
void Game::DoPhysics()
{
    //Check every ball against every other ball, bar balls stuck to the paddle
    for (int i = 0; i < Ball::_ballList.size(); i++) if (Ball::_ballList[i]->_stuckPaddle == nullptr)
    {
        Ball* ball1 = Ball::_ballList[i];

        //Below code checks collision between balls
        //It works pretty well, except balls can easily get stuck in each-other, other oddities. Very annoying to work with and is kinda broken.
        bool doBallPhysics = false;
        
        if (doBallPhysics)
        {
            //Check every other ball ONLY if it isn't stuck to a paddle, and if it has a valid previous position AKA not spawned while colliding...
            for (int j = i + 1; j < Ball::_ballList.size(); j++) if (Ball::_ballList[j]->_stuckPaddle == nullptr && !Helper::isNaNVector(Ball::_ballList[j]->_prevPos))
            {
                bool tett = Helper::isNaNVector(Ball::_ballList[j]->_prevPos);
                cout << tett << endl;
                if (tett)
                {
                    cout << "a" << endl;
                }

                Ball* ball2 = Ball::_ballList[j];

                Vector2 ballDelta = { ball1->_pos.x - ball2->_pos.x, ball1->_pos.y - ball2->_pos.y };

                //get the magnitude^2 of ball positions against squared combinations of both balls' radii
                //avoids square root calculations
                bool collided = Helper::Dot(ballDelta, ballDelta) <= ((ball1->_radius + ball2->_radius) * (ball1->_radius + ball2->_radius));

                if (collided && (ball1->_ballID != ball2->_ballID))  //don't check against itself
                {
                    cout << "COLLISION!" << endl << endl;
                    ball1->_numCol++;
                    ball2->_numCol++;

                    float deltaX = ball1->_pos.x - ball2->_pos.x;
                    float deltaY = ball1->_pos.y - ball2->_pos.y;

                    ball1->SetPos(ball1->_prevPos);
                    ball2->SetPos(ball2->_prevPos);

                    //Here, push the balls away from each other by their radius
                    //For now, just on the X axis...
                    //Get average radius between the two circles...
                    float averageRadius = (ball1->_radius + ball2->_radius) / 2.0f;
                    ball1->SetPos({ ball1->_pos.x + deltaX + (averageRadius / 2.0f), ball1->_pos.y });
                    ball2->SetPos({ ball2->_pos.x - deltaX - (averageRadius / 2.0f), ball2->_pos.y });

                    Vector2 logic = { (Helper::FloorCeil(ball1->_direction.x) != Helper::FloorCeil(ball2->_direction.x)), (Helper::FloorCeil(ball1->_direction.y) != Helper::FloorCeil(ball2->_direction.y)) };

                    if (logic.x)
                    {
                        float temp = ball1->_direction.x;
                        ball1->_direction.x = ball2->_direction.x;
                        ball2->_direction.x = temp;
                    }

                    if (logic.y)
                    {
                        float temp = ball1->_direction.y;
                        ball1->_direction.y = ball2->_direction.y;
                        ball2->_direction.y = temp;
                    }
                }
                else
                {
                    //Remember the previous position the ball was in before a collision
                    if (ball2->_numCol == 0)
                        ball2->_prevPos = ball2->_pos;
                }
            }
        }


        //Remember the previous position the ball was in BEFORE a collision
        if (ball1->_numCol == 0)
            ball1->_prevPos = ball1->_pos;




        //Check each ball against each paddle
        _paddle->PaddleBallColRes(ball1);

        //Check each ball against each brick
        for (int i = 0; i < Brick::_brickList.size(); i++)
        {
            ball1->BallRectColRes(Brick::_brickList[i]);
        }
    }
}


void Game::BallPhysicsCheck()
{
}

void Game::BulletPhysicsCheck()
{
}

void Game::EnemyPhysicsCheck()
{
}

void Game::PaddlePhysicsCheck()
{
}


void Game::LoadLevel(int level)
{
    char emptyLevel[20][13] = {
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' }
    };

    char level1[20][13] = {
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 's', 's', 's', 's', 's', 's', 's', 's', 's', 's', 's', 's', 's' },
        { 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r' },
        { 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y' },
        { 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b' },
        { 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm' },
        { 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' }
    };

    for (int i = 0; i < 20; i++)    //rows
    {
        for (int j = 0; j < 13; j++)     //columns
        {
            float offsetX = (float)j;
            float offsetY = (float)i;
            if (level1[i][j] != '0')
                new Brick(Vector2{ offsetX, offsetY }, level1[i][j]);

            cout << offsetX << ", " << offsetY << endl;
        }
    }

}