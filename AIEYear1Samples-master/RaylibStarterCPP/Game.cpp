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

    _paddle = new Paddle(Vector2{ (float)screenMidX, (float)GetScreenHeight() - 100});

    //set the play area boundary --paddle and balls cannot go outside of this rectangle
    _borderPadding = { (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 50};
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
    if (IsKeyPressed(KEY_P))
    {
        cout << "PAUSE TOGGLE" << endl;
        //_paused = !_paused;
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

            cout << "===DISRUPT ATTEMPT===" << endl;

            for (int i = 0; i < savedSize; i++)
            {
                Ball::_ballList[i]->Disrupt();
            }

            cout << "===DISRUPT END===" << endl << endl;
        }

        //Debug: breaking things on purpose (trying to)
        if (IsKeyPressed(KEY_X))
        {
            //Have to save the vector size beforehand otherwise the size changes every time a new ball is added; infinite loop
            int savedSize = Brick::_brickList.size();

            for (int i = 0; i < savedSize; i++)
            {
                new Brick({Brick::_brickList[i]->_pos}, 'r');
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
        for (int j = i + 1; j < Ball::_ballList.size(); j++) if (Ball::_ballList[j]->_stuckPaddle == nullptr && Helper::isNaNVector(Ball::_ballList[j]->_prevPos))
        {
            Ball* ball2 = Ball::_ballList[j];

            Vector2 ballDelta = {ball1->_pos.x - ball2->_pos.x, ball1->_pos.y - ball2->_pos.y};

            //get the magnitude^2 of ball positions against squared combinations of both balls' radii
            //avoids square root calculations
            bool collided = Helper::Dot(ballDelta, ballDelta) <= ((ball1->_radius + ball2->_radius) * (ball1->_radius + ball2->_radius));

            if (collided && (ball1->_ballID != ball2->_ballID))  //don't check against itself
            {
                //cout << "COLLISION!" << endl << endl;
                ball1->_numCol++;
                ball2->_numCol++;

                float deltaX = ball1->_pos.x - ball2->_pos.x;
                float deltaY = ball1->_pos.y - ball2->_pos.y;

                ball1->SetPos(ball1->_prevPos);
                ball2->SetPos(ball2->_prevPos);

                //Vector2 temp = { ball1->_direction.x, ball1->_direction.y };
                //ball1->_direction = {ball1->_direction.x * ball2->_direction.x, ball1->_direction.y * ball2->_direction.y };
                //ball2->_direction = {ball2->_direction.x * temp.x, ball2->_direction.y * temp.y };

                //
                Vector2 logic = { (Helper::FloorCeil(ball1->_direction.x) != Helper::FloorCeil(ball2->_direction.x)), (Helper::FloorCeil(ball1->_direction.y) != Helper::FloorCeil(ball2->_direction.y)) };

                //cout << "( " << logic.x << ", " << logic.y << " )" << endl;

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


                //Vector2 temp = { ball1->_direction.x, ball1->_direction.y };
                //ball1->_direction = { ball2->_direction.x, ball2->_direction.y };
                //Helper::Normalise(ball1->_direction);
                //ball2->_direction = { temp.x, temp.y };
                //Helper::Normalise(ball2->_direction);
                //TODO: here. Need to flip them like logic, sort of:
                /*
                1,0 --right-up
                1,1 --right-down

                this means they are colliding from one coming from above and another coming from below
                we just need to invert their Y axis and not touch the X axis

                maybe round their direction up to 1 (eg 0.5 = 1)

                example: and then if both balls' Y directions are different, we flip their Y directions--maybe inherit the others' Y direction?

                should only flip where it's opposite
                */
            }
            else
            {
                //Remember the previous position the ball was in before a collision
                if (ball2->_numCol == 0)
                    ball2->_prevPos = ball2->_pos;
            }
        }

        //Remember the previous position the ball was in before a collision
        if (ball1->_numCol == 0)
                ball1->_prevPos = ball1->_pos;



        //Check against the paddle here
        //Clamp point inside the paddle
        Vector2 clampVal = { Helper::Clamp(ball1->_pos.x, _paddle->_pos.x, _paddle->_pos.x + _paddle->Size().x), Helper::Clamp(ball1->_pos.y, _paddle->_pos.y, _paddle->_pos.y + _paddle->Size().y)};

        //Get difference between the clamped paddle point and the ball
        Vector2 paddleBallDelta = { clampVal.x - ball1->_pos.x, clampVal.y - ball1->_pos.y };

        //determine if the ball collided with the paddle
        //get the magnitude^2 of ball positions against squared combinations of both balls' radii
        //avoids square root calculations
        bool paddleCollided = Helper::Dot(paddleBallDelta, paddleBallDelta) <= ((ball1->_radius) * (ball1->_radius));

        //determine which face the ball touched the paddle
        bool touchU = (ball1->_pos.y < _paddle->_pos.y);
        bool touchD = (ball1->_pos.y > _paddle->_pos.y + _paddle->Size().y);
        bool touchL = (ball1->_pos.x < _paddle->_pos.x);
        bool touchR = (ball1->_pos.x > _paddle->_pos.x + _paddle->Size().x);

        //check if the ball landed on the right half of the paddle. For top collision.
        //in Arkanoid, the ball is resolved differently depending on which half of the paddle the ball lands on
        bool rightHalf = (ball1->_pos.x > _paddle->_pos.x + (_paddle->Size().x / 2));

        //If the ball indeed collided with the paddle somehow...
        if (paddleCollided)
        {
            //cout << "--------" << endl;
            //cout << touchU << endl;
            //cout << touchD << endl;
            //cout << touchL << endl;
            //cout << touchR << endl;
            //cout << "--------" << endl;

            // Make one axis win!
            // We only want one axis to respond, otherwise collision resolution makes no sense
            if ((touchU || touchD) && (touchL || touchR))
            {
                //if there's a bigger difference on the X axis, then choose to report X axis collision
                //Otherwise, if there's a bigger difference on the Y axis, then choose to report Y axis collision
                if (abs(paddleBallDelta.x) > abs(paddleBallDelta.y))
                {
                    touchU = false;
                    touchD = false;
                }
                else
                {
                    touchL = false;
                    touchR = false;
                }
            }

            //If the ball touches the top of the paddle, execute some gameplay logic to keep the ball's direction predictable and controllable,
            //but allso varied.
            if (touchU)
            {
                ball1->_direction.y = -abs(ball1->_direction.y);    //Set ball's Y direction to point up

                //Reflection (below) proved difficult when colliders were inside each-other (i.e. it would reflect back-and-forth over and over)
                //ball1->_direction.y *= -1;

                //calculate how much X direction bias based on how close the ball is to the centre of the paddle
                float sharpness = -((_paddle->_pos.x + (_paddle->Size().x / 2)) - ball1->_pos.x) / (_paddle->Size().x / 2);

                //if on the right, also reflect the X direction
                if (rightHalf)
                    ball1->_direction.x = sharpness;
                else
                    ball1->_direction.x += sharpness;

                //normalise direction
                // Actually, in Arkanoid, it doesn't look like the direction is normalised on purpose
                // This way, the ball speeds up when hit on the paddle edges, and slows down when hitting the centre of the paddle
                // AKA varied gameplay and controlled skill.
                //Helper::Normalise(ball1->_direction);

                // Make sure the ball's vertical direction doesn't become too shallow
                // Otherwise, the ball can get stuck in the level without much player input
                // Arkanoid also seems to impose this limit

                // If the Y direction is in the exclusive range -0.5 to 0.5, the ball is moving too slow on the Y axis
                // So, clamp the Y direction outside of this range (inclusive)
                if (ball1->_direction.y > -0.5f && ball1->_direction.y < 0.5)
                    ball1->_direction.y = Helper::ClampOut(ball1->_direction.y, -0.5f, 0.5f);

                // Also impose an X/horizontal direction limit, so that the game is not dead easy with the
                // ball bouncing directly up and down.
                // Again, Arkanoid appears to impose this limit.
                   
                // If the X direction is in the exclusive range -0.5 to 0.5, the ball is moving too slow on the X axis
                // So, clamp the X direction outside of this range (inclusive)
                if (ball1->_direction.x > -0.5f && ball1->_direction.x < 0.5)
                    ball1->_direction.x = Helper::ClampOut(ball1->_direction.x, -0.5f, 0.5f);


                //If the paddle is in "catch" mode, then attach the ball to the paddle if it hits the top
                if (_paddle->_paddleState == Paddle::PaddleState::Catch)
                {
                    ball1->StickToPaddle(_paddle);
                }
            }
            //For all other directions, just set the ball's respective axis in the respective opposite direction. The player won't be worrying
            //about these nearly as much as the top of the paddle.
            else if (touchD)
                ball1->_direction.y = abs(ball1->_direction.y);     //Set ball's Y direction to point down
            else if (touchL)
                ball1->_direction.x = -abs(ball1->_direction.x);    //Set ball's X direction to point right
            else if (touchR)
                ball1->_direction.x = abs(ball1->_direction.x);     //Set ball's X directino to point left
        }
    }
}

//bool Game::Phys_RectCircleCol(GameObject* rect, Ball* circ)
//{
//    //Clamp the circle's point inside the rectangle
//    Vector2 clampVal = { Helper::Clamp(circ->_pos.x, rect->_pos.x, rect->_pos.x + rect->Size().x), Helper::Clamp(circ->_pos.y, rect->_pos.y, rect->_pos.y + rect->Size().y) };
//
//    //Get difference between the clamped rectangle point and the circle
//    Vector2 paddleBallDelta = { clampVal.x - circ->_pos.x, clampVal.y - circ->_pos.y };
//
//    //determine if the ball collided with the paddle
//    //get the magnitude^2 of ball positions against squared combinations of both balls' radii
//    //avoids square root calculations
//    bool paddleCollided = Helper::Dot(paddleBallDelta, paddleBallDelta) <= ((circ->_size) * (circ->_size));
//
//    //determine which face the ball touched the paddle
//    bool touchU = (circ->_pos.y < rect->_pos.y);
//    bool touchD = (circ->_pos.y > rect->_pos.y + _paddle->Size().y);
//    bool touchL = (circ->_pos.x < rect->_pos.x);
//    bool touchR = (circ->_pos.x > rect->_pos.x + rect->Size().x);
//
//    //check if the ball landed on the right half of the paddle. For top collision.
//    //in Arkanoid, the ball is resolved differently depending on which half of the paddle the ball lands on
//    bool rightHalf = (circ->_pos.x > rect->_pos.x + (rect->Size().x / 2));
//
//    //If the ball indeed collided with the paddle somehow...
//    if (paddleCollided)
//    {
//        //cout << "--------" << endl;
//        //cout << touchU << endl;
//        //cout << touchD << endl;
//        //cout << touchL << endl;
//        //cout << touchR << endl;
//        //cout << "--------" << endl;
//
//        //make one axis win!
//        //We only want one axis to respond
//        if ((touchU || touchD) && (touchL || touchR))
//        {
//            //if there's a bigger difference on the X axis, then choose to report X axis collision
//            //Otherwise, if there's a bigger difference on the Y axis, then choose to report Y axis collision
//            if (abs(paddleBallDelta.x) > abs(paddleBallDelta.y))
//            {
//                touchU = false;
//                touchD = false;
//            }
//            else
//            {
//                touchL = false;
//                touchR = false;
//            }
//        }
//
//        if (touchU || touchD)
//        {
//            //calculate how much X direction bias based on how close the ball is to the centre of the paddle
//            float sharpness = -((rect->_pos.x + (rect->Size().x / 2)) - circ->_pos.x) / (rect->Size().x / 2);
//
//            //reflect ball's Y direction to bounce it back--usually, bounce it upwards
//            circ->_direction.y *= -1;
//
//            //if on the right, also reflect the X direction
//            if (rightHalf)
//            {
//                circ->_direction.x = sharpness;
//            }
//            else
//            {
//                circ->_direction.x += sharpness;
//            }
//
//            //cout << sharpness << endl;
//            //add sharpness to ball's X direction
//
//            //normalise direction
//            // Actually, in Arkanoid, it doesn't look like the direction is normalised on purpose
//            // This way, the ball speeds up when hit on the paddle edges, and slows down when hitting the centre of the paddle
//            // AKA varied gameplay and controlled skill.
//            //Helper::Normalise(ball1->_direction);
//
//            //Make sure the ball's vertical direction doesn't become too shallow
//            //Otherwise, the ball can get stuck in the level without much player input
//            //Arkanoid also seems to impose this limit
//
//            //If the Y direction is in the exclusive range -0.5 to 0.5, the ball is moving too slow on the Y axis
//            //So, clamp the Y direction outside of this range (inclusive)
//            if (circ->_direction.y > -0.5f && circ->_direction.y < 0)
//                circ->_direction.y = Helper::ClampOut(circ->_direction.y, -0.5f, 0.5f);
//        }
//        else if (touchL || touchR)
//        {
//            circ->_direction.x *= -1;
//        }
//    }
//}

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

    char level1 [20][13] = {
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
