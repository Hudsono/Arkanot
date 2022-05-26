#include "Level.h"

Level::Level(std::string levelPath)
{
    //DEBUG: Testing predefined level format for initialisation...
    char testlvl[COLUMNS][ROWS] = {
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
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' }
    };

    char testlvl3[COLUMNS][ROWS] = {
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'w', 'w', 'w', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'w', 'w', 'w' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'b', 'b', 'b', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c' },
        { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
        { 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'c', 'c', 'c' }
    };


    // Load in brick types from the filePath
    LoadLevelFile(levelPath);

    //Loop through each row Y
    //for (int i = 0; i < COLUMNS; i++)
    //{
    //    //For each row, loop through each of its columns X
    //    for (int j = 0; j < ROWS; j++)
    //    {
    //        _levelBrickTypes[i][j] = testlvl3[i][j];
    //    }
    //}
}

Level::~Level()
{
}

void Level::InitLevel()
{
    // Construct the Level 2D array of Brick pointers
    // Loop through each row Y
    for (int i = 0; i < COLUMNS; i++)
    {
        // For each row, loop through each of its columns X
        for (int j = 0; j < ROWS; j++)
        {
            // If this point in the level array isn't 0, it's a brick.
            // Initialise one respective to its position in _levelBrickTypes and point to it.
            if (_levelBrickTypes[i][j] != '0' && _levelBrickTypes[i][j] != '-')
                //_levelBrickPtrs[i][j] = new Brick(Vector2{ (float)i, (float)j }, _levelBrickTypes[i][j]);
                new Brick(Vector2{ (float)i, (float)j }, _levelBrickTypes[i][j]);

            cout << j << ", " << i << endl;
        }
    }
}

void Level::DeInitLevel()
{
    // Destroy each remaining brick present in the level.
    // This is done by destroying the first member of the _brickList vector every time.
    // Since it dynamically resizes, we can always know that if _brickList isn't empty, there's always an element in [0].
    // An if-loop based on the size would not work given it resizes and often misses most of the bricks.
    while (Brick::_brickList.size() > 0)
    {
        Brick::_brickList[0]->~Brick();
    }
}

void Level::RemoveBrick(int col, int row)
{
    _levelBrickPtrs[col][row] = nullptr;
}

void Level::LoadLevelFile(std::string filePath)
{
    // Create a file input stream (read from) and open it with the provided file path at the same time
    std::ifstream file(filePath);

    // Double-check that opening it was successful.
    // Otherwise, notify user the file did not open successfully.
    if (file.is_open())
    {
        std::cout << "*> Level \"" << filePath << "\" opened successfully!" << std::endl;   // Feedback
        char tettt = 0;
        char tettt2 = 0;

        // Here, loop through our _levelBrickTypes array to fill in the characters provided by the level text file...
        //std::cout << "contents of peek() = " << (unsigned char)file.peek() << endl;
        //file.get(tettt);
        //file.get(tettt2);
        //std::cout << "contents of get = " << tettt << endl;
        //std::cout << "contents of get2 = " << tettt2 << endl;

        //Loop through each row Y
        for (int i = 0; i < COLUMNS; i++)
        {
            //For each row, loop through each of its columns X
            //+1 as there's the newline character to read and disregard each time...
            for (int j = 0; j < ROWS + 1; j++)
            {
                char temp = file.get();

                //std::cout << "temp = " << temp << " | temp (int) = " << (int)temp << std::endl;
                _levelBrickTypes[i][j] = temp;

                if (temp == '\n')
                {
                    _levelBrickTypes[i][j] = '-';
                }
            }
        }
        
    }
    else
    {
        std::cout << "*> Level \"" << filePath << "\" did not open successfully!" << std::endl; // Feedback
    }

    file.close();   // Close the file stream; let other processes use the file. Avoids potential corruption/other issues.
}