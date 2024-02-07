#include <iostream>
#include <../headers/board.h>
/*
    TODO
    En passent
    Castling
    Restrict move if mate
    Visualize captured pieces
    SVG files for pieces
*/
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
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&x, &y);
                chessBoard.setSelected(x, y);
                if (chessBoard.gameState() == 4 || chessBoard.gameState() == 5)
                {
                    chessBoard.setupBoard();
                }
                chessBoard.calcMoves();
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    goto quit;
                    break;
                case SDLK_m:
                    chessBoard.printHasMoved();
                    break;
                case SDLK_t:
                    chessBoard.printType();
                    break;
                case SDLK_s:
                    chessBoard.printSelected();
                    break;
                case SDLK_p:
                    chessBoard.printCastlePath();
                    break;
                case SDLK_r:
                    chessBoard.setupBoard();
                    break;
                default:
                    break;
                }
            }
        }
        chessBoard.drawChessboard();
    }
quit:
    chessBoard.closeSDL();
    return 0;
}
