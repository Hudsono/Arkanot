#include "Game.h"

using namespace std;

// Initialise static variables to keep the linker happy
bool Game::_debugMode;
Rectangle Game::_boundary;
bool Game::_paused;
Level Game::_level("NONE");
int Game::_score;
int Game::_lives;
int Game::_levelNum;

Game::Game()
{
    Game::_debugMode = false;
    Game::_paused = false;

    Game::_levelNum = 0;   // Start on 0th level.
    Game::_lives = 3;   // Start with 3 lives.
    Game::_score = 0;   // No score.
    Game::_debugMode = false;   // Default to not being in Debug mode

    cout << "New game!" << endl;    // Feedback
}

Game::~Game()
{
    delete _paddle; // Delete the paddle and set its pointer to nullptr to prevent any future memory accidents.
    _paddle = nullptr;

    cout << "Deleted Game" << endl; // Feedback
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

    // Set the play area boundary --paddle and balls cannot go outside of this rectangle
    _borderPadding = { (float)GetScreenWidth() / 4.175f, (float)GetScreenHeight() / 50 };
    _boundary = Rectangle{ _borderPadding.x, _borderPadding.y, (float)GetScreenWidth() - (_borderPadding.x * 2), (float)GetScreenHeight() - (_borderPadding.y * 2) };
    //_boundRight = _boundary.x + _boundary.width;
    //_boundBottom = _boundary.y + _boundary.height;

    //GameObject::_boundArea = &_boundary;
    //GameObject::_debug = &_debugMode;

    // Initialise some GameObject static variables
    GameObject::_boundaryPtr = &Game::_boundary;
    GameObject::_gamePtr = this;    // Set the GameObject Game reference to this instance.
    Ball::_ballSpeedMod = 1;    // Speed modifier for all Ball objects. Starts at 1x
    
    // Start by setting the the level to Level 1.
    SetLevel(1);
}

void Game::Update()
{
    ////////////////////////////////////////////
    //Update logic NOT SUBJECT to game pausing//
    ////////////////////////////////////////////

    // Toggle debug view
    if (IsKeyPressed(KEY_B))
    {
        _debugMode = !_debugMode;
    }

    // Pause/Unpause the game
    if (IsKeyPressed(KEY_P))
    {
        _paused = !_paused;
        // Message dependent on boolean value.
        cout << (_paused ? "***Game Paused***" : "***Game Unpaused") << endl;
    }

    // Forcefully advance to the next level
    if (IsKeyPressed(KEY_ENTER))
    {
        SetLevel(++_levelNum);
    }

    // Forcefully retract to the previous level
    if (IsKeyPressed(KEY_BACKSLASH))
    {
        SetLevel(--_levelNum);
    }

    // Frame timer updates
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
        // Move the paddle
        // Left
        if (IsKeyDown(KEY_A))
            _paddle->MoveLeft();

        // Right
        if (IsKeyDown(KEY_D))
            _paddle->MoveRight();

        // Fire balls/lasers/etc.
        if (IsKeyPressed(KEY_SPACE))
            _paddle->Fire();

        // Debug: set paddle state
        if (IsKeyPressed(KEY_ZERO))
            _paddle->_paddleState = Paddle::PaddleState::Normal;
        if (IsKeyPressed(KEY_ONE))
            _paddle->_paddleState = Paddle::PaddleState::Catch;
        if (IsKeyPressed(KEY_TWO))
            _paddle->_paddleState = Paddle::PaddleState::Laser;

        // Debug: disruption powerup
        if (IsKeyPressed(KEY_Q))
        {
            _paddle->DisruptAllBalls();
        }

        // Debug: spawn new ball above paddle
        if (IsKeyPressed(KEY_E))
        {
            new Ball(Vector2{ _paddle->_pos.x + _paddle->Size().x / 2, _paddle->_pos.y - _paddle->Size().y }, Vector2{ 0.7, -0.7 });
        }


        // Update paddle logic
        _paddle->Update(deltaTime);

        // Update all balls' logic
        //for (std::vector<Ball*>::iterator itBall = Ball::_ballList.begin(); itBall != Ball::_ballList.end(); itBall++)
        //{
        //    (*itBall)->Update(deltaTime);
        //}
        for (int i = 0; i < Ball::_ballList.size(); i++)
        {
            Ball::_ballList[i]->Update(deltaTime);
        }

        // Update all bricks for shininess
        for (int i = 0; i < Brick::_brickList.size(); i++)
        {
            Brick::_brickList[i]->Update(deltaTime);
        }

        // Update all bullets
        for (int i = 0; i < Bullet::_bulletList.size(); i++)
        {
            Bullet::_bulletList[i]->Update(deltaTime);
        }

        // Update all powerups
        for (int i = 0; i < Powerup::_powerupList.size(); i++)
        {
            Powerup::_powerupList[i]->Update(deltaTime);
        }

        // Calculate entity physics for the whole game.
        DoPhysics();

        // Check if we have broken all breakable bricks, win the game if so.
        if (Brick::_numBricksToDestroy == 0)
            WinGame();

        // Check if we have lost all our balls--lose a life if so.
        if (Ball::_ballList.size() == 0)
            LoseLife();

        // Count down the Slow modifier, if it's in use
        if (Game::_slowTimer > 0)
        {
            // If this timer decrement reaches the end, put the modifier back to 1x
            if (--Game::_slowTimer == 0) {
                Ball::_ballSpeedMod = 1;
            }
        }
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

    DrawText(("SCORE = " + to_string(_score)).c_str(), screenMidX - 500, 50, 20, RED);
    DrawText(("LIVES = " + to_string(_lives)).c_str(), screenMidX - 500, GetScreenHeight() - 50, 20, RED);

    //cout << "FPS = " << fps << endl;

    // Draw the paddle
    _paddle->Render();

    // Draw all balls
    for (int i = 0; i < Ball::_ballList.size(); i++)
    {
        Ball::_ballList[i]->Render();
    }

    // Draw all bricks
    for (int i = 0; i < Brick::_brickList.size(); i++)
    {
        Brick::_brickList[i]->Render();
    }

    // Draw all bullets
    for (int i = 0; i < Bullet::_bulletList.size(); i++)
    {
        Bullet::_bulletList[i]->Render();
    }

    // Draw all powerups
    for (int i = 0; i < Powerup::_powerupList.size(); i++)
    {
        Powerup::_powerupList[i]->Render();
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
                    // Remember the previous position the ball was in before a collision
                    if (ball2->_numCol == 0)
                        ball2->_prevPos = ball2->_pos;
                }
            }
        }


        // Remember the previous position the ball was in BEFORE a collision
        if (ball1->_numCol == 0)
            ball1->_prevPos = ball1->_pos;




        // Check each ball against each paddle
        _paddle->PaddleBallColRes(ball1);

        // Check each ball against each brick
        for (int i = 0; i < Brick::_brickList.size(); i++)
        {
            ball1->BallRectColRes(Brick::_brickList[i]);
        }
    }


    // Check collisions for each bullet
    for (int i = 0; i < Bullet::_bulletList.size(); i++)
    {
        Bullet* bullet = Bullet::_bulletList[i];

        // Check each bullet against each brick
        for (int j = 0; j < Brick::_brickList.size(); j++)
        {
            bullet->BulletBrickCol(Brick::_brickList[j]);
        }

        // Check each bullet against each enemy
    }

    // Check collisions for each paddle
    // Check each paddle against each powerup
    for (int i = 0; i < Powerup::_powerupList.size(); i++)
    {
        _paddle->PaddlePowerupCol(Powerup::_powerupList[i]);
    }
    
}

