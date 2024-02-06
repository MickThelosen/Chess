#ifndef Board_H
#define Board_H

#include <iostream>
#include <map>
#include <cstring>
#include <../SDL2/SDL.h>
#include <../SDL2/SDL_image.h>

using namespace std;

struct Piece
{
    char type;
    bool isWhite;
    bool hasMoved = false;
    bool isSelected = false;
    bool possible = false;
};

class Board
{
private:
    Piece board[8][8];
    const int SCREEN_WIDTH = 736;
    const int SCREEN_HEIGHT = 736;
    const int SQUARE_SIZE = SCREEN_WIDTH / 8;
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    std::map<std::string, SDL_Texture*> gPieceTextures;
    int selectedX, selectedY;
    bool whiteTurn = true;
    bool pieceSelected = false;
    int gameStatus = 0; // 0 = stopped 1 = playing 2 = white check 3 = black check 4 = white win 5 = black win
public:
    Board(){};
    void setupBoard();
    bool initSDL();
    void closeSDL();
    void drawChessboard();
    bool loadMedia();
    void calcMoves();
    void setSelected(int x, int y);
    bool onBoard(int x, int y);
    int gameState();
    SDL_Texture* loadTexture(const std::string& path);
};

#endif