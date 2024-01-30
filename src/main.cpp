#include <iostream>
#include "include\board.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(512, 512), "SFML Chess");
    Texture chessPiecesTexture;


    Board chessBoard;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        // Draw the chessboard and pieces
        chessBoard.displayBoard(window, chessPiecesTexture);

        window.display();
    }
    return 0;
}