void Game::SlowPowerup()
{
    // Reset the timer and further slow down balls with cumulative Slow powerup.
    Ball::_ballSpeedMod /= 2;
    Game::_slowTimer = 500;
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

void Game::SetLevel(int level)
{
    // Set current level number to what was specified, in case the function was not called using _levelNum
    Game::_levelNum = level;

    // First, destroy the current level
    Game::_level.DeInitLevel();

    // Then, destroy all entities
    Game::ClearEntities();

    // Check if there is a next level available
    // Expands to "../resources/levels/level1.txt" if _level = 1, etc.
    std::string levelPath = FILE_PATH + "levels/level" + to_string(level) + ".txt";
    ifstream file(levelPath.c_str());

    // If the file exists, load it up.
    // If it doesn't, we assume we have reached the end of the levels and we have won the whole game.
    if (file.good())
    {
        // Close file stream after we're done checking.
        file.close();
        LoadLevel(levelPath);
    }
    else
    {
        // Close file stream after we're done checking.
        file.close();

        std::cout << "Level " << Game::_levelNum << " does not exist." << std::endl;
    }
}

void Game::ClearEntities()
{
    // We know that for std::vector containers, so long as the size is > 0, there's something in element [0] to remove.
    // Remove all ball objects
    while (Ball::_ballList.size() > 0)
        Ball::_ballList[0]->~Ball();

    // Remove all bullet objects
    while (Bullet::_bulletList.size() > 0)
        Bullet::_bulletList[0]->~Bullet();

    // Remove all powerup objects
    while (Powerup::_powerupList.size() > 0)
        Powerup::_powerupList[0]->~Powerup();

    // Remove all enemy objects
    //while (Enemy::_enemyList.size() > 0)
    //    Enemy::_enemyList[0]->~Enemy();
}

void Game::WinGame()
{
    _paused = true;
    std::cout << "-----------------------YOU WIN!------------------------" << std::endl;
    SetLevel(++_levelNum);
}

void Game::LoseLife()
{
    // Debug mode grants us god mode
    if (!Game::_debugMode)
    {
        // Decrement the lives
        Game::_lives--;

        // If we have no lives left, we've lost the game
        // If we still have lives left, reset the paddle and clear entities.
        if (_lives == 0)
        {
            GameOver();
        }
        else
        {
            // Clear entities first, THEN reset the paddle. Otherwise, we delete the paddle's ball and incur an infinite death loop.
            Game::ClearEntities();
            _paddle->ResetPaddle();
        }
    }
}

void Game::GameOver()
{
    SetLevel(1);
    _lives = 3;
    _score = 0;
}


void Game::LoadLevel(std::string levelPath)
{
    // Load a new level given the file path.
    Game::_level = Level(levelPath);

    // Initialise this new level--load up all interactive elements.
    Game::_level.InitLevel();
    _paddle->ResetPaddle();
}