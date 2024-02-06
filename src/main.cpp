#include <iostream>
#include <../headers/board.h>


int main(int argc, char *args[])
{
    Board chessBoard;
    
    if (!chessBoard.initSDL())
    {
        cout << "Failed to initialize SDL!" << endl;
        return -1;
    }

    if (!chessBoard.loadMedia())
    {
        cout << "Failed to load media!" << endl;
        return -1;
    }

    bool quit = false;
    chessBoard.setupBoard();
    SDL_Event event;
    int x, y;
    
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                goto quit;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                chessBoard.setSelected(x, y);
                cout << "X: " << x << " Y: " << y << endl;
                chessBoard.calcMoves();
            }
        }

        chessBoard.drawChessboard();
    }
quit:
    cout << "quit" << endl;
    chessBoard.closeSDL();
    return 0;
}
