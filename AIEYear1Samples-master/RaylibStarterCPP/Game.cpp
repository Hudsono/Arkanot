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
Vector2 Game::_shadowOffset;

Game::Game()
{
    Game::_debugMode = false;   // Start without debugging
    Game::_paused = false;  // Start the game unpaused

    Game::_levelNum = 0;   // Start on 0th level.
    Game::_lives = 3;   // Start with 3 lives.
    Game::_score = 0;   // No score.
    Game::_debugMode = false;   // Default to not being in Debug mode
    Game::_mouseCtrl = true;    // Default to using the mouse (it feels much better--more analogue; akin to an arcade or Atari stick than keyboard controls do)
    Game::_freeze = true;  // Default to player being frozen.

    // Default timers to -1 so they don't do anything off the bat.
    Game::_delayTimeWin = -1;
    Game::_delayTimeDie = -1;

    Game::_shadowOffset = { 10, 10 };   // 10 units X and Y as a default shadow offset.

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
    // Set the game screen dimensions to some hard-coded values
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    InitWindow(screenWidth, screenHeight, "ArkaNot");   // Initialise the RayLib game window
    SetTargetFPS(60);   // 60 frames a second is relatively standard, even for brick-breakers.

    // Timer variables attempting to replicate the deltaTime functionality--can't seem to get it to work...
    //currentTime = 0;
    //lastTime = 0;
    //timer = 0;
    //fps = 1;
    //frames = 0;
    //
    //deltaTime = 0.005f;

    // Convenient halfway values of the screen
    screenMidX = screenWidth / 2;
    screenMidY = screenHeight / 2;

    // Our paddle GameObject we'll be using throughout the whole game.
    // Spawns roughly in the bottom-middle of the play area, but it's immediately adjusted upon level reset and controls anyway...
    _paddle = new Paddle(Vector2{ (float)screenMidX, (float)GetScreenHeight() - 100 });

    // Set the play area boundary --paddle and balls cannot go outside of this rectangle
    _borderPadding = { (float)GetScreenWidth() / 4.175f, (float)GetScreenHeight() / 50 };
    _boundary = Rectangle{ _borderPadding.x, _borderPadding.y, (float)GetScreenWidth() - (_borderPadding.x * 2), (float)GetScreenHeight() - (_borderPadding.y * 2) };

    // Initialise some GameObject static variables
    GameObject::_boundaryPtr = &Game::_boundary;    // Static reference to our game boundary used by other GameObjects to know their place.
    GameObject::_gamePtr = this;    // Set the GameObject Game reference to this instance. Useful for other GameObjects to communicate with this game.
    Ball::_ballSpeedMod = 1;    // Speed modifier for all Ball objects. Starts at 1x

   
    Game::SetBG(1); // Set the background to #1 to start with
    
    
    SetLevel(1);    // Start by setting the the level to Level 1.

    if (_mouseCtrl)
        DisableCursor();    // Make sure we start with the cursor disabled if mouse controls is enabled by default.
}

