#include <iostream>
#include <../headers/board.h>


int main(int argc, char *args[])
{
    Board chessBoard;
    chessBoard.createDebugConsole();
    
    if (!chessBoard.initSDL())
    {
        cerr << "Failed to initialize SDL!" << endl;
        return -1;
    }

    if (!chessBoard.loadMedia())
    {
        cerr << "Failed to load media!" << endl;
        return -1;
    }

    bool quit = false;
    chessBoard.setupBoard();
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        chessBoard.drawChessboard();
    }

    chessBoard.closeSDL();
    return 0;
}
