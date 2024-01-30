#include <iostream>
#include "../include/headers/board.h"

using namespace sf;
using namespace std;

int main()
{
    cout << "test" << endl;
    RenderWindow window(VideoMode(512, 512), "Mick's Chess");

    Board chessBoard;
    chessBoard.setupBoard();

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
        chessBoard.displayBoard(window);

        window.display();
    }
    return 0;
}