void Game::SetBG(int bg)
{
    int loopedNum = ((bg - 1) % 4) + 1;   // Whatever the given number, the background will always be 1-4 and loop back on higher numbers. This process increments bg as well, so decrement that in the middle to compensate.
    Helper::LoadSprite(("bg" + to_string(loopedNum) + ".png"), Game::_backgroundSprite); // Loads in the background sprite of the given number

    // Set the colour of the sprite...
    switch (loopedNum)
    {
    case 1:
        Game::_bgColour = BLUE;
        break;

    case 2:
        Game::_bgColour = GREEN;
        break;

    case 3:
        Game::_bgColour = YELLOW;
        break;

    case 4:
        Game::_bgColour = RED;
        break;
    }
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
        cout << (_paused ? "***Game Paused***" : "***Game Unpaused***") << endl;    // Message dependent on boolean value.
    }

    // Toggle Paddle mouse control.
    if (IsKeyPressed(KEY_M))
    {
        _mouseCtrl = !_mouseCtrl;
        cout << (_mouseCtrl ? "***Mouse Control***" : "***Keyboard Control***") << endl;    // Message dependent on boolean value.

        _mouseCtrl ? DisableCursor() : EnableCursor();  // Disable/enable the cursor based on the mouse controls. This hides the mouse and locks it to the window.
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
        // Increment the level timer.
        Game::_levelTimer++;

        // Make all of the metallic bricks shine a couple times just after the level being set (Silver and Gold bricks)
        if (Game::_levelTimer == 30 || Game::_levelTimer == 40)
        {
            for (int i = 0; i < Brick::_brickList.size(); i++) if (Brick::_brickList[i]->_brickType == Brick::BrickTypes::Gold || Brick::_brickList[i]->_brickType == Brick::BrickTypes::Silver)
            {
                Brick::_brickList[i]->Shine();
            }
        }

        // Only permit paddle to be controlled if they aren't frozen
        if (!_freeze)
        {
            // Paddle controls are dependent on if the user has enabled mouse conrtols or keyboard controls.
            // Keyboard controls
            if (!_mouseCtrl)
            {
                // Move paddle left
                if (IsKeyDown(KEY_A))
                    _paddle->MoveLeft();

                // Move paddle right
                if (IsKeyDown(KEY_D))
                    _paddle->MoveRight();

                // Fire balls/lasers/etc.
                if (IsKeyPressed(KEY_SPACE))
                    _paddle->Fire();
            }
            // Mouse controls
            else
            {
                // Clamp the mouse within the window when using these controls so we don't click outside it...
                SetMousePosition(Helper::Clamp(GetMouseX(), _boundary.x, _boundary.x + _boundary.width), screenMidY);

                float halfPaddle = (_paddle->Size().x / 2); // Record what half the paddle is since it's reused a lot here...

                // Here are mouse controls for the paddle instead.
                // The paddle's X position follows the mouse's X position, however it's clamped to the game's boundary. 
                // We offset it by half the paddle's width so the mouse controls the centre of the paddle, but that also means we have to offset the boundary values by the same amount...
                _paddle->SetPos({ Helper::Clamp((float)GetMouseX(), Game::_boundary.x + halfPaddle, (Game::_boundary.x + Game::_boundary.width) - halfPaddle) - halfPaddle, _paddle->_pos.y });

                // Fire balls/lasers/etc.
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    _paddle->Fire();
            }
        }
        else if (_mouseCtrl)
        {
            // Completely lock the mouse onto the paddle at the start of the level, so it doesn't shoot off somewhere after initialisation...
            if (_levelTimer < DELAY_LEVELSTART)
                SetMousePosition(_paddle->_pos.x + (_paddle->Size().x / 2), screenMidY);
        }

        // Check game timers
        // Set the next level once the win delay is over
        if (_levelTimer == _delayTimeWin)
            SetLevel(++_levelNum);

        // Check death level logic after the timer is over...
        if (_levelTimer == _delayTimeDie)
            CheckLife();    // Check if the player can continue or if their game is over.

        if (_levelTimer == DELAY_LEVELSTART)
            _freeze = false;


        // Debug: set paddle state
        if (IsKeyPressed(KEY_ZERO))
            _paddle->_paddleState = Paddle::PaddleState::Normal;
        if (IsKeyPressed(KEY_ONE))
            _paddle->_paddleState = Paddle::PaddleState::Catch;
        if (IsKeyPressed(KEY_TWO))
            _paddle->_paddleState = Paddle::PaddleState::Laser;
        if (IsKeyPressed(KEY_THREE))
            _paddle->_paddleState = Paddle::PaddleState::Enlarge;

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

        // Get the average point of all on-screen balls...
        int leftMost = INT_MAX;
        int rightMost = INT_MIN;
        int topMost = INT_MAX;
        int botMost = INT_MIN;

        // Update each ball's logic, but also work through in getting the average centre of all balls.
        for (int i = 0; i < Ball::_ballList.size(); i++)
        {
            if (Ball::_ballList[i]->_pos.x < leftMost)
                leftMost = Ball::_ballList[i]->_pos.x;
            if (Ball::_ballList[i]->_pos.x > rightMost)
                rightMost = Ball::_ballList[i]->_pos.x;
            if (Ball::_ballList[i]->_pos.y < topMost)
                topMost = Ball::_ballList[i]->_pos.y;
            if (Ball::_ballList[i]->_pos.y > botMost)
                botMost = Ball::_ballList[i]->_pos.y;


            Ball::_ballList[i]->Update(deltaTime);  // Update logic
        }

        // If balls are present, update the shadow offset to match the centre of the screen + the average ball position.
        // Divide by 10 to soften the effect a little...
        if (Ball::_ballList.size() > 0)
            Game::_shadowOffset = { (screenMidX - ((float)(leftMost + rightMost) / 2)) / 10, (screenMidY - ((float)(topMost + botMost) / 2)) / 10 };

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

        // Win conditions do not execute if the player is frozen as they have no control over said conditions in this state...
        if (!_freeze)
        {
            // Check if we have broken all breakable bricks, win the game if so.
            if (Brick::_numBricksToDestroy == 0)
                WinGame();

            // Check if we have lost all our balls--lose a life if so.
            if (Ball::_ballList.size() == 0)
                LoseLife();
        }


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
    BeginDrawing(); // Tell RayLib we want to execute graphical commands

    ClearBackground(BLACK);     // Clear the window with black. This is the background.
    //DrawTextureRec(Game::_backgroundSprite.texture, Game::_boundary, { Game::_boundary.x, Game::_boundary.y }, GREEN);
    DrawTextureQuad(Game::_backgroundSprite.texture, { 5, 6 }, { 0, 0 }, Game::_boundary, Game::_bgColour);
    
    // Drawing shadows for the border.
    // Essentially, just drawing 4 rectangles for each side of the border, offset by the shadow offset.
    DrawRectangle(_boundary.x, (_boundary.y - 100) + _shadowOffset.y, _boundary.width, 100, SHADOW);
    DrawRectangle(_boundary.x, (_boundary.y + _boundary.height) + _shadowOffset.y, _boundary.width, 100, SHADOW);

    DrawRectangle((_boundary.x - 100) + _shadowOffset.x, _boundary.y, 100, _boundary.height, SHADOW);
    DrawRectangle((_boundary.x + _boundary.width) + _shadowOffset.x, _boundary.y, 100, _boundary.height, SHADOW);


    
    // Dark gradient to give some variety in the background graphics and give contrast to the paddle from the background, where the player will be focusing most of their attention on.
    DrawRectangleGradientV(Game::_boundary.x, Game::_boundary.y, Game::_boundary.width + 1, Game::_boundary.height + 1, BLANK, BLACK);  // Off by 1 unit for some reason...
        
    // Show the FPS in debug mode
    if (_debugMode)
        DrawText(("FPS = " + to_string(GetFPS())).c_str(), 20, 20, 20, RED);

    // Display the player's score and number of lives left.
    DrawText(("SCORE = " + to_string(_score)).c_str(), screenMidX - 500, 50, 20, RED);
    DrawText(("LIVES = " + to_string(_lives)).c_str(), screenMidX - 500, GetScreenHeight() - 50, 20, RED);

    // Shadow draws first
    {
        _paddle->RenderShadow();

        // Draw all balls
        for (int i = 0; i < Ball::_ballList.size(); i++)
        {
            Ball::_ballList[i]->RenderShadow();
        }

        // Draw all bricks
        for (int i = 0; i < Brick::_brickList.size(); i++)
        {
            Brick::_brickList[i]->RenderShadow();
        }

        // Draw all bullets
        for (int i = 0; i < Bullet::_bulletList.size(); i++)
        {
            Bullet::_bulletList[i]->RenderShadow();
        }

        // Draw all powerups
        for (int i = 0; i < Powerup::_powerupList.size(); i++)
        {
            Powerup::_powerupList[i]->RenderShadow();
        }
    }
    // Then draw the actual objects...
    {
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
    }


    // Show the boundary via a rectangle
    if (_debugMode)
    {
        DrawRectangleLines(_boundary.x, _boundary.y, _boundary.width, _boundary.height, BLUE);
    }

    EndDrawing();       // Tell RayLib we're done with graphical commands. This displays the image.
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

    // Then, destroy all entities. Also resets the ball speed modifier.
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

    Game::SetBG(Game::_levelNum);   // Change the background texture.
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

    // Also reset some gameplay values
    Ball::_ballSpeedMod = 1;

    // And reset timers and offset times
    Game::_levelTimer = 0;
    Game::_delayTimeWin = -1;
    Game::_delayTimeDie = -1;

    Game::_freeze = true;   // Freeze the player at the start of a level.
}

void Game::WinGame()
{
    _freeze = true;
    _delayTimeWin = _levelTimer + 100; // 100 tick offset from now, the level will change.
    Ball::_ballSpeedMod = 0.2;  // Slow-mo finish
    std::cout << "-----------------------YOU WIN!------------------------" << std::endl;
}

void Game::LoseLife()
{
    // Debug mode grants us god mode
    if (!Game::_debugMode)
    {
        Game::_lives--; // Decrement the lives
        Game::_freeze = true;   // Freeze the player's controls
        Game::_delayTimeDie = Game::_levelTimer + 100;  // Set the delay to commit to lost-life level logic 100 ticks after now.
    }
}

void Game::CheckLife()
{
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