#include "Game.h"

int main(int argc, char* argv[])
{
    //Create a game
    Game game;
    
    //Initialise the game
    game.Init();

    //Game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game.Update();
        game.Draw();
    }

    return game.DeInit();   //return close code--0 by default
